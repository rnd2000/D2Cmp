#pragma once

#include "DLLmain.h"

//  TileCache.h -- the functions of TileCache.cpp declared, and the unit's four globals. Eight of the twelve are exports (declared extern "C" for
//  D2CMP.def); the other four are called only inside D2CMP -- OnEvict through the tile cache's pfnOnEvict, the rest by Tilecmp.cpp and
//  TileProjects.cpp code.

//  The unit's four globals, 0x6FEF1D98..0x6FEF1DD7, in .bss in the original, so zero at load, as these are.

//  D2CMP 0x6FEF1D98: the tile cache (LRUCache.cpp).
inline D2CmpLruCache gTileLruCache = {};

//  D2CMP 0x6FEF1DCC: set by TileCacheInit; tested by TileCacheDestroy.
inline int32_t gbTileCacheInitialized = 0;

//  D2CMP 0x6FEF1DD0: the budget TileCacheInit was given; cleared by TileCacheDestroy, reported by GetTileCacheHits.
inline uint32_t gnTileCacheConfiguredBytes = 0;

//  D2CMP 0x6FEF1DD4: the renderer's hardware-free callback (#10076), called by OnEvict.
inline D2CmpTileHardwareFreeCallback sgpfnHardwareFreeCallback = nullptr;

extern "C" void __stdcall TILECACHE_TileCacheInit(void *pPool, uint32_t dwCacheBytes);                       // #10089
extern "C" void __stdcall TILECACHE_TileCacheDestroy(int32_t bFreeCache);                                            // #10090
void __fastcall TILECACHE_OnEvict(D2CmpLruEntry *pEntry);
extern "C" int32_t __stdcall TILECACHE_TileValidate(D2TileLibraryEntryStrc *pTile, int32_t nUnused0, int32_t nUnused1);      // #10091
uint32_t __fastcall TILECACHE_GetProjectAllocationSize(const char *szFileName);
void __fastcall TILECACHE_SetHardwareFreeCallback(D2CmpTileHardwareFreeCallback pfnCallback);
extern "C" void __stdcall TILECACHE_GetTileCacheHits(uint32_t *pConfiguredBytes, uint32_t *pAllocRequests, uint32_t *pAccessRequests, uint32_t *pPercentAllocated);  // #10092
extern "C" void __cdecl TILECACHE_ResetTileCacheHits();                                                          // #10093
extern "C" void __stdcall TILECACHE_TileFreeAsyncLoads();                                                        // #10102
void __fastcall TILECACHE_DetachProjectTiles(D2CmpNormalizedTileProject *pProject);
extern "C" void __stdcall TILECACHE_10105(int32_t nUnused1, int32_t nUnused2);                                           // #10105
extern "C" void __stdcall TILECACHE_CallSetHardwareFreeCallback(D2CmpTileHardwareFreeCallback pfnCallback);              // #10076
