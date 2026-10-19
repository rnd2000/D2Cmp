#include "D2CMP.h"

//  Tilecmp.cpp -- the tile compiler of D2CMP.dll 1.10: the API a tool builds floor and wall tiles with, and the tile PROJECT it produces -- a header,
//  an array of tile entries, each entry an array of components, each component its pixel data. A tool makes a list (TileListCreate), adds builder
//  tiles to it (TileListAddTile) and gives each its pixels -- 32x32 sub-tiles (TileAddAddSubTile) or a 160x80 floor diamond cut into 25 pieces
//  (TileAddAddFloorTile) -- turns the list into a project (TileListCreateProject), converts a 24-bit project to an indexed and compressed one
//  (TileProjectConvert24To8) and serializes it into one block of relative offsets (TileProjectSerialize). The game reads such a block back:
//  NormalizeProject turns the offsets into pointers, and TileCache.cpp loads each tile's components on demand (RebaseComponentDataPointers).
//  The original source file is Tilecmp.cpp (C:\projects\D2\head\Diablo2\Source\D2CMP\SRC\Tilecmp.cpp, the path every assert pushes).
//  Fourteen functions over 0x6FDFE8D0..0x6FDFFDC8; eleven are exports, #10065..#10075.
//
//  dwFlags of a project: 0x1 TILEPROJECT_FLAG_24BIT; 0x2 set once it is serialized (and on the serialized header), cleared by normalizing; 0x4 set
//  on the header and again by normalizing -- a project in ONE block, which TileProjectDestroy frees whole; 0x8 TILEPROJECT_FEWBLOCKS, which
//  TileProjectDestroy refuses. wFlags of a component: 0x1 the component holds pixels, 0x1000 a 32x32 square sub-tile, 0x2000 TILECOMP_GAMESQUARE,
//  a floor piece. The fields are only set, cleared or tested by mask.

//  The path every assert, allocation and free of the unit pushes.
static const char szTilecmpCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2CMP\\SRC\\Tilecmp.cpp";

//  Layout checks: the offsets the functions below read, each with the instruction it was measured from.
static_assert(offsetof(D2CmpTileList, b24Bit) == 0x00, "D2CmpTileList.b24Bit: mov [eax], ecx (0x6FDFE919)");
static_assert(offsetof(D2CmpTileList, dwTransparentRGB24) == 0x04, "D2CmpTileList.dwTransparentRGB24: mov [eax+4], edx (0x6FDFE91B)");
static_assert(offsetof(D2CmpTileList, nNumTiles) == 0x08, "D2CmpTileList.nNumTiles: mov eax, [esi+8] (0x6FDFEA42)");
static_assert(offsetof(D2CmpTileList, pTiles) == 0x0C, "D2CmpTileList.pTiles: mov [esi+0Ch], edx (0x6FDFEA46)");
static_assert(offsetof(D2CmpTileBuilder, nMinX) == 0x00 && offsetof(D2CmpTileBuilder, nMaxY) == 0x04, "D2CmpTileBuilder bounds: mov [ebp+0], eax (0x6FDFEC54), mov [ebp+4], eax (0x6FDFEC6F)");
static_assert(offsetof(D2CmpTileBuilder, nMaxX) == 0x08 && offsetof(D2CmpTileBuilder, nMinY) == 0x0C, "D2CmpTileBuilder bounds: mov [ebp+8], eax (0x6FDFEC61), mov [ebp+0Ch], eax (0x6FDFEC81)");
static_assert(offsetof(D2CmpTileBuilder, nLightDirection) == 0x10, "D2CmpTileBuilder.nLightDirection: mov [edx+10h], ecx (0x6FDFEA79)");
static_assert(offsetof(D2CmpTileBuilder, nRoofHeight) == 0x14 && offsetof(D2CmpTileBuilder, nFlags) == 0x16, "D2CmpTileBuilder: mov [edx+14h], ax (0x6FDFEA59), mov [edx+16h], cx (0x6FDFEA51)");
static_assert(offsetof(D2CmpTileBuilder, nType) == 0x24 && offsetof(D2CmpTileBuilder, nRarity_Frame) == 0x30, "D2CmpTileBuilder: mov [edx+24h], ecx (0x6FDFEA61), mov [edx+30h], eax (0x6FDFEA76)");
static_assert(offsetof(D2CmpTileBuilder, dwTransparentRGB24) == 0x34, "D2CmpTileBuilder.dwTransparentRGB24: mov [edx+34h], eax (0x6FDFEA8B)");
static_assert(offsetof(D2CmpTileBuilder, nSubtileFlags) == 0x38, "D2CmpTileBuilder.nSubtileFlags: lea edi, [edx+38h] (0x6FDFEA83)");
static_assert(offsetof(D2CmpTileBuilder, nPixelFormatFlags) == 0x51, "D2CmpTileBuilder.nPixelFormatFlags: mov byte ptr [edx+51h], 2 (0x6FDFEA8E)");
static_assert(offsetof(D2CmpTileBuilder, nComponentCount) == 0x54, "D2CmpTileBuilder.nComponentCount: mov [ebp+54h], edx (0x6FDFEBBF)");
static_assert(offsetof(D2CmpTileBuilder, pComponents) == 0x58 && offsetof(D2CmpTileBuilder, pNext) == 0x5C, "D2CmpTileBuilder: mov esi, [ebx+58h], mov ebp, [ebx+5Ch] (0x6FDFE961, 0x6FDFE964)");
static_assert(offsetof(D2CmpTileBuildComponent, nGridX) == 0x04 && offsetof(D2CmpTileBuildComponent, nGridY) == 0x05, "D2CmpTileBuildComponent grid: mov dl, [eax+4], mov dl, [eax+5] (0x6FDFF149, 0x6FDFF14F)");
static_assert(offsetof(D2CmpTileBuildComponent, wFlags) == 0x06 && offsetof(D2CmpTileBuildComponent, nDataLength) == 0x08, "D2CmpTileBuildComponent: mov word ptr [esi+6], 1001h (0x6FDFEC3E), mov word ptr [esi+8], 0C00h (0x6FDFEC35)");
static_assert(offsetof(D2CmpTileBuildComponent, pData) == 0x0C && offsetof(D2CmpTileBuildComponent, pNext) == 0x10, "D2CmpTileBuildComponent: mov ecx, [esi+0Ch], mov edi, [esi+10h] (0x6FDFE96B, 0x6FDFE96E)");
static_assert(offsetof(D2CmpTileRuntimeComponent, nGridX) == 0x06 && offsetof(D2CmpTileRuntimeComponent, nGridY) == 0x07, "D2CmpTileRuntimeComponent grid: mov [ecx+6], dl, mov [ecx+7], dl (0x6FDFF14C, 0x6FDFF152)");
static_assert(offsetof(D2CmpTileRuntimeComponent, wFlags) == 0x08 && offsetof(D2CmpTileRuntimeComponent, wDataBytesLow) == 0x0A, "D2CmpTileRuntimeComponent: and byte ptr [esi+8], 0FEh (0x6FDFFC28), mov cx, [eax+0Ah] (0x6FDFF5A4)");
static_assert(offsetof(D2CmpTileRuntimeComponent, pData) == 0x10, "D2CmpTileRuntimeComponent.pData: mov esi, [eax+10h] (0x6FDFF5A8)");
static_assert(offsetof(D2TileLibraryEntryStrc, nRoofHeight) == 0x04 && offsetof(D2TileLibraryEntryStrc, nFlags) == 0x06, "D2TileLibraryEntryStrc: mov [ebp+4], dx (0x6FDFF094), mov [ebp+6], ax (0x6FDFF09C)");
static_assert(offsetof(D2TileLibraryEntryStrc, nTotalHeight) == 0x08 && offsetof(D2TileLibraryEntryStrc, nWidth) == 0x0C, "D2TileLibraryEntryStrc: mov [ebp+8], edx (0x6FDFF0B3), mov [ebp+0Ch], ecx (0x6FDFF0AA)");
static_assert(offsetof(D2TileLibraryEntryStrc, transparentColorRGB24) == 0x24, "D2TileLibraryEntryStrc.transparentColorRGB24: mov [ebp+24h], ecx (0x6FDFF0D1)");
static_assert(offsetof(D2TileLibraryEntryStrc, nSubtileFlags) == 0x28, "D2TileLibraryEntryStrc.nSubtileFlags: lea edi, [ebp+28h] (0x6FDFF0A5)");
static_assert(offsetof(D2CmpNormalizedTileProject, dwFlags) == 0x04, "D2CmpNormalizedTileProject.dwFlags: mov eax, [esi+4] (0x6FDFF25E)");
static_assert(offsetof(D2TileLibraryHeaderStrc, dwTileStart_pFirst) == 0x110, "D2TileLibraryHeaderStrc.dwTileStart_pFirst: mov esi, [ecx+110h] (0x6FDFF7B0)");


