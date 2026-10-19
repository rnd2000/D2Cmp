#pragma once

#include "DLLmain.h"

//  Palette.h -- the functions of Palette.cpp, declared, and the unit's globals. The exports #10000..#10012 are declared extern "C"; the other
//  functions are called inside the unit only. The 21 read-only floating-point constants the unit reads (gdPalette*, gfPalette*;
//  0x6FE0BB10..0x6FE0BBB7 in the original) are defined in D2Constants.h, bit for bit. The comment line "D2CMP 0x6F..." above a global gives its
//  address in the original.

extern "C" void __stdcall PALETTE_UpdateD2TransparencyTable(D2CmpPaletteTables *pTables, int32_t nUnused);   // #10003
void __fastcall PALETTE_BuildAdditiveTable(D2CmpPaletteTables *pTables);
extern "C" D2CmpPaletteTables *__stdcall PALETTE_CreateD2Palette(const D2CmpPaletteEntry *pPalette);     // #10000
void __fastcall PALETTE_BuildMultiplyTable(D2CmpPaletteTables *pTables);
void __fastcall PALETTE_BuildDarkenAndDarkBlend(D2CmpPaletteTables *pTables);
void __fastcall PALETTE_BuildStandardShifts(D2CmpPaletteTables *pTables);
void __fastcall PALETTE_BuildBrightenTables(D2CmpPaletteTables *pTables);
void __fastcall PALETTE_BuildColorShiftTables(D2CmpPaletteTables *pTables, const D2CmpHSL *pSourceHSL);
void __fastcall PALETTE_HSLToRGB256(const D2CmpHSL *pHSL, D2CmpPaletteEntry *pRGB);
void __fastcall PALETTE_BuildLuminanceTable(D2CmpPaletteTables *pTables, D2CmpHSL *pHSL);
extern "C" D2CmpPaletteTables *__stdcall PALETTE_CreateD2PaletteByteStream(const D2CmpPaletteEntry *pPalette, uint32_t *pdwSize);    // #10001
extern "C" void __stdcall PALETTE_DestroyD2Palette(D2CmpPaletteTables *pTables);                         // #10002
extern "C" uint8_t __stdcall PALETTE_GetClosestColor(const D2CmpPaletteEntry *pPalette, uint32_t nColors, int32_t nRed, int32_t nGreen, int32_t nBlue);      // #10004
extern "C" uint8_t __stdcall PALETTE_GetFurthestColor(const D2CmpPaletteEntry *pPalette, uint32_t nColors, int32_t nRed, int32_t nGreen, int32_t nBlue);     // #10005
extern "C" void __stdcall PALETTE_PaletteSetTransparency(uint32_t dwColor);                          // #10009
extern "C" int32_t __stdcall PALETTE_PaletteInit(const char *pszPalette, int32_t nPaletteStartEntry, int32_t nPaletteEndEntry);                                 // #10006
extern "C" void __stdcall PALETTE_PaletteTestBackground(int32_t bTestBackground);                            // #10010
extern "C" uint8_t __stdcall PALETTE_Palette24BitTo8BitPixel(uint8_t nBlue, uint8_t nGreen, uint8_t nRed);         // #10007
// #10008
extern "C" int32_t __stdcall PALETTE_Palette24BitTo8BitImage(const uint8_t *pSource, uint8_t *pDest, uint32_t nPixels, uint8_t (__stdcall *pfnConvert)(uint8_t, uint8_t, uint8_t));
extern "C" void __stdcall PALETTE_PaletteFixBlack(int32_t bFixBlack);                                        // #10011
extern "C" void __stdcall PALETTE_PaletteSetMask(const uint8_t *pMask);                          // #10012
void *__fastcall PALETTE_VirtualAllocRWX(uint32_t dwSize);
BOOL __fastcall PALETTE_VirtualRelease(void *pAllocation);

//  D2CMP 0x6FE0EDB8
//  The thirteen standard colours, red, green, blue each, D2's text colours: white, red, green, blue, gold, grey, black, tan, orange, yellow, dark
//  green, purple, light green. CreateD2Palette copies them into the tables; BuildStandardShifts tints by entries 1..12 (entry 0 has no row).
inline const D2CmpRGB24 gaPaletteStandardColors[13] = {
    {0xFF, 0xFF, 0xFF}, {0xFF, 0x4D, 0x4D}, {0x00, 0xFF, 0x00}, {0x69, 0x69, 0xFF}, {0xC7, 0xB3, 0x77},
    {0x69, 0x69, 0x69}, {0x00, 0x00, 0x00}, {0xD0, 0xC2, 0x7D}, {0xFF, 0xA8, 0x00}, {0xFF, 0xFF, 0x64},
    {0x00, 0x80, 0x00}, {0xAE, 0x00, 0xFF}, {0x00, 0xC8, 0x00},
};

//  D2CMP 0x6FE94BF8
//  n * n for n = -256..255, at [n + 256]. PaletteInit fills it; Palette24BitTo8BitPixel reads it with a channel difference, -255..255.
inline int32_t gnPaletteSquares[512] = {};

//  D2CMP 0x6FE953F8
//  PaletteTestBackground's flag: Palette24BitTo8BitPixel answers 0 for the transparent colour while it is set.
inline int32_t gbPaletteTestBackground = 0;

//  D2CMP 0x6FE953FC
//  The last entry Palette24BitTo8BitPixel tries: PaletteInit's nPaletteEndEntry, compared unsigned.
inline uint32_t gnPaletteEndEntry = 0;

//  D2CMP 0x6FE95400
//  The first entry Palette24BitTo8BitPixel tries: PaletteInit stores 1 here, not its nPaletteStartEntry.
inline uint32_t gnPaletteStartEntry = 0;

//  D2CMP 0x6FE95408
//  A 768-byte palette file as PaletteInit reads it, three bytes an entry.
inline uint8_t gabPaletteFileBytes[768] = {};

//  D2CMP 0x6FE95708
//  PaletteSetMask's copy: while gbPaletteMaskEnabled is set, an entry whose byte is 0 is never chosen.
inline uint8_t gabPaletteMask[256] = {};

//  D2CMP 0x6FE95808
//  PaletteInit's palette: red, green, blue, flags (the flags byte is never written). Palette24BitTo8BitPixel matches against it.
inline D2CmpPaletteEntry gaPaletteEntries[256] = {};

//  D2CMP 0x6FE95C08
//  1 once PaletteInit has a palette; Palette24BitTo8BitImage converts nothing without it.
inline int32_t gbPaletteLoaded = 0;

//  D2CMP 0x6FE95C0C
//  The transparent colour: PaletteSetTransparency's argument with its low and third bytes swapped; 0 after PaletteInit.
inline uint32_t gdwPaletteTransparentColor = 0;

//  D2CMP 0x6FE95C10
//  PaletteFixBlack's flag: while set, Palette24BitTo8BitImage turns a pixel whose channels are all 10 or less into blue 4, green 0, red 0.
inline int32_t gbPaletteFixBlack = 0;

//  D2CMP 0x6FE95C14
//  1 once PaletteSetMask has been called; never cleared.
inline int32_t gbPaletteMaskEnabled = 0;
