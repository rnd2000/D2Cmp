#include "D2CMP.h"

//  Codec.cpp -- the DCC decoder of D2CMP.dll 1.10: it reads a .dcc file's header and direction offsets, and decodes a run of its directions into
//  DC6-style frames (each direction's bit-packed header, its frames' fields, the cell codebooks, the cells drawn into one image the size of the
//  direction's box) and run-length encodes every frame out of that image into the pool buffer the sprite cache keeps.
//  The original source file is D2CMP\SRC\Codec.cpp (the asserts in this unit name it). Address range 0x6FDF3EE0..0x6FDF5A59.
//  The bit reader (D2CmpCodecBitReader) walks dwords: pWord, and nBitPosition (0..31), the next bit in that word, low bits first.

//  The path of the original's source file, as its asserts and traces spell it (__FILE__ in the original).
static const char szCodecCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2CMP\\SRC\\Codec.cpp";

static_assert(offsetof(D2CmpCodecBitReader, pWord) == 0x04 && offsetof(D2CmpCodecBitReader, nBitPosition) == 0x0C, "mov edi, [esi+4] / mov ecx, [esi+0Ch] (0x6FDF4F7B, 0x6FDF4F6B)");
static_assert(offsetof(D2CmpDccFrameState, nWidth) == 0x04 && offsetof(D2CmpDccFrameState, nHeight) == 0x08, "cmp [eax+4], ebp / cmp [eax+8], ebp (0x6FDF50D4, 0x6FDF50D9)");
static_assert(offsetof(D2CmpDccFrameState, nOffsetX) == 0x0C && offsetof(D2CmpDccFrameState, nOffsetY) == 0x10, "mov edx, [eax+0Ch] / mov edi, [eax+10h] (0x6FDF50FA, 0x6FDF5111)");
static_assert(offsetof(D2CmpDccFrameState, dwExtraBytes) == 0x14 && offsetof(D2CmpDccFrameState, pExtraData) == 0x18, "mov esi, [edx] / mov [edx+4], eax, edx = frame + 14h (0x6FDF45A1, 0x6FDF45A7)");
static_assert(offsetof(D2CmpDccFrameState, dwControlBytes) == 0x1C && offsetof(D2CmpDccFrameState, pImage) == 0x20, "mov edx, [ecx+1Ch] / cmp [ecx+20h], eax (0x6FDF4D5A, 0x6FDF4D68)");
static_assert(offsetof(D2CmpDccFrameState, nPitch) == 0x24 && offsetof(D2CmpDccFrameState, bFlip) == 0x28, "mov eax, [ecx+24h] / mov edx, [ecx+28h] (0x6FDF3EE3, 0x6FDF3EEA)");
static_assert(offsetof(D2CmpDccDirectionContext, pFrames) == 0x04 && offsetof(D2CmpDccDirectionContext, nMinLeft) == 0x08 && offsetof(D2CmpDccDirectionContext, nMinTop) == 0x0C,
              "mov edi, [ecx+4] / sub edx, [ecx+8] / mov ebx, [ecx+0Ch] (0x6FDF50C3, 0x6FDF50FD, 0x6FDF5114)");
static_assert(offsetof(D2CmpDccDirectionContext, nFrames) == 0x1C && offsetof(D2CmpDccDirectionContext, nGridColumns) == 0x28, "cmp edx, [ecx+1Ch] / imul eax, [ecx+28h] (0x6FDF50CF, 0x6FDF5138)");
static_assert(offsetof(D2CmpDccCellCursor, nLeft) == 0x0C && offsetof(D2CmpDccCellCursor, pImage) == 0x14 && offsetof(D2CmpDccCellCursor, nPitch) == 0x18,
              "mov [esi+0Ch], edx / mov [esi+14h], edx / mov [esi+18h], eax (0x6FDF5100, 0x6FDF5124, 0x6FDF512A)");
static_assert(offsetof(D2CmpDccCellCursor, nRowCellIndex) == 0x1C && offsetof(D2CmpDccCellCursor, nCellIndex) == 0x20, "mov [esi+1Ch], eax / mov [esi+20h], eax (0x6FDF515A, 0x6FDF515D)");
static_assert(offsetof(D2CmpDccCellCursor, nX) == 0x24 && offsetof(D2CmpDccCellCursor, nY) == 0x28 && offsetof(D2CmpDccCellCursor, nRowImageOffset) == 0x2C && offsetof(D2CmpDccCellCursor, nImageOffset) == 0x30,
              "mov ebp, [esi+24h] / mov ebx, [esi+28h] / mov ebp, [esi+2Ch] / mov ebp, [esi+30h] (0x6FDF4FEA, 0x6FDF5037, 0x6FDF5073, 0x6FDF4FFB)");
static_assert(offsetof(D2CmpDccCellCursor, nCellWidth) == 0x34 && offsetof(D2CmpDccCellCursor, nCellHeight) == 0x38, "mov edi, [esi+34h] / mov edx, [esi+38h] (0x6FDF4FE7, 0x6FDF5034)");
static_assert(offsetof(D2CmpDccCellState, pImage) == 0x04 && offsetof(D2CmpDccCellState, nPitch) == 0x08 && offsetof(D2CmpDccCellState, nWidth) == 0x0C && offsetof(D2CmpDccCellState, nHeight) == 0x10,
              "mov edx, [ebp+4] / mov ebx, [ebp+8] / mov eax, [ebp+0Ch] / mov ecx, [ebp+10h] (0x6FDF52D5, 0x6FDF534B, 0x6FDF52B9, 0x6FDF52C7)");
static_assert(offsetof(D2CmpDccPaletteMap, nCount) == 0x100, "mov [edi+100h], ebx (0x6FDF5210)");
static_assert(offsetof(D2CmpDccFileHeader, bVersion) == 0x01 && offsetof(D2CmpDccFileHeader, nDirections) == 0x02 && offsetof(D2CmpDccFileHeader, nFramesPerDirection) == 0x03 && offsetof(D2CmpDccFileHeader, dwTag) == 0x07,
              "the 15-byte header's dwords read at +1, +2, +3, +7 (0x6FDF40A0..0x6FDF40B8)");
static_assert(offsetof(D2CmpSpriteHeaderData, dwVersion) == 0x04 && offsetof(D2CmpSpriteHeaderData, dwFileFlags) == 0x08 && offsetof(D2CmpSpriteHeaderData, dwFormat) == 0x0C,
              "mov [esi+4], ecx / mov [esi+8], edx / mov dword ptr [esi+0Ch], 0 (0x6FDF40B5, 0x6FDF40AE, 0x6FDF40BC)");
