#include "D2CMP.h"

//  Raw.cpp -- the RGB24 ("raw") cel format of D2CMP.dll 1.10: a frame is a 32-byte header followed by its pixels inline, three bytes each,
//  uncompressed. RAW_EncodeRGB24CellFrames builds such frames from a cel builder's entries; RAW_Rgb24Frame_VisitNonTransparentColors hands
//  each pixel of one frame that is not the transparent colour to a callback.
//  The original source file is D2CMP\SRC\Raw.cpp (the unit's FOG_Alloc call names it). Address range 0x6FDFB7B0..0x6FDFB929.

//  The path literal of the unit's allocation (__FILE__ in the original).
static const char szRawCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2CMP\\SRC\\Raw.cpp";

static_assert(offsetof(D2CmpCellBuilder, nFrameCount) == 0x24, "mov eax, [ecx+24h] (0x6FDFB7B3)");
static_assert(offsetof(D2CmpCellBuilder, nDirectionCount) == 0x28, "div dword ptr [ecx+28h] (0x6FDFB7BD)");
static_assert(offsetof(D2CmpCellBuilder, frames.terminator.pNextOrSentinel) == 0x34, "mov eax, [ecx+34h] (0x6FDFB7D1)");
static_assert(offsetof(D2CmpCellBuildEntry, link.pNextOrSentinel) == 0x04, "mov ebx, [ebx+4] (0x6FDFB88A)");
static_assert(offsetof(D2CmpCellBuildEntry, nPixelWidth) == 0x0C, "mov eax, [ebx+0Ch] / imul eax, [ebx+10h] (0x6FDFB7EA)");
static_assert(offsetof(D2CmpCellBuildEntry, nPixelHeight) == 0x10, "imul eax, [ebx+10h] (0x6FDFB7EE)");
static_assert(offsetof(D2CmpCellBuildEntry, nOffsetX) == 0x18, "mov eax, [ebx+18h] (0x6FDFB818)");
static_assert(offsetof(D2CmpCellBuildEntry, pPixels) == 0x20, "mov esi, [ebx+20h] (0x6FDFB831)");
static_assert(offsetof(D2CmpRgb24FrameHeader, dwWidth) == 0x04, "mov edx, [ebx+4] (0x6FDFB8C1)");
static_assert(offsetof(D2CmpRgb24FrameHeader, dwHeight) == 0x08, "imul edx, [ebx+8] (0x6FDFB8C9)");
static_assert(offsetof(D2CmpBuiltCellFile, pFrames) == 0x18 && offsetof(D2CmpRgb24RuntimeFilePrefix, pFrames) == 0x18, "the two views of the file header: mov [edx+edi*4+18h], ebp (0x6FDFB886)");