//1.10f: D2CMP.0x6FDFE8D0
//  #10065. A new, empty tile list, carrying whether its tiles are 24-bit and their transparent colour for every tile added.
D2CmpTileList *__stdcall TILECMP_TileListCreate(int32_t b24Bit, uint32_t dwTransparentRGB24)
{
    D2CmpTileList *pList = (D2CmpTileList *)FOG_Alloc(sizeof(D2CmpTileList), szTilecmpCpp, 109, 0);
    if (!pList)
    {
        FOG_DisplayAssert("ptList", szTilecmpCpp, 110);
        exit(-1);
    }

    memset(pList, 0, sizeof(D2CmpTileList));
    pList->b24Bit = b24Bit;
    pList->dwTransparentRGB24 = dwTransparentRGB24;
    return pList;
}


//1.10f: D2CMP.0x6FDFE930
//  #10066. Frees a list: each builder tile's components and their pixels, each tile, the list. Every next pointer is loaded before the frees. The
//  list's own fields are not cleared. The original inlines the freeing of a builder tile here.
void __stdcall TILECMP_TileListDestroy(D2CmpTileList *pList)
{
    if (!pList)
    {
        FOG_DisplayAssert("ptList", szTilecmpCpp, 128);
        exit(-1);
    }

    D2CmpTileBuilder *pTile = pList->pTiles;
    while (pTile)
    {
        D2CmpTileBuildComponent *pComp = pTile->pComponents;
        D2CmpTileBuilder *pNextTile = pTile->pNext;
        while (pComp)
        {
            D2CmpTileBuildComponent *pNextComp = pComp->pNext;
            if (pComp->pData)
                FOG_Free(pComp->pData, szTilecmpCpp, 53, 0);
            FOG_Free(pComp, szTilecmpCpp, 56, 0);
            pComp = pNextComp;
        }
        FOG_Free(pTile, szTilecmpCpp, 96, 0);
        pTile = pNextTile;
    }

    FOG_Free(pList, szTilecmpCpp, 140, 0);
}


//1.10f: D2CMP.0x6FDFE9D0
//  #10067. A new builder tile at the head of the list, counted, carrying its material flags, roof height, type, style, sequence, rarity or frame
//  and light direction, the list's transparent colour, its 25 subtile flags (copied from pSubtileFlags with no test) and pixel format 2 (RGB24).
//  The original inlines the making of a builder tile here.
D2CmpTileBuilder *__stdcall TILECMP_TileListAddTile(D2CmpTileList *pList, uint16_t nFlags, uint16_t nRoofHeight, int32_t nType, int32_t nStyle, int32_t nSequence, int32_t nRarity_Frame, int32_t nLightDirection, const uint8_t *pSubtileFlags)
{
    if (!pList)
    {
        FOG_DisplayAssert("ptList", szTilecmpCpp, 162);
        exit(-1);
    }

    D2CmpTileBuilder *pTile = (D2CmpTileBuilder *)FOG_Alloc(sizeof(D2CmpTileBuilder), szTilecmpCpp, 69, 0);
    if (!pTile)
    {
        FOG_DisplayAssert("ptTile", szTilecmpCpp, 70);
        exit(-1);
    }
    memset(pTile, 0, sizeof(D2CmpTileBuilder));

    pTile->pNext = pList->pTiles;
    ++pList->nNumTiles;
    pList->pTiles = pTile;

    pTile->nFlags = nFlags;
    pTile->nRoofHeight = nRoofHeight;
    pTile->nType = nType;
    pTile->nStyle = nStyle;
    pTile->nSequence = nSequence;
    pTile->nRarity_Frame = nRarity_Frame;
    pTile->nLightDirection = nLightDirection;
    pTile->dwTransparentRGB24 = pList->dwTransparentRGB24;
    pTile->nPixelFormatFlags = 2;
    memcpy(pTile->nSubtileFlags, pSubtileFlags, sizeof(pTile->nSubtileFlags));
    return pTile;
}