static_assert(offsetof(D2CmpSpriteHeaderData, nDirections) == 0x14 && offsetof(D2CmpSpriteHeaderData, nFrames) == 0x18 && offsetof(D2CmpSpriteHeaderData, nDirectionOffsets) == 0x1C,
              "mov [esi+14h], eax / mov [esi+18h], edx / lea edx, [esi+1Ch] (0x6FDF40D0, 0x6FDF40C5, 0x6FDF410A)");
static_assert(offsetof(D2CmpSpriteGfxBlockPrefix, nOffsets) == 0x0C, "lea edx, [ecx+esi*4+0Ch] (0x6FDF420A)");
static_assert(offsetof(D2CmpSpriteFrameHeaderPrefix, dwAuxiliaryBytes) == 0x14 && offsetof(D2CmpSpriteFrameHeaderPrefix, pHardwareData) == 0x18 && offsetof(D2CmpSpriteFrameHeaderPrefix, dwControlBytes) == 0x1C,
              "mov [ebx+14h], eax / mov [ebx+18h], eax / mov [ebx+1Ch], edx (0x6FDF4D62, 0x6FDF4D65, 0x6FDF4D5F)");
static_assert(offsetof(D2CmpDccSourceInfo, dwTag) == 0x04 && offsetof(D2CmpDccSourceInfo, nDirections) == 0x08 && offsetof(D2CmpDccSourceInfo, nFrames) == 0x0C, "var_514, var_510, var_50C after var_518 (0x6FDF41A6..0x6FDF41C8)");


//1.10f: D2CMP.0x6FDF3EE0
//  Encodes one decoded frame as DC6 control bytes into pDestination, which CODEC_DecodeDccDirections points just past the frame's 32-byte header.
//  The rows run from the image's first row down by the pitch when bFlip is set, else from its last row up. Each row: a transparent run of pixel 0
//  is written as chunks of at most 127, each 0x80 | length, but only when a non-zero pixel follows (a run to the row's end writes nothing); a run
//  of non-zero pixels is written as chunks of at most 127, each its length byte and the pixels; and 0x80 ends the row.
//  Answers 1 when the bytes written end exactly at the header's +20h plus its control byte count, else traces both ends and answers 0. The test
//  comes after the writes: it reports an overrun, it does not stop one, and a short frame fails as well as a long one.
int32_t __fastcall CODEC_EncodeDccFrameRle(D2CmpDccFrameState *pFrame, D2CmpSpriteFrameHeaderPrefix *pOutputFrame, uint8_t *pDestination)
{
    const int32_t nPitch = pFrame->nPitch;
    const int32_t bFlip = pFrame->bFlip;
    uint8_t *pRow = pFrame->pImage;
    const int32_t nRowStep = bFlip ? nPitch : -nPitch;
    if (!bFlip)
        pRow += nPitch * (pFrame->nHeight - 1);

    for (int32_t nY = 0; nY < pFrame->nHeight; ++nY)
    {
        int32_t nX = 0;
        while (nX < pFrame->nWidth)
        {
            int32_t nStart = nX;
            while (nX < pFrame->nWidth && !pRow[nX])
                ++nX;
            if (nX >= pFrame->nWidth)
                break;

            while (nStart < nX)
            {
                const int32_t nChunk = nX - nStart > 127 ? 127 : nX - nStart;
                nStart += nChunk;
                *pDestination++ = (uint8_t)(nChunk | 0x80);
            }

            nStart = nX;
            while (nX < pFrame->nWidth && pRow[nX])
                ++nX;
            while (nStart < nX)
            {
                const uint32_t dwChunk = nX - nStart > 127 ? 127 : nX - nStart;
                *pDestination = (uint8_t)dwChunk;
                memcpy(pDestination + 1, &pRow[nStart], dwChunk);
                nStart += dwChunk;
                pDestination += dwChunk + 1;
            }
        }
        *pDestination++ = 0x80;
        pRow += nRowStep;
    }

    uint8_t *pExpectedEnd = (uint8_t *)(pOutputFrame + 1) + pOutputFrame->dwControlBytes;
    if (pDestination == pExpectedEnd)
        return 1;

    FOG_Trace("Sprite Decompression Error -- EXP: (dest != &pCel->bControlData[pCel->dwControlSize])   VALUE: dest == %x   &pCel->bControlData[pCel->dwControlSize] == %x   FILE: %s   LINE: %d", pDestination, pExpectedEnd, szCodecCpp, 1247);
    return 0;
}


//1.10f: D2CMP.0x6FDF4050
//  Reads a .dcc file's header into pHeader. Answers 1 on success. Answers 0 when the file does not open, and also (closing the file) when a read
//  fails or comes up short or the first byte is not 0x74, with whatever was written so far left in pHeader. The 15-byte file header gives the
//  version, tag, frames per direction and direction count (the format is set to 0); then the direction offsets are read, four bytes each, and
//  the file's size is stored as the offset after the last. bFlags bit 1 says the header was read.
//  More than 32 directions fails the assert and exits, after the header's fields were written and with the file still open. The file's size, the
//  uncompressed size at +0Bh and the version are not checked.
int32_t __fastcall CODEC_ReadDccHeader(const char *szPath, D2CmpSpriteHeaderData *pHeader)
{
    void *hFile;
    FOG_FOpenFile(szPath, &hFile);
    if (!hFile)
        return 0;

    D2CmpDccFileHeader tFileHeader;
    uint32_t dwBytesRead;
    if (!FOG_FReadFile(hFile, &tFileHeader, 15, &dwBytesRead, 0, 0, 0) || dwBytesRead != 15 || tFileHeader.bSignature != 0x74)
    {
        FOG_FCloseFile(hFile);
        return 0;
    }

    pHeader->dwVersion = tFileHeader.bVersion;
    pHeader->dwFileFlags = tFileHeader.dwTag;
    pHeader->dwFormat = 0;
    pHeader->nFrames = tFileHeader.nFramesPerDirection;
    pHeader->nDirections = tFileHeader.nDirections;
    if (tFileHeader.nDirections > 32)
    {
        FOG_DisplayAssert("header.spriteDirections <= 32", szCodecCpp, 1286);
        exit(-1);
    }

    const uint32_t dwOffsetBytes = 4 * tFileHeader.nDirections;
    if (!FOG_FReadFile(hFile, pHeader->nDirectionOffsets, dwOffsetBytes, &dwBytesRead, 0, 0, 0) || dwBytesRead != dwOffsetBytes)
    {
        FOG_FCloseFile(hFile);
        return 0;
    }

    pHeader->nDirectionOffsets[tFileHeader.nDirections] = FOG_FGetFileSize(hFile, NULL);
    FOG_FCloseFile(hFile);
    pHeader->bFlags |= 2;
    return 1;
}


