#include "D2CMP.h"

//  FastCmp.cpp -- the DC6 cel format of D2CMP.dll 1.10: its run-length encoder, its in-place palette transform, its 24-to-8-bit converter and its
//  drawers (the cel blit with its twelve table combinations, and the shadow blit), with the lens warp (#10048) and the switch to the generic decoder
//  (#10049). The original source file is D2CMP\SRC\FastCmp.cpp (the asserts in this unit name it). Address range 0x6FDF5C10..0x6FDF8291.
//  A DC6 frame is its 32-byte D2CmpRleCellHeader with the stream inline after it, row by row, one control byte at a time: 0x80 ends a row,
//  0x81..0xFF skip (control & 0x7F) transparent pixels, and 0x00..0x7F are a run of that many literal pixels, which follow it. A control is read as
//  a signed byte, so a 0x00 control is a literal run of none.
//  The original has twelve leaf drawers (Map, PreMap, Blend, MapBlend, PreMapMap and PreMapBlend, each clipped and unclipped), one body each with
//  only the pixel's expression different; here they are twelve one-line functions over the two shared templates FastCmpDrawRows and
//  FastCmpDrawRowsClipped. Every function but FASTCMP_ToggleCelDrawMode is declared void: what the original leaves in eax is read by no caller.

//  The path of the original's source file, as its asserts and traces spell it (__FILE__ in the original).
static const char szFastCmpCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2CMP\\SRC\\FastCmp.cpp";

static_assert(offsetof(D2CmpCellBuilder, dwPixelFormatFlags) == 0x18, "mov al, [ecx+18h] (0x6FDF5C13)");
static_assert(offsetof(D2CmpCellBuildEntry, nOrientation) == 0x08, "mov edx, [ebx+8] (0x6FDF5C6C)");
static_assert(offsetof(D2CmpCellBuildEntry, nOffsetY) == 0x1C, "mov eax, [ebx+1Ch] (0x6FDF5CC1)");
static_assert(offsetof(D2CmpRleCellHeader, dwOrientationFlags) == 0x00, "test byte ptr [esi], 1 (0x6FDF7B20)");
static_assert(offsetof(D2CmpRleCellHeader, dwWidth) == 0x04, "mov ebx, [esi+4] (0x6FDF7AB5)");
static_assert(offsetof(D2CmpRleCellHeader, dwHeight) == 0x08, "mov edi, [esi+8] (0x6FDF7AE2)");
static_assert(offsetof(D2CmpRleCellHeader, nXOffset) == 0x0C && offsetof(D2CmpRleCellHeader, nYOffset) == 0x10, "mov [ebp+0Ch], edx / mov [ebp+10h], eax (0x6FDF5CBE, 0x6FDF5CC4)");
static_assert(offsetof(D2CmpRleCellHeader, unknown14) == 0x14 && offsetof(D2CmpRleCellHeader, dwNextBlockOrCellNode) == 0x18, "mov [ebp+14h], esi / mov [ebp+18h], esi (0x6FDF5CC7, 0x6FDF5CCA)");
static_assert(offsetof(D2CmpRleCellHeader, dwLength) == 0x1C, "mov [ebp+1Ch], ecx (0x6FDF5CD1)");
static_assert(offsetof(D2CmpBuiltCellFile, dwFlags) == 0x04, "mov al, [ecx+4] (0x6FDF5EA3)");
static_assert(offsetof(D2CmpBuiltCellFile, nDirections) == 0x10 && offsetof(D2CmpBuiltCellFile, nFramesPerDirection) == 0x14, "mov eax, [ecx+10h] / imul eax, [ecx+14h] (0x6FDF5ED0, 0x6FDF5EDB)");
static_assert(offsetof(D2CmpRgb24FrameHeader, nOrientation) == 0x00 && offsetof(D2CmpRgb24FrameHeader, nOffsetX) == 0x0C &&
                  offsetof(D2CmpRgb24FrameHeader, nOffsetY) == 0x10,
              "mov edx, [eax] / mov ecx, [eax+0Ch] / mov eax, [edx+10h] (0x6FDF5F8A, 0x6FDF5FE8, 0x6FDF5FF2)");


