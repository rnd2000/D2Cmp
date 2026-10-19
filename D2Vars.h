#ifdef _D2VARS_H
#define VAR(Type, Name)         Type Name;
#else
#define VAR(Type, Name)         extern Type Name;
#endif

/****************************************************************************
*                                                                           *
*   D2Vars.h                                                                *
*                                                                           *
*   Licensed under the Apache License, Version 2.0 (the "License");         *
*   you may not use this file except in compliance with the License.        *
*   You may obtain a copy of the License at                                 *
*                                                                           *
*   http://www.apache.org/licenses/LICENSE-2.0                              *
*                                                                           *
*   Unless required by applicable law or agreed to in writing, software     *
*   distributed under the License is distributed on an "AS IS" BASIS,       *
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
*   See the License for the specific language governing permissions and     *
*   limitations under the License.                                          *
*                                                                           *
*---------------------------------------------------------------------------*
*                                                                           *
*   https://github.com/olivier-verville/D2Template                          *
*                                                                           *
*   This file is used to declare your own global variables to use           *
*   within your code. These variables can be used anywhere in your code     *
*                                                                           *
*****************************************************************************/


///////////////////////////////////////////////////////////////////////////////
//
//  GENERATED from the reverse-engineering database of D2CMP.dll 1.10 (image base 0x6FDF0000).
//
//  Every global of the original, at its RVA. Every line is commented out: this source holds its own copy
//  of each in the module header the line names, or, for "OURS BY ITS READERS", the unit's own constant
//  or literal stands for it. Ordered by address.
//
///////////////////////////////////////////////////////////////////////////////


// ---- .text ----
// OURS, FastCmp.cpp: D2VAR(D2CMP, gaFastCmpDC6MapBlendRemainderTargets, unsigned __int32, 0x6370)   // [8]
// OURS, FastCmp.cpp: D2VAR(D2CMP, gaFastCmpDC6MapBlendClippedRemainderTargets, unsigned __int32, 0x666C)   // [8]
// OURS, FastCmp.cpp: D2VAR(D2CMP, gaFastCmpDC6PreMapMapRemainderTargets, unsigned __int32, 0x6810)   // [8]
// OURS, FastCmp.cpp: D2VAR(D2CMP, gaFastCmpDC6PreMapMapClippedRemainderTargets, unsigned __int32, 0x6A38)   // [8]
// OURS, FastCmp.cpp: D2VAR(D2CMP, gaFastCmpDC6MapRemainderTargets, unsigned __int32, 0x6B78)   // [8]
// OURS, FastCmp.cpp: D2VAR(D2CMP, gaFastCmpDC6MapClippedRemainderTargets, unsigned __int32, 0x6D44)   // [8]
// OURS, FastCmp.cpp: D2VAR(D2CMP, gaFastCmpDC6PreMapBlendRemainderTargets, unsigned __int32, 0x6FB0)   // [8]
// OURS, FastCmp.cpp: D2VAR(D2CMP, gaFastCmpDC6PreMapBlendClippedRemainderTargets, unsigned __int32, 0x72AC)   // [8]
// OURS, FastCmp.cpp: D2VAR(D2CMP, gaFastCmpDC6PreMapRemainderTargets, unsigned __int32, 0x73E8)   // [8]
// OURS, FastCmp.cpp: D2VAR(D2CMP, gaFastCmpDC6PreMapClippedRemainderTargets, unsigned __int32, 0x75B4)   // [8]
// OURS, FastCmp.cpp: D2VAR(D2CMP, gaFastCmpDC6BlendRemainderTargets, unsigned __int32, 0x77C0)   // [8]
// OURS, FastCmp.cpp: D2VAR(D2CMP, gaFastCmpDC6BlendClippedRemainderTargets, unsigned __int32, 0x7A58)   // [8]