//1.10f: D2CMP.0x6FDF4160
//  Decodes directions nStartDirection .. + nDirections of a .dcc file, whose bytes from nStartDirection's offset on are pInput, into one buffer from
//  pPool, under gCodecCriticalSection for the whole call. Answers that buffer; directions that were not decoded leave it as FOG_AllocPool made it.
//  The original inlines CODEC_ReadBits, CODEC_StartDccCells and CODEC_SkipBits here.
//  First the sizes: each direction's first dword, at its offset less the first one's, is its decoded size; pGfxBlock->nOffsets[dir] takes the sum
//  before it and *pDecodedBytes the total, which FOG_AllocPool allocates. Then per direction:
//    * its first 32 bits (that size) skipped, the 2-bit compression flags and seven 4-bit field codes read (28 bits); per frame its variable,
//      width, height, x and y offsets (the two signed, sign-extended by their width), extra-byte count and control-byte count, each as wide as
//      gaCodecFieldBits says, then a 1-bit flip; the frames' extra bytes, byte aligned after the last frame, assigned in frame order and skipped.
//      The direction must be below the file's direction count (the assert, line 883 of the original, is an inlined callee's and has no
//      out-of-line copy, so it is written in place).
//    * the box: the least left and top and the greatest right and bottom over its frames, the top being y less the height unless the frame is
//      flipped; a grid of 4x4 cells over it, (w + 3) / 4 by (h + 3) / 4, signed; the cells counted once (nVisitedCells, which nothing reads);
//      the grid's history cleared.
//    * the streams: the equal-cell stream's size (20 bits) if flag 2, the change masks' (20), the encoding types' and raw codes' (20 each) if
//      flag 1, and eight 32-bit words of palette presence, one bit per colour; the compact palette maps; a reader at the start of each stream;
//      the codebooks decoded and remapped through the inverse map, and the pixel codes they consumed skipped.
//    * the frames placed in gabCodecImage, each at its left and top in the box (both asserted not negative), pitch the box's width.
//    * the raster pass, the history cleared whole: every cell drawn, its state kept in the history; when the cursor's frame moves on, the frame
//      left behind gets its header (its variable as the flags, its size, offsets and control bytes, no auxiliary bytes, no hardware data), its
//      image asserted, and its RLE through CODEC_EncodeDccFrameRle. A frame with no width or height has no cells and is written when the next
//      frame's first cell is done. A failure traces the key, the frame, the control size and a checksum of the input, and halts. The next header
//      follows at control + aux + 23h: the 20h header and three bytes nothing writes.
//    * the reader aligned to a byte for the next direction.
//  BUG: tSource.nFrames is not checked against gaCodecFrames[256], so a .dcc with more frames writes past the array in the frame loop (0x6FDF43FF).
//  NOT ESTABLISHED: whether a stock .dcc exceeds the extent is not known.
//  BUG: the size prepass reads a dword at every requested direction's offset before anything asserts the direction count. NOT ESTABLISHED: whether
//  a call requests a direction past the file's count is not known.
//  BUG: FOG_AllocPool's answer is used untested. LATENT: only a failed allocation reaches it.
void *__fastcall CODEC_DecodeDccDirections(void *pInput, void *pPool, int32_t nInputBytes, D2CmpSpriteHeaderData *pHeader, D2CmpSpriteGfxBlockPrefix *pGfxBlock, int32_t nStartDirection, int32_t nDirections, uint32_t *pDecodedBytes, const char *szKey)
{
    EnterCriticalSection(&gCodecCriticalSection);

    D2CmpCodecBitReader tReader;
    tReader.pStart = (const uint32_t *)pInput;
    tReader.pWord = (const uint32_t *)pInput;
    tReader.nBitPosition = 0;
    D2CmpDccSourceInfo tSource;
    tSource.dwVersion = pHeader->dwVersion;
    tSource.dwTag = pHeader->dwFileFlags;
    tSource.nDirections = pHeader->nDirections;
    tSource.nFrames = pHeader->nFrames;
    tReader.nTotalBits = 8 * nInputBytes;
    *pDecodedBytes = 0;

    const int32_t nEndDirection = nStartDirection + nDirections;
    if (nStartDirection < nEndDirection)
    {
        const int32_t *pDirectionOffset = &pHeader->nDirectionOffsets[nStartDirection];
        int32_t *pBlockOffset = &pGfxBlock->nOffsets[nStartDirection];
        int32_t nCount = nEndDirection - nStartDirection;
        do
        {
            const int32_t nRelative = *pDirectionOffset++ - pHeader->nDirectionOffsets[nStartDirection];
            const uint32_t dwDirectionBytes = *(const uint32_t *)((const uint8_t *)pInput + nRelative);
            *pBlockOffset++ = *pDecodedBytes;
            *pDecodedBytes += dwDirectionBytes;
        }
        while (--nCount);
    }

    D2CmpSpriteFrameHeaderPrefix *pDecodedBase = (D2CmpSpriteFrameHeaderPrefix *)FOG_AllocPool(pPool, *pDecodedBytes, szCodecCpp, 1387, 0);
    D2CmpSpriteFrameHeaderPrefix *pOutputFrame = pDecodedBase;

    for (int32_t nDirection = nStartDirection; nDirection < nEndDirection; ++nDirection)
    {
        //  The direction's first 32 bits (its decoded size) are skipped: CODEC_ReadBits(32) written out in place because its answer is not read; its
        //  effect is pWord + 1 with the bit position unchanged.
        if (32 - tReader.nBitPosition < 32)
            ++tReader.pWord;
        else if ((tReader.nBitPosition += 32) == 32)
        {
            ++tReader.pWord;
            tReader.nBitPosition = 0;
        }

        const uint32_t dwCompressionFlags = CODEC_ReadBits(&tReader, 2);
        const uint32_t dwFieldCodes = CODEC_ReadBits(&tReader, 28);
        const int32_t nVariableCode = dwFieldCodes & 0xF;
        const int32_t nWidthCode = (dwFieldCodes >> 4) & 0xF;
        const int32_t nHeightCode = (dwFieldCodes >> 8) & 0xF;
        const int32_t nOffsetXCode = (dwFieldCodes >> 12) & 0xF;
        const int32_t nOffsetYCode = (dwFieldCodes >> 16) & 0xF;
        const int32_t nExtraBytesCode = (dwFieldCodes >> 20) & 0xF;
        const int32_t nControlBytesCode = (dwFieldCodes >> 24) & 0xF;

        D2CmpDccFrameState *pFramesEnd = &gaCodecFrames[tSource.nFrames];
        int32_t nExtraBytesTotal = 0;
        if (pFramesEnd > gaCodecFrames)
        {
            for (D2CmpDccFrameState *pFrame = gaCodecFrames; pFrame < pFramesEnd; ++pFrame)
            {
                pFrame->dwVariable = CODEC_ReadBits(&tReader, gaCodecFieldBits[nVariableCode]);
                pFrame->nWidth = CODEC_ReadBits(&tReader, gaCodecFieldBits[nWidthCode]);
                pFrame->nHeight = CODEC_ReadBits(&tReader, gaCodecFieldBits[nHeightCode]);

                //  The sign bit of an nBits-bit field is gadwCodecSignBits[nBits - 1].
                //  BUG: for a 32-bit field the original ORs in the dword after gadwCodecSignExtend (0x6FE0BB08, a relocated
                //  pointer to "D2CMP.DLL"), a read past the table, so a negative x or y offset coded in 32 bits is ORed with that
                //  address. NOT ESTABLISHED: whether any stock .dcc codes an offset in 32 bits is not known.
                //  DIFFERENCE: ours ORs nothing for a 32-bit field, which is its correct sign extension; it differs from the
                //  original there and only there.
                int32_t nBits = gaCodecFieldBits[nOffsetXCode];
                uint32_t dwValue = CODEC_ReadBits(&tReader, nBits);
                if (nBits && (dwValue & gadwCodecSignBits[nBits - 1]))
                    dwValue |= nBits < 32 ? gadwCodecSignExtend[nBits] : 0;
                pFrame->nOffsetX = dwValue;

                nBits = gaCodecFieldBits[nOffsetYCode];
                dwValue = CODEC_ReadBits(&tReader, nBits);
                if (nBits && (dwValue & gadwCodecSignBits[nBits - 1]))
                    dwValue |= nBits < 32 ? gadwCodecSignExtend[nBits] : 0;
                pFrame->nOffsetY = dwValue;

                pFrame->dwExtraBytes = CODEC_ReadBits(&tReader, gaCodecFieldBits[nExtraBytesCode]);
                pFrame->pExtraData = NULL;
                pFrame->dwControlBytes = CODEC_ReadBits(&tReader, gaCodecFieldBits[nControlBytesCode]);
                pFrame->pImage = NULL;
                pFrame->nPitch = 0;
                pFrame->bFlip = CODEC_ReadBits(&tReader, 1);
                nExtraBytesTotal += pFrame->dwExtraBytes;
            }

            if (nExtraBytesTotal)
            {
                //  The position is aligned up to a byte on the whole value, the word advance is a logical shift, and the byte
                //  within the word and the end use a signed division, as the original does.
                const uint32_t dwAligned = (tReader.nBitPosition + 7) & ~7u;
                const int32_t nAlignedBit = dwAligned & 0x1F;
                const uint32_t *pAlignedWord = tReader.pWord + (dwAligned >> 5);
                uint8_t *pExtraData = (uint8_t *)pAlignedWord + (nAlignedBit + 7) / 8;
                for (D2CmpDccFrameState *pFrame = gaCodecFrames; pFrame < pFramesEnd; ++pFrame)
                {
                    if (pFrame->dwExtraBytes)
                    {
                        pFrame->pExtraData = pExtraData;
                        pExtraData += pFrame->dwExtraBytes;
                    }
                }
                const int32_t nEndBit = nAlignedBit + 8 * nExtraBytesTotal;
                tReader.nBitPosition = nEndBit & 0x1F;
                tReader.pWord = pAlignedWord + nEndBit / 32;
            }
        }

        if (nDirection >= (int32_t)tSource.nDirections)
        {
            FOG_DisplayAssert("dir < sprite->directions", szCodecCpp, 883);
            exit(-1);
        }

        D2CmpDccDirectionContext tDirection;
        tDirection.nDirection = nDirection;
        tDirection.pSource = &tSource;
        tDirection.pFrames = gaCodecFrames;
        tDirection.nFrames = tSource.nFrames;
        int32_t nMinLeft = 0x7FFFFFFF;
        int32_t nMinTop = 0x7FFFFFFF;
        int32_t nMaxRight = (int32_t)0x80000000;
        int32_t nMaxBottom = (int32_t)0x80000000;
        for (D2CmpDccFrameState *pFrame = gaCodecFrames; pFrame < pFramesEnd; ++pFrame)
        {
            const int32_t nTop = pFrame->nOffsetY - (pFrame->bFlip ? 0 : pFrame->nHeight);
            if (pFrame->nOffsetX < nMinLeft)
                nMinLeft = pFrame->nOffsetX;
            if (pFrame->nOffsetX + pFrame->nWidth > nMaxRight)
                nMaxRight = pFrame->nOffsetX + pFrame->nWidth;
            if (nTop < nMinTop)
                nMinTop = nTop;
            if (nTop + pFrame->nHeight > nMaxBottom)
                nMaxBottom = nTop + pFrame->nHeight;
        }
        //  DIFFERENCE: with no frame the box is 0x80000000 - 0x7FFFFFFF = 1 each way, which overflows a signed int in C;
        //  ours subtracts as unsigned, which is defined and gives the original's wrapped result.
        tDirection.nMinLeft = nMinLeft;
        tDirection.nMinTop = nMinTop;
        tDirection.nPixelWidth = (int32_t)((uint32_t)nMaxRight - (uint32_t)nMinLeft);
        tDirection.nVisitedCells = 0;
        tDirection.nPixelHeight = (int32_t)((uint32_t)nMaxBottom - (uint32_t)nMinTop);
        tDirection.nGridColumns = (tDirection.nPixelWidth + 3) / 4;
        tDirection.nGridCells = (tDirection.nPixelHeight + 3) / 4 * tDirection.nGridColumns;

        D2CmpDccCellCursor tCursor;
        if (CODEC_StartDccCells(&tDirection, &tCursor))
        {
            do
                ++tDirection.nVisitedCells;
            while (CODEC_AdvanceDccCell(&tDirection, &tCursor));
        }

        //  BUG: the clear is 20 bytes a cell, unbounded against gaCodecCellHistory's 5625 cells, so a larger grid clears past the array (0x6FDF4775).
        //  NOT ESTABLISHED: whether a stock .dcc exceeds the extent is not known.
        memset(gaCodecCellHistory, 0, sizeof(D2CmpDccCellState) * tDirection.nGridCells);

        int32_t nEqualCellBits = 0;
        int32_t nEncodingTypeBits = 0;
        int32_t nRawCodeBits = 0;
        if (dwCompressionFlags & 2)
            nEqualCellBits = CODEC_ReadBits(&tReader, 20);
        const int32_t nMaskBits = CODEC_ReadBits(&tReader, 20);
        if (dwCompressionFlags & 1)
        {
            nEncodingTypeBits = CODEC_ReadBits(&tReader, 20);
            nRawCodeBits = CODEC_ReadBits(&tReader, 20);
        }

        uint32_t adwPresence[8];
        for (int32_t nWord = 0; nWord < 8; ++nWord)
            adwPresence[nWord] = CODEC_ReadBits(&tReader, 32);

        uint8_t abPresence[256];
        for (int32_t nColor = 0; nColor < 256; ++nColor)
            abPresence[nColor] = (gdwBitMasks[nColor & 0x1F] & adwPresence[nColor >> 5]) != 0;
        D2CmpDccPaletteMap tForward;
        D2CmpDccPaletteMap tInverse;
        CODEC_BuildDccPaletteMaps(abPresence, &tForward, &tInverse);

        D2CmpCodecBitReader tEqualCells = tReader;
        CODEC_SkipBits(&tReader, nEqualCellBits);
        D2CmpCodecBitReader tMasks = tReader;
        CODEC_SkipBits(&tReader, nMaskBits);
        D2CmpCodecBitReader tEncodingTypes = tReader;
        CODEC_SkipBits(&tReader, nEncodingTypeBits);
        D2CmpCodecBitReader tRawCodes = tReader;
        CODEC_SkipBits(&tReader, nRawCodeBits);
        D2CmpCodecBitReader tPixelCodes = tReader;
        D2CmpCodecBitReader tEqualCodebookCells = tEqualCells;

        int32_t nConsumedPixelBits = 0;
        D2CmpCodecByteSpan tCodebooks;
        tCodebooks.pData = gabCodecCodebooks;
        tCodebooks.nBytes = 0;
        CODEC_DecodeDccCodebooks(&tDirection, gaCodecCellHistory, nEqualCellBits ? &tEqualCodebookCells : NULL, &tMasks, nEncodingTypeBits ? &tEncodingTypes : NULL, nRawCodeBits ? &tRawCodes : NULL, &tPixelCodes, &tCodebooks, &nConsumedPixelBits);
        CODEC_SkipBits(&tReader, nConsumedPixelBits);
        CODEC_RemapDccCodebooks(&tCodebooks, tInverse.entries);

        for (D2CmpDccFrameState *pFrame = gaCodecFrames; pFrame < pFramesEnd; ++pFrame)
        {
            const int32_t nLeft = pFrame->nOffsetX - tDirection.nMinLeft;
            const int32_t nTop = pFrame->nOffsetY - (pFrame->bFlip ? 0 : pFrame->nHeight) - tDirection.nMinTop;
            if (nLeft < 0)
            {
                FOG_DisplayAssert("left >= 0", szCodecCpp, 1575);
                exit(-1);
            }
            if (nTop < 0)
            {
                FOG_DisplayAssert("top >= 0", szCodecCpp, 1576);
                exit(-1);
            }
            pFrame->nPitch = tDirection.nPixelWidth;
            //  BUG: the image's 120000 bytes bound neither a frame's place nor the box, so a larger box writes past gabCodecImage.
            //  NOT ESTABLISHED: whether a stock .dcc exceeds the extent is not known.
            pFrame->pImage = &gabCodecImage[nTop * tDirection.nPixelWidth + nLeft];
        }

        uint8_t *pPaletteCursor = tCodebooks.pData;
        D2CmpDccCellCursor tRaster;
        if (CODEC_StartDccCells(&tDirection, &tRaster))
        {
            memset(gaCodecCellHistory, 0, sizeof(gaCodecCellHistory));
            D2CmpCodecBitReader *pEqualCells = nEqualCellBits ? &tEqualCells : NULL;
            int32_t nFrame = 0;
            int32_t bMoreCells;
            do
            {
                D2CmpDccCellState tCurrent;
                tCurrent.pCodebook = pPaletteCursor;
                tCurrent.pImage = &tRaster.pImage[tRaster.nImageOffset];
                tCurrent.nPitch = tRaster.nPitch;
                tCurrent.nWidth = tRaster.nCellWidth;
                tCurrent.nHeight = tRaster.nCellHeight;
                CODEC_RasterizeDccCell(&tCurrent, &tReader, &gaCodecCellHistory[tRaster.nCellIndex], pEqualCells, &pPaletteCursor);
                gaCodecCellHistory[tRaster.nCellIndex] = tCurrent;
                bMoreCells = CODEC_AdvanceDccCell(&tDirection, &tRaster);
                if (tRaster.nFrame != nFrame)
                {
                    D2CmpDccFrameState *pDone = &gaCodecFrames[nFrame];
                    pOutputFrame->dwFlags = pDone->dwVariable;
                    pOutputFrame->dwWidth = pDone->nWidth;
                    pOutputFrame->dwHeight = pDone->nHeight;
                    pOutputFrame->nOffsetX = pDone->nOffsetX;
                    pOutputFrame->nOffsetY = pDone->nOffsetY;
                    pOutputFrame->dwControlBytes = pDone->dwControlBytes;
                    pOutputFrame->dwAuxiliaryBytes = 0;
                    pOutputFrame->pHardwareData = NULL;
                    if (!pDone->pImage)
                    {
                        FOG_DisplayAssert("curcell->image", szCodecCpp, 1622);
                        exit(-1);
                    }
                    if (!CODEC_EncodeDccFrameRle(pDone, pOutputFrame, (uint8_t *)(pOutputFrame + 1)))
                    {
                        FOG_Trace("Sprite Decompression Error -- File:%s", szKey);
                        FOG_Trace("Dir:%d  Cell#:%d  Wid:%d  Ht:%d  OffX:%d  OffY:%d", nDirection, tRaster.nFrame, pOutputFrame->dwWidth, pOutputFrame->dwHeight, pOutputFrame->nOffsetX, pOutputFrame->nOffsetY);
                        FOG_Trace("ControlSize:%d", pOutputFrame->dwControlBytes);
                        int32_t nChecksum = 0;
                        for (int32_t nByte = 0; nByte < nInputBytes; ++nByte)
                            nChecksum += ((const uint8_t *)pInput)[nByte];
                        FOG_Trace("Buffer -- Len:%d  Checksum:%d", nInputBytes, nChecksum);
                        //  DIFFERENCE: the original calls its own VC6 runtime's sprintf here; ours calls the build's.
                        char szMessage[512];
                        sprintf(szMessage, "Error decompressing sprite - Possible corruption in data file: %s", szKey);
                        FOG_DisplayHalt(szMessage, szCodecCpp, 1639);
                        exit(-1);
                    }
                    nFrame = tRaster.nFrame;
                    pOutputFrame = (D2CmpSpriteFrameHeaderPrefix *)((uint8_t *)pOutputFrame + pOutputFrame->dwControlBytes + pOutputFrame->dwAuxiliaryBytes + 0x23);
                }
            }
            while (bMoreCells);
        }
        CODEC_AlignReaderToByte(&tReader);
    }

    LeaveCriticalSection(&gCodecCriticalSection);
    return pDecodedBase;
}