//1.10f: D2CMP.0x6FDFEAA0
//  #10068. Adds a 32x32 square sub-tile cut from pImage at its stride, unless every pixel is transparent: with pixel format bit 2 every RGB24 pixel
//  is compared with the tile's transparent colour, with bit 1 every byte with 0 (an indexed image), the second scan going on from where the first
//  stopped (the scan pointer is not reset). A pixel that differs makes a component -- linked and counted BEFORE the coordinates are checked
//  (signed compares) -- with 3072 bytes holding the 32 rows of 96 bytes copied from pImage, flags 0x1001 (pixels, square); then the tile's bounds
//  grow to hold it (signed compares). The original inlines the making of a builder component here.
//  BUG: the RGB24 scan loads each pixel as a DWORD (`mov eax, [ecx]` / `and eax, 0FFFFFFh`, 0x6FDFEB1A, 0x6FDFEB1F), so the last pixel of
//  the 32nd row reads one byte past the 96 the region holds. UNREACHABLE: no stock module imports #10068. Ours reproduces it with
//  a four-byte memcpy, the fourth byte masked off.
//  The transparent colour is rebuilt from the field's three low bytes (three zero-extended bytes shifted and added, which is the field's low 24
//  bits exactly). The offsets are stored as 16 bits after a check that lets 0x8000..0xFFFF through; the conversion to __int16 keeps the low 16
//  bits, as the store does.
void __stdcall TILECMP_TileAddAddSubTile(D2CmpTileBuilder *pTile, const uint8_t *pImage, int32_t nImageStride, int32_t nOffsetX, int32_t nOffsetY)
{
    if (!pTile)
    {
        FOG_DisplayAssert("hTileAdd", szTilecmpCpp, 256);
        exit(-1);
    }
    const uint8_t *pScan = pImage;
    if (!pImage)
    {
        FOG_DisplayAssert("ptImage", szTilecmpCpp, 257);
        exit(-1);
    }

    if (pTile->nPixelFormatFlags & 0x2)
    {
        const uint32_t dwTransparent = pTile->dwTransparentRGB24 & 0x00FFFFFF;
        for (int32_t nRow = 0; nRow < 32; ++nRow)
        {
            for (int32_t nColumn = 0; nColumn < 32; ++nColumn)
            {
                uint32_t dwPixel;
                memcpy(&dwPixel, pScan, sizeof(dwPixel));
                pScan += 3;
                if ((dwPixel & 0x00FFFFFF) != dwTransparent)
                    goto AddComponent;
            }
            pScan += nImageStride - 96;
        }
    }
    if (!(pTile->nPixelFormatFlags & 0x1))
        return;
    for (int32_t nRow = 0; nRow < 32; ++nRow)
    {
        for (int32_t nColumn = 0; nColumn < 32; ++nColumn)
        {
            if (*pScan++)
                goto AddComponent;
        }
        pScan += nImageStride - 32;
    }
    return;

AddComponent:
    D2CmpTileBuildComponent *pComp = (D2CmpTileBuildComponent *)FOG_Alloc(sizeof(D2CmpTileBuildComponent), szTilecmpCpp, 35, 0);
    if (!pComp)
    {
        FOG_DisplayAssert("ptComp", szTilecmpCpp, 36);
        exit(-1);
    }
    memset(pComp, 0, sizeof(D2CmpTileBuildComponent));
    pComp->pNext = pTile->pComponents;
    pTile->pComponents = pComp;
    ++pTile->nComponentCount;

    if (nOffsetX >= 0x10000 || nOffsetY >= 0x10000)
    {
        FOG_DisplayAssert("nOffsetX < 0x00010000 && nOffsetY < 0x00010000", szTilecmpCpp, 274);
        exit(-1);
    }
    pComp->nOffsetX = (int16_t)nOffsetX;
    pComp->nOffsetY = (int16_t)nOffsetY;

    uint8_t *pData = (uint8_t *)FOG_Alloc(0xC00, szTilecmpCpp, 281, 0);
    if (!pData)
    {
        FOG_DisplayAssert("ptData", szTilecmpCpp, 282);
        exit(-1);
    }
    memset(pData, 0, 0xC00);
    pComp->nDataLength = 0xC00;
    pComp->pData = pData;
    pComp->wFlags = 0x1001;
    SHARED_CopyImageRows(pData, pImage, 0x60, nImageStride, 0x20, 0x60);

    if (nOffsetX < pTile->nMinX)
        pTile->nMinX = nOffsetX;
    if (nOffsetX + 32 > pTile->nMaxX)
        pTile->nMaxX = nOffsetX + 32;
    if (nOffsetY > pTile->nMaxY)
        pTile->nMaxY = nOffsetY;
    if (nOffsetY - 32 < pTile->nMinY)
        pTile->nMinY = nOffsetY - 32;
}


