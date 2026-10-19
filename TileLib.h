#pragma once

#include "DLLmain.h"

//  TileLib.h -- the functions of TileLib.cpp declared, and the unit's constant. All fourteen are exports, declared extern "C" for D2CMP.def.

//  The original's own name, out of TileLibraryAddProject's assert text ("ii < MAX_TILEPROJECT"); the value is the bound both walks test. `int`,
//  because both compares are signed (`jl`): an unsigned enum would turn the C's `ii < MAX_TILEPROJECT` into a `jb`.
constexpr int32_t MAX_TILEPROJECT = 32;

extern "C" void __stdcall TILELIB_TileLibraryAddProject(D2TileLibraryHashStrc **ptTileLibrary, const char *szName);   // #10087
extern "C" uint32_t __stdcall TILELIB_TileLibraryGetTiles(D2TileLibraryHashStrc **ptTileLibrary, int32_t nType, int32_t nStyle, int32_t nSequence, D2TileLibraryEntryStrc **pTileList, uint32_t nCapacity);   // #10088
extern "C" int32_t __stdcall TILELIB_TileGetType(D2TileLibraryEntryStrc *hTile);                     // #10077
extern "C" int32_t __stdcall TILELIB_TileGetStyle(D2TileLibraryEntryStrc *hTile);                    // #10078
extern "C" int32_t __stdcall TILELIB_TileGetFlags(D2TileLibraryEntryStrc *hTile);                    // #10079
extern "C" int32_t __stdcall TILELIB_TileGetWidth(D2TileLibraryEntryStrc *hTile);                    // #10080
extern "C" int32_t __stdcall TILELIB_TileGetRarity(D2TileLibraryEntryStrc *hTile);                   // #10081
extern "C" int32_t __stdcall TILELIB_TileGetSequence(D2TileLibraryEntryStrc *hTile);                 // #10082
extern "C" int32_t __stdcall TILELIB_TileGetTileLight(D2TileLibraryEntryStrc *hTile);                // #10083
extern "C" int32_t __stdcall TILELIB_TileGetTotalHeight(D2TileLibraryEntryStrc *hTile);              // #10084
extern "C" uint8_t *__stdcall TILELIB_TileGetCollisionInfo(D2TileLibraryEntryStrc *hTile);   // #10085
extern "C" int32_t __stdcall TILELIB_TileGetHeightToBottom(D2TileLibraryEntryStrc *hTile);           // #10086
extern "C" uint16_t __stdcall TILELIB_TileGetRoofHeight(D2TileLibraryEntryStrc *hTile);  // #10100
extern "C" char *__stdcall TILELIB_TileGetFilename(D2TileLibraryEntryStrc *hTile);               // #10101