//1.10f: D2CMP.0x6FDF4F10
//  Moves the position on by nBits: the low five bits of the sum are the new position, and the signed quotient by 32, truncated toward zero, is
//  the number of words pWord moves.
void __fastcall CODEC_SkipBits(D2CmpCodecBitReader *pReader, int32_t nBits)
{
    const int32_t nPosition = pReader->nBitPosition + nBits;
    pReader->nBitPosition = nPosition & 0x1F;
    pReader->pWord += nPosition / 32;
}


//1.10f: D2CMP.0x6FDF4F40
//  Rounds the position up to a byte: the low five bits of the result are the new position and the rest, as a logical shift by five, is the
//  number of words pWord moves (not CODEC_SkipBits' signed division).
void __fastcall CODEC_AlignReaderToByte(D2CmpCodecBitReader *pReader)
{
    const uint32_t dwPosition = (pReader->nBitPosition + 7) & ~7u;
    pReader->nBitPosition = dwPosition & 0x1F;
    pReader->pWord += dwPosition >> 5;
}


//1.10f: D2CMP.0x6FDF4F60
//  Reads the next nBits bits, low bits first; 0 bits answers 0 before the reader is touched. A field that fits the current word (a signed compare
//  against 32 - position) is shifted down and masked, the position moved on, and at exactly 32 the next word taken; one that does not takes the
//  rest of this word and the low bits of the next. Nothing tests nTotalBits, the width or the end of the input.
uint32_t __fastcall CODEC_ReadBits(D2CmpCodecBitReader *pReader, int32_t nBits)
{
    if (!nBits)
        return 0;

    const int32_t nPosition = pReader->nBitPosition;
    const int32_t nFirstWordBits = 32 - nPosition;
    const uint32_t *pWord = pReader->pWord;
    if (nBits > nFirstWordBits)
    {
        const uint32_t dwFirst = *pWord >> nPosition;
        pReader->nBitPosition = nBits - nFirstWordBits;
        pReader->pWord = pWord + 1;
        return (dwFirst & gadwCodecLowMasks[nFirstWordBits]) | ((pWord[1] & gadwCodecLowMasks[nBits - nFirstWordBits]) << nFirstWordBits);
    }

    const uint32_t dwValue = (*pWord >> nPosition) & gadwCodecLowMasks[nBits];
    const int32_t nNextPosition = nPosition + nBits;
    pReader->nBitPosition = nNextPosition;
    if (nNextPosition == 32)
    {
        pReader->nBitPosition = 0;
        pReader->pWord = pWord + 1;
    }
    return dwValue;
}