//1.10f: D2CMP.0x6FDFECB0
//  #10069. Cuts a 160x80 RGB24 floor diamond (stride 480 bytes) into 25 pieces of 32x16 at gaFloorComponentOffsets' origins, each piece copied as
//  the diamond's 15 rows -- 4 pixels wide at the top, 4 more a row up to 32 at row 7, then back to 4 -- onto a square filled with the transparent
//  colour. A piece left all transparent is freed and adds nothing; any other becomes a component, flags 0x2001 (pixels, TILECOMP_GAMESQUARE), 1536
//  bytes, grid column nComp % 5 and row nComp / 5 (signed divide), its offsets read again from the table as 16 bits. The tile's bounds are then
//  SET, not grown, to the diamond's: 0..160 by -128..0. The component is built in a local (five dwords cleared) and copied whole into the
//  allocation before it is linked and its offsets stored over the copy. The original inlines the cutting of one piece and the linking of a
//  component here.
void __stdcall TILECMP_TileAddAddFloorTile(D2CmpTileBuilder *pTile, const uint8_t *pImage)
{
    if (!pTile)
    {
        FOG_DisplayAssert("ptTileAdd", szTilecmpCpp, 451);
        exit(-1);
    }
    if (!pImage)
    {
        FOG_DisplayAssert("ptImage", szTilecmpCpp, 452);
        exit(-1);
    }

    for (int32_t nComp = 0; nComp < 25; ++nComp)
    {
        const int32_t nX = gaFloorComponentOffsets[nComp][0];
        const int32_t nY = gaFloorComponentOffsets[nComp][1];
        D2CmpTileBuildComponent tComp;
        memset(&tComp, 0, sizeof(tComp));
        const uint32_t dwTransparent = pTile->dwTransparentRGB24;

        uint8_t *pSquare = (uint8_t *)FOG_Alloc(0x600, szTilecmpCpp, 340, 0);
        if (!pSquare)
        {
            FOG_DisplayAssert("ptSquare", szTilecmpCpp, 341);
            exit(-1);
        }
        memset(pSquare, 0, 0x600);
        for (uint8_t *pFill = pSquare; pFill < pSquare + 0x600; pFill += 3)
        {
            pFill[0] = (uint8_t)dwTransparent;
            pFill[1] = (uint8_t)(dwTransparent >> 8);
            pFill[2] = (uint8_t)(dwTransparent >> 16);
        }

        const uint8_t *pSrcRow = pImage + 3 * (nX + 160 * nY);
        uint8_t *pDstRow = pSquare;
        int32_t nStart = 42;
        uint32_t nBytes = 12;
        for (int32_t nRow = 0; nRow < 15; ++nRow)
        {
            memcpy(pDstRow + nStart, pSrcRow + nStart, nBytes);
            if (nRow >= 7)
            {
                nStart += 6;
                nBytes -= 12;
            }
            else
            {
                nStart -= 6;
                nBytes += 12;
            }
            pDstRow += 96;
            pSrcRow += 480;
        }

        int32_t nPixel = 0;
        for (; nPixel < 512; ++nPixel)
        {
            const uint8_t *pPixel = pSquare + 3 * nPixel;
            if (pPixel[0] != (uint8_t)dwTransparent || pPixel[1] != (uint8_t)(dwTransparent >> 8) || pPixel[2] != (uint8_t)(dwTransparent >> 16))
                break;
        }
        if (nPixel >= 512)
        {
            FOG_Free(pSquare, szTilecmpCpp, 390, 0);
        }
        else
        {
            tComp.wFlags |= 0x2001;
            tComp.nDataLength = 0x600;
            tComp.pData = pSquare;
        }

        tComp.nGridY = (uint8_t)(nComp / 5);
        tComp.nGridX = (uint8_t)(nComp % 5);
        if (tComp.wFlags & 0x1)
        {
            D2CmpTileBuildComponent *pComp = (D2CmpTileBuildComponent *)FOG_Alloc(sizeof(D2CmpTileBuildComponent), szTilecmpCpp, 423, 0);
            if (!pComp)
            {
                FOG_DisplayAssert("ptComp", szTilecmpCpp, 424);
                exit(-1);
            }
            memset(pComp, 0, sizeof(D2CmpTileBuildComponent));
            *pComp = tComp;
            pComp->pNext = pTile->pComponents;
            pTile->pComponents = pComp;
            ++pTile->nComponentCount;
            pComp->nOffsetX = (int16_t)gaFloorComponentOffsets[nComp][0];
            pComp->nOffsetY = (int16_t)gaFloorComponentOffsets[nComp][1];
        }
    }

    pTile->nMaxY = 0;
    pTile->nMinY = -128;
    pTile->nMinX = 0;
    pTile->nMaxX = 160;
}


//1.10f: D2CMP.0x6FDFEF60
//  #10070. Turns a list into a version-7 project: 0 and the project in *ppProject, or, for a list with no tile, NULL and 0x85200004 -- the one
//  error the function answers; anything else asserts. Each builder tile becomes an entry: its bounds become the width and the (negative) total
//  height, its fields and subtile flags are copied, and its components become an array whose pixels the entry TAKES -- each builder component's
//  data pointer is cleared, so the list can be destroyed after without freeing them. The list's head is its last tile added, so the array is in
//  the reverse order of the adds. The original inlines the conversion of one builder tile here.
//  D2MOO BUG: D2TileLibraryEntryStrc declares +0x28 as uint8_t dwTileFlags[4]; the binary copies 25 bytes there (`rep movsd` x6 + `movsb`,
//  0x6FDFF0D7..0x6FDFF0E3), the subtile flags.
int32_t __stdcall TILECMP_TileListCreateProject(D2CmpTileList *pTileList, D2CmpNormalizedTileProject **ppProject)
{
    if (!pTileList)
    {
        FOG_DisplayAssert("ptTileList", szTilecmpCpp, 548);
        exit(-1);
    }
    if (!pTileList->nNumTiles)
    {
        *ppProject = nullptr;
        return 0x85200004;
    }

    D2CmpNormalizedTileProject *pProject = (D2CmpNormalizedTileProject *)FOG_Alloc(sizeof(D2CmpNormalizedTileProject), szTilecmpCpp, 556, 0);
    if (!pProject)
    {
        FOG_DisplayAssert("ptProject", szTilecmpCpp, 557);
        exit(-1);
    }
    memset(pProject, 0, sizeof(D2CmpNormalizedTileProject));
    pProject->dwVersion = TILEPROJECT_VERSION;
    if (pTileList->b24Bit)
        pProject->dwFlags = TILEPROJECT_FLAG_24BIT;

    pProject->nNumTiles = pTileList->nNumTiles;
    const uint32_t nArrayBytes = (uint32_t)pProject->nNumTiles * sizeof(D2TileLibraryEntryStrc);
    pProject->ptTileArray = (D2TileLibraryEntryStrc *)FOG_Alloc(nArrayBytes, szTilecmpCpp, 568, 0);
    if (!pProject->ptTileArray)
    {
        FOG_DisplayAssert("ptProject->ptTileArray", szTilecmpCpp, 569);
        exit(-1);
    }
    memset(pProject->ptTileArray, 0, nArrayBytes);

    int32_t nTile = 0;
    for (D2CmpTileBuilder *pBuild = pTileList->pTiles; pBuild; pBuild = pBuild->pNext, ++nTile)
    {
        D2TileLibraryEntryStrc *pTile = &pProject->ptTileArray[nTile];
        pTile->nLightDirection = pBuild->nLightDirection;
        pTile->nRoofHeight = pBuild->nRoofHeight;
        pTile->nFlags = pBuild->nFlags;
        pTile->nWidth = pBuild->nMaxX - pBuild->nMinX;
        pTile->nTotalHeight = pBuild->nMinY - pBuild->nMaxY;
        pTile->nType = pBuild->nType;
        pTile->nStyle = pBuild->nStyle;
        pTile->nSequence = pBuild->nSequence;
        pTile->nRarity_Frame = pBuild->nRarity_Frame;
        pTile->transparentColorRGB24 = (int32_t)pBuild->dwTransparentRGB24;
        pTile->nCompCount = (int32_t)pBuild->nComponentCount;
        memcpy(pTile->nSubtileFlags, pBuild->nSubtileFlags, sizeof(pTile->nSubtileFlags));

        const uint32_t nCompBytes = (uint32_t)pTile->nCompCount * sizeof(D2CmpTileRuntimeComponent);
        if (nCompBytes)
        {
            pTile->ptCompArray = (D2CmpTileRuntimeComponent *)FOG_Alloc(nCompBytes, szTilecmpCpp, 513, 0);
            if (!pTile->ptCompArray)
            {
                FOG_DisplayAssert("ptTile->ptCompArray", szTilecmpCpp, 514);
                exit(-1);
            }
            memset(pTile->ptCompArray, 0, nCompBytes);
        }
        else
        {
            pTile->ptCompArray = nullptr;
        }

        int32_t nTileComp = 0;
        for (D2CmpTileBuildComponent *pBuildComp = pBuild->pComponents; pBuildComp; pBuildComp = pBuildComp->pNext, ++nTileComp)
        {
            D2CmpTileRuntimeComponent *pComp = &pTile->ptCompArray[nTileComp];
            pComp->nOffsetX = pBuildComp->nOffsetX;
            pComp->nOffsetY = pBuildComp->nOffsetY;
            pComp->nGridX = pBuildComp->nGridX;
            pComp->nGridY = pBuildComp->nGridY;
            pComp->wFlags = pBuildComp->wFlags;
            pComp->wDataBytesLow = pBuildComp->nDataLength;
            pComp->pData = pBuildComp->pData;
            pBuildComp->pData = nullptr;
        }
        if (nTileComp != pTile->nCompCount)
        {
            FOG_DisplayAssert("nTileComp == ptTile->nCompCount", szTilecmpCpp, 531);
            exit(-1);
        }
    }

    if (nTile != pProject->nNumTiles)
    {
        FOG_DisplayAssert("nTile == ptProject->nNumTiles", szTilecmpCpp, 583);
        exit(-1);
    }
    *ppProject = pProject;
    return 0;
}


