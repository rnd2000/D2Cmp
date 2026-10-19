#include "D2CMP.h"

//  SubTile.cpp -- the pixel formats of a tile's components of D2CMP.dll 1.10 (image base 0x6FDF0000). A component is a D2CmpTileRuntimeComponent
//  (0x14 bytes: flags at +0x08, byte count at +0x0A, pixels at +0x10). TILECMP_TileProjectConvert24To8 makes an 8-bit component of a 24-bit
//  one (ConvertRGB24ToIndexed) and compresses it: a 32x32 square by run-length (EncodeSquareRle), a 32x15 floor diamond either packed whole when
//  it has no transparent pixel (PackOpaqueIsometric, 256 bytes) or run-length encoded (EncodeIsometricRle), CompressIsometric choosing.
//  The original's source file is D2CMP\SRC\SubTile.cpp (named by its asserts). Address range 0x6FDFE050..0x6FDFE5FA.
//  Only the WORD at +0x0A of a component (wDataBytesLow) is read and written; the high word of the byte count stays as the struct copy left it.
//  The original's own names for the components are ptComp8, ptComp24 and ptData (the asserts); ptData is kept as pData.

//  The path literal of the unit's Fog calls (__FILE__ in the original).
static const char szSubTileCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2CMP\\SRC\\SubTile.cpp";


//1.10f: D2CMP.0x6FDFE050
//  Run-length encodes a 32x32 square of 8-bit pixels, row by row: a run of zeros (COUNT_CountConsecutive, at most 127), then, unless the row
//  ended or only zeros are left in it, a run of non-zeros (COUNT_CountConsecutiveDiff, at most 127) as the two counts and the bytes. A zero run
//  of exactly 127 is written alone and the row goes on; each row ends with two zero bytes. Answers the block (FOG_Alloc 0xC20 bytes), and
//  its length in *pEncodedBytes. The row counter and the literal count are compared unsigned (`jb` 0x6FDFE116, `jbe` 0x6FDFE0DD); the same for
//  every count from 0 to 127. __fastcall: ECX and EDX only.
//  BUG: the allocation is not tested; the block is written at once (FOG_Alloc at 0x6FDFE06B). UNREACHABLE: the stock game never runs this
//  function.
uint8_t *__fastcall SUBTILE_EncodeSquareRle(const uint8_t *pPixels, uint32_t *pEncodedBytes)
{
    const uint8_t *pSrc = pPixels;
    const uint8_t *pNextRow = pPixels + 32;
    uint8_t *pBlock = (uint8_t *)FOG_Alloc(0xC20, szSubTileCpp, 28, 0);
    uint8_t *pDst = pBlock;
    const uint8_t *pRowEnd = pNextRow;
    uint32_t nRow = 0;

    do
    {
        int32_t bCountEqualsDist, bIsRemainingSame;
        int32_t nZeros = COUNT_CountConsecutive(0, pSrc, pRowEnd, 127, &bCountEqualsDist, &bIsRemainingSame);
        pSrc += nZeros;
        if (!bCountEqualsDist && !bIsRemainingSame)
        {
            if (nZeros == 127)
            {
                *pDst++ = 127;
                continue;
            }
            int32_t nLiterals = COUNT_CountConsecutiveDiff(pSrc, pRowEnd, 0, 127, &bCountEqualsDist, &bIsRemainingSame);
            *pDst++ = (uint8_t)nZeros;
            *pDst++ = (uint8_t)nLiterals;
            for (uint32_t nLeft = (uint32_t)nLiterals; nLeft; --nLeft)
                *pDst++ = *pSrc++;
            if (!bCountEqualsDist && !bIsRemainingSame)
                continue;
        }
        *pDst++ = 0;
        pSrc = pNextRow;
        pRowEnd += 32;
        pNextRow += 32;
        *pDst++ = 0;
        ++nRow;
    } while (nRow < 32);

    *pEncodedBytes = (uint32_t)(pDst - pBlock);
    return pBlock;
}