//1.10f: D2CMP.0x6FDF5C10
//  Builds the new cel file's DC6 frames from the builder's entries; it is RAW_EncodeRGB24CellFrames with the pixels run-length encoded. The
//  builder must not be RGB24 (bit 2 of its pixel format flags; the assert exits). The frames per direction are its frame count over its
//  direction count (unsigned). For each entry of its Storm list, in order, the pixels go through FASTCMP_EncodeDC6RlePixels, then a frame of
//  32 + the encoded bytes + 3 is allocated (three bytes more than anything writes): its header takes the entry's orientation, width, height and
//  offsets, 0 for +14h and +18h, and the encoded byte count, and its body is a copy of the encoded bytes; the encoder's buffer is freed. The
//  frame's slot in the file's table is the one RAW_EncodeRGB24CellFrames uses: the game direction is 1 for a builder of at most one direction,
//  else the frame's direction index times 64 over the direction count (both divisions unsigned); CELCMP_CelGameDirToFileDir (#10018) turns it
//  into the file's direction, and the frame goes to that row at its index within the direction.
//  The list is Storm's: its head is the terminator's next link, taken only when positive as an int, and the walk goes on while the next link is
//  positive.
//  BUG: FOG_Alloc's answer for the frame is used untested (0x6FDF5CA3, then the stores from 0x6FDF5CAC). UNREACHABLE: the function belongs to the
//  cel compressor's tool chain, which no module of the stock game reaches.
//  BUG: with fewer frames than directions the frames per direction are 0, and the first entry divides by zero (at 0x6FDF5D11 when there is more than
//  one direction, at 0x6FDF5D35 always): #DE in the original, written here as the bytes divide. UNREACHABLE: the function belongs to the cel
//  compressor's tool chain, which no module of the stock game reaches.
void __fastcall FASTCMP_EncodeIndexedCellFrames(D2CmpCellBuilder *pBuilder, D2CmpBuiltCellFile *pOutput)
{
    if (pBuilder->dwPixelFormatFlags & 4)
    {
        FOG_DisplayAssert("!( pList->dwFlags & CELFILE_24BIT )", szFastCmpCpp, 114);
        exit(-1);
    }

    const uint32_t dwFramesPerDirection = pBuilder->nFrameCount / pBuilder->nDirectionCount;
    uint32_t dwFrameIndex = 0;

    D2CmpCellBuildEntry *pEntry = (intptr_t)pBuilder->frames.terminator.pNextOrSentinel <= 0
                                      ? nullptr
                                      : (D2CmpCellBuildEntry *)pBuilder->frames.terminator.pNextOrSentinel;
    while ((intptr_t)pEntry > 0)
    {
        uint8_t *pEncoded;
        uint32_t dwEncodedBytes;
        //  The original pushes the orientation as a whole dword; the encoder reads a byte, hence the cast.
        FASTCMP_EncodeDC6RlePixels(pEntry->pPixels, pEntry->nPixelWidth, pEntry->nPixelHeight, (uint8_t)pEntry->nOrientation, &pEncoded, &dwEncodedBytes);

        D2CmpRleCellHeader *pFrame = (D2CmpRleCellHeader *)FOG_Alloc(dwEncodedBytes + 35, szFastCmpCpp, 130, 0);
        pFrame->dwOrientationFlags = pEntry->nOrientation;
        pFrame->dwWidth = pEntry->nPixelWidth;
        pFrame->dwHeight = pEntry->nPixelHeight;
        pFrame->nXOffset = pEntry->nOffsetX;
        pFrame->nYOffset = pEntry->nOffsetY;
        pFrame->unknown14 = 0;
        pFrame->dwNextBlockOrCellNode = 0;
        pFrame->dwLength = dwEncodedBytes;
        memcpy(pFrame + 1, pEncoded, dwEncodedBytes);
        FOG_Free(pEncoded, szFastCmpCpp, 141, 0);

        const uint32_t nDirectionCount = pBuilder->nDirectionCount;
        const uint32_t nDirection64 = nDirectionCount <= 1 ? 1 : ((dwFrameIndex / dwFramesPerDirection) << 6) / nDirectionCount;
        const uint32_t nFileDirection = CELCMP_CelGameDirToFileDir(nDirection64, nDirectionCount);
        pOutput->pFrames[nFileDirection * dwFramesPerDirection + dwFrameIndex % dwFramesPerDirection] = pFrame;

        pEntry = (D2CmpCellBuildEntry *)pEntry->link.pNextOrSentinel;
        ++dwFrameIndex;
    }
}


//1.10f: D2CMP.0x6FDF5D60
//  Encodes an indexed image as a DC6 stream into a buffer of height * (2 * width + 1) bytes from FOG_Alloc (the worst case: a control byte per
//  pixel and one per row), handed back with its length through the two out pointers. With bit 0 of the orientation set the rows are encoded top
//  down, clear from the last row up. Index 0 is transparent. Per row: a run of zeros, at most 127, becomes 0x80 | its count
//  (COUNT_CountConsecutiveFirst) unless it reaches the row's end or nothing but zeros follows it, in which case the row ends there with 0x80 and
//  the zeros are not written; a run of other indexes, at most 127, becomes its count and the indexes (COUNT_CountConsecutiveDiff, value 0), and
//  ends the row with 0x80 when it reached the row's end. The row counter is compared unsigned. A height of 0 encodes nothing and hands back the
//  buffer and 0.
//  The orientation is a byte: the original reads only the low byte of its stack slot and then reuses the slot for the next row's pointer. Both
//  callers push a whole dword.
//  BUG: FOG_Alloc's answer is used untested (0x6FDF5D84, then the stores from 0x6FDF5DEE), and the buffer size is computed in 32 bits and wraps for
//  a large frame. UNREACHABLE: the function belongs to the cel compressor's tool chain, which no module of the stock game reaches.
void __fastcall FASTCMP_EncodeDC6RlePixels(const uint8_t *pPixels, int32_t nPixelWidth, uint32_t nPixelHeight, uint8_t nOrientationFlags, uint8_t **ppEncoded, uint32_t *pnEncodedBytes)
{
    uint8_t *pEncoded = (uint8_t *)FOG_Alloc(nPixelHeight * (2 * nPixelWidth + 1), szFastCmpCpp, 37, 0);
    uint8_t *pOut = pEncoded;
    uint32_t dwRow = 0;

    int32_t nRowStep;
    if (nOrientationFlags & 1)
    {
        nRowStep = nPixelWidth;
    }
    else
    {
        pPixels += (nPixelHeight - 1) * nPixelWidth;
        nRowStep = -nPixelWidth;
    }
    const uint8_t *pNextRow = pPixels + nRowStep;
    const uint8_t *pRowEnd = pPixels + nPixelWidth;

    while (dwRow < nPixelHeight)
    {
        int32_t bReachedEnd;
        int32_t bRemainingSame;
        if (*pPixels == 0)
        {
            const int32_t nCount = COUNT_CountConsecutiveFirst(pPixels, pRowEnd, 127, &bReachedEnd, &bRemainingSame);
            if (!bRemainingSame && !bReachedEnd)
            {
                *pOut++ = (uint8_t)(nCount | 0x80);
                pPixels += nCount;
                continue;
            }
        }
        else
        {
            int32_t nCount = COUNT_CountConsecutiveDiff(pPixels, pRowEnd, 0, 127, &bReachedEnd, &bRemainingSame);
            *pOut++ = (uint8_t)nCount;
            for (; nCount != 0; --nCount)
            {
                *pOut++ = *pPixels++;
            }
            if (!bReachedEnd)
            {
                continue;
            }
        }

        *pOut++ = 0x80;
        pPixels = pNextRow;
        pNextRow += nRowStep;
        pRowEnd += nRowStep;
        ++dwRow;
    }

    *ppEncoded = pEncoded;
    *pnEncodedBytes = (uint32_t)(pOut - pEncoded);
}