//1.10f: D2CMP.0x6FDFF220
//  #10073. Frees a project, unless nKeepProject is set -- then nothing at all is done, the project not even tested. Its tiles leave the tile cache
//  first (TILECACHE_DetachProjectTiles); a project in one block (flag 0x4) is one free; one with TILEPROJECT_FEWBLOCKS is refused; any other has
//  every component's pixels, every component array, the tile array and the project freed in that order, the counts and pointers cleared as it
//  goes. Counts are signed and read again each pass; the tile array is freed with no test of it. Its one caller passes 0.
void __stdcall TILECMP_TileProjectDestroy(D2CmpNormalizedTileProject *pProject, int32_t nKeepProject)
{
    if (nKeepProject)
        return;

    if (!pProject)
    {
        FOG_DisplayAssert("ptProject", szTilecmpCpp, 602);
        exit(-1);
    }
    TILECACHE_DetachProjectTiles(pProject);

    if (pProject->dwFlags & 0x4)
    {
        FOG_Free(pProject, szTilecmpCpp, 610, 0);
        return;
    }
    if (pProject->dwFlags & TILEPROJECT_FEWBLOCKS)
    {
        FOG_DisplayAssert("!(ptProject->dwFlags & TILEPROJECT_FEWBLOCKS)", szTilecmpCpp, 615);
        exit(-1);
    }

    for (int32_t nTile = 0; nTile < pProject->nNumTiles; ++nTile)
    {
        D2TileLibraryEntryStrc *pTile = &pProject->ptTileArray[nTile];
        for (int32_t nComp = 0; nComp < pTile->nCompCount; ++nComp)
        {
            D2CmpTileRuntimeComponent *pComp = &pTile->ptCompArray[nComp];
            if (pComp->pData)
                FOG_Free(pComp->pData, szTilecmpCpp, 628, 0);
            pComp->wDataBytesLow = 0;
            pComp->pData = nullptr;
        }
        if (pTile->ptCompArray)
            FOG_Free(pTile->ptCompArray, szTilecmpCpp, 635, 0);
        pTile->ptCompArray = nullptr;
        pTile->nCompCount = 0;
    }

    FOG_Free(pProject->ptTileArray, szTilecmpCpp, 641, 0);
    pProject->ptTileArray = nullptr;
    pProject->nNumTiles = 0;
    FOG_Free(pProject, szTilecmpCpp, 645, 0);
}