//1.10f: D2CMP.0x6FDF4FE0
//  Moves the cursor to the next cell: along the row, then down a row, then to the next frame that has a width and a height. A cell is 4x4 except
//  at the edges: a row's first cell is 4 - (left % 4) wide, so the cells line up with the direction's grid, and a last cell takes what is left
//  when the edge is 5 or less away, so an edge cell can be 5 wide or tall. Every compare, division and remainder is signed. The position steps
//  by the old cell width and height before the new ones are stored. A frame's first cell index is (top / 4) * columns + left / 4, top being y
//  less the height unless the frame is flipped. Answers 1 for a cell and 0 past the last frame, the cursor left as advanced.
int32_t __fastcall CODEC_AdvanceDccCell(D2CmpDccDirectionContext *pDirection, D2CmpDccCellCursor *pCursor)
{
    const int32_t nCellWidth = pCursor->nCellWidth;
    pCursor->nX += nCellWidth;
    if (pCursor->nX < pCursor->nFrameWidth)
    {
        pCursor->nImageOffset += nCellWidth;
        ++pCursor->nCellIndex;
        pCursor->nCellWidth = pCursor->nX + 5 < pCursor->nFrameWidth ? 4 : pCursor->nFrameWidth - pCursor->nX;
        return 1;
    }

    pCursor->nY += pCursor->nCellHeight;
    if (pCursor->nY < pCursor->nFrameHeight)
    {
        const int32_t nNegLeftRemainder = -(pCursor->nLeft % 4);
        pCursor->nX = 0;
        pCursor->nCellWidth = nNegLeftRemainder + 5 < pCursor->nFrameWidth ? nNegLeftRemainder + 4 : pCursor->nFrameWidth;
        pCursor->nRowImageOffset += pCursor->nCellHeight * pCursor->nPitch;
        pCursor->nImageOffset = pCursor->nRowImageOffset;
        pCursor->nRowCellIndex += pDirection->nGridColumns;
        pCursor->nCellIndex = pCursor->nRowCellIndex;
        pCursor->nCellHeight = pCursor->nY + 5 < pCursor->nFrameHeight ? 4 : pCursor->nFrameHeight - pCursor->nY;
        return 1;
    }

    int32_t nFrame = ++pCursor->nFrame;
    D2CmpDccFrameState *pFrame = &pDirection->pFrames[nFrame];
    for (;;)
    {
        if (nFrame >= pDirection->nFrames)
            return 0;
        if (pFrame->nWidth && pFrame->nHeight)
            break;
        ++nFrame;
        ++pFrame;
        pCursor->nFrame = nFrame;
    }

    pCursor->nFrameWidth = pFrame->nWidth;
    pCursor->nFrameHeight = pFrame->nHeight;
    pCursor->nLeft = pFrame->nOffsetX - pDirection->nMinLeft;
    const int32_t nTop = pFrame->nOffsetY - pDirection->nMinTop - (pFrame->bFlip ? 0 : pFrame->nHeight);
    pCursor->nTop = nTop;
    pCursor->pImage = pFrame->pImage;
    pCursor->nPitch = pFrame->nPitch;
    pCursor->nX = 0;
    pCursor->nY = 0;
    pCursor->nRowImageOffset = 0;
    pCursor->nRowCellIndex = nTop / 4 * pDirection->nGridColumns + pCursor->nLeft / 4;
    pCursor->nCellIndex = pCursor->nRowCellIndex;
    pCursor->nImageOffset = 0;
    const int32_t nNegLeftRemainder = -(pCursor->nLeft % 4);
    const int32_t nNegTopRemainder = -(nTop % 4);
    pCursor->nCellWidth = nNegLeftRemainder + 5 < pCursor->nFrameWidth ? nNegLeftRemainder + 4 : pCursor->nFrameWidth;
    pCursor->nCellHeight = nNegTopRemainder + 5 < pCursor->nFrameHeight ? nNegTopRemainder + 4 : pCursor->nFrameHeight;
    return 1;
}