// ---- .rdata ----
// OURS, CelCmp.h: D2VAR(D2CMP, gpCellBuilderEncoders, const D2CmpCellBuilderEncoder, 0x191B0)   // [2]
// OURS, CelCmp.h: D2VAR(D2CMP, sgpfnDecodeFunc, const D2CmpCelDecodeFunc, 0x191B8)   // [2]
// OURS, CelCmp.h: D2VAR(D2CMP, gpfnCelConvert24To8, const D2CmpCelConvert24To8Func, 0x191C0)   // [2]
// OURS, CelCmp.h: D2VAR(D2CMP, gpfnCelTransform, const D2CmpCelTransformFunc, 0x191C8)   // [2]
// OURS, CelCmp.h: D2VAR(D2CMP, gnDirection32ToFrameIndex, const uint32_t, 0x19550)   // [736]
// OURS, CelCmp.h: D2VAR(D2CMP, gnDirection64ToFrameIndex, const uint32_t, 0x1A0D0)   // [512]
// OURS, CelCmp.h: D2VAR(D2CMP, gnCelShadowGameDirection, const int32_t, 0x1A8D0)   // [512]
// OURS, CelCmp.h: D2VAR(D2CMP, gnCelNormalDirection, const int32_t, 0x1B0D0)   // [512]
// OURS, CelCmp.h: D2VAR(D2CMP, gCelDirection8To4Default, const uint8_t, 0x1B8D0)   // [8]
// OURS, CelCmp.h: D2VAR(D2CMP, gCelDirection16To8Default, const uint8_t, 0x1B8D8)   // [16]
// OURS, CelCmp.h: D2VAR(D2CMP, gCelDirection8To4Alternate, const uint8_t, 0x1B8E8)   // [8]
// OURS, CelCmp.h: D2VAR(D2CMP, gCelDirection16To8Alternate, const uint8_t, 0x1B8F0)   // [16]
// OURS, CelCmp.h: D2VAR(D2CMP, gVtblCellBuilderTableBase, const D2CmpCellBuilderTableVtable, 0x1B900)
// OURS, CelCmp.h: D2VAR(D2CMP, gVtblCellBuilderTableReuse, const D2CmpCellBuilderTableVtable, 0x1B910)
// OURS, CelCmp.h: D2VAR(D2CMP, gVtblCellBuilderTableSync, const D2CmpCellBuilderTableVtable, 0x1B920)
// OURS, Codec.h: D2VAR(D2CMP, gaCodecFieldBits, const int32_t, 0x1B940)   // [16]
// OURS, Codec.h: D2VAR(D2CMP, gadwCodecSignBits, const uint32_t, 0x1B980)   // [32]
// OURS, Codec.h: D2VAR(D2CMP, gadwCodecLowMasks, const uint32_t, 0x1BA00)   // [33]
// OURS, Codec.h: D2VAR(D2CMP, gadwCodecSignExtend, const uint32_t, 0x1BA88)   // [32]
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteHueCircle, const double, 0x1BB10)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteHueSextant, const double, 0x1BB18)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteBlueHueOffset, const double, 0x1BB20)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteTwo, const double, 0x1BB28)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteOne, const double, 0x1BB30)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteZero, const double, 0x1BB38)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteHalf, const double, 0x1BB40)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteInv255, const volatile double, 0x1BB48)
// OURS, Palette.cpp: D2VAR(D2CMP, gfPaletteFixedHueStep, const float, 0x1BB50)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteSelectiveHueMax, const double, 0x1BB58)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteSelectiveHueMin, const double, 0x1BB60)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteGrayLightBias, const double, 0x1BB68)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteGrayLightScale, const double, 0x1BB70)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteLuminanceBoost, const double, 0x1BB78)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteLuminanceReduction, const double, 0x1BB80)
// OURS, Palette.cpp: D2VAR(D2CMP, gfPaletteHueStep, const float, 0x1BB88)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteHueBlueBoundary, const double, 0x1BB90)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteHueGreenBoundary, const double, 0x1BB98)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteInv60, const volatile double, 0x1BBA0)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteChannelHueOffset, const double, 0x1BBA8)
// OURS, Palette.cpp: D2VAR(D2CMP, gdPaletteChannelScale255, const double, 0x1BBB0)

