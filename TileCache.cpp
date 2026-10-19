#include "D2CMP.h"

//  TileCache.cpp -- the tile cache of D2CMP.dll 1.10: one LRU cache (LRUCache.cpp), gTileLruCache, holding the COMPONENT DATA of loaded floor and
//  wall tiles. A tile library entry (D2TileLibraryEntryStrc, 0x60 bytes) names its file and where its components lie in it (+0x58, +0x48, +0x4C);
//  TileValidate loads them into a cache entry on first use and touches the entry after, and the budget evicts the oldest -- OnEvict then tells
//  the renderer, which may hold the tile in video memory, through sgpfnHardwareFreeCallback.
//  The original source file is TileCache.cpp (C:\projects\D2\head\Diablo2\Source\D2CMP\SRC\TileCache.cpp, named by OnEvict's assert).
//  Eleven functions over 0x6FDFE600..0x6FDFE8C2, and #10076 (TILECACHE_CallSetHardwareFreeCallback, 0x6FDFFDD0), which lies outside that span.
//  The four globals (gTileLruCache, gbTileCacheInitialized, gnTileCacheConfiguredBytes, sgpfnHardwareFreeCallback) are referenced by this unit only.

//  The path literal of OnEvict's assert.
static const char szTileCacheCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2CMP\\SRC\\TileCache.cpp";

//  Layout checks: the offsets every function below reads, each with the instruction it was measured from.
static_assert(offsetof(D2TileLibraryEntryStrc, dwComponentFileOffset) == 0x48, "D2TileLibraryEntryStrc.dwComponentFileOffset: mov ecx, [esi+48h] (0x6FDFE740)");
static_assert(offsetof(D2TileLibraryEntryStrc, dwComponentDataBytes) == 0x4C, "D2TileLibraryEntryStrc.dwComponentDataBytes: mov edx, [esi+4Ch] (0x6FDFE6FC)");
static_assert(offsetof(D2TileLibraryEntryStrc, nCompCount) == 0x50, "D2TileLibraryEntryStrc.nCompCount: mov eax, [esi+50h] (0x6FDFE6EF)");
static_assert(offsetof(D2TileLibraryEntryStrc, ptCompArray) == 0x54, "D2TileLibraryEntryStrc.ptCompArray: mov [esi+54h], edi (0x6FDFE717)");
static_assert(offsetof(D2TileLibraryEntryStrc, szFileName) == 0x58, "D2TileLibraryEntryStrc.szFileName: mov edx, [esi+58h] (0x6FDFE728)");
static_assert(offsetof(D2TileLibraryEntryStrc, ptLRUCache) == 0x5C, "D2TileLibraryEntryStrc.ptLRUCache: mov edx, [esi+5Ch] (0x6FDFE6D5)");
static_assert(offsetof(D2CmpNormalizedTileProject, nNumTiles) == 0x10C, "D2CmpNormalizedTileProject.nNumTiles: mov eax, [edi+10Ch] (0x6FDFE876)");
static_assert(offsetof(D2CmpNormalizedTileProject, ptTileArray) == 0x110, "D2CmpNormalizedTileProject.ptTileArray: mov esi, [edi+110h] (0x6FDFE884)");
static_assert(offsetof(D2TileLibraryHeaderStrc, nTiles) == 0x10C, "D2TileLibraryHeaderStrc.nTiles: the buffer at esp+4, nTiles at esp+110h (0x6FDFE7C1)");


//1.10f: D2CMP.0x6FDFE600
//  #10089. Clears the cache (13 dwords, the 52 bytes of D2CmpLruCache), records the budget, sets the cache up for 4096 entries with OnEvict as its
//  callback, and sets gbTileCacheInitialized. D2Client passes a budget of 0x600000 bytes. `void`: the original leaves LRUCACHE_Init's eax, which
//  the caller does not read.
void __stdcall TILECACHE_TileCacheInit(void *pPool, uint32_t dwCacheBytes)
{
    memset(&gTileLruCache, 0, sizeof(D2CmpLruCache));
    gnTileCacheConfiguredBytes = dwCacheBytes;
    LRUCACHE_Init(&gTileLruCache, pPool, dwCacheBytes, TILECACHE_OnEvict, 0x1000);
    gbTileCacheInitialized = 1;
}


//1.10f: D2CMP.0x6FDFE640
//  #10090. When the cache was set up: frees it if asked (LRUCACHE_Destroy), clears the whole struct and the budget. Either way it then calls
//  DestroyTileHeaders (#10094, TileProjects.cpp) -- twice in the original, once per arm. gbTileCacheInitialized is not cleared (its one writer
//  is TileCacheInit), so after a Destroy a second one clears the cache again, and LRUCACHE_Destroy, asked for, finds nothing active and does
//  nothing. `void`: the caller does not read the answer.
void __stdcall TILECACHE_TileCacheDestroy(int32_t bFreeCache)
{
    if (gbTileCacheInitialized)
    {
        if (bFreeCache)
            LRUCACHE_Destroy(&gTileLruCache);
        memset(&gTileLruCache, 0, sizeof(D2CmpLruCache));
        gnTileCacheConfiguredBytes = 0;
    }
    TILEPROJECTS_DestroyTileHeaders();
}


