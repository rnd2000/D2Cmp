#pragma once

#include "DLLmain.h"

//  SpriteCache.h -- declarations for SpriteCache.cpp: the exports (extern "C", the ordinal in the trailing comment), the unit's internal functions
//  and its globals. The globals are C++17 `inline` variables; the comment line "D2CMP 0x6F..." above each gives its address in the original.

extern "C" void __stdcall SPRITECACHE_SpriteCacheInit(void *pPool, int32_t nSpriteCacheBytes, int32_t nCelHeaderCacheBytes, uint32_t dwMemoryOverride); // #10052
extern "C" void __stdcall SPRITECACHE_SpriteCacheDestroy(int32_t bFreeCaches);                                                                       // #10053
extern "C" int32_t __stdcall SPRITECACHE_SpriteValidate(D2GfxDataStrc *pContext, int32_t bAsyncWait, int32_t bAsyncLoad);                                     // #10055
extern "C" int32_t __stdcall SPRITECACHE_SpriteRequest(D2GfxDataStrc *ptCelContext);                                                                  // #10056
void __fastcall SPRITECACHE_OnDataEvict(D2CmpSpriteGfxHashEntryPrefix *pElement);
void __fastcall SPRITECACHE_OnHeaderEvict(D2CmpLruEntry *pEntry);
uint32_t __stdcall SPRITECACHE_RunDecodeJob(void *pInput, uint32_t dwInputBytes, D2CmpSpriteDecodeJobPrefix *pJob);
int32_t __fastcall SPRITECACHE_BuildFilePath(D2GfxDataStrc *pContext, char *szPath, const char *szKey);
D2CmpSpriteHeaderData *__fastcall SPRITECACHE_ResolveHeader(D2GfxDataStrc *pContext, char *szPath, char *szKey);
int32_t __fastcall SPRITECACHE_BuildHeaderKey(D2GfxDataStrc *pContext, char *szKey);
void __fastcall SPRITECACHE_InitializeHardwareSlots(D2CmpSpriteHeaderData *ptCelData, D2GfxDataStrc *ptCelContext, D2CmpSpriteGfxNodePrefix *ptGfxNode);
int32_t __fastcall SPRITECACHE_LoadDirectionBlockSync(D2GfxDataStrc *pContext, D2CmpSpriteHeaderData *pHeader, uint32_t dwDirBlock, uint32_t dwDirectionsPerBlock, const char *szPath);
D2CmpSpriteFrameHeaderPrefix *__fastcall SPRITECACHE_FindFrame(D2GfxDataStrc *ptCelContext, D2CmpSpriteGfxBlockPrefix *ptGfxBlock, uint8_t *pBuffer);
void __fastcall SPRITECACHE_PrefetchSecondDirectionBlock(D2GfxDataStrc *pContext, uint32_t dwDirBlock, uint32_t dwDirBlockSize, D2CmpSpriteHeaderData *pHeader, char *szKey, char *szPath, int32_t bAsyncLoad);
int32_t __fastcall SPRITECACHE_LoadDirectionBlockAsync(D2GfxDataStrc *pContext, D2CmpSpriteHeaderData *pHeader, uint32_t dwDirBlock, uint32_t dwDirectionsPerBlock, const char *szPath);

extern "C" int32_t __cdecl SPRITECACHE_SpriteNumAsyncLoads();                                                     // #10057
extern "C" int32_t __stdcall SPRITECACHE_SpriteFindAsynchSiblings(int32_t nUnitType, int32_t nMode, int32_t nUnitIndex);     // #10058
extern "C" void __cdecl SPRITECACHE_SpriteCacheDumpHashTableStats();                                          // #10059
extern "C" uint32_t __cdecl SPRITECACHE_SpriteCacheGetUsedMemory();                                       // #10060
extern "C" uint32_t __cdecl SPRITECACHE_SpriteCacheGetMaxMemory();                                        // #10061
extern "C" void __cdecl SPRITECACHE_CelDataCacheDumpHashTableStats();                                         // #10062
extern "C" uint32_t __cdecl SPRITECACHE_CelDataCacheGetUsedMemory();                                      // #10063
extern "C" uint32_t __cdecl SPRITECACHE_CelDataCacheGetMaxMemory();                                       // #10064
extern "C" void __stdcall SPRITECACHE_SpriteFreeAsyncLoads();                                                 // #10099
extern "C" const char *__stdcall SPRITECACHE_GetGfxFileExtension(int32_t bAllowCompressed);                       // #10051
extern "C" void __stdcall SPRITECACHE_BuildFileName(char *Buffer, int32_t bAllowCompressed, const char *Format, va_list ArgList); // #10050
extern "C" void __stdcall SPRITECACHE_SetNoCompress(int32_t bAllowCompressedMode);                                // #10054