//1.10f: D2CMP.0x6FDF5E60
//  Maps a DC6 frame's literal pixels through a 256-entry palette map, in place; the control bytes are kept. The rows are counted down from the
//  frame's height.
//  BUG: the row count is decremented before it is tested (`dec` / `jnz`, 0x6FDF5E76), so a frame of height 0 is not 0 rows: the first row's end
//  takes the count to 0xFFFFFFFF and the walk runs on past the frame's stream, mapping whatever it reads as literal runs, until it has counted 2^32
//  row ends. FASTCMP_EncodeDC6RlePixels makes such a frame, with no stream at all, for a height of 0. UNREACHABLE: the function belongs to the cel
//  compressor's tool chain, which no module of the stock game reaches.
void __fastcall FASTCMP_TransformDC6PaletteInPlace(D2CmpRleCellHeader *pCell, const uint8_t *pPaletteMap)
{
    uint32_t dwRows = pCell->dwHeight;
    uint8_t *pRle = (uint8_t *)(pCell + 1);
    for (;;)
    {
        const int32_t nCommand = (int8_t)*pRle++;
        if (nCommand >= 0)
        {
            for (int32_t nCount = nCommand; nCount > 0; --nCount, ++pRle)
            {
                *pRle = pPaletteMap[*pRle];
            }
        }
        else if (!(nCommand & 0x7F) && --dwRows == 0)
        {
            return;
        }
    }
}


//1.10f: D2CMP.0x6FDF5EA0
//  Converts an RGB24 cel file's frames into DC6 frames of a new cel file. The source must be RGB24 (bit 2 of its flags; the assert exits). For each
//  of its directions * frames-per-direction frames (recomputed every pass, compared unsigned): a buffer of width * height indexes is allocated; each
//  pixel whose three bytes equal the low three bytes of dwTransparentColor (compared one byte at a time) becomes 0, every other one
//  pfMapRgbToIndex's answer, which must not be 0 (the "bTemp" assert exits); the indexes go through FASTCMP_EncodeDC6RlePixels and their buffer is
//  freed; a frame of 32 + the encoded bytes + 3 is allocated with the source frame's orientation, width, height and offsets, 0 at +14h and +18h,
//  the encoded byte count and the encoded bytes; their buffer is freed; and the frame is stored in the new file's table at the same index. Every
//  field of the source frame is read through its table slot again, as written here.
//  pfMapRgbToIndex is __stdcall and takes and answers bytes: the original pushes whole registers of which only the low byte was written, and tests
//  and stores the answer as AL. The source is typed D2CmpBuiltCellFile like the other formats' files but read as D2CmpRgb24RuntimeFilePrefix,
//  whose frames are RGB24 frame headers; both have the same 24 bytes before the frame table.
//  BUG: both FOG_Alloc answers are used untested (0x6FDF5F10, 0x6FDF5FC6), and pfMapRgbToIndex is called without a NULL test. UNREACHABLE: the
//  function belongs to the cel compressor's tool chain, which no module of the stock game reaches.
void __fastcall FASTCMP_ConvertRGB24CellFramesToIndexed(const D2CmpBuiltCellFile *pSrc, D2CmpBuiltCellFile *pDst,
                                                        uint8_t (__stdcall *pfMapRgbToIndex)(uint8_t nRed,
                                                                                                      uint8_t nGreen,
                                                                                                      uint8_t nBlue),
                                                        uint32_t dwTransparentColor)
{
    if (!(pSrc->dwFlags & 4))
    {
        FOG_DisplayAssert("pSrc->dwFlags & CELFILE_24BIT", szFastCmpCpp, 206);
        exit(-1);
    }

    const D2CmpRgb24RuntimeFilePrefix *pRgbSrc = (const D2CmpRgb24RuntimeFilePrefix *)pSrc;
    const uint8_t *pKey = (const uint8_t *)&dwTransparentColor;
    for (uint32_t dwFrame = 0; dwFrame < pSrc->nDirections * pSrc->nFramesPerDirection; ++dwFrame)
    {
        const uint32_t dwPixels = pRgbSrc->pFrames[dwFrame]->dwHeight * pRgbSrc->pFrames[dwFrame]->dwWidth;
        uint8_t *pIndexed = (uint8_t *)FOG_Alloc(dwPixels, szFastCmpCpp, 212, 0);
        const uint8_t *pRgb = (const uint8_t *)(pRgbSrc->pFrames[dwFrame] + 1);
        uint8_t *pOut = pIndexed;
        for (uint32_t dwPixel = 0; dwPixel < dwPixels; ++dwPixel)
        {
            if (pRgb[0] == pKey[0] && pRgb[1] == pKey[1] && pRgb[2] == pKey[2])
            {
                *pOut = 0;
            }
            else
            {
                const uint8_t nIndex = pfMapRgbToIndex(pRgb[0], pRgb[1], pRgb[2]);
                if (!nIndex)
                {
                    FOG_DisplayAssert("bTemp", szFastCmpCpp, 224);
                    exit(-1);
                }
                *pOut = nIndex;
            }
            ++pOut;
            pRgb += 3;
        }

        uint8_t *pEncoded;
        uint32_t dwEncodedBytes;
        //  The original pushes the orientation as a whole dword; the encoder reads a byte, hence the cast.
        FASTCMP_EncodeDC6RlePixels(pIndexed, pRgbSrc->pFrames[dwFrame]->dwWidth, pRgbSrc->pFrames[dwFrame]->dwHeight, (uint8_t)pRgbSrc->pFrames[dwFrame]->nOrientation, &pEncoded, &dwEncodedBytes);
        FOG_Free(pIndexed, szFastCmpCpp, 240, 0);

        D2CmpRleCellHeader *pFrame = (D2CmpRleCellHeader *)FOG_Alloc(dwEncodedBytes + 35, szFastCmpCpp, 242, 0);
        pFrame->dwOrientationFlags = pRgbSrc->pFrames[dwFrame]->nOrientation;
        pFrame->dwWidth = pRgbSrc->pFrames[dwFrame]->dwWidth;
        pFrame->dwHeight = pRgbSrc->pFrames[dwFrame]->dwHeight;
        pFrame->nXOffset = pRgbSrc->pFrames[dwFrame]->nOffsetX;
        pFrame->nYOffset = pRgbSrc->pFrames[dwFrame]->nOffsetY;
        pFrame->unknown14 = 0;
        pFrame->dwNextBlockOrCellNode = 0;
        pFrame->dwLength = dwEncodedBytes;
        memcpy(pFrame + 1, pEncoded, dwEncodedBytes);
        FOG_Free(pEncoded, szFastCmpCpp, 253, 0);

        pDst->pFrames[dwFrame] = pFrame;
    }
}