//1.10f: D2CMP.0x6FDFB7B0
//  Builds the RGB24 frames of a new cel file from the cel builder's entries. The frames per direction are nFrameCount / nDirectionCount (unsigned).
//  For each entry of the builder's frame list, in order, a frame of 32 + width * height * 3 + 3 bytes is allocated (three bytes more than
//  anything writes). Its header takes the entry's orientation, size and offsets and the pixel byte count (the auxiliary byte count is 0, the
//  hardware pointer NULL), and its body is a copy of the entry's pixels; the entry's own pixel count (+0x14) is not read.
//  The frame's slot in the file's frame table: the game direction is 1 for a builder of at most one direction, else the frame's direction index
//  times 64 over the direction count (both divisions unsigned); CELCMP_CelGameDirToFileDir (#10018) turns it into the file's direction, and the
//  frame goes to that direction's row at the frame's index within it.
//  The frame list is Storm's: its head is the terminator's next link, taken only when positive as an int, and the walk goes on while the next link
//  is positive; a link that is not positive is the terminator's, not an entry.
//  pOutput is a D2CmpBuiltCellFile, as the caller and the encoders' typedef have it, whose pFrames are typed as RLE frame headers
//  (D2CmpRleCellHeader); the frames are stored through D2CmpRgb24RuntimeFilePrefix, whose pFrames are these RGB24 frames (the same 24 bytes
//  precede the table).
//  BUG: FOG_Alloc's answer is used untested (0x6FDFB804, then the stores from 0x6FDFB809). UNREACHABLE: the stock game never runs this encoder.
//  BUG: with fewer frames than directions the frames per direction are 0, and the first entry divides by zero (0x6FDFB859 when there is more than
//  one direction, 0x6FDFB87D always): a #DE in the original, written as the bytes divide. UNREACHABLE: the stock game never runs this encoder.
void __fastcall RAW_EncodeRGB24CellFrames(D2CmpCellBuilder *pBuilder, D2CmpBuiltCellFile *pOutput)
{
    D2CmpRgb24RuntimeFilePrefix *pRgbFile = (D2CmpRgb24RuntimeFilePrefix *)pOutput;
    const uint32_t dwFramesPerDirection = pBuilder->nFrameCount / pBuilder->nDirectionCount;
    uint32_t dwFrameIndex = 0;

    D2CmpCellBuildEntry *pEntry = (intptr_t)pBuilder->frames.terminator.pNextOrSentinel <= 0
                                      ? nullptr
                                      : (D2CmpCellBuildEntry *)pBuilder->frames.terminator.pNextOrSentinel;
    while ((intptr_t)pEntry > 0)
    {
        const uint32_t dwPixelBytes = pEntry->nPixelWidth * pEntry->nPixelHeight * 3;
        D2CmpRgb24FrameHeader *pRgbFrame = (D2CmpRgb24FrameHeader *)FOG_Alloc(dwPixelBytes + 35, szRawCpp, 34, 0);
        pRgbFrame->nOrientation = pEntry->nOrientation;
        pRgbFrame->dwWidth = pEntry->nPixelWidth;
        pRgbFrame->dwHeight = pEntry->nPixelHeight;
        pRgbFrame->nOffsetX = pEntry->nOffsetX;
        pRgbFrame->nOffsetY = pEntry->nOffsetY;
        pRgbFrame->dwAuxiliaryBytes = 0;
        pRgbFrame->pHardwareData = nullptr;
        pRgbFrame->dwPixelBytes = dwPixelBytes;
        memcpy(pRgbFrame + 1, pEntry->pPixels, dwPixelBytes);

        const uint32_t nDirectionCount = pBuilder->nDirectionCount;
        const uint32_t nDirection64 = nDirectionCount <= 1 ? 1 : ((dwFrameIndex / dwFramesPerDirection) << 6) / nDirectionCount;
        const uint32_t nFileDirection = CELCMP_CelGameDirToFileDir(nDirection64, nDirectionCount);
        pRgbFile->pFrames[nFileDirection * dwFramesPerDirection + dwFrameIndex % dwFramesPerDirection] = pRgbFrame;

        pEntry = (D2CmpCellBuildEntry *)pEntry->link.pNextOrSentinel;
        ++dwFrameIndex;
    }
}


//1.10f: D2CMP.0x6FDFB8B0
//  Hands every pixel of an RGB24 frame, as 0x00RRGGBB from its three bytes (the first the low one), to pVisitColor unless it equals the low three
//  bytes of dwTransparentColor (the high byte of the key is ignored). The pixel count width * height is read again after every pixel, the
//  callback's included, and compared unsigned; the frame's byte count (+0x1C) is not read. pVisitColor is __stdcall; neither it nor the frame
//  is tested.
//  DIFFERENCE: the original loads the key as dwords at the argument's +1 and +2 (0x6FDFB8B2, 0x6FDFB8C5) and +0 (0x6FDFB8BD), masking each to its
//  low byte, so two of the loads read one and two bytes past the argument, into the caller's frame (mapped; the masks discard them). Ours reads
//  the three bytes, which is the same value.
void __fastcall RAW_Rgb24Frame_VisitNonTransparentColors(const D2CmpRgb24FrameHeader *pFrame, D2CmpPackedColorCallback pVisitColor, uint32_t dwTransparentColor)
{
    const uint8_t *pKey = (const uint8_t *)&dwTransparentColor;
    const uint32_t dwKeyColor = ((((uint32_t)pKey[2] << 8) + pKey[1]) << 8) + pKey[0];

    const uint8_t *pPixel = (const uint8_t *)(pFrame + 1);
    for (uint32_t dwPixelIndex = 0; dwPixelIndex < pFrame->dwWidth * pFrame->dwHeight; ++dwPixelIndex)
    {
        const uint32_t dwPackedColor = ((((uint32_t)pPixel[2] << 8) + pPixel[1]) << 8) + pPixel[0];
        pPixel += 3;
        if (dwPackedColor != dwKeyColor)
        {
            pVisitColor(dwPackedColor);
        }
    }
}