//1.10f: D2CMP.0x6FDFF380
//  #10071. Writes a project into ONE allocation, returned through *pptData and *pnSize: the header, the tile array, then for each tile its
//  component array followed by its components' pixels. Every pointer becomes an offset: the header's tile array from the block's start (it is
//  0x114), a tile's component block from the block's start (+0x48) with its size (+0x4C) and its component array cleared (+0x54), a component's
//  pixels from the start of its tile's component block -- what TileCache.cpp seeks, reads and rebases. The size is the 16-bit lengths summed. The
//  SOURCE project is marked serialized (flag 0x2) and the copy gets 0x2 | 0x4.
void __stdcall TILECMP_TileProjectSerialize(D2CmpNormalizedTileProject *pProject, D2TileLibraryHeaderStrc **pptData, uint32_t *pnSize)
{
    if (!pProject)
    {
        FOG_DisplayAssert("ptProject", szTilecmpCpp, 729);
        exit(-1);
    }
    if (!pptData)
    {
        FOG_DisplayAssert("ptptData", szTilecmpCpp, 730);
        exit(-1);
    }
    if (!pnSize)
    {
        FOG_DisplayAssert("ptnSize", szTilecmpCpp, 731);
        exit(-1);
    }
    if (!pProject->nNumTiles)
    {
        FOG_DisplayAssert("ptProject->nNumTiles", szTilecmpCpp, 732);
        exit(-1);
    }

    uint32_t nSize = (uint32_t)pProject->nNumTiles * sizeof(D2TileLibraryEntryStrc) + sizeof(D2TileLibraryHeaderStrc);
    for (int32_t nTile = 0; nTile < pProject->nNumTiles; ++nTile)
    {
        const D2TileLibraryEntryStrc *pTile = &pProject->ptTileArray[nTile];
        nSize += (uint32_t)pTile->nCompCount * sizeof(D2CmpTileRuntimeComponent);
        for (int32_t nComp = 0; nComp < pTile->nCompCount; ++nComp)
            nSize += pTile->ptCompArray[nComp].wDataBytesLow;
    }
    if (!nSize)
    {
        FOG_DisplayAssert("nSize", szTilecmpCpp, 736);
        exit(-1);
    }

    uint8_t *pData = (uint8_t *)FOG_Alloc(nSize, szTilecmpCpp, 739, 0);
    if (!pData)
    {
        FOG_DisplayAssert("ptData", szTilecmpCpp, 740);
        exit(-1);
    }
    memset(pData, 0, nSize);
    *pptData = (D2TileLibraryHeaderStrc *)pData;
    *pnSize = nSize;

    D2TileLibraryHeaderStrc *pHeader = (D2TileLibraryHeaderStrc *)pData;
    memcpy(pHeader, pProject, sizeof(D2TileLibraryHeaderStrc));
    uint8_t *pTiles = pData + sizeof(D2TileLibraryHeaderStrc);
    const uint32_t nTileBytes = (uint32_t)pProject->nNumTiles * sizeof(D2TileLibraryEntryStrc);
    memcpy(pTiles, pProject->ptTileArray, nTileBytes);
    pHeader->dwTileStart_pFirst = (uint32_t)(pTiles - pData);

    uint8_t *pBlock = pTiles + nTileBytes;
    D2TileLibraryEntryStrc *pTileOut = (D2TileLibraryEntryStrc *)pTiles;
    for (int32_t nTile = 0; nTile < pProject->nNumTiles; ++nTile, ++pTileOut)
    {
        const D2TileLibraryEntryStrc *pTile = &pProject->ptTileArray[nTile];
        pTileOut->ptCompArray = nullptr;
        uint32_t nBlockBytes = (uint32_t)pTile->nCompCount * sizeof(D2CmpTileRuntimeComponent);
        memcpy(pBlock, pTile->ptCompArray, nBlockBytes);

        D2CmpTileRuntimeComponent *pCompOut = (D2CmpTileRuntimeComponent *)pBlock;
        uint8_t *pPixelsOut = pBlock + nBlockBytes;
        for (int32_t nComp = 0; nComp < pTile->nCompCount; ++nComp)
        {
            const D2CmpTileRuntimeComponent *pComp = &pTile->ptCompArray[nComp];
            memcpy(pPixelsOut, pComp->pData, pComp->wDataBytesLow);
            //  An offset where the field holds a pointer once loaded.
            pCompOut[nComp].pData = (uint8_t *)(uintptr_t)(pPixelsOut - pBlock);
            pPixelsOut += pComp->wDataBytesLow;
            nBlockBytes += pComp->wDataBytesLow;
        }
        pTileOut->dwComponentDataBytes = nBlockBytes;
        pTileOut->dwComponentFileOffset = (uint32_t)(pBlock - pData);
        pBlock += nBlockBytes;
    }

    pProject->dwFlags |= 0x2;
    pHeader->dwFlags |= 0x6;
}


//1.10f: D2CMP.0x6FDFF670
//  #10072. Turns a serialized block into a project IN PLACE: when the block is version 7 and marked, its tile array's offset becomes a pointer,
//  szFileName is copied into its name (unless NULL; no length check, 260 bytes), each tile gets the name, its component array a pointer and each
//  component's pixels a pointer, and the flags lose 0x2 and gain 0x4 (a project in one block). *ppProject is written on that path only. The
//  component pixels are rebased against the running start of each tile's component block, which advances by the component array and the 16-bit
//  lengths -- the inverse of TileProjectSerialize's layout.
//  BUG: the gate is dwFlags & 3 (`test byte ptr [ebx+4], 3`, 0x6FDFF699), where NormalizeProject gates the same work on & 2: a 24-bit project that
//  was never serialized passes, and its pointers are rebased as if they were offsets. UNREACHABLE: no stock module imports #10072. Reproduced.
void __stdcall TILECMP_TileProjectNormalize(D2TileLibraryHeaderStrc *pSerializedProject, D2CmpNormalizedTileProject **ppProject, const char *szFileName)
{
    if (pSerializedProject->dwVersion != TILEPROJECT_VERSION)
    {
        FOG_DisplayAssert("ptProject->dwVersion == TILEPROJECT_VERSION", szTilecmpCpp, 840);
        exit(-1);
    }
    if (!(pSerializedProject->dwFlags & 0x3))
        return;

    uint8_t *pBase = (uint8_t *)pSerializedProject;
    pSerializedProject->dwTileStart_pFirst += (uint32_t)(uintptr_t)pBase;
    if (szFileName)
        strcpy(pSerializedProject->szLibraryName, szFileName);

    uint8_t *pBlock = pBase + sizeof(D2TileLibraryHeaderStrc) + (uint32_t)pSerializedProject->nTiles * sizeof(D2TileLibraryEntryStrc);
    for (int32_t nTile = 0; nTile < pSerializedProject->nTiles; ++nTile)
    {
        D2TileLibraryEntryStrc *pTile = &((D2TileLibraryEntryStrc *)(uintptr_t)pSerializedProject->dwTileStart_pFirst)[nTile];
        pTile->szFileName = pSerializedProject->szLibraryName;
        pTile->ptCompArray = (D2CmpTileRuntimeComponent *)(pBase + pTile->dwComponentFileOffset);
        uint32_t nAdvance = (uint32_t)pTile->nCompCount * sizeof(D2CmpTileRuntimeComponent);
        for (int32_t nComp = 0; nComp < pTile->nCompCount; ++nComp)
        {
            D2CmpTileRuntimeComponent *pComp = &pTile->ptCompArray[nComp];
            pComp->pData = (uint8_t *)((uintptr_t)pComp->pData + (uintptr_t)pBlock);
            nAdvance += pComp->wDataBytesLow;
        }
        pBlock += nAdvance;
    }

    pSerializedProject->dwFlags = (pSerializedProject->dwFlags & ~0x2u) | 0x4;
    *ppProject = (D2CmpNormalizedTileProject *)pSerializedProject;
}