//1.10f: D2CMP.0x6FDF6080
//  The plain copy, clipped: each literal run's pixels from max(nClipLeft - x, 0) to min(nClipRight - x, the run) are copied when that span is not
//  empty; the whole run still advances the stream, the surface and x. x starts each row at nX. The clip edges are compared signed.
//  BUG: the rows are counted down and the count is decremented before it is tested (`dec` / `jz`), so nLinesToDraw 0 is not 0 rows but 2^32, and
//  FASTCMP_DrawDC6 does not test it. The same holds in every drawer of the unit. NOT ESTABLISHED: whether a caller passes 0 lines is not known.
void __fastcall FASTCMP_DC6CopyClipped(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight)
{
    const int32_t nRowStartX = nX;
    uint8_t *pNextRow = pDst + nRowStep;
    for (;;)
    {
        const int32_t nCommand = (int8_t)*pRle++;
        if (nCommand >= 0)
        {
            int32_t nLeftSkip = nClipLeft - nX;
            if (nLeftSkip < 0)
            {
                nLeftSkip = 0;
            }
            int32_t nRightEnd = nClipRight - nX;
            if (nRightEnd > nCommand)
            {
                nRightEnd = nCommand;
            }
            const int32_t nCount = nRightEnd - nLeftSkip;
            if (nCount > 0)
            {
                memcpy(pDst + nLeftSkip, pRle + nLeftSkip, nCount);
            }
            nX += nCommand;
            pRle += nCommand;
            pDst += nCommand;
        }
        else if (nCommand & 0x7F)
        {
            nX += nCommand & 0x7F;
            pDst += nCommand & 0x7F;
        }
        else
        {
            if (--nLinesToDraw == 0)
            {
                return;
            }
            nX = nRowStartX;
            pDst = pNextRow;
            pNextRow += nRowStep;
        }
    }
}


//  A helper of ours, not a function of the original: the body the twelve DC6 leaf drawers share (they differ only in the pixel's expression).
//  It draws one literal run of nCount pixels in the order the original writes it: the nCount & 7 remainder first, from its last pixel down to its
//  first, then the nCount >> 3 blocks of 8 in order, each from [7] down to [0]. The shift is `sar`, on a count of 0..127.
template <typename PixelOp>
static void FastCmpDrawRun(uint8_t *pDst, const uint8_t *pRle, int32_t nCount, PixelOp fnPixel)
{
    const int32_t nRemainder = nCount & 7;
    for (int32_t i = nRemainder - 1; i >= 0; --i)
    {
        pDst[i] = fnPixel(pDst[i], pRle[i]);
    }
    pDst += nRemainder;
    pRle += nRemainder;
    for (int32_t nBlocks = nCount >> 3; nBlocks != 0; --nBlocks)
    {
        for (int32_t i = 7; i >= 0; --i)
        {
            pDst[i] = fnPixel(pDst[i], pRle[i]);
        }
        pDst += 8;
        pRle += 8;
    }
}


//  A helper of ours, not a function of the original: the unclipped leaves' body. Every literal run is drawn whole; a row ends on 0x80 and the
//  next starts nRowStep from the last one's start.
//  BUG: the rows are counted down (the count is decremented before it is tested), so nLinesToDraw 0 runs 2^32 rows. NOT ESTABLISHED: whether a
//  caller passes 0 lines is not known.
template <typename PixelOp>
static void FastCmpDrawRows(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, PixelOp fnPixel)
{
    uint8_t *pNextRow = pDst + nRowStep;
    for (;;)
    {
        const int32_t nCommand = (int8_t)*pRle++;
        if (nCommand >= 0)
        {
            FastCmpDrawRun(pDst, pRle, nCommand, fnPixel);
            pDst += nCommand;
            pRle += nCommand;
        }
        else if (nCommand & 0x7F)
        {
            pDst += nCommand & 0x7F;
        }
        else
        {
            if (--nLinesToDraw == 0)
            {
                return;
            }
            pDst = pNextRow;
            pNextRow += nRowStep;
        }
    }
}


