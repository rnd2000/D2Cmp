#pragma once

#include "DLLmain.h"

//  PalShift.h -- the functions of PalShift.cpp, declared, and the unit's two globals. PalShiftInit, PalShiftSetAct and PalShiftGetTransform are the
//  exports #10096..#10098 and are declared extern "C"; LoadItemPaletteGroup is called by PalShiftInit alone. The comment line "D2CMP 0x6F..." above a
//  global gives its address in the original.

extern "C" void __stdcall PALSHIFT_PalShiftInit();                                                   // #10096
void __fastcall PALSHIFT_LoadItemPaletteGroup(const char *szBaseName, int32_t nGroupIndex);
extern "C" void __stdcall PALSHIFT_PalShiftSetAct(int32_t nAct);                                         // #10097
extern "C" uint8_t *__stdcall PALSHIFT_PalShiftGetTransform(int32_t nTransform, int32_t nColor);     // #10098

//  D2CMP 0x6FE0EF64
//  The act PalShiftSetAct (#10097) stores, 1 to 5 (anything else is ignored); it starts at 1. Nothing in the module reads it.
inline int32_t gnPalShiftAct = 1;

//  D2CMP 0x6FE95C18
//  The item palette shifts: nine groups of 105 rows of 256 bytes, each row a palette-index map. PalShiftInit loads groups 1 to 8, one .dat file of
//  1500h bytes each, into their first 21 rows; group 0 and rows 21..104 are never written (zero at load: the original's array lies in .bss).
inline uint8_t gPalShiftTransforms[9][105][256] = {};