//1.10f: D2CMP.0x6FDFE680
//  The cache's eviction callback (pfnOnEvict): the entry's user data is the tile. When a hardware callback is set it must be code (IsBadCodePtr,
//  else the assert at TileCache.cpp:180 and exit), and gets the tile in ECX; then the tile's component pointer and its cache entry are cleared.
//  LRUCACHE_EvictEntry frees the entry's block after this returns. `void`: the original leaves the callback's eax, or the NULL just tested, and
//  the cache reads neither. The original loads the callback once for the test and IsBadCodePtr and calls through memory again.
void __fastcall TILECACHE_OnEvict(D2CmpLruEntry *pEntry)
{
    D2TileLibraryEntryStrc *pTile = (D2TileLibraryEntryStrc *)pEntry->pUserData;

    if (sgpfnHardwareFreeCallback)
    {
        if (IsBadCodePtr((FARPROC)sgpfnHardwareFreeCallback))
        {
            FOG_DisplayAssert("sgpfnHardwareFreeCallback", szTileCacheCpp, 180);
            exit(-1);
        }
        sgpfnHardwareFreeCallback(pTile);
    }

    pTile->ptCompArray = nullptr;
    pTile->ptLRUCache = nullptr;
}


//1.10f: D2CMP.0x6FDFE6D0
//  #10091, the renderers' "make this tile's data resident": a tile with a cache entry has it touched and answers 1; a tile with no components
//  answers 0; otherwise an entry of dwComponentDataBytes is taken from the cache (which may evict others), zeroed, filled from the tile's file at
//  dwComponentFileOffset, closed, and the component pointers rebased (TILECMP_RebaseComponentDataPointers), answering 1. The second and third
//  arguments are never read (`retn 0Ch`). The original keeps the file handle in the slot of the first argument; ours has a local. The memset is
//  `rep stosd` / `rep stosb` on the size read before the two stores.
//  BUG: the entry is allocated, linked into the tile and zeroed before the open; when ARCHIVE_OpenFile fails it answers 0 and leaves both
//  pointers set, so the next call finds ptLRUCache, touches it and answers 1 for components that were never read. NOT ESTABLISHED: left in the
//  C, not certainly a defect. Reproduced.
//  D2MOO BUG: D2TileLibraryEntryStrc names +0x00..+0x3F, calls +0x40..+0x4F `unk0x40[4]` and leaves field_50..field_5C commented out; the binary
//  reads +0x48, +0x4C, +0x58 and the entry is 0x60 bytes.
int32_t __stdcall TILECACHE_TileValidate(D2TileLibraryEntryStrc *pTile, int32_t nUnused0, int32_t nUnused1)
{
    if (pTile->ptLRUCache)
    {
        LRUCACHE_TouchEntry(&gTileLruCache, pTile->ptLRUCache);
        return 1;
    }
    if (!pTile->nCompCount)
        return 0;

    D2CmpLruEntry *pEntry = LRUCACHE_AllocEntry(&gTileLruCache, pTile->dwComponentDataBytes, pTile);
    pTile->ptLRUCache = pEntry;
    pTile->ptCompArray = (D2CmpTileRuntimeComponent *)pEntry->pData;
    memset(pEntry->pData, 0, pTile->dwComponentDataBytes);

    void *hFile;
    if (!ARCHIVE_OpenFile(0, pTile->szFileName, &hFile, 0))
        return 0;

    ARCHIVE_SeekFile(0, hFile, pTile->dwComponentFileOffset, nullptr, 0);
    ARCHIVE_ReadFileExact(0, hFile, pTile->ptCompArray, pTile->dwComponentDataBytes);
    ARCHIVE_CloseFile(0, hFile);
    TILECMP_RebaseComponentDataPointers(pTile);
    return 1;
}