void __fastcall SPRITECACHE_SpriteDataCache_Init(D2CmpSpriteDataCache *pCache, void *pPool, int32_t nCacheBytes, void(__fastcall *pfnOnEvict)(void *));
D2CmpSpriteDataNode *__fastcall SPRITECACHE_SpriteDataCache_AllocateNode(D2CmpSpriteDataCache *ptCache, uint32_t dwSize, void *pUserData, int32_t bQueue1);
D2CmpSpriteDataNode *__fastcall SPRITECACHE_SpriteDataCache_EvictNode(D2CmpSpriteDataCache *pCache, D2CmpSpriteDataNode *pNode, int32_t bAllowQueue1);
void __fastcall SPRITECACHE_SpriteDataCache_AddNodeBytes(D2CmpSpriteDataCache *pCache, D2CmpSpriteDataNode *ptBlock, uint32_t dwDeltaBytes, int32_t bAllowQueue1);
void __fastcall SPRITECACHE_SpriteDataCache_AppendQueuedNode(D2CmpSpriteDataCache *pCache, D2CmpSpriteDataNode *pNode, int32_t bQueue1);
D2CmpSpriteDataNode *__fastcall SPRITECACHE_SpriteDataCache_PopQueueHead(D2CmpSpriteDataCache *pCache, int32_t bQueue1);
void __fastcall SPRITECACHE_SpriteDataCache_DetachQueuedNode(D2CmpSpriteDataCache *pCache, D2CmpSpriteDataNode *ptBlock);

//  The unit's globals: four in .data, which carry the image's values, then nine in .bss (past .data's raw end, 0x6FE13000), which are zero at load.

//  D2CMP 0x6FE0F054
//  The cel header LRU cache's entry count, 0x1000 in the image; read once, by SpriteCacheInit.
inline uint32_t gnCelHeaderCacheEntryCount = 0x1000;

//  D2CMP 0x6FE0F058
//  The compressed-data setting, 1 in the image: SetNoCompress (#10054) loads it from the registry and clears it when the caller does not allow
//  compressed files; #10051, #10050, BuildFilePath and BuildHeaderKey read it.
inline uint32_t gdwSpriteCompressedDataSetting = 1;

//  D2CMP 0x6FE0F05C
//  The pointer to the ".dcc" extension string, read by #10051 and #10050.
inline const char *gpszDccExtension = ".dcc";

//  D2CMP 0x6FE0F060
//  The pointer to the ".dc6" extension string, read by #10051 and #10050.
inline const char *gpszDc6Extension = ".dc6";

//  D2CMP 0x6FED0D18
//  CelDataHash.cpp's storage of cel header entries, 4096 bytes; only its address is used.
inline D2CmpCelHeaderHash gCelHeaderHash = {};

//  D2CMP 0x6FED1D18
//  GfxHash.cpp's storage of direction-block entries, 65536 bytes; only its address is used.
inline D2CmpSpriteGfxHashStorage gSpriteGfxHashStorage = {};

//  D2CMP 0x6FEE1D18
//  The sprite data cache, 60 bytes (the original's assert text calls it sgtGfxCache).
inline D2CmpSpriteDataCache sgtGfxCache = {};

//  D2CMP 0x6FEE1D54
//  The cel header LRU cache (LRUCache.cpp), 52 bytes.
inline D2CmpLruCache gCelHeaderLruCache = {};

//  D2CMP 0x6FEE1D88
//  The frame-offset table of the DC6 header that ResolveHeader reads, 0x4000 dwords; ResolveHeader is its only user.
inline uint32_t gSpriteHeaderFileOffsets[0x4000] = {};

//  D2CMP 0x6FEF1D88
//  The "low end machine" flag: written by SpriteCacheInit, read by PrefetchSecondDirectionBlock.
inline int32_t gbSpriteLowEndMachine = 0;

//  D2CMP 0x6FEF1D8C
//  The sprite hardware-free callback that OnDataEvict calls for each evicted direction block, with its unit type, unit index and mode. Nothing
//  sets it (.bss, zero), so that call never runs.
inline D2CmpSpriteHardwareFreeCallback gSpriteHardwareFreeCallback = nullptr;

//  D2CMP 0x6FEF1D90
//  SpriteValidate's "the handle trace was printed" flag: set when the trace is first written, so the trace appears once per process.
inline int32_t gbSpriteHandleErrorReported = 0;

//  D2CMP 0x6FEF1D94
//  Four bytes nothing writes, so an empty string: the file sub-name that AllocateNode hands FOG_TraceF.
inline char gszEmptyString[4] = {};