//1.10f: D2CMP.0x6FDF51B0
//  Answers 0 for a direction with no frames, the cursor untouched; otherwise clears the cursor, sets its frame to -1 and answers the first cell
//  CODEC_AdvanceDccCell finds.
int32_t __fastcall CODEC_StartDccCells(D2CmpDccDirectionContext *pDirection, D2CmpDccCellCursor *pCursor)
{
    if (!pDirection->nFrames)
        return 0;
    memset(pCursor, 0, sizeof(D2CmpDccCellCursor));
    pCursor->nFrame = -1;
    return CODEC_AdvanceDccCell(pDirection, pCursor);
}


//1.10f: D2CMP.0x6FDF51E0
//  Builds the compact palette: each colour present gets the next index, pForward mapping colour to index and pInverse index to colour.
//  pForward's entries for absent colours are not written, and nothing reads pForward after this (CODEC_DecodeDccDirections passes pInverse alone
//  on); its count is set to 256, pInverse's to the number of colours present.
void __fastcall CODEC_BuildDccPaletteMaps(const uint8_t *pPresence, D2CmpDccPaletteMap *pForward, D2CmpDccPaletteMap *pInverse)
{
    uint32_t dwCount = 0;
    for (int32_t nColor = 0; nColor < 256; ++nColor)
    {
        if (pPresence[nColor])
        {
            pForward->entries[nColor] = (uint8_t)dwCount;
            pInverse->entries[dwCount++] = (uint8_t)nColor;
        }
    }
    pForward->nCount = 256;
    pInverse->nCount = dwCount;
}