//1.10f: D2CMP.0x6FDFF780
//  The half of loading a tile file that TILEPROJECTS_LoadProjectRecord calls: the same version check and the same in-place conversion as #10072,
//  gated on 0x2 alone, with szFileName copied untested, and WITHOUT touching the components -- each tile's must still be NULL and so must its cache
//  entry: TileCache.cpp loads them from the file later. The name is stored before the asserts.
void __stdcall TILECMP_NormalizeProject(D2TileLibraryHeaderStrc *pSerializedProject, D2CmpNormalizedTileProject **ppProject, const char *szFileName)
{
    if (pSerializedProject->dwVersion != TILEPROJECT_VERSION)
    {
        FOG_DisplayAssert("ptProject->dwVersion == TILEPROJECT_VERSION", szTilecmpCpp, 895);
        exit(-1);
    }
    if (!(pSerializedProject->dwFlags & 0x2))
        return;

    pSerializedProject->dwTileStart_pFirst += (uint32_t)(uintptr_t)pSerializedProject;
    strcpy(pSerializedProject->szLibraryName, szFileName);

    for (int32_t nTile = 0; nTile < pSerializedProject->nTiles; ++nTile)
    {
        D2TileLibraryEntryStrc *pTile = &((D2TileLibraryEntryStrc *)(uintptr_t)pSerializedProject->dwTileStart_pFirst)[nTile];
        pTile->szFileName = pSerializedProject->szLibraryName;
        if (pTile->ptCompArray)
        {
            FOG_DisplayAssert("ptTile->ptCompArray == NULL", szTilecmpCpp, 930);
            exit(-1);
        }
        if (pTile->ptLRUCache)
        {
            FOG_DisplayAssert("ptTile->ptLRUCache == NULL", szTilecmpCpp, 931);
            exit(-1);
        }
    }

    pSerializedProject->dwFlags = (pSerializedProject->dwFlags & ~0x2u) | 0x4;
    *ppProject = (D2CmpNormalizedTileProject *)pSerializedProject;
}


//1.10f: D2CMP.0x6FDFF860
//  Builds a find-tiles hash of a project's tiles, which TILEPROJECTS_LoadProjectRecord keeps. The hash is allocated before the project is tested.
D2TileLibraryHashStrc *__fastcall TILECMP_BuildProjectHash(D2CmpNormalizedTileProject *pProject)
{
    D2TileLibraryHashStrc *pHash = FINDTILES_AllocHash();
    if (!pHash)
    {
        FOG_DisplayAssert("hFindTile", szTilecmpCpp, 953);
        exit(-1);
    }
    if (!pProject)
    {
        FOG_DisplayAssert("hProject", szTilecmpCpp, 954);
        exit(-1);
    }

    for (int32_t nTile = 0; nTile < pProject->nNumTiles; ++nTile)
        FINDTILES_AddTile(pHash, &pProject->ptTileArray[nTile]);
    return pHash;
}


//1.10f: D2CMP.0x6FDFF8F0
//  Turns the pixel offsets of a tile's components, just read from its file, into pointers, against the component array they were read into
//  (called by TILECACHE_TileValidate). The array is read once for the base and again for each component.
void __stdcall TILECMP_RebaseComponentDataPointers(D2TileLibraryEntryStrc *pTile)
{
    if (!pTile)
    {
        FOG_DisplayAssert("ptTile", szTilecmpCpp, 969);
        exit(-1);
    }

    const uintptr_t nBase = (uintptr_t)pTile->ptCompArray;
    for (int32_t nComp = 0; nComp < pTile->nCompCount; ++nComp)
        pTile->ptCompArray[nComp].pData = (uint8_t *)((uintptr_t)pTile->ptCompArray[nComp].pData + nBase);
}


//1.10f: D2CMP.0x6FDFF950
//  #10074. Calls pfnVisit with every RGB24 pixel of a 24-bit project (asserted), as one dword with the first byte lowest.
//  BUG: a length not a multiple of 3 is rounded up: the count is (nBytes - 1) / 3 + 1 (`dec ecx` / `mul` by 0AAAAAAABh / `shr edi, 1` / `inc edi`,
//  0x6FDFF9E4..0x6FDFF9F0, unsigned), so up to two bytes past a component's data are read into its last pixel. UNREACHABLE: no stock module
//  imports #10074. Reproduced.
void __stdcall TILECMP_TileProjectIteratePixels(D2CmpNormalizedTileProject *pProject, void (__stdcall *pfnVisit)(uint32_t dwRGB24))
{
    if (!pProject)
    {
        FOG_DisplayAssert("ptProject", szTilecmpCpp, 1002);
        exit(-1);
    }
    if (!(pProject->dwFlags & TILEPROJECT_FLAG_24BIT))
    {
        FOG_DisplayAssert("ptProject->dwFlags & TILEPROJECT_FLAG_24BIT", szTilecmpCpp, 1003);
        exit(-1);
    }

    for (int32_t nTile = 0; nTile < pProject->nNumTiles; ++nTile)
    {
        const D2TileLibraryEntryStrc *pTile = &pProject->ptTileArray[nTile];
        for (int32_t nComp = 0; nComp < pTile->nCompCount; ++nComp)
        {
            const D2CmpTileRuntimeComponent *pComp = &pTile->ptCompArray[nComp];
            const uint32_t nBytes = pComp->wDataBytesLow;
            const uint8_t *pPixel = pComp->pData;
            if (!nBytes)
                continue;
            uint32_t nPixels = (nBytes - 1) / 3 + 1;
            do
            {
                pfnVisit(((uint32_t)pPixel[2] << 16) + ((uint32_t)pPixel[1] << 8) + pPixel[0]);
                pPixel += 3;
            } while (--nPixels);
        }
    }
}