// ---- .data ----
// OURS, Codec.cpp: D2VAR(D2CMP, Format, char, 0x1D608)   // []
// OURS, FastCmp.cpp: D2VAR(D2CMP, szFastCmpSourcePath, char, 0x1D790)   // []
// OURS, FastCmp.cpp: D2VAR(D2CMP, szFastCmpAssertLineRange, char, 0x1D7F4)   // []
// OURS, FastCmp.cpp: D2VAR(D2CMP, szFastCmpAssertStride, char, 0x1D828)   // []
// OURS, Palette.h: D2VAR(D2CMP, gaPaletteStandardColors, const D2CmpRGB24, 0x1EDB8)   // [13]
// OURS, Palette.cpp: D2VAR(D2CMP, szPaletteAssertLuminance, char, 0x1EDE0)   // []
// OURS, Palette.cpp: D2VAR(D2CMP, szPaletteAssertSaturation, char, 0x1EE00)   // []
// OURS, Palette.cpp: D2VAR(D2CMP, szPaletteAssertHue, char, 0x1EE20)   // []
// OURS, Palette.cpp: D2VAR(D2CMP, szPaletteAssertInput, char, 0x1EE40)   // []
// OURS, Palette.cpp: D2VAR(D2CMP, szPaletteSourcePath, char, 0x1EE48)   // []
// OURS, Palette.cpp: D2VAR(D2CMP, OutputString, CHAR, 0x1EED4)   // []
// OURS, PalShift.h: D2VAR(D2CMP, gnPalShiftAct, int32_t, 0x1EF64)
// OURS, PalShift.cpp: D2VAR(D2CMP, szMsg, char, 0x1EFB0)   // [2]
// OURS, PalShift.cpp: D2VAR(D2CMP, szFile, char, 0x1EFB4)   // []
// OURS, SpriteCache.h: D2VAR(D2CMP, gnCelHeaderCacheEntryCount, uint32_t, 0x1F054)
// OURS, SpriteCache.h: D2VAR(D2CMP, gdwSpriteCompressedDataSetting, uint32_t, 0x1F058)
// OURS, SpriteCache.h: D2VAR(D2CMP, gpszDccExtension, const char *, 0x1F05C)
// OURS, SpriteCache.h: D2VAR(D2CMP, gpszDc6Extension, const char *, 0x1F060)
// OURS, SpriteCache.cpp: D2VAR(D2CMP, szSourceFile, char, 0x1F0E4)   // []
// OURS, SpriteCache.cpp: D2VAR(D2CMP, szFormat, char, 0x1F674)   // []
// OURS, Tilecmp.h: D2VAR(D2CMP, gaFloorComponentOffsets, int32_t, 0x1F7E8)   // [25][2]
// OURS, CelCmp.h: D2VAR(D2CMP, gCellBuilderTable, D2CmpCellBuilderTablePrefix, 0x22CA8)
// OURS, CelCmp.h: D2VAR(D2CMP, gCelHardwareFreeCallback, D2CmpCelHardwareFreeCallback, 0x22D10)
// OURS, Codec.h: D2VAR(D2CMP, gaCodecFrames, D2CmpDccFrameState, 0x22D18)   // [256]
// OURS, Codec.h: D2VAR(D2CMP, gaCodecCellHistory, D2CmpDccCellState, 0x25918)   // [5625]
// OURS, Codec.h: D2VAR(D2CMP, gabCodecCodebooks, uint8_t, 0x41090)   // [262144]
// OURS, Codec.h: D2VAR(D2CMP, gabCodecImage, uint8_t, 0x81090)   // [120000]
// OURS, Codec.h: D2VAR(D2CMP, gCodecCriticalSection, CRITICAL_SECTION, 0x9E550)
// OURS, Codec.h: D2VAR(D2CMP, gbCodecCriticalSectionInitialized, int32_t, 0x9E568)
// OURS, FastCmp.h: D2VAR(D2CMP, gaLensLeftBoundary, int32_t, 0x9E570)   // [80]
// OURS, FastCmp.h: D2VAR(D2CMP, gaLensRightBoundary, int32_t, 0x9E6B0)   // [80]
// OURS, FastCmp.h: D2VAR(D2CMP, gaLensSourceOffset, int32_t, 0x9E7F0)   // [6400]
// OURS, FastCmp.h: D2VAR(D2CMP, gbFastCmpUseGenericDecoder, int32_t, 0xA4BF0)
// OURS, Palette.h: D2VAR(D2CMP, gnPaletteSquares, int32_t, 0xA4BF8)   // [512]
// OURS, Palette.h: D2VAR(D2CMP, gbPaletteTestBackground, int32_t, 0xA53F8)
// OURS, Palette.h: D2VAR(D2CMP, gnPaletteEndEntry, uint32_t, 0xA53FC)
// OURS, Palette.h: D2VAR(D2CMP, gnPaletteStartEntry, uint32_t, 0xA5400)
// OURS, Palette.h: D2VAR(D2CMP, gabPaletteFileBytes, uint8_t, 0xA5408)   // [768]
// OURS, Palette.h: D2VAR(D2CMP, gabPaletteMask, uint8_t, 0xA5708)   // [256]
// OURS, Palette.h: D2VAR(D2CMP, gaPaletteEntries, D2CmpPaletteEntry, 0xA5808)   // [256]
// OURS, Palette.h: D2VAR(D2CMP, gbPaletteLoaded, int32_t, 0xA5C08)
// OURS, Palette.h: D2VAR(D2CMP, gdwPaletteTransparentColor, uint32_t, 0xA5C0C)
// OURS, Palette.h: D2VAR(D2CMP, gbPaletteFixBlack, int32_t, 0xA5C10)
// OURS, Palette.h: D2VAR(D2CMP, gbPaletteMaskEnabled, int32_t, 0xA5C14)
// OURS, PalShift.h: D2VAR(D2CMP, gPalShiftTransforms, uint8_t, 0xA5C18)   // [9][105][256]
// OURS, SpriteCache.h: D2VAR(D2CMP, gCelHeaderHash, D2CmpCelHeaderHash, 0xE0D18)
// OURS, SpriteCache.h: D2VAR(D2CMP, gSpriteGfxHashStorage, D2CmpSpriteGfxHashStorage, 0xE1D18)
// OURS, SpriteCache.h: D2VAR(D2CMP, sgtGfxCache, D2CmpSpriteDataCache, 0xF1D18)
// OURS, SpriteCache.h: D2VAR(D2CMP, gCelHeaderLruCache, D2CmpLruCache, 0xF1D54)
// OURS, SpriteCache.h: D2VAR(D2CMP, gSpriteHeaderFileOffsets, uint32_t, 0xF1D88)   // [16384]
// OURS, SpriteCache.h: D2VAR(D2CMP, gbSpriteLowEndMachine, int32_t, 0x101D88)
// OURS, SpriteCache.h: D2VAR(D2CMP, gSpriteHardwareFreeCallback, D2CmpSpriteHardwareFreeCallback, 0x101D8C)
// OURS, SpriteCache.h: D2VAR(D2CMP, gbSpriteHandleErrorReported, int32_t, 0x101D90)
// OURS, SpriteCache.h: D2VAR(D2CMP, gszEmptyString, char, 0x101D94)   // [4]
// OURS, TileCache.h: D2VAR(D2CMP, gTileLruCache, D2CmpLruCache, 0x101D98)
// OURS, TileCache.h: D2VAR(D2CMP, gbTileCacheInitialized, int32_t, 0x101DCC)
// OURS, TileCache.h: D2VAR(D2CMP, gnTileCacheConfiguredBytes, uint32_t, 0x101DD0)
// OURS, TileCache.h: D2VAR(D2CMP, sgpfnHardwareFreeCallback, D2CmpTileHardwareFreeCallback, 0x101DD4)
// OURS, TileProjects.h: D2VAR(D2CMP, gpTileProjectRecords, D2TileRecordStrc *, 0x101DD8)

///////////////////////////////////////////////////////////////////////////////
//
//  NOT EXPRESSIBLE THROUGH D2VAR -- listed, not dropped.
//
//
///////////////////////////////////////////////////////////////////////////////


// end of file ---------------------------------------------------------------
#undef _D2VARS_H
