#pragma once

#include "DLLmain.h"

//  Codec.h -- the declarations of Codec.cpp's functions and the unit's globals: four lookup tables of the bit reader and the header decode, the four
//  scratch buffers of one decode, and the critical section that guards them with its flag. Each global is a C++17 `inline` variable, one object for
//  every file that includes this header. The comment line "D2CMP 0x6F..." above a global gives its address in the original.
//  None of the 14 functions is exported. SpriteCache.cpp calls four of them: SPRITECACHE_ResolveHeader calls CODEC_ReadDccHeader,
//  SPRITECACHE_RunDecodeJob calls CODEC_DecodeDccDirections, SPRITECACHE_SpriteCacheInit (#10052) calls CODEC_InitCriticalSection and
//  SPRITECACHE_SpriteCacheDestroy (#10053) calls CODEC_ShutdownCriticalSection. The other ten are called inside the unit only.

int32_t __fastcall CODEC_EncodeDccFrameRle(D2CmpDccFrameState *pFrame, D2CmpSpriteFrameHeaderPrefix *pOutputFrame, uint8_t *pDestination);
int32_t __fastcall CODEC_ReadDccHeader(const char *szPath, D2CmpSpriteHeaderData *pHeader);
void *__fastcall CODEC_DecodeDccDirections(void *pInput, void *pPool, int32_t nInputBytes, D2CmpSpriteHeaderData *pHeader, D2CmpSpriteGfxBlockPrefix *pGfxBlock, int32_t nStartDirection, int32_t nDirections, uint32_t *pDecodedBytes, const char *szKey);
void __fastcall CODEC_SkipBits(D2CmpCodecBitReader *pReader, int32_t nBits);
void __fastcall CODEC_AlignReaderToByte(D2CmpCodecBitReader *pReader);
uint32_t __fastcall CODEC_ReadBits(D2CmpCodecBitReader *pReader, int32_t nBits);
int32_t __fastcall CODEC_AdvanceDccCell(D2CmpDccDirectionContext *pDirection, D2CmpDccCellCursor *pCursor);
int32_t __fastcall CODEC_StartDccCells(D2CmpDccDirectionContext *pDirection, D2CmpDccCellCursor *pCursor);
void __fastcall CODEC_BuildDccPaletteMaps(const uint8_t *pPresence, D2CmpDccPaletteMap *pForward, D2CmpDccPaletteMap *pInverse);
void __fastcall CODEC_RasterizeDccCell(D2CmpDccCellState *pCurrent, D2CmpCodecBitReader *pPixels, D2CmpDccCellState *pPrevious, D2CmpCodecBitReader *pEqualCells, uint8_t **ppPaletteCursor);
void __fastcall CODEC_DecodeDccCodebooks(D2CmpDccDirectionContext *pDirection, D2CmpDccCellState *pHistory,
                                         D2CmpCodecBitReader *pEqualCells, D2CmpCodecBitReader *pMasks,
                                         D2CmpCodecBitReader *pEncodingTypes, D2CmpCodecBitReader *pRawCodes,
                                         D2CmpCodecBitReader *pPixelCodes, D2CmpCodecByteSpan *pCodebooks,
                                         int32_t *pConsumedPixelBits);
void __fastcall CODEC_RemapDccCodebooks(D2CmpCodecByteSpan *pCodebooks, const uint8_t *pInverseMap);
void __cdecl CODEC_InitCriticalSection();
void __cdecl CODEC_ShutdownCriticalSection();

//  D2CMP 0x6FE0B940
//  The bit width each of the 16 four-bit field codes of a direction header selects (.rdata).
inline const int32_t gaCodecFieldBits[16] = {0, 1, 2, 4, 6, 8, 10, 12, 14, 16, 20, 24, 26, 28, 30, 32};

//  D2CMP 0x6FE0B980
//  1 << i, i = 0..31: the sign bit of an (i + 1)-bit field; it is read at [width - 1].
inline const uint32_t gadwCodecSignBits[32] = {
    0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080,
    0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000,
    0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000,
    0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000,
};

//  D2CMP 0x6FE0BA00
//  (1 << i) - 1, i = 0..32: the low-bit mask of an i-bit field, read by CODEC_ReadBits with the width.
inline const uint32_t gadwCodecLowMasks[33] = {
    0x00000000, 0x00000001, 0x00000003, 0x00000007, 0x0000000F, 0x0000001F, 0x0000003F, 0x0000007F,
    0x000000FF, 0x000001FF, 0x000003FF, 0x000007FF, 0x00000FFF, 0x00001FFF, 0x00003FFF, 0x00007FFF,
    0x0000FFFF, 0x0001FFFF, 0x0003FFFF, 0x0007FFFF, 0x000FFFFF, 0x001FFFFF, 0x003FFFFF, 0x007FFFFF,
    0x00FFFFFF, 0x01FFFFFF, 0x03FFFFFF, 0x07FFFFFF, 0x0FFFFFFF, 0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF,
    0xFFFFFFFF,
};

//  D2CMP 0x6FE0BA88
//  ~((1 << i) - 1), i = 0..31: the bits a negative i-bit field takes when sign-extended; the original reads one dword past it for a width of 32.
inline const uint32_t gadwCodecSignExtend[32] = {
    0xFFFFFFFF, 0xFFFFFFFE, 0xFFFFFFFC, 0xFFFFFFF8, 0xFFFFFFF0, 0xFFFFFFE0, 0xFFFFFFC0, 0xFFFFFF80,
    0xFFFFFF00, 0xFFFFFE00, 0xFFFFFC00, 0xFFFFF800, 0xFFFFF000, 0xFFFFE000, 0xFFFFC000, 0xFFFF8000,
    0xFFFF0000, 0xFFFE0000, 0xFFFC0000, 0xFFF80000, 0xFFF00000, 0xFFE00000, 0xFFC00000, 0xFF800000,
    0xFF000000, 0xFE000000, 0xFC000000, 0xF8000000, 0xF0000000, 0xE0000000, 0xC0000000, 0x80000000,
};

//  D2CMP 0x6FE12D18
//  One direction's frames as CODEC_DecodeDccDirections parses them: 256 records of 44 bytes (.bss).
inline D2CmpDccFrameState gaCodecFrames[256] = {};

//  D2CMP 0x6FE15918
//  The last state of every cell of the direction's grid: 5625 records of 20 bytes, cleared whole by the raster pass.
inline D2CmpDccCellState gaCodecCellHistory[5625] = {};

//  D2CMP 0x6FE31090
//  The direction's four-byte cell codebooks, written by CODEC_DecodeDccCodebooks and read as the raster pass's palette cursor; 0x40000 bytes.
inline uint8_t gabCodecCodebooks[0x40000] = {};

//  D2CMP 0x6FE71090
//  The direction's image: every frame drawn at its place in the direction's box, pitch the box's width; 120000 bytes.
inline uint8_t gabCodecImage[120000] = {};

//  D2CMP 0x6FE8E550
//  Held by CODEC_DecodeDccDirections for the whole decode; it guards the four scratch buffers above.
inline CRITICAL_SECTION gCodecCriticalSection = {};

//  D2CMP 0x6FE8E568
//  Set by CODEC_InitCriticalSection, cleared by CODEC_ShutdownCriticalSection.
inline int32_t gbCodecCriticalSectionInitialized = 0;