//  A helper of ours, not a function of the original: the clipped leaves' body, with FASTCMP_DC6CopyClipped's clip: the span max(nClipLeft - x, 0)
//  .. min(nClipRight - x, the run) of each run is drawn when it is not empty, and the whole run advances the stream, the surface and x.
//  BUG: the rows are counted down (the count is decremented before it is tested), so nLinesToDraw 0 runs 2^32 rows. NOT ESTABLISHED: whether a
//  caller passes 0 lines is not known.
template <typename PixelOp>
static void FastCmpDrawRowsClipped(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, PixelOp fnPixel)
{
    const int32_t nRowStartX = nX;
    uint8_t *pNextRow = pDst + nRowStep;
    for (;;)
    {
        const int32_t nCommand = (int8_t)*pRle++;
        if (nCommand >= 0)
        {
            int32_t nLeftSkip = nClipLeft - nX;
            if (nLeftSkip < 0)
            {
                nLeftSkip = 0;
            }
            int32_t nRightEnd = nClipRight - nX;
            if (nRightEnd > nCommand)
            {
                nRightEnd = nCommand;
            }
            const int32_t nCount = nRightEnd - nLeftSkip;
            if (nCount > 0)
            {
                FastCmpDrawRun(pDst + nLeftSkip, pRle + nLeftSkip, nCount, fnPixel);
            }
            nX += nCommand;
            pRle += nCommand;
            pDst += nCommand;
        }
        else if (nCommand & 0x7F)
        {
            nX += nCommand & 0x7F;
            pDst += nCommand & 0x7F;
        }
        else
        {
            if (--nLinesToDraw == 0)
            {
                return;
            }
            nX = nRowStartX;
            pDst = pNextRow;
            pNextRow += nRowStep;
        }
    }
}


//1.10f: D2CMP.0x6FDF6130
//  Draws each pixel blended over the surface: pBlendTable[(surface << 8) + pPaletteMap[index]].
void __fastcall FASTCMP_DC6MapBlend(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pPaletteMap, const uint8_t *pBlendTable)
{
    FastCmpDrawRows(pRle, pDst, nLinesToDraw, nRowStep, [=](uint8_t nSurface, uint8_t nIndex) { return pBlendTable[(nSurface << 8) + pPaletteMap[nIndex]]; });
}


//1.10f: D2CMP.0x6FDF6390
//  As FASTCMP_DC6MapBlend, clipped to nClipLeft..nClipRight.
void __fastcall FASTCMP_DC6MapBlendClipped(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pPaletteMap, const uint8_t *pBlendTable)
{
    FastCmpDrawRowsClipped(pRle, pDst, nLinesToDraw, nRowStep, nX, nClipLeft, nClipRight, [=](uint8_t nSurface, uint8_t nIndex) { return pBlendTable[(nSurface << 8) + pPaletteMap[nIndex]]; });
}


//1.10f: D2CMP.0x6FDF6690
//  Draws each pixel through both maps: pPaletteMap[pPreMap[index]].
void __fastcall FASTCMP_DC6PreMapMap(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pPaletteMap, const uint8_t *pPreMap)
{
    FastCmpDrawRows(pRle, pDst, nLinesToDraw, nRowStep, [=](uint8_t, uint8_t nIndex) { return pPaletteMap[pPreMap[nIndex]]; });
}


//1.10f: D2CMP.0x6FDF6830
//  As FASTCMP_DC6PreMapMap, clipped to nClipLeft..nClipRight.
void __fastcall FASTCMP_DC6PreMapMapClipped(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pPaletteMap, const uint8_t *pPreMap)
{
    FastCmpDrawRowsClipped(pRle, pDst, nLinesToDraw, nRowStep, nX, nClipLeft, nClipRight, [=](uint8_t, uint8_t nIndex) { return pPaletteMap[pPreMap[nIndex]]; });
}


//1.10f: D2CMP.0x6FDF6A60
//  Draws each pixel through the palette map: pPaletteMap[index].
void __fastcall FASTCMP_DC6Map(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pPaletteMap)
{
    FastCmpDrawRows(pRle, pDst, nLinesToDraw, nRowStep, [=](uint8_t, uint8_t nIndex) { return pPaletteMap[nIndex]; });
}


//1.10f: D2CMP.0x6FDF6BA0
//  As FASTCMP_DC6Map, clipped to nClipLeft..nClipRight.
void __fastcall FASTCMP_DC6MapClipped(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pPaletteMap)
{
    FastCmpDrawRowsClipped(pRle, pDst, nLinesToDraw, nRowStep, nX, nClipLeft, nClipRight, [=](uint8_t, uint8_t nIndex) { return pPaletteMap[nIndex]; });
}


//1.10f: D2CMP.0x6FDF6D70
//  Draws each pixel blended over the surface through the pre-map: pBlendTable[(surface << 8) + pPreMap[index]].
void __fastcall FASTCMP_DC6PreMapBlend(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pPreMap, const uint8_t *pBlendTable)
{
    FastCmpDrawRows(pRle, pDst, nLinesToDraw, nRowStep, [=](uint8_t nSurface, uint8_t nIndex) { return pBlendTable[(nSurface << 8) + pPreMap[nIndex]]; });
}


//1.10f: D2CMP.0x6FDF6FD0
//  As FASTCMP_DC6PreMapBlend, clipped to nClipLeft..nClipRight.
void __fastcall FASTCMP_DC6PreMapBlendClipped(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pPreMap, const uint8_t *pBlendTable)
{
    FastCmpDrawRowsClipped(pRle, pDst, nLinesToDraw, nRowStep, nX, nClipLeft, nClipRight, [=](uint8_t nSurface, uint8_t nIndex) { return pBlendTable[(nSurface << 8) + pPreMap[nIndex]]; });
}


//1.10f: D2CMP.0x6FDF72D0
//  Draws each pixel through the pre-map: pPreMap[index].
void __fastcall FASTCMP_DC6PreMap(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pPreMap)
{
    FastCmpDrawRows(pRle, pDst, nLinesToDraw, nRowStep, [=](uint8_t, uint8_t nIndex) { return pPreMap[nIndex]; });
}


//1.10f: D2CMP.0x6FDF7410
//  As FASTCMP_DC6PreMap, clipped to nClipLeft..nClipRight.
void __fastcall FASTCMP_DC6PreMapClipped(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pPreMap)
{
    FastCmpDrawRowsClipped(pRle, pDst, nLinesToDraw, nRowStep, nX, nClipLeft, nClipRight, [=](uint8_t, uint8_t nIndex) { return pPreMap[nIndex]; });
}


