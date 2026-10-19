#pragma once

#include "DLLmain.h"

/*---------------------------------------------------------------------------
 *
 *  FindTiles.h -- the functions of FindTiles.cpp, declared, and the unit's constant. None of the four is exported.
 *
 *-------------------------------------------------------------------------*/

//  The original's own name, out of the key's assert ("tKey < FIND_TILE_HASH_SIZE"): the bucket count, D2TileLibraryHashStrc's pNodes[128].
//  `int` because the key's reduction is a signed `%` and FreeHash's bucket compare is signed.
constexpr int32_t FIND_TILE_HASH_SIZE = 128;

D2TileLibraryHashStrc *__cdecl FINDTILES_AllocHash();
void __fastcall FINDTILES_FreeHash(D2TileLibraryHashStrc *pHash);
void __fastcall FINDTILES_AddTile(D2TileLibraryHashStrc *ptHash, D2TileLibraryEntryStrc *ptTile);
uint32_t __fastcall FINDTILES_CollectTiles(D2TileLibraryHashStrc *ptHash, D2TileLibraryEntryStrc **hTileList, uint32_t nCapacity, int32_t nType, int32_t nStyle, int32_t nSequence);