//1.10f: D2CMP.0x6FDF5220
//  Draws one cell into the image. The original inlines CODEC_ReadBits here.
//  When there is an equal-cell stream, the cell has a previous state and the stream's bit is set, the cell repeats its previous one: the same
//  size copies the previous pixels (through a local buffer, five bytes a row, so the copy is right however the two overlap; skipped when both
//  are one place), another size clears the cell to 0; either way the palette cursor stays. Otherwise the cell's codebook is probed for how many
//  index bits it needs (0 if its first two entries match, 1 if its second and third do, else 2) and the cell is filled with the first colour or
//  with one colour per pixel, each the codebook entry its bits pick out of *ppPaletteCursor; then the cursor moves on 4.
//  The width and height are the cursor's, 5 at most for a frame whose left and top are not negative (CODEC_DecodeDccDirections asserts that
//  before the raster pass), so five rows of five fit abCellRows. It is 28 bytes, the original's frame slot; 25 are used.
void __fastcall CODEC_RasterizeDccCell(D2CmpDccCellState *pCurrent, D2CmpCodecBitReader *pPixels, D2CmpDccCellState *pPrevious, D2CmpCodecBitReader *pEqualCells, uint8_t **ppPaletteCursor)
{
    if (pEqualCells && pPrevious->pCodebook && CODEC_ReadBits(pEqualCells, 1))
    {
        if (pCurrent->nWidth == pPrevious->nWidth && pCurrent->nHeight == pPrevious->nHeight)
        {
            if (pCurrent->pImage != pPrevious->pImage)
            {
                uint8_t abCellRows[28];
                const uint8_t *pSource = pPrevious->pImage;
                uint8_t *pTemporary = abCellRows;
                if (pCurrent->nHeight > 0)
                {
                    const int32_t nPreviousPitch = pPrevious->nPitch;
                    int32_t nRows = pCurrent->nHeight;
                    do
                    {
                        memcpy(pTemporary, pSource, pCurrent->nWidth);
                        pTemporary += 5;
                        pSource += nPreviousPitch;
                    }
                    while (--nRows);
                }
                uint8_t *pDestination = pCurrent->pImage;
                const uint8_t *pTemporaryRow = abCellRows;
                for (int32_t nRow = 0; nRow < pCurrent->nHeight; ++nRow)
                {
                    memcpy(pDestination, pTemporaryRow, pCurrent->nWidth);
                    pDestination += pCurrent->nPitch;
                    pTemporaryRow += 5;
                }
            }
            return;
        }

        uint8_t *pRow = pCurrent->pImage;
        for (int32_t nRow = 0; nRow < pCurrent->nHeight; ++nRow)
        {
            memset(pRow, 0, pCurrent->nWidth);
            pRow += pCurrent->nPitch;
        }
        return;
    }

    uint8_t *pRow = pCurrent->pImage;
    int32_t nIndexBits = 0;
    int32_t nProbe = 1;
    while (pCurrent->pCodebook[nProbe] != pCurrent->pCodebook[nProbe - 1])
    {
        nProbe = 1 << ++nIndexBits;
        if (nProbe + 1 > 4)
            break;
    }

    if (!nIndexBits)
    {
        const uint8_t bColor = **ppPaletteCursor;
        for (int32_t nRow = 0; nRow < pCurrent->nHeight; ++nRow)
        {
            memset(pRow, bColor, pCurrent->nWidth);
            pRow += pCurrent->nPitch;
        }
    }
    else
    {
        for (int32_t nRow = 0; nRow < pCurrent->nHeight; ++nRow)
        {
            for (int32_t nX = 0; nX < pCurrent->nWidth; ++nX)
                pRow[nX] = (*ppPaletteCursor)[CODEC_ReadBits(pPixels, nIndexBits)];
            pRow += pCurrent->nPitch;
        }
    }
    *ppPaletteCursor += 4;
}