//1.10f: D2CMP.0x6FDF75E0
//  Draws each pixel's index blended over the surface: pBlendTable[(surface << 8) + index].
void __fastcall FASTCMP_DC6Blend(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pBlendTable)
{
    FastCmpDrawRows(pRle, pDst, nLinesToDraw, nRowStep, [=](uint8_t nSurface, uint8_t nIndex) { return pBlendTable[(nSurface << 8) + nIndex]; });
}


//1.10f: D2CMP.0x6FDF77E0
//  As FASTCMP_DC6Blend, clipped to nClipLeft..nClipRight.
void __fastcall FASTCMP_DC6BlendClipped(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pBlendTable)
{
    FastCmpDrawRowsClipped(pRle, pDst, nLinesToDraw, nRowStep, nX, nClipLeft, nClipRight, [=](uint8_t nSurface, uint8_t nIndex) { return pBlendTable[(nSurface << 8) + nIndex]; });
}


//1.10f: D2CMP.0x6FDF7A80
//  #10049: toggles the generic decoder's flag (gbFastCmpUseGenericDecoder) and answers its new value.
extern "C" int32_t __cdecl FASTCMP_ToggleCelDrawMode()
{
    const int32_t bGeneric = gbFastCmpUseGenericDecoder == 0;
    gbFastCmpUseGenericDecoder = bGeneric;
    return bGeneric;
}