//1.10f: D2CMP.0x6FDFFA60
//  #10075. A new 8-bit project from a 24-bit one (asserted): the header copied and TILEPROJECT_FLAG_24BIT cleared, each tile copied and given a
//  new component array; each component with pixels is converted to indexed (SUBTILE_ConvertRGB24ToIndexed, pfnMapColor, the tile's transparent
//  colour) and then, unless every index is 0 -- then its pixels are freed and it is emptied --, compressed: a square one by
//  SUBTILE_EncodeSquareRle (flag 0x1000; the result asserted, and below 0x10000 bytes, signed compare), a floor piece in place by
//  SUBTILE_CompressIsometric (TILECOMP_GAMESQUARE, asserted). A component without pixels is emptied. The 24-bit project is left as it was. The
//  tile copy keeps the 24-bit tile's component count; the loop runs on the 24-bit tile's. The original inlines the conversion of one component here.
void __stdcall TILECMP_TileProjectConvert24To8(D2CmpNormalizedTileProject *pProject24, D2CmpNormalizedTileProject **ppProject8, uint8_t (__stdcall *pfnMapColor)(uint8_t nByte0, uint8_t nByte1, uint8_t nByte2))
{
    if (!pProject24)
    {
        FOG_DisplayAssert("ptProject24", szTilecmpCpp, 1103);
        exit(-1);
    }
    if (!(pProject24->dwFlags & TILEPROJECT_FLAG_24BIT))
    {
        FOG_DisplayAssert("ptProject24->dwFlags & TILEPROJECT_FLAG_24BIT", szTilecmpCpp, 1104);
        exit(-1);
    }

    D2CmpNormalizedTileProject *pProject8 = (D2CmpNormalizedTileProject *)FOG_Alloc(sizeof(D2CmpNormalizedTileProject), szTilecmpCpp, 1107, 0);
    if (!pProject8)
    {
        FOG_DisplayAssert("ptProject8", szTilecmpCpp, 1108);
        exit(-1);
    }
    memset(pProject8, 0, sizeof(D2CmpNormalizedTileProject));
    memcpy(pProject8, pProject24, sizeof(D2CmpNormalizedTileProject));
    pProject8->dwFlags &= ~TILEPROJECT_FLAG_24BIT;
    pProject8->ptTileArray = nullptr;

    const uint32_t nArrayBytes = (uint32_t)pProject8->nNumTiles * sizeof(D2TileLibraryEntryStrc);
    pProject8->ptTileArray = (D2TileLibraryEntryStrc *)FOG_Alloc(nArrayBytes, szTilecmpCpp, 1118, 0);
    if (!pProject8->ptTileArray)
    {
        FOG_DisplayAssert("ptProject8->ptTileArray", szTilecmpCpp, 1119);
        exit(-1);
    }
    memset(pProject8->ptTileArray, 0, nArrayBytes);

    for (int32_t nTile = 0; nTile < pProject24->nNumTiles; ++nTile)
    {
        D2TileLibraryEntryStrc *pTile8 = &pProject8->ptTileArray[nTile];
        const D2TileLibraryEntryStrc *pTile24 = &pProject24->ptTileArray[nTile];
        memcpy(pTile8, pTile24, sizeof(D2TileLibraryEntryStrc));
        pTile8->ptCompArray = nullptr;
        if (!pTile8->nCompCount)
            continue;

        const uint32_t nSize = (uint32_t)pTile8->nCompCount * sizeof(D2CmpTileRuntimeComponent);
        if (!nSize)
        {
            FOG_DisplayAssert("nSize", szTilecmpCpp, 1136);
            exit(-1);
        }
        pTile8->ptCompArray = (D2CmpTileRuntimeComponent *)FOG_Alloc(nSize, szTilecmpCpp, 1138, 0);
        if (!pTile8->ptCompArray)
        {
            FOG_DisplayAssert("ptTile8->ptCompArray", szTilecmpCpp, 1139);
            exit(-1);
        }
        memset(pTile8->ptCompArray, 0, nSize);

        for (int32_t nComp = 0; nComp < pTile24->nCompCount; ++nComp)
        {
            const D2CmpTileRuntimeComponent *pComp24 = &pTile24->ptCompArray[nComp];
            D2CmpTileRuntimeComponent *pComp8 = &pTile8->ptCompArray[nComp];

            if (!pComp24->pData)
            {
                pComp8->wFlags &= ~0x1;
                pComp8->wDataBytesLow = 0;
                pComp8->pData = nullptr;
                continue;
            }

            SUBTILE_ConvertRGB24ToIndexed(pComp8, pComp24, (uint32_t)pTile24->transparentColorRGB24, pfnMapColor);
            uint8_t *pPixels = pComp8->pData;
            const int32_t nBytes = pComp8->wDataBytesLow;
            int32_t nIndex = 0;
            for (; nIndex < nBytes; ++nIndex)
            {
                if (pPixels[nIndex])
                    break;
            }
            if (nIndex >= nBytes)
            {
                FOG_Free(pPixels, szTilecmpCpp, 1065, 0);
                pComp8->wFlags &= ~0x1;
                pComp8->wDataBytesLow = 0;
                pComp8->pData = nullptr;
            }
            else if (pComp8->wFlags & 0x1000)
            {
                uint32_t nEncodedBytes;
                uint8_t *pEncoded = SUBTILE_EncodeSquareRle(pPixels, &nEncodedBytes);
                if (!pEncoded)
                {
                    FOG_DisplayAssert("ptData", szTilecmpCpp, 1076);
                    exit(-1);
                }
                if ((int32_t)nEncodedBytes >= 0x10000)
                {
                    FOG_DisplayAssert("nSize < 0x00010000", szTilecmpCpp, 1077);
                    exit(-1);
                }
                FOG_Free(pComp8->pData, szTilecmpCpp, 1078, 0);
                pComp8->pData = pEncoded;
                pComp8->wDataBytesLow = (uint16_t)nEncodedBytes;
            }
            else
            {
                if (!(pComp8->wFlags & TILECOMP_GAMESQUARE))
                {
                    FOG_DisplayAssert("ptComp8->wFlags & TILECOMP_GAMESQUARE", szTilecmpCpp, 1083);
                    exit(-1);
                }
                SUBTILE_CompressIsometric(pComp8);
            }
        }
    }

    *ppProject8 = pProject8;
}
