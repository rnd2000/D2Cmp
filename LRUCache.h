#pragma once

#include "DLLmain.h"

/*---------------------------------------------------------------------------
 *
 *  LRUCache.h -- the functions of LRUCache.cpp, declared. None of the five is exported.
 *
 *-------------------------------------------------------------------------*/

void __fastcall LRUCACHE_Init(D2CmpLruCache *pCache, void *pPool, uint32_t dwMaxAllocated, void (__fastcall *pfnOnEvict)(D2CmpLruEntry *), uint32_t nEntries);
void __fastcall LRUCACHE_Destroy(D2CmpLruCache *pCache);
D2CmpLruEntry *__fastcall LRUCACHE_AllocEntry(D2CmpLruCache *pCache, uint32_t dwSize, void *pUserData);
void __fastcall LRUCACHE_EvictEntry(D2CmpLruCache *pCache, D2CmpLruEntry *pEntry);
void __fastcall LRUCACHE_TouchEntry(D2CmpLruCache *pCache, D2CmpLruEntry *pEntry);