//1.10f: D2CMP.0x6FDF7AA0
//  Draws a DC6 frame: pDst is the surface at the frame's first drawn pixel, nX its column, nClipLeft..nClipRight the columns that may be drawn
//  (both 0: all), dwStride the surface's pitch. The stride must cover the frame's width and the lines to skip and draw must fit its height (the
//  asserts); that second compare is unsigned (`jbe`) whatever the assert text's (int) says, hence the cast. The first nLinesToSkip rows of the
//  stream are passed over; with bit 0 of the orientation set the rows go down the surface, clear they go up.
//  Then one of two decoders. The flag #10049 sets chooses the generic one, written in place: run by run, the clip span is drawn through whichever
//  tables are given, in this priority:
//      palette map and blend table   pBlendTable[(surface << 8) + pPaletteMap[index]]   (the pre-map is ignored)
//      palette map and pre-map       pPaletteMap[pPreMap[index]]
//      palette map                   pPaletteMap[index]
//      pre-map and blend table       pBlendTable[(surface << 8) + pPreMap[index]]
//      pre-map                       pPreMap[index]
//      blend table                   pBlendTable[(surface << 8) + index]
//      none                          index
//  The generic decoder clamps the right edge at 0 before the run's length, which the leaves' clip does not do; the span drawn is the same, an
//  empty one either way. Otherwise the fast decoder: the same choice made once and the leaf for it called, unclipped when the clip is off or the
//  whole frame lies inside it (nX >= nClipLeft and nX + width < nClipRight, signed), else clipped. The plain unclipped copy is written in place;
//  it is plausibly a thirteenth leaf that the original's compiler inlined.
//  BUG: the rows are counted down (the count is decremented before it is tested), so nLinesToDraw 0 passes the assert and runs 2^32 rows, in
//  every decoder. NOT ESTABLISHED: whether a caller passes 0 lines is not known.
void __fastcall FASTCMP_DrawDC6(const D2CmpRleCellHeader *pCel, uint8_t *pDst, int32_t nX, int32_t nClipLeft, int32_t nClipRight,
                                uint32_t dwStride, int32_t nLinesToSkip, int32_t nLinesToDraw, const uint8_t *pPaletteMap,
                                const uint8_t *pBlendTable, const uint8_t *pPreMap)
{
    const int32_t nRowStartX = nX;
    if (dwStride < pCel->dwWidth)
    {
        FOG_DisplayAssert("dwStride >= pCel->dwWidth", szFastCmpCpp, 1514);
        exit(-1);
    }
    if ((uint32_t)(nLinesToSkip + nLinesToDraw) > pCel->dwHeight)
    {
        FOG_DisplayAssert("nLinesToSkip + nLinesToDraw <= (int)pCel->dwHeight", szFastCmpCpp, 1515);
        exit(-1);
    }

    const uint8_t *pRle = (const uint8_t *)(pCel + 1);
    for (int32_t nSkip = nLinesToSkip; nSkip != 0;)
    {
        const int32_t nCommand = (int8_t)*pRle++;
        if (nCommand >= 0)
        {
            pRle += nCommand;
        }
        else if (!(nCommand & 0x7F))
        {
            --nSkip;
        }
    }

    const int32_t nRowStep = (pCel->dwOrientationFlags & 1) ? (int32_t)dwStride : -(int32_t)dwStride;

    if (gbFastCmpUseGenericDecoder)
    {
        uint8_t *pNextRow = pDst + nRowStep;
        for (;;)
        {
            const int32_t nCommand = (int8_t)*pRle++;
            if (nCommand >= 0)
            {
                const uint8_t *pRunEnd = pRle + nCommand;
                uint8_t *pDstRunEnd = pDst + nCommand;
                int32_t nCount = nCommand;
                if (nClipLeft || nClipRight)
                {
                    int32_t nLeftSkip = nClipLeft - nX;
                    if (nLeftSkip <= 0)
                    {
                        nLeftSkip = 0;
                    }
                    int32_t nRightEnd = nClipRight - nX;
                    if (nRightEnd <= 0)
                    {
                        nRightEnd = 0;
                    }
                    if (nRightEnd >= nCommand)
                    {
                        nRightEnd = nCommand;
                    }
                    nCount = nRightEnd - nLeftSkip;
                    if (nCount <= 0)
                    {
                        nCount = 0;
                    }
                    else
                    {
                        pDst += nLeftSkip;
                        pRle += nLeftSkip;
                    }
                }

                const uint8_t *pDrawEnd = pRle + nCount;
                if (pPaletteMap)
                {
                    if (pBlendTable)
                    {
                        for (; pRle < pDrawEnd; ++pRle, ++pDst)
                            *pDst = pBlendTable[(*pDst << 8) + pPaletteMap[*pRle]];
                    }
                    else if (pPreMap)
                    {
                        for (; pRle < pDrawEnd; ++pRle, ++pDst)
                            *pDst = pPaletteMap[pPreMap[*pRle]];
                    }
                    else
                    {
                        for (; pRle < pDrawEnd; ++pRle, ++pDst)
                            *pDst = pPaletteMap[*pRle];
                    }
                }
                else if (pPreMap)
                {
                    if (pBlendTable)
                    {
                        for (; pRle < pDrawEnd; ++pRle, ++pDst)
                            *pDst = pBlendTable[(*pDst << 8) + pPreMap[*pRle]];
                    }
                    else
                    {
                        for (; pRle < pDrawEnd; ++pRle, ++pDst)
                            *pDst = pPreMap[*pRle];
                    }
                }
                else if (pBlendTable)
                {
                    for (; pRle < pDrawEnd; ++pRle, ++pDst)
                        *pDst = pBlendTable[(*pDst << 8) + *pRle];
                }
                else
                {
                    for (; pRle < pDrawEnd; ++pRle, ++pDst)
                        *pDst = *pRle;
                }

                nX += nCommand;
                pDst = pDstRunEnd;
                pRle = pRunEnd;
            }
            else if (nCommand & 0x7F)
            {
                pDst += nCommand & 0x7F;
                nX += nCommand & 0x7F;
            }
            else
            {
                nX = nRowStartX;
                pDst = pNextRow;
                pNextRow += nRowStep;
                if (--nLinesToDraw == 0)
                {
                    return;
                }
            }
        }
    }

    const BOOL bNoClip = (!nClipLeft && !nClipRight) || (nX >= nClipLeft && (int32_t)(nX + pCel->dwWidth) < nClipRight);
    if (pPaletteMap)
    {
        if (pBlendTable)
        {
            if (bNoClip)
                FASTCMP_DC6MapBlend(pRle, pDst, nLinesToDraw, nRowStep, pPaletteMap, pBlendTable);
            else
                FASTCMP_DC6MapBlendClipped(pRle, pDst, nLinesToDraw, nRowStep, nX, nClipLeft, nClipRight, pPaletteMap, pBlendTable);
        }
        else if (pPreMap)
        {
            if (bNoClip)
                FASTCMP_DC6PreMapMap(pRle, pDst, nLinesToDraw, nRowStep, pPaletteMap, pPreMap);
            else
                FASTCMP_DC6PreMapMapClipped(pRle, pDst, nLinesToDraw, nRowStep, nX, nClipLeft, nClipRight, pPaletteMap, pPreMap);
        }
        else if (bNoClip)
            FASTCMP_DC6Map(pRle, pDst, nLinesToDraw, nRowStep, pPaletteMap);
        else
            FASTCMP_DC6MapClipped(pRle, pDst, nLinesToDraw, nRowStep, nX, nClipLeft, nClipRight, pPaletteMap);
    }
    else if (pPreMap)
    {
        if (pBlendTable)
        {
            if (bNoClip)
                FASTCMP_DC6PreMapBlend(pRle, pDst, nLinesToDraw, nRowStep, pPreMap, pBlendTable);
            else
                FASTCMP_DC6PreMapBlendClipped(pRle, pDst, nLinesToDraw, nRowStep, nX, nClipLeft, nClipRight, pPreMap, pBlendTable);
        }
        else if (bNoClip)
            FASTCMP_DC6PreMap(pRle, pDst, nLinesToDraw, nRowStep, pPreMap);
        else
            FASTCMP_DC6PreMapClipped(pRle, pDst, nLinesToDraw, nRowStep, nX, nClipLeft, nClipRight, pPreMap);
    }
    else if (pBlendTable)
    {
        if (bNoClip)
            FASTCMP_DC6Blend(pRle, pDst, nLinesToDraw, nRowStep, pBlendTable);
        else
            FASTCMP_DC6BlendClipped(pRle, pDst, nLinesToDraw, nRowStep, nX, nClipLeft, nClipRight, pBlendTable);
    }
    else if (bNoClip)
    {
        uint8_t *pNextRow = pDst + nRowStep;
        for (;;)
        {
            const int32_t nCommand = (int8_t)*pRle++;
            if (nCommand >= 0)
            {
                memcpy(pDst, pRle, nCommand);
                pRle += nCommand;
                pDst += nCommand;
            }
            else if (nCommand & 0x7F)
            {
                pDst += nCommand & 0x7F;
            }
            else
            {
                if (--nLinesToDraw == 0)
                {
                    return;
                }
                pDst = pNextRow;
                pNextRow += nRowStep;
            }
        }
    }
    else
    {
        FASTCMP_DC6CopyClipped(pRle, pDst, nLinesToDraw, nRowStep, nX, nClipLeft, nClipRight);
    }
}


//  A helper of ours, not a function of the original: one row of the lens, which the original's FASTCMP_CmpDoLensEffect writes out in place in each
//  of its two row loops. The left half goes forward from the row's left boundary up to its right boundary capped at 41, the right half backward
//  from the right boundary down to the left one raised to 40, each pixel taking the pixel its signed offset names; the walks compare their
//  pointers unsigned.
static void FastCmpLensRow(uint8_t *pRow, int32_t nRow)
{
    int32_t nLeft = gaLensLeftBoundary[nRow];
    int32_t nRight = gaLensRightBoundary[nRow];
    const int32_t *pOffset = &gaLensSourceOffset[nRow * 80 + nLeft];
    uint8_t *pPixel = pRow + nLeft;
    if (nRight >= 41)
    {
        nRight = 41;
    }
    for (const uint8_t *pEnd = pRow + nRight; pPixel < pEnd; ++pPixel, ++pOffset)
    {
        *pPixel = pPixel[*pOffset];
    }

    nRight = gaLensRightBoundary[nRow];
    nLeft = gaLensLeftBoundary[nRow];
    pOffset = &gaLensSourceOffset[nRow * 80 + nRight];
    pPixel = pRow + nRight;
    if (nLeft < 40)
    {
        nLeft = 40;
    }
    for (const uint8_t *pEnd = pRow + nLeft; pPixel > pEnd; --pPixel, --pOffset)
    {
        *pPixel = pPixel[*pOffset];
    }
}