//1.10f: D2CMP.0x6FDF5520
//  The codebook pass: per cell, in CODEC_AdvanceDccCell's order, its four codebook entries into the span, and its history slot pointed at them.
//  The original inlines CODEC_StartDccCells and CODEC_ReadBits here.
//  A cell seen before may be equal (one bit, when there is an equal-cell stream): no codebook, the history keeping the old one. Otherwise its
//  4-bit change mask (15 for a cell never seen), and when the mask is not 0 and there is an encoding-type stream, one bit for raw codes.
//  Entries 3 down to 0: one whose bit is clear copies the previous codebook's; one whose bit is set is decoded (8 raw bits, or the last decoded
//  entry plus 4-bit steps, each added while the step is 15, the byte wrapping) until an entry decodes equal to the one decoded before it (the
//  first compares with 0). Then the decoded entries after that one are packed down over the set slots from the lowest, the set slots left over
//  cleared, and the clear slots below the stop copied from the previous codebook. The span moves on 4 a cell decoded.
//  *pConsumedPixelBits is how far the pixel-code reader moved, and pCodebooks->nBytes the bytes written.
//  BUG: the span is gabCodecCodebooks, 0x40000 bytes, four a decoded cell, and nothing bounds the cells, so a larger direction writes past it.
//  NOT ESTABLISHED: whether a stock .dcc exceeds the extent is not known.
void __fastcall CODEC_DecodeDccCodebooks(D2CmpDccDirectionContext *pDirection, D2CmpDccCellState *pHistory,
                                         D2CmpCodecBitReader *pEqualCells, D2CmpCodecBitReader *pMasks,
                                         D2CmpCodecBitReader *pEncodingTypes, D2CmpCodecBitReader *pRawCodes,
                                         D2CmpCodecBitReader *pPixelCodes, D2CmpCodecByteSpan *pCodebooks,
                                         int32_t *pConsumedPixelBits)
{
    uint8_t *pOutput = pCodebooks->pData;
    const int32_t nStartBit = pPixelCodes->nBitPosition + 32 * (int32_t)(pPixelCodes->pWord - pPixelCodes->pStart);

    D2CmpDccCellCursor tCursor;
    if (CODEC_StartDccCells(pDirection, &tCursor))
    {
        do
        {
            D2CmpDccCellState *pCell = &pHistory[tCursor.nCellIndex];
            const uint8_t *pPrevious = pCell->pCodebook;
            uint32_t dwChangeMask = 15;
            if (pPrevious)
            {
                if (pEqualCells && CODEC_ReadBits(pEqualCells, 1))
                    continue;
                dwChangeMask = CODEC_ReadBits(pMasks, 4);
            }

            int32_t bRawCodes = 0;
            if (dwChangeMask && pEncodingTypes)
                bRawCodes = CODEC_ReadBits(pEncodingTypes, 1);

            //  The slot mask has bits 4..31 set (0xFFFFFFF0), so each slot search stops at slot 4; bit n is tested through
            //  Fog's gdwBitMasks[n & 0x1F] in word n >> 5, which is always word 0 here. A clear slot takes the previous
            //  codebook's own entry; with no previous codebook the mask is 15 and nothing copies.
            const uint32_t adwSlots[1] = {dwChangeMask | 0xFFFFFFF0};
            uint8_t *pNext = pOutput + 4;
            uint8_t bPrevious = 0;
            bool bStopped = false;
            int32_t nSlot = 4;
            do
            {
                --nSlot;
                if (!(gdwBitMasks[nSlot & 0x1F] & adwSlots[nSlot >> 5]))
                {
                    pOutput[nSlot] = pPrevious[nSlot];
                    continue;
                }

                uint8_t bCode;
                if (bRawCodes)
                {
                    bCode = (uint8_t)CODEC_ReadBits(pRawCodes, 8);
                }
                else
                {
                    bCode = bPrevious;
                    uint32_t dwStep;
                    do
                    {
                        dwStep = CODEC_ReadBits(pPixelCodes, 4);
                        bCode += (uint8_t)dwStep;
                    }
                    while (dwStep == 15);
                }
                pOutput[nSlot] = bCode;
                if (bPrevious == bCode)
                {
                    bStopped = true;
                    break;
                }
                bPrevious = bCode;
            }
            while (nSlot);

            if (bStopped)
            {
                int32_t nRead = nSlot;
                int32_t nWrite = -1;
                for (;;)
                {
                    do
                        ++nRead;
                    while (!(gdwBitMasks[nRead & 0x1F] & adwSlots[nRead >> 5]));
                    if (nRead >= 4)
                        break;
                    do
                        ++nWrite;
                    while (!(gdwBitMasks[nWrite & 0x1F] & adwSlots[nWrite >> 5]));
                    pOutput[nWrite] = pOutput[nRead];
                }
                for (;;)
                {
                    do
                        ++nWrite;
                    while (!(gdwBitMasks[nWrite & 0x1F] & adwSlots[nWrite >> 5]));
                    if (nWrite >= 4)
                        break;
                    pOutput[nWrite] = 0;
                }
                while (nSlot)
                {
                    --nSlot;
                    if (!(gdwBitMasks[nSlot & 0x1F] & adwSlots[nSlot >> 5]))
                        pOutput[nSlot] = pPrevious[nSlot];
                }
            }

            pCell->pCodebook = pOutput;
            pOutput = pNext;
        }
        while (CODEC_AdvanceDccCell(pDirection, &tCursor));
    }

    *pConsumedPixelBits = pPixelCodes->nBitPosition + 32 * (int32_t)(pPixelCodes->pWord - pPixelCodes->pStart) - nStartBit;
    pCodebooks->nBytes = (int32_t)(pOutput - pCodebooks->pData);
}


//1.10f: D2CMP.0x6FDF59E0
//  Maps every codebook byte through the inverse palette, the byte being an unsigned index.
void __fastcall CODEC_RemapDccCodebooks(D2CmpCodecByteSpan *pCodebooks, const uint8_t *pInverseMap)
{
    for (int32_t nByte = 0; nByte < pCodebooks->nBytes; ++nByte)
        pCodebooks->pData[nByte] = pInverseMap[pCodebooks->pData[nByte]];
}


//1.10f: D2CMP.0x6FDF5A10
//  Called by SPRITECACHE_SpriteCacheInit (#10052): initializes the critical section, once.
void __cdecl CODEC_InitCriticalSection()
{
    if (!gbCodecCriticalSectionInitialized)
    {
        InitializeCriticalSection(&gCodecCriticalSection);
        gbCodecCriticalSectionInitialized = 1;
    }
}


//1.10f: D2CMP.0x6FDF5A30
//  Called by SPRITECACHE_SpriteCacheDestroy (#10053): enters the critical section (so a decode in progress is waited for), clears the flag and
//  deletes the critical section while held; it is never left.
void __cdecl CODEC_ShutdownCriticalSection()
{
    if (gbCodecCriticalSectionInitialized)
    {
        EnterCriticalSection(&gCodecCriticalSection);
        gbCodecCriticalSectionInitialized = 0;
        DeleteCriticalSection(&gCodecCriticalSection);
    }
}
