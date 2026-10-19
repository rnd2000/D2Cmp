#pragma once

#include "DLLmain.h"

//  TileProjects.h -- the functions of TileProjects.cpp, declared (both are exports, extern "C" for D2CMP.def), and its global.

//  D2CMP 0x6FEF1DD8
//  The head of the loaded tile projects, newest first, linked through pPrev; each record holds the project, its find-tiles hash and the file
//  name it was loaded from. In .bss in the original, so zero at load, as this one is.
inline D2TileRecordStrc *gpTileProjectRecords = nullptr;

extern "C" void __cdecl TILEPROJECTS_DestroyTileHeaders();                                                       // #10094
extern "C" D2CmpNormalizedTileProject *__stdcall TILEPROJECTS_LoadProjectRecord(D2TileLibraryHashStrc **ppHash, const char *szFileName);   // #10095