//1.10f: D2CMP.0x6FDF7FB0
//  #10048: warps an 80x80 block of an indexed surface in place through the lens tables: rows 0..39 top down, then rows 79..40 bottom up, each row
//  nPitch bytes after the one above it. The tables (FastCmp.h) are .bss data that nothing in the module writes, so they stay zero, every boundary
//  is 0 and no walk of a row moves: in a stock game this moves nothing. The full walk is kept all the same.
extern "C" void __stdcall FASTCMP_CmpDoLensEffect(uint8_t *pPixels, int32_t nPitch)
{
    for (int32_t nRow = 0; nRow < 40; ++nRow)
    {
        FastCmpLensRow(pPixels + nRow * nPitch, nRow);
    }
    for (int32_t nRow = 79; nRow >= 40; --nRow)
    {
        FastCmpLensRow(pPixels + nRow * nPitch, nRow);
    }
}


//1.10f: D2CMP.0x6FDF8100
//  Draws a DC6 frame as its shadow, for CELCMP_CelDrawShadow (#10035). The stride must cover the width (the assert). Twice nLinesToSkip rows are
//  passed over; then for each of nLinesToDraw rows one source row is drawn, every literal pixel in the clip becoming nShadowColor, or
//  pBlendTable[(surface << 8) + nShadowColor] with a blend table (the pixels' own indexes are never read), and the next source row is passed
//  over, while each drawn row moves up the surface and one column left (pDst - dwStride - 1) and x one less. The clip is FASTCMP_DrawDC6's generic
//  decoder's, (0, 0) meaning all. The orientation bit is not read: the rows always go up. The colour is a byte.
//  BUG: the rows are counted down (the count is decremented before it is tested), so nLinesToDraw 0 runs 2^32 rows. NOT ESTABLISHED: whether a
//  caller passes 0 lines is not known.
//  BUG: the skip of the second row ends on 0x80 and on 0x00 (`test al, al` / `jle`, 0x6FDF81A3..0x6FDF81B9), where every other walk here takes a
//  0x00 control for an empty run, so a 0x00 control in a passed-over row ends that row early and the draw goes on reading its rest as the next
//  row. NOT ESTABLISHED: FASTCMP_EncodeDC6RlePixels never writes a 0x00 control, but whether a stock .dc6 holds one is not known.
void __fastcall FASTCMP_DrawDC6Shadow(const D2CmpRleCellHeader *pCel, uint8_t *pDst, int32_t nX, int32_t nClipLeft, int32_t nClipRight,
                                      uint32_t dwStride, int32_t nLinesToSkip, int32_t nLinesToDraw, const uint8_t *pBlendTable,
                                      uint8_t nShadowColor)
{
    int32_t nRowStartX = nX;
    if (dwStride < pCel->dwWidth)
    {
        FOG_DisplayAssert("dwStride >= pCel->dwWidth", szFastCmpCpp, 1871);
        exit(-1);
    }

    const uint8_t *pRle = (const uint8_t *)(pCel + 1);
    for (int32_t nSkip = 2 * nLinesToSkip; nSkip != 0;)
    {
        const int32_t nCommand = (int8_t)*pRle++;
        if (nCommand >= 0)
        {
            pRle += nCommand;
        }
        else if (!(nCommand & 0x7F))
        {
            --nSkip;
        }
    }

    const int32_t nRowStep = -(int32_t)dwStride;
    uint8_t *pNextRow = pDst + nRowStep;
    for (;;)
    {
        const int32_t nCommand = (int8_t)*pRle++;
        if (nCommand >= 0)
        {
            uint8_t *pAfterRun = pDst + nCommand;
            int32_t nCount = nCommand;
            bool bDraw = true;
            if (nClipLeft || nClipRight)
            {
                int32_t nLeftSkip = nClipLeft - nX;
                if (nLeftSkip <= 0)
                {
                    nLeftSkip = 0;
                }
                int32_t nRightEnd = nClipRight - nX;
                if (nRightEnd <= 0)
                {
                    nRightEnd = 0;
                }
                if (nRightEnd >= nCommand)
                {
                    nRightEnd = nCommand;
                }
                nCount = nRightEnd - nLeftSkip;
                if (nCount <= 0)
                {
                    bDraw = false;
                }
                else
                {
                    pDst += nLeftSkip;
                }
            }
            if (bDraw && nCount != 0)
            {
                if (pBlendTable)
                {
                    for (uint8_t *pEnd = pDst + nCount; pDst < pEnd; ++pDst)
                    {
                        *pDst = pBlendTable[(*pDst << 8) + nShadowColor];
                    }
                }
                else
                {
                    memset(pDst, nShadowColor, nCount);
                }
            }
            nX += nCommand;
            pDst = pAfterRun;
            pRle += nCommand;
        }
        else if (nCommand & 0x7F)
        {
            pDst += nCommand & 0x7F;
            nX += nCommand & 0x7F;
        }
        else
        {
            pDst = pNextRow - 1;
            pNextRow = pNextRow + nRowStep - 1;
            nX = --nRowStartX;
            if (--nLinesToDraw == 0)
            {
                return;
            }
            for (;;)
            {
                const int32_t nPassed = (int8_t)*pRle++;
                if (nPassed > 0)
                {
                    pRle += nPassed;
                }
                else if (!(nPassed & 0x7F))
                {
                    break;
                }
            }
        }
    }
}