//1.10f: D2CMP.0x6FDFE130
//  Makes ptComp8 the 8-bit copy of the 24-bit ptComp24. Asserts both components and ptComp24's pixels; copies the component whole, clears its
//  bit 0, its pixels and its count; nSize, a third of the 24-bit byte count (a signed divide), is asserted non-zero and below 0x10000; a zeroed
//  block of nSize bytes gets bit 0 set and the count. Each pixel equal to the three low bytes of dwTransparentColor is index 0; any other is
//  the index pfnMapColor gives for its three bytes, which must not be 0 (assert "bColor", then exit). dwTransparentColor and pfnMapColor are
//  stack arguments.
//  DIFFERENCE: the original pushes the three bytes for pfnMapColor from registers whose upper bytes are leftovers, and reads only al of the
//  answer; ours passes clean bytes. A callback that reads them as bytes sees the same.
void __fastcall SUBTILE_ConvertRGB24ToIndexed(D2CmpTileRuntimeComponent *ptComp8, const D2CmpTileRuntimeComponent *ptComp24,
                                              uint32_t dwTransparentColor,
                                              uint8_t (__stdcall *pfnMapColor)(uint8_t nByte0, uint8_t nByte1,
                                                                                       uint8_t nByte2))
{
    if (!ptComp8)
    {
        FOG_DisplayAssert("ptComp8", szSubTileCpp, 130);
        exit(-1);
    }
    if (!ptComp24)
    {
        FOG_DisplayAssert("ptComp24", szSubTileCpp, 131);
        exit(-1);
    }
    if (!ptComp24->pData)
    {
        FOG_DisplayAssert("ptComp24->ptData", szSubTileCpp, 132);
        exit(-1);
    }

    *ptComp8 = *ptComp24;
    ptComp8->wFlags &= ~1;
    ptComp8->pData = nullptr;
    ptComp8->wDataBytesLow = 0;

    int32_t nSize = ptComp24->wDataBytesLow / 3;
    if (!nSize)
    {
        FOG_DisplayAssert("nSize", szSubTileCpp, 142);
        exit(-1);
    }
    if (nSize >= 0x10000)
    {
        FOG_DisplayAssert("nSize < 0x00010000", szSubTileCpp, 143);
        exit(-1);
    }
    ptComp8->pData = (uint8_t *)FOG_Alloc(nSize, szSubTileCpp, 144, 0);
    if (!ptComp8->pData)
    {
        FOG_DisplayAssert("ptComp8->ptData", szSubTileCpp, 145);
        exit(-1);
    }
    memset(ptComp8->pData, 0, nSize);
    ptComp8->wFlags |= 1;
    ptComp8->wDataBytesLow = (uint16_t)nSize;

    uint8_t *pOut = ptComp8->pData;
    const uint8_t *pIn = ptComp24->pData;
    for (int32_t i = 0; i < nSize; ++i)
    {
        uint8_t bColor = 0;
        if (pIn[0] != (uint8_t)dwTransparentColor || pIn[1] != (uint8_t)(dwTransparentColor >> 8) || pIn[2] != (uint8_t)(dwTransparentColor >> 16))
        {
            bColor = pfnMapColor(pIn[0], pIn[1], pIn[2]);
            if (!bColor)
            {
                FOG_DisplayAssert("bColor", szSubTileCpp, 161);
                exit(-1);
            }
        }
        *pOut++ = bColor;
        pIn += 3;
    }
}


//1.10f: D2CMP.0x6FDFE2F0
//  Compresses a floor piece's 8-bit pixels in place. The 15 rows of the diamond (4 pixels wide from x 14, two out and four wider a row to 32
//  wide at row 7, then back) are scanned for a 0: with one the pixels are run-length encoded (EncodeIsometricRle), with none packed
//  (PackOpaqueIsometric); then the old pixels are freed. The two arms share the original's one FOG_Free, and so does the C. Void.
void __fastcall SUBTILE_CompressIsometric(D2CmpTileRuntimeComponent *pComponent)
{
    uint8_t *pPixels = pComponent->pData;
    const uint8_t *pRow = pPixels;
    int32_t nLeft = 14;
    int32_t nWidth = 4;

    for (int32_t nRow = 0; nRow < 15; ++nRow)
    {
        for (int32_t nX = 0; nX < nWidth; ++nX)
        {
            if (!pRow[nLeft + nX])
            {
                SUBTILE_EncodeIsometricRle(pComponent, pPixels);
                goto Free;
            }
        }
        pRow += 32;
        if (nRow >= 7)
        {
            nLeft += 2;
            nWidth -= 4;
        }
        else
        {
            nLeft -= 2;
            nWidth += 4;
        }
    }
    SUBTILE_PackOpaqueIsometric(pComponent, pPixels);
Free:
    FOG_Free(pPixels, szSubTileCpp, 478, 0);
}