//1.10f: D2CMP.0x6FDFE780
//  The bytes a tile project needs: 0 when the file does not open; otherwise its 0x114-byte header is read and the file closed, and the answer is
//  the header's size plus nTiles 0x60-byte entries (0x60 is D2TileLibraryEntryStrc's size and the stride DetachProjectTiles walks a project's
//  tiles at). TILEPROJECTS_LoadProjectRecord allocates that much.
//  BUG: the count is the file's, unchecked: a negative or huge nTiles wraps the 32-bit size. NOT ESTABLISHED: left in the C, not certainly a
//  defect. Ours multiplies unsigned, which gives the wrap and no signed overflow. ARCHIVE_ReadFileExact halts on a failed or short read, so the
//  header is whole when this runs on.
uint32_t __fastcall TILECACHE_GetProjectAllocationSize(const char *szFileName)
{
    void *hFile;
    D2TileLibraryHeaderStrc tHeader;

    if (!ARCHIVE_OpenFile(0, szFileName, &hFile, 0))
        return 0;

    ARCHIVE_ReadFileExact(0, hFile, &tHeader, sizeof(tHeader));
    ARCHIVE_CloseFile(0, hFile);
    return (uint32_t)tHeader.nTiles * sizeof(D2TileLibraryEntryStrc) + sizeof(D2TileLibraryHeaderStrc);
}


//1.10f: D2CMP.0x6FDFE7E0
//  Stores the renderer's hardware callback, unchecked; OnEvict checks it before each use.
void __fastcall TILECACHE_SetHardwareFreeCallback(D2CmpTileHardwareFreeCallback pfnCallback)
{
    sgpfnHardwareFreeCallback = pfnCallback;
}


//1.10f: D2CMP.0x6FDFE7F0
//  #10092, the debug overlay's statistics: the budget, the cache's two request counters, and the share of the budget in use in percent --
//  0xFFFFFFFF when there is no budget. The stores are in the original's order: the budget, the access counter, the alloc counter. `void`.
//  BUG: the percentage wraps: `lea` / `lea` / `shl 2` multiply by 100 in 32 bits and `div` is unsigned (0x6FDFE824..0x6FDFE82D), so above
//  0x28F5C28 bytes in use the answer wraps. LATENT: AllocEntry keeps the bytes in use within the budget, and the game's one budget is 0x600000.
void __stdcall TILECACHE_GetTileCacheHits(uint32_t *pConfiguredBytes, uint32_t *pAllocRequests, uint32_t *pAccessRequests, uint32_t *pPercentAllocated)
{
    *pConfiguredBytes = gnTileCacheConfiguredBytes;
    *pAccessRequests = gTileLruCache.nAccessRequests;
    *pAllocRequests = gTileLruCache.nAllocRequests;
    if (gTileLruCache.dwMaxAllocated)
        *pPercentAllocated = 100 * gTileLruCache.dwAllocated / gTileLruCache.dwMaxAllocated;
    else
        *pPercentAllocated = 0xFFFFFFFF;
}


//1.10f: D2CMP.0x6FDFE850
//  #10093: clears the two request counters, the access one first. The original also clears eax to store it; nothing reads it, so `void`.
void __cdecl TILECACHE_ResetTileCacheHits()
{
    gTileLruCache.nAccessRequests = 0;
    gTileLruCache.nAllocRequests = 0;
}


//1.10f: D2CMP.0x6FDFE860
//  #10102. Frees the tile cache's entries and array (`jmp LRUCACHE_Destroy`) and has nothing to do with asynchronous loading; unlike
//  TileCacheDestroy it leaves the struct and gbTileCacheInitialized as they are.
void __stdcall TILECACHE_TileFreeAsyncLoads()
{
    LRUCACHE_Destroy(&gTileLruCache);
}


//1.10f: D2CMP.0x6FDFE870
//  Takes every tile of a project out of the cache: each tile holding an entry has it evicted -- OnEvict clears the two pointers -- and clears them
//  again itself. The count is re-read every pass and compared signed, the tile array too. No test of the project. `void`.
void __fastcall TILECACHE_DetachProjectTiles(D2CmpNormalizedTileProject *pProject)
{
    for (int32_t i = 0; i < pProject->nNumTiles; ++i)
    {
        D2TileLibraryEntryStrc *pTile = &pProject->ptTileArray[i];
        if (pTile->ptLRUCache)
        {
            LRUCACHE_EvictEntry(&gTileLruCache, pTile->ptLRUCache);
            pTile->ptCompArray = nullptr;
            pTile->ptLRUCache = nullptr;
        }
    }
}


//1.10f: D2CMP.0x6FDFE8C0
//  #10105: `retn 8` and nothing else -- two stack arguments, neither read.
void __stdcall TILECACHE_10105(int32_t nUnused1, int32_t nUnused2)
{
}


//1.10f: D2CMP.0x6FDFFDD0
//  #10076, TileSetHardwareFreeCallback: hands its one stack argument to TILECACHE_SetHardwareFreeCallback in ECX (`mov ecx, [esp+4]` / `call` /
//  `retn 4`). Called by the Direct3D and Glide renderers, which do not read eax afterwards -- `void`.
extern "C" void __stdcall TILECACHE_CallSetHardwareFreeCallback(D2CmpTileHardwareFreeCallback pfnCallback)
{
    TILECACHE_SetHardwareFreeCallback(pfnCallback);
}