//1.10f: D2CMP.0x6FDFE370
//  Run-length encodes a floor piece's 15 rows of 32 pixels as EncodeSquareRle does a square's, into a zeroed scratch block of 0x5AF bytes; copies
//  the result into a zeroed block of its own length (asserted below 0x10000), sets bits 0 and 2 of the flags, the pixels and the count, and frees
//  the scratch block. The literal count is tested signed here (`jle` 0x6FDFE430), the row counter unsigned (`jb` 0x6FDFE46A).
void __fastcall SUBTILE_EncodeIsometricRle(D2CmpTileRuntimeComponent *pComponent, const uint8_t *pPixels)
{
    uint8_t *ptDestBuffer = (uint8_t *)FOG_Alloc(0x5AF, szSubTileCpp, 296, 0);
    if (!ptDestBuffer)
    {
        FOG_DisplayAssert("ptDestBuffer", szSubTileCpp, 297);
        exit(-1);
    }
    const uint8_t *pNextRow = pPixels + 32;
    memset(ptDestBuffer, 0, 0x5AF);

    const uint8_t *pSrc = pPixels;
    uint8_t *pDst = ptDestBuffer;
    const uint8_t *pRowEnd = pNextRow;
    uint32_t nRow = 0;
    do
    {
        int32_t bCountEqualsDist, bIsRemainingSame;
        int32_t nZeros = COUNT_CountConsecutive(0, pSrc, pRowEnd, 127, &bCountEqualsDist, &bIsRemainingSame);
        pSrc += nZeros;
        if (!bCountEqualsDist && !bIsRemainingSame)
        {
            if (nZeros == 127)
            {
                *pDst++ = 127;
                continue;
            }
            int32_t nLiterals = COUNT_CountConsecutiveDiff(pSrc, pRowEnd, 0, 127, &bCountEqualsDist, &bIsRemainingSame);
            *pDst++ = (uint8_t)nZeros;
            *pDst++ = (uint8_t)nLiterals;
            if (nLiterals > 0)
            {
                do
                    *pDst++ = *pSrc++;
                while (--nLiterals);
            }
            if (!bCountEqualsDist && !bIsRemainingSame)
                continue;
        }
        *pDst++ = 0;
        pSrc = pNextRow;
        pRowEnd += 32;
        pNextRow += 32;
        *pDst++ = 0;
        ++nRow;
    } while (nRow < 15);

    int32_t nSize = (int32_t)(pDst - ptDestBuffer);
    if (nSize >= 0x10000)
    {
        FOG_DisplayAssert("nSize < 0x00010000", szSubTileCpp, 358);
        exit(-1);
    }
    uint8_t *ptNewData = (uint8_t *)FOG_Alloc(nSize, szSubTileCpp, 361, 0);
    if (!ptNewData)
    {
        FOG_DisplayAssert("ptNewData", szSubTileCpp, 362);
        exit(-1);
    }
    memset(ptNewData, 0, nSize);
    memcpy(ptNewData, ptDestBuffer, nSize);
    pComponent->wFlags |= 5;
    pComponent->pData = ptNewData;
    pComponent->wDataBytesLow = (uint16_t)nSize;
    FOG_Free(ptDestBuffer, szSubTileCpp, 374, 0);
}


//1.10f: D2CMP.0x6FDFE530
//  Packs a floor piece with no transparent pixel: the diamond's 15 rows (as CompressIsometric scans them), 4 + 8 + ... + 32 + ... + 4 = 256 bytes,
//  one after the other into a zeroed block of 0x100. The whole flags WORD is SET to 1 (`mov word ptr [eax+8], 1`, 0x6FDFE5E7), where the
//  converter and the encoder change only its low byte; then the count 0x100 and the pixels. The row copy is memcpy (the blocks do not overlap).
void __fastcall SUBTILE_PackOpaqueIsometric(D2CmpTileRuntimeComponent *pComponent, const uint8_t *pPixels)
{
    uint8_t *ptSquished = (uint8_t *)FOG_Alloc(0x100, szSubTileCpp, 389, 0);
    if (!ptSquished)
    {
        FOG_DisplayAssert("ptSquished", szSubTileCpp, 390);
        exit(-1);
    }
    memset(ptSquished, 0, 0x100);

    uint8_t *pDst = ptSquished;
    int32_t nLeft = 14;
    uint32_t nWidth = 4;
    for (int32_t nRow = 0; nRow < 15; ++nRow)
    {
        memcpy(pDst, pPixels + nLeft, nWidth);
        pDst += nWidth;
        pPixels += 32;
        if (nRow >= 7)
        {
            nLeft += 2;
            nWidth -= 4;
        }
        else
        {
            nLeft -= 2;
            nWidth += 4;
        }
    }
    pComponent->wFlags = 1;
    pComponent->wDataBytesLow = 0x100;
    pComponent->pData = ptSquished;
}
