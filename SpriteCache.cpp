#include "D2CMP.h"

//  SpriteCache.cpp -- the sprite cache of D2CMP.dll 1.10: cel file headers and direction blocks loaded on demand, synchronously or through Fog's
//  async loader, kept in a byte-budgeted data cache and found again through GfxHash.cpp's and CelDataHash.cpp's hashes.
//  The original source file is D2CMP\Src\SpriteCache.cpp (its asserts name it). Address range 0x6FDFB930..0x6FDFE043. #10051, #10050 and #10054, at
//  the start of the range, name no file; they read or set the module's compressed-data setting, as BuildFilePath and BuildHeaderKey read it.
//  The data cache is 5120 nodes of 28 bytes in one pool block, a free list and two queues (queue 0 and queue 1; a node's flag bit 0 says which), and
//  a byte budget (dwMaxAllocated) against the bytes of the nodes' data. A node's flag bit 1 is "busy": set while AddNodeBytes grows it or
//  RunDecodeJob decodes into it; a busy node is neither evicted, popped nor taken off the free list. The unit's globals are declared in
//  SpriteCache.h.

//  The path literal of the unit's asserts and allocations (__FILE__ in the original).
static const char szSpriteCacheCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2CMP\\Src\\SpriteCache.cpp";

//  The data cache's node count: the pool block is 0x23000 bytes of 0x1C-byte nodes, and AllocateNode tests the active count against 0x1400.
constexpr int32_t SPRITE_DATA_CACHE_NODES = 5120;

static_assert(sizeof(D2CmpSpriteDataNode) == 0x1C, "the node stride: add eax, 1Ch (0x6FDFD8DD)");
static_assert(offsetof(D2CmpSpriteDataCache, dwMaxAllocated) == 0x04, "cmp ebx, eax with eax = [esi+4] (0x6FDFD94B)");
static_assert(offsetof(D2CmpSpriteDataCache, pNodes) == 0x10, "mov [esi+10h], edi (0x6FDFD8AC)");
static_assert(offsetof(D2CmpSpriteDataCache, pFreeHead) == 0x24, "mov edi, [esi+24h] (0x6FDFD9CF)");
static_assert(offsetof(D2CmpSpriteDataCache, pfnOnEvict) == 0x2C, "call dword ptr [esi+2Ch] (0x6FDFDC42)");
static_assert(offsetof(D2CmpSpriteDataCache, pcs) == 0x30, "mov eax, [esi+30h] (0x6FDFD9A6)");
static_assert(offsetof(D2CmpSpriteDataNode, dwFlags) == 0x0C, "test byte ptr [edi+0Ch], 2 (0x6FDFD9DA)");
static_assert(offsetof(D2CmpSpriteDataNode, dwQueuedTick) == 0x10, "mov [esi+10h], eax after GetTickCount (0x6FDFDD7E)");


//1.10f: D2CMP.0x6FDFD870
//  Sets up the cache: the budget, the pool and the eviction callback are stored; the 5120 nodes are allocated in one pool block, cleared and chained
//  into the free list in address order; both queues are empty, the counters at +0x34 and +0x38 are zeroed, and a critical section is created unless
//  the cache already holds one. The chaining loop reloads the node base from the cache at every pass and compares signed.
//  BUG: the answers of FOG_AllocPool (the node block, written through by the `rep stosd` at 0x6FDFD8AF) and of FOG_Alloc (the critical section,
//  0x6FDFD921) are used untested. LATENT: only a failed allocation reaches them.
void __fastcall SPRITECACHE_SpriteDataCache_Init(D2CmpSpriteDataCache *pCache, void *pPool, int32_t nCacheBytes, void(__fastcall *pfnOnEvict)(void *))
{
    pCache->dwMaxAllocated = nCacheBytes;
    pCache->pfnOnEvict = pfnOnEvict;
    pCache->pPool = pPool;
    pCache->dwAllocatedBytes = 0;

    D2CmpSpriteDataNode *pNodes = (D2CmpSpriteDataNode *)FOG_AllocPool(pPool, SPRITE_DATA_CACHE_NODES * sizeof(D2CmpSpriteDataNode), szSpriteCacheCpp, 2268, 0);
    pCache->pNodes = pNodes;
    memset(pNodes, 0, SPRITE_DATA_CACHE_NODES * sizeof(D2CmpSpriteDataNode));

    for (int32_t i = 0; i < SPRITE_DATA_CACHE_NODES; ++i)
    {
        D2CmpSpriteDataNode *pBase = pCache->pNodes;
        if (i < SPRITE_DATA_CACHE_NODES - 1)
        {
            pBase[i].pNext = &pBase[i + 1];
        }
        if (i > 0)
        {
            pBase[i].pPrev = &pCache->pNodes[i - 1];
        }
        pBase[i].pData = nullptr;
        pBase[i].dwSize = 0;
        pBase[i].pUserData = nullptr;
    }

    pCache->nActive = 0;
    pCache->pFreeHead = pCache->pNodes;
    pCache->pFreeTail = pCache->pNodes + (SPRITE_DATA_CACHE_NODES - 1);
    pCache->pQueue0Head = nullptr;
    pCache->pQueue0Tail = nullptr;
    pCache->pQueue1Head = nullptr;
    pCache->pQueue1Tail = nullptr;
    pCache->unknown_34 = 0;
    pCache->unknown_38 = 0;
    if (!pCache->pcs)
    {
        pCache->pcs = (CRITICAL_SECTION *)FOG_Alloc(sizeof(CRITICAL_SECTION), szSpriteCacheCpp, 2308, 0);
        InitializeCriticalSection(pCache->pcs);
    }
}


//1.10f: D2CMP.0x6FDFD940
//  Returns a node for dwSize bytes of data on queue bQueue1, or NULL (the original's "SpriteCacheBlockNew", from its trace text). dwSize must be
//  below the budget (asserted). A node is taken from the free list under the lock when not all 5120 are active and the bytes fit (compared
//  unsigned); otherwise nodes are evicted one at a time, from either queue, and the take is retried until one succeeds or nothing is left to evict
//  (traced, NULL). On queue 1 the loop also gives up, untraced, while less than 15000 ms have passed since the floor tick. The node's queue bit is
//  then set to bQueue1, its data is allocated and the counters at +0x34 and +0x38 are counted up.
//  The original inlines the take from the free list here, twice: before the loop and after each eviction.
//  BUG: the floor tick is the older of the two queues' head ticks, compared signed (0x6FDFD9A0), and it starts at 0: with queue 0 empty only a
//  queue-1 tick of 0 or a negative one lowers it, so the 15 s wait is measured from tick 0, not from queue 1's head. NOT ESTABLISHED: the intent is
//  not known.
//  BUG: the pool allocation's answer is stored as the node's data untested (0x6FDFDB6D). LATENT: only a failed allocation reaches it.
D2CmpSpriteDataNode *__fastcall SPRITECACHE_SpriteDataCache_AllocateNode(D2CmpSpriteDataCache *ptCache, uint32_t dwSize, void *pUserData, int32_t bQueue1)
{
    if (dwSize >= ptCache->dwMaxAllocated)
    {
        FOG_DisplayAssert("dwSize < ptCache->dwMaxAllocated", szSpriteCacheCpp, 2376);
        exit(-1);
    }

    int32_t nTickFloor = 0;
    if (bQueue1)
    {
        if (ptCache->pQueue0Head)
        {
            nTickFloor = ptCache->pQueue0Head->dwQueuedTick;
        }
        if (ptCache->pQueue1Head)
        {
            const int32_t nQueue1Tick = ptCache->pQueue1Head->dwQueuedTick;
            if (nTickFloor >= nQueue1Tick)
            {
                nTickFloor = nQueue1Tick;
            }
        }
    }

    D2CmpSpriteDataNode *ptBlock = nullptr;
    if (ptCache->pcs)
    {
        //  BUG: the take hands its node to AppendQueuedNode before the "ptBlock" assert (0x6FDFDA05, 0x6FDFDA0A). When the free
        //  head is busy the node is NULL, AppendQueuedNode writes through it, and the assert is not reached by that path.
        //  NOT ESTABLISHED: whether a free head can be busy here is not settled. The second copy of the take below does the same.
        EnterCriticalSection(ptCache->pcs);
        if (ptCache->nActive != SPRITE_DATA_CACHE_NODES && ptCache->dwAllocatedBytes + dwSize <= ptCache->dwMaxAllocated)
        {
            ptBlock = ptCache->pFreeHead;
            if (ptBlock)
            {
                if (ptBlock->dwFlags & 0x02)
                {
                    ptBlock = nullptr;
                }
                else
                {
                    ptCache->pFreeHead = ptBlock->pNext;
                    if (ptCache->pFreeHead)
                    {
                        ptCache->pFreeHead->pPrev = nullptr;
                    }
                    else
                    {
                        ptCache->pFreeTail = nullptr;
                    }
                    ptBlock->pNext = nullptr;
                    ptBlock->pPrev = nullptr;
                }
            }
            SPRITECACHE_SpriteDataCache_AppendQueuedNode(ptCache, ptBlock, bQueue1);
            if (!ptBlock)
            {
                FOG_DisplayAssert("ptBlock", szSpriteCacheCpp, 2461);
                exit(-1);
            }
            ++ptCache->nActive;
            ptCache->dwAllocatedBytes += dwSize;
        }
        LeaveCriticalSection(ptCache->pcs);
    }

    while (!ptBlock)
    {
        if (bQueue1 && GetTickCount() - nTickFloor < 15000)
        {
            return nullptr;
        }
        if (!SPRITECACHE_SpriteDataCache_EvictNode(ptCache, nullptr, 1))
        {
            FOG_TraceF(gszEmptyString, "SpriteCacheBlockNew: SpriteCacheBlockFree failed");
            return nullptr;
        }

        if (ptCache->pcs)
        {
            EnterCriticalSection(ptCache->pcs);
            if (ptCache->nActive != SPRITE_DATA_CACHE_NODES && ptCache->dwAllocatedBytes + dwSize <= ptCache->dwMaxAllocated)
            {
                ptBlock = ptCache->pFreeHead;
                if (ptBlock)
                {
                    if (ptBlock->dwFlags & 0x02)
                    {
                        ptBlock = nullptr;
                    }
                    else
                    {
                        ptCache->pFreeHead = ptBlock->pNext;
                        if (ptCache->pFreeHead)
                        {
                            ptCache->pFreeHead->pPrev = nullptr;
                        }
                        else
                        {
                            ptCache->pFreeTail = nullptr;
                        }
                        ptBlock->pNext = nullptr;
                        ptBlock->pPrev = nullptr;
                    }
                }
                SPRITECACHE_SpriteDataCache_AppendQueuedNode(ptCache, ptBlock, bQueue1);
                if (!ptBlock)
                {
                    FOG_DisplayAssert("ptBlock", szSpriteCacheCpp, 2461);
                    exit(-1);
                }
                ++ptCache->nActive;
                ptCache->dwAllocatedBytes += dwSize;
            }
            LeaveCriticalSection(ptCache->pcs);
        }
    }

    //  Bit 0 alone changes; the original loads and stores the whole dword.
    ptBlock->dwFlags = bQueue1 ? (ptBlock->dwFlags | 0x01) : (ptBlock->dwFlags & ~0x01u);
    ptBlock->pData = FOG_AllocPool(ptCache->pPool, dwSize, szSpriteCacheCpp, 2418, 0);
    ptBlock->dwSize = dwSize;
    ptBlock->pUserData = pUserData;
    ++ptCache->unknown_34;
    ++ptCache->unknown_38;
    return ptBlock;
}


//1.10f: D2CMP.0x6FDFDBA0
//  Evicts pNode, or with NULL the head of a queue (the original's "SpriteCacheBlockFree", from its trace text): queue 1 when it has a head and
//  either queue 0 has none and bAllowQueue1 is set, or bAllowQueue1 is set and queue 1's head was queued earlier (compared unsigned); else queue 0.
//  Returns the node, now on the free list, or NULL: no node active, a busy node, an empty queue, or no critical section. The node's bytes leave the
//  budget under the lock, its user data goes to the cache's callback (whose answer is not read) and its data back to the pool. The busy test after
//  the pop repeats PopQueueHead's own and can fire only if another thread marks the node in between.
//  BUG: with no critical section the node, already taken off its queue, is returned to no list and NULL is returned (0x6FDFDC13..0x6FDFDC1C).
//  NOT ESTABLISHED: whether anything evicts between SpriteCacheDestroy(0), which leaves the section NULL, and the next
//  SpriteCacheInit is not settled.
D2CmpSpriteDataNode *__fastcall SPRITECACHE_SpriteDataCache_EvictNode(D2CmpSpriteDataCache *pCache, D2CmpSpriteDataNode *pNode, int32_t bAllowQueue1)
{
    if (!pCache->nActive)
    {
        return nullptr;
    }

    if (!pNode)
    {
        int32_t bSelectQueue1;
        if (!pCache->pQueue1Head)
        {
            bSelectQueue1 = 0;
        }
        else if (!pCache->pQueue0Head)
        {
            bSelectQueue1 = bAllowQueue1;
        }
        else if (bAllowQueue1)
        {
            bSelectQueue1 = pCache->pQueue1Head->dwQueuedTick < pCache->pQueue0Head->dwQueuedTick;
        }
        else
        {
            bSelectQueue1 = 0;
        }

        pNode = SPRITECACHE_SpriteDataCache_PopQueueHead(pCache, bSelectQueue1);
        if (!pNode)
        {
            return nullptr;
        }
        if (pNode->dwFlags & 0x02)
        {
            return nullptr;
        }
    }
    else
    {
        if (pNode->dwFlags & 0x02)
        {
            return nullptr;
        }
        SPRITECACHE_SpriteDataCache_DetachQueuedNode(pCache, pNode);
    }

    if (!pCache->pcs)
    {
        return nullptr;
    }

    EnterCriticalSection(pCache->pcs);
    pCache->dwAllocatedBytes -= pNode->dwSize;
    LeaveCriticalSection(pCache->pcs);

    if (pNode->pUserData)
    {
        pCache->pfnOnEvict(pNode->pUserData);
        pNode->pUserData = nullptr;
    }
    if (pNode->pData)
    {
        FOG_FreePool(pCache->pPool, pNode->pData, szSpriteCacheCpp, 2551, 0);
        pNode->pData = nullptr;
    }

    if (pCache->pFreeTail)
    {
        pCache->pFreeTail->pNext = pNode;
        pNode->pPrev = pCache->pFreeTail;
    }
    else
    {
        pCache->pFreeHead = pNode;
    }
    pCache->pFreeTail = pNode;
    --pCache->nActive;
    return pNode;
}


//1.10f: D2CMP.0x6FDFDCA0
//  Grows ptBlock by dwDeltaBytes: under the lock its bytes leave the budget, it is marked busy, its size grows and the new size joins the budget.
//  Then, while the budget is exceeded (compared unsigned), nodes are evicted from the queues bAllowQueue1 permits (evicting ptBlock itself is
//  fatal, and an eviction that finds nothing ends the loop). Last the busy mark is cleared under the lock. Nothing happens without a critical
//  section.
void __fastcall SPRITECACHE_SpriteDataCache_AddNodeBytes(D2CmpSpriteDataCache *pCache, D2CmpSpriteDataNode *ptBlock, uint32_t dwDeltaBytes, int32_t bAllowQueue1)
{
    if (!pCache->pcs)
    {
        return;
    }

    EnterCriticalSection(pCache->pcs);
    pCache->dwAllocatedBytes -= ptBlock->dwSize;
    ptBlock->dwSize += dwDeltaBytes;
    ptBlock->dwFlags |= 0x02;
    pCache->dwAllocatedBytes += ptBlock->dwSize;
    uint32_t dwAllocatedBytes = pCache->dwAllocatedBytes;
    LeaveCriticalSection(pCache->pcs);

    while (dwAllocatedBytes > pCache->dwMaxAllocated)
    {
        D2CmpSpriteDataNode *ptFreedBlock = SPRITECACHE_SpriteDataCache_EvictNode(pCache, nullptr, bAllowQueue1);
        if (!ptFreedBlock)
        {
            break;
        }
        if (ptFreedBlock == ptBlock)
        {
            FOG_DisplayAssert("ptFreedBlock != ptBlock", szSpriteCacheCpp, 2654);
            exit(-1);
        }
        EnterCriticalSection(pCache->pcs);
        dwAllocatedBytes = pCache->dwAllocatedBytes;
        LeaveCriticalSection(pCache->pcs);
    }

    EnterCriticalSection(pCache->pcs);
    ptBlock->dwFlags &= ~0x02u;
    LeaveCriticalSection(pCache->pcs);
}


//1.10f: D2CMP.0x6FDFDD60
//  Appends pNode to the tail of queue bQueue1 and stamps it with GetTickCount. pNode is not tested: AllocateNode's NULL is written through, at
//  +0x18 when the queue has a tail and at +0x10 when it has none. void: the original leaves the stored tick in eax and no caller reads it.
void __fastcall SPRITECACHE_SpriteDataCache_AppendQueuedNode(D2CmpSpriteDataCache *pCache, D2CmpSpriteDataNode *pNode, int32_t bQueue1)
{
    if (bQueue1)
    {
        if (pCache->pQueue1Tail)
        {
            pCache->pQueue1Tail->pNext = pNode;
            pNode->pPrev = pCache->pQueue1Tail;
        }
        else
        {
            pCache->pQueue1Head = pNode;
        }
        pCache->pQueue1Tail = pNode;
    }
    else
    {
        if (pCache->pQueue0Tail)
        {
            pCache->pQueue0Tail->pNext = pNode;
            pNode->pPrev = pCache->pQueue0Tail;
        }
        else
        {
            pCache->pQueue0Head = pNode;
        }
        pCache->pQueue0Tail = pNode;
    }
    pNode->dwQueuedTick = GetTickCount();
}


//1.10f: D2CMP.0x6FDFDDE0
//  Takes the head off queue bQueue1 and returns it, or NULL for an empty queue or a busy head.
D2CmpSpriteDataNode *__fastcall SPRITECACHE_SpriteDataCache_PopQueueHead(D2CmpSpriteDataCache *pCache, int32_t bQueue1)
{
    D2CmpSpriteDataNode *pNode;
    if (bQueue1)
    {
        pNode = pCache->pQueue1Head;
        if (!pNode || (pNode->dwFlags & 0x02))
        {
            return nullptr;
        }
        pCache->pQueue1Head = pNode->pNext;
        if (pNode->pNext)
        {
            pNode->pNext->pPrev = nullptr;
        }
        else
        {
            pCache->pQueue1Tail = nullptr;
        }
    }
    else
    {
        pNode = pCache->pQueue0Head;
        if (!pNode || (pNode->dwFlags & 0x02))
        {
            return nullptr;
        }
        pCache->pQueue0Head = pNode->pNext;
        if (pNode->pNext)
        {
            pNode->pNext->pPrev = nullptr;
        }
        else
        {
            pCache->pQueue0Tail = nullptr;
        }
    }
    pNode->pNext = nullptr;
    pNode->pPrev = nullptr;
    return pNode;
}


//1.10f: D2CMP.0x6FDFDE60
//  Takes ptBlock off the queue its bit 0 names (asserted non-NULL, then exit): the head through PopQueueHead (a tail jump in the original), the
//  tail by its own pPrev, any other node by its two neighbours. Nothing happens when the queue is empty, or for a node with no links that is
//  neither head nor tail. void: the original leaves a node in eax and no caller reads it.
//  BUG: a busy head is left where it is (PopQueueHead refuses it, and its answer is not read). NOT ESTABLISHED: not certainly a defect.
//  BUG: a middle node with exactly one link writes through the missing one (0x6FDFDEF0..0x6FDFDEFC). NOT ESTABLISHED: a consistent queue
//  has no such node.
void __fastcall SPRITECACHE_SpriteDataCache_DetachQueuedNode(D2CmpSpriteDataCache *pCache, D2CmpSpriteDataNode *ptBlock)
{
    if (!ptBlock)
    {
        FOG_DisplayAssert("ptBlock", szSpriteCacheCpp, 2841);
        exit(-1);
    }

    if (ptBlock->dwFlags & 0x01)
    {
        if (!pCache->pQueue1Head)
        {
            return;
        }
        if (ptBlock == pCache->pQueue1Head)
        {
            SPRITECACHE_SpriteDataCache_PopQueueHead(pCache, 1);
            return;
        }
        if (ptBlock == pCache->pQueue1Tail)
        {
            pCache->pQueue1Tail = ptBlock->pPrev;
            pCache->pQueue1Tail->pNext = nullptr;
            ptBlock->pNext = nullptr;
            ptBlock->pPrev = nullptr;
            return;
        }
    }
    else
    {
        if (!pCache->pQueue0Head)
        {
            return;
        }
        if (ptBlock == pCache->pQueue0Head)
        {
            SPRITECACHE_SpriteDataCache_PopQueueHead(pCache, 0);
            return;
        }
        if (ptBlock == pCache->pQueue0Tail)
        {
            pCache->pQueue0Tail = ptBlock->pPrev;
            pCache->pQueue0Tail->pNext = nullptr;
            ptBlock->pNext = nullptr;
            ptBlock->pPrev = nullptr;
            return;
        }
    }

    if (ptBlock->pPrev || ptBlock->pNext)
    {
        ptBlock->pPrev->pNext = ptBlock->pNext;
        ptBlock->pNext->pPrev = ptBlock->pPrev;
        ptBlock->pNext = nullptr;
        ptBlock->pPrev = nullptr;
    }
}


//1.10f: D2CMP.0x6FDFD7D0
//  #10059: hands the sprite GfxHash storage to CELDATAHASH_10106 (the argument in ECX, then a tail jump). That function is a one-byte `ret`, which
//  #10062 jumps to as well, so the hash statistics of a debug build evidently compiled to nothing.
extern "C" void __cdecl SPRITECACHE_SpriteCacheDumpHashTableStats()
{
    CELDATAHASH_10106(&gSpriteGfxHashStorage);
}


//1.10f: D2CMP.0x6FDFD7E0
//  #10060: the data cache's bytes in use, read under its lock; the lock must exist (asserted: "sgtGfxCache.pcs", then exit).
extern "C" uint32_t __cdecl SPRITECACHE_SpriteCacheGetUsedMemory()
{
    if (!sgtGfxCache.pcs)
    {
        FOG_DisplayAssert("sgtGfxCache.pcs", szSpriteCacheCpp, 1996);
        exit(-1);
    }

    EnterCriticalSection(sgtGfxCache.pcs);
    const uint32_t dwAllocatedBytes = sgtGfxCache.dwAllocatedBytes;
    LeaveCriticalSection(sgtGfxCache.pcs);
    return dwAllocatedBytes;
}


//1.10f: D2CMP.0x6FDFD830
//  #10061: the data cache's budget, read unlocked.
extern "C" uint32_t __cdecl SPRITECACHE_SpriteCacheGetMaxMemory()
{
    return sgtGfxCache.dwMaxAllocated;
}


//1.10f: D2CMP.0x6FDFD840
//  #10062: as #10059, for the cel header hash.
extern "C" void __cdecl SPRITECACHE_CelDataCacheDumpHashTableStats()
{
    CELDATAHASH_10106(&gCelHeaderHash);
}


//1.10f: D2CMP.0x6FDFD850
//  #10063: the header LRU cache's bytes in use, read unlocked (unlike #10060).
extern "C" uint32_t __cdecl SPRITECACHE_CelDataCacheGetUsedMemory()
{
    return gCelHeaderLruCache.dwAllocated;
}


//1.10f: D2CMP.0x6FDFD860
//  #10064: the header LRU cache's budget.
extern "C" uint32_t __cdecl SPRITECACHE_CelDataCacheGetMaxMemory()
{
    return gCelHeaderLruCache.dwMaxAllocated;
}


//1.10f: D2CMP.0x6FDFDF10
//  #10057: counts the queue-1 nodes whose direction block is still loading: each node with data whose block's bFlags bit 0 is clear, its async data
//  being required (asserted "hAsyncData", then exit) and tested for completion. The queue is walked unlocked.
extern "C" int32_t __cdecl SPRITECACHE_SpriteNumAsyncLoads()
{
    int32_t nPendingLoads = 0;
    for (D2CmpSpriteDataNode *pNode = sgtGfxCache.pQueue1Head; pNode; pNode = pNode->pNext)
    {
        D2CmpSpriteGfxNodePrefix *pGfxNode = (D2CmpSpriteGfxNodePrefix *)pNode->pData;
        if (!pGfxNode || (pGfxNode->tGfxBlock.bFlags & 0x01))
        {
            continue;
        }
        if (!pGfxNode->tGfxBlock.lpAsyncData)
        {
            FOG_DisplayAssert("hAsyncData", szSpriteCacheCpp, 2938);
            exit(-1);
        }
        if (!FOG_AsyncDataTestLoaded(pGfxNode->tGfxBlock.lpAsyncData))
        {
            ++nPendingLoads;
        }
    }
    return nPendingLoads;
}


//1.10f: D2CMP.0x6FDFDF70
//  #10058: returns 1 when a queue-1 node holds a direction block of this unit type, mode and unit index (the gfx node's +0, +4 and +8, compared in
//  the order +8, +4, +0) whose async load has not finished, else 0; the same tests as #10057 (bFlags bit 0 clear, async data asserted).
extern "C" int32_t __stdcall SPRITECACHE_SpriteFindAsynchSiblings(int32_t nUnitType, int32_t nMode, int32_t nUnitIndex)
{
    for (D2CmpSpriteDataNode *pNode = sgtGfxCache.pQueue1Head; pNode; pNode = pNode->pNext)
    {
        D2CmpSpriteGfxNodePrefix *pGfxNode = (D2CmpSpriteGfxNodePrefix *)pNode->pData;
        if (!pGfxNode || (pGfxNode->tGfxBlock.bFlags & 0x01) || pGfxNode->nUnitIndex != nUnitIndex || pGfxNode->nMode != nMode || pGfxNode->nUnitType != nUnitType)
        {
            continue;
        }
        if (!pGfxNode->tGfxBlock.lpAsyncData)
        {
            FOG_DisplayAssert("hAsyncData", szSpriteCacheCpp, 2970);
            exit(-1);
        }
        if (!FOG_AsyncDataTestLoaded(pGfxNode->tGfxBlock.lpAsyncData))
        {
            return 1;
        }
    }
    return 0;
}


//1.10f: D2CMP.0x6FDFE000
//  #10099: frees the async data of every queue-1 node whose load has not finished and clears the block's pointer. Unlike #10057 and #10058 it
//  neither tests bFlags bit 0 nor asserts a missing async object; it skips it. The nodes themselves stay queued.
extern "C" void __stdcall SPRITECACHE_SpriteFreeAsyncLoads()
{
    for (D2CmpSpriteDataNode *pNode = sgtGfxCache.pQueue1Head; pNode; pNode = pNode->pNext)
    {
        D2CmpSpriteGfxNodePrefix *pGfxNode = (D2CmpSpriteGfxNodePrefix *)pNode->pData;
        if (!pGfxNode)
        {
            continue;
        }
        AsyncData *pAsyncData = pGfxNode->tGfxBlock.lpAsyncData;
        if (pAsyncData && !FOG_AsyncDataTestLoaded(pAsyncData))
        {
            FOG_AsyncDataFree(pAsyncData);
            pGfxNode->tGfxBlock.lpAsyncData = nullptr;
        }
    }
}


//1.10f: D2CMP.0x6FDFB930
//  #10051: the extension of a cel file name: ".dcc" when the caller allows compressed files (any non-zero argument) and the module's setting is on,
//  else ".dc6", each read through its pointer (gpszDccExtension, gpszDc6Extension). A zero argument answers ".dc6" without reading the setting.
extern "C" const char *__stdcall SPRITECACHE_GetGfxFileExtension(int32_t bAllowCompressed)
{
    if (bAllowCompressed && gdwSpriteCompressedDataSetting)
    {
        return gpszDccExtension;
    }
    return gpszDc6Extension;
}


//1.10f: D2CMP.0x6FDFB950
//  #10050: formats Format and its va_list into Buffer with vsprintf, then appends the extension that #10051 would answer for bAllowCompressed (the
//  same test written out again rather than a call; the original appends it with an inline strcat). Nothing bounds either write. It takes four stack
//  arguments and is not variadic itself. void: the original leaves 0 in eax, but that is its inline strcat's leftover, and no module of the game
//  imports #10050.
//  DIFFERENCE: the original calls the statically linked VC6 runtime's vsprintf; ours calls the toolchain's, as the runtime is not rewritten.
extern "C" void __stdcall SPRITECACHE_BuildFileName(char *Buffer, int32_t bAllowCompressed, const char *Format, va_list ArgList)
{
    vsprintf(Buffer, Format, ArgList);

    const char *szExtension = gpszDc6Extension;
    if (bAllowCompressed && gdwSpriteCompressedDataSetting)
    {
        szExtension = gpszDccExtension;
    }
    strcat(Buffer, szExtension);
}


//1.10f: D2CMP.0x6FDFB9C0
//  #10054: loads the compressed-data setting from the registry (Storm's SRegLoadValue, key "Diablo II", value "CompressedData", flags 1, into
//  gdwSpriteCompressedDataSetting; its answer is not read) and, when the caller does not allow compressed files, clears the setting whatever the
//  registry said. void: the eax the original leaves is the argument, and neither importer reads it.
extern "C" void __stdcall SPRITECACHE_SetNoCompress(int32_t bAllowCompressedMode)
{
    STORM_SRegLoadValue("Diablo II", "CompressedData", 1, &gdwSpriteCompressedDataSetting);
    if (!bAllowCompressedMode)
    {
        gdwSpriteCompressedDataSetting = 0;
    }
}


static_assert(sizeof(D2CmpLruCache) == 0x34, "the header LRU cache cleared whole: rep stosd, ecx = 0Dh (0x6FDFBA72)");
static_assert(sizeof(D2CmpSpriteDataCache) == 0x3C, "sgtGfxCache cleared whole: rep stosd, ecx = 0Fh (0x6FDFBB3A)");
static_assert(offsetof(D2CmpCelHeaderHashEntry, lpBlock) == 0x24, "OnHeaderEvict: mov dword ptr [eax+24h], 0 (0x6FDFBBD9)");
static_assert(offsetof(D2CmpSpriteDecodeJobPrefix, dwDecodedBytes) == 0x18, "RunDecodeJob: lea eax, [esi+18h] (0x6FDFBCE8)");
static_assert(offsetof(D2CmpSpriteGfxBlockPrefix, pCacheNode) == 0x94, "RunDecodeJob: mov edi, [ebx+94h] (0x6FDFBCCC)");
static_assert(offsetof(D2CmpSpriteHeaderData, szKey) == 0xA0, "RunDecodeJob: lea ecx, [ebp+0A0h] (0x6FDFBCD2)");
static_assert(offsetof(D2CmpFogSystemInfoPrefix, dwTotalPhysicalBytes) == 0x0C, "SpriteCacheInit: mov eax, [eax+0Ch] (0x6FDFBA24)");


//1.10f: D2CMP.0x6FDFB9F0
//  #10052: sets the sprite cache up, tearing down what an earlier call left. It starts the codec's critical section and sets the "low end machine"
//  flag: true when the physical memory Fog reports less 11 MB (0xB00000), or dwMemoryOverride when that is non-zero, is below 64 MB (compared
//  unsigned); the flag is traced. Header side: the cel header hash is cleared and the header LRU cache is destroyed, cleared and set up again with
//  OnHeaderEvict and gnCelHeaderCacheEntryCount entries (its budget traced). Sprite side: the GfxHash storage is cleared twice over (the original's
//  inline `rep stosd`, written here as a memset, then GFXHASH_ClearStorage), every node of sgtGfxCache is evicted while any is active and an
//  eviction succeeds, its node block and critical section are freed, and the cache is cleared and set up with OnDataEvict.
extern "C" void __stdcall SPRITECACHE_SpriteCacheInit(void *pPool, int32_t nSpriteCacheBytes, int32_t nCelHeaderCacheBytes, uint32_t dwMemoryOverride)
{
    CODEC_InitCriticalSection();

    uint32_t dwMemoryClassMetric = dwMemoryOverride;
    if (!dwMemoryOverride)
    {
        D2CmpFogSystemInfoPrefix *pSysInfo = FOG_GetSystemInfo();
        if (!pSysInfo)
        {
            FOG_DisplayAssert("pSysInfo != NULL", szSpriteCacheCpp, 269);
            exit(-1);
        }
        dwMemoryClassMetric = pSysInfo->dwTotalPhysicalBytes - 0xB00000;
    }
    gbSpriteLowEndMachine = dwMemoryClassMetric < 0x4000000;
    FOG_Trace("Low End Machine: %s", gbSpriteLowEndMachine ? "TRUE" : "FALSE");

    CELDATAHASH_Clear(&gCelHeaderHash);
    LRUCACHE_Destroy(&gCelHeaderLruCache);
    memset(&gCelHeaderLruCache, 0, sizeof(D2CmpLruCache));
    LRUCACHE_Init(&gCelHeaderLruCache, pPool, nCelHeaderCacheBytes, SPRITECACHE_OnHeaderEvict, gnCelHeaderCacheEntryCount);
    FOG_Trace("D2CMPSpriteCacheInit: %d", gCelHeaderLruCache.dwMaxAllocated);

    memset(&gSpriteGfxHashStorage, 0, sizeof(D2CmpSpriteGfxHashStorage));
    GFXHASH_ClearStorage(&gSpriteGfxHashStorage);

    if (sgtGfxCache.nActive)
    {
        while (SPRITECACHE_SpriteDataCache_EvictNode(&sgtGfxCache, nullptr, 1) && sgtGfxCache.nActive)
        {
        }
    }
    if (sgtGfxCache.pNodes)
    {
        FOG_FreePool(sgtGfxCache.pPool, sgtGfxCache.pNodes, szSpriteCacheCpp, 2338, 0);
        sgtGfxCache.pNodes = nullptr;
    }
    if (sgtGfxCache.pcs)
    {
        DeleteCriticalSection(sgtGfxCache.pcs);
        FOG_Free(sgtGfxCache.pcs, szSpriteCacheCpp, 2356, 0);
    }
    memset(&sgtGfxCache, 0, sizeof(D2CmpSpriteDataCache));
    SPRITECACHE_SpriteDataCache_Init(&sgtGfxCache, pPool, nSpriteCacheBytes, (void(__fastcall *)(void *))SPRITECACHE_OnDataEvict);
}


//1.10f: D2CMP.0x6FDFBB70
//  The data cache's eviction callback for a direction block: pElement, the GfxHash entry the node's user data holds, is deleted from the storage,
//  after the sprite hardware-free callback is told of its gfx node when that callback is set; nothing ever sets it, so that branch does not run.
//  The data cache hands the entry as `void *`, hence the cast where SpriteCacheInit registers this function. void: the eax the original leaves is
//  GFXHASH_DeleteEntry's, and EvictNode never reads it.
void __fastcall SPRITECACHE_OnDataEvict(D2CmpSpriteGfxHashEntryPrefix *pElement)
{
    if (gSpriteHardwareFreeCallback)
    {
        D2CmpSpriteGfxNodePrefix *ptGfxNode = pElement->pGfxNode;
        if (!ptGfxNode)
        {
            FOG_DisplayAssert("ptGfxNode", szSpriteCacheCpp, 179);
            exit(-1);
        }
        gSpriteHardwareFreeCallback(ptGfxNode->nUnitType, ptGfxNode->nUnitIndex, ptGfxNode->nMode);
    }
    GFXHASH_DeleteEntry(sgtGfxCache.pPool, &gSpriteGfxHashStorage, pElement);
}


//1.10f: D2CMP.0x6FDFBBD0
//  The header LRU cache's eviction callback: the cel header hash entry that the LRU entry's user data points to is unhooked from its LRU block and
//  deleted from the hash.
void __fastcall SPRITECACHE_OnHeaderEvict(D2CmpLruEntry *pEntry)
{
    D2CmpCelHeaderHashEntry *pHashEntry = (D2CmpCelHeaderHashEntry *)pEntry->pUserData;
    pHashEntry->lpBlock = nullptr;
    CELDATAHASH_DeleteEntry(gCelHeaderLruCache.pPool, &gCelHeaderHash, pHashEntry);
}


//1.10f: D2CMP.0x6FDFBBF0
//  #10053: with bFreeCaches, every node of sgtGfxCache is evicted, its node block and critical section are freed and the header LRU cache is
//  destroyed; either way both caches are then cleared and the codec's critical section is shut down.
//  BUG: with bFreeCaches 0 the two caches are cleared without being freed: the 0x23000-byte node block, the critical section and the LRU cache's
//  entries are forgotten, not released (0x6FDFBBF8 jumps over the frees, 0x6FDFBC80 over LRUCACHE_Destroy). REACHABLE: D2Client passes 0 at the end
//  of every game (D2Win passes 1), the section FOG_Alloc gave is neither deleted nor freed, and the next SpriteCacheInit makes another.
extern "C" void __stdcall SPRITECACHE_SpriteCacheDestroy(int32_t bFreeCaches)
{
    if (bFreeCaches)
    {
        if (sgtGfxCache.nActive)
        {
            while (SPRITECACHE_SpriteDataCache_EvictNode(&sgtGfxCache, nullptr, 1) && sgtGfxCache.nActive)
            {
            }
        }
        if (sgtGfxCache.pNodes)
        {
            FOG_FreePool(sgtGfxCache.pPool, sgtGfxCache.pNodes, szSpriteCacheCpp, 2338, 0);
            sgtGfxCache.pNodes = nullptr;
        }
        if (sgtGfxCache.pcs)
        {
            DeleteCriticalSection(sgtGfxCache.pcs);
            FOG_Free(sgtGfxCache.pcs, szSpriteCacheCpp, 2356, 0);
        }
    }
    memset(&sgtGfxCache, 0, sizeof(D2CmpSpriteDataCache));
    if (bFreeCaches)
    {
        LRUCACHE_Destroy(&gCelHeaderLruCache);
    }
    memset(&gCelHeaderLruCache, 0, sizeof(D2CmpLruCache));
    CODEC_ShutdownCriticalSection();
}


//1.10f: D2CMP.0x6FDFBCB0
//  The decode step of a direction block's load, stored as the job's callback by both LoadDirectionBlock functions. With no critical section on
//  sgtGfxCache it returns 0. Otherwise the block's cache node is marked busy while the DCC directions are decoded into the job's buffer and byte
//  count, the block's bFlags bit 2 is cleared, the node is grown by the decoded bytes (only queue 0 may be evicted for them), a failed decode is
//  marked in the header's bFlags bit 0, and it returns 1. The busy mark is set and cleared unlocked here, where AddNodeBytes takes the lock for it.
uint32_t __stdcall SPRITECACHE_RunDecodeJob(void *pInput, uint32_t dwInputBytes, D2CmpSpriteDecodeJobPrefix *pJob)
{
    if (!sgtGfxCache.pcs)
    {
        return 0;
    }

    D2CmpSpriteGfxBlockPrefix *pGfxBlock = pJob->pGfxBlock;
    D2CmpSpriteHeaderData *pHeaderData = pJob->pHeaderData;
    D2CmpSpriteDataNode *pCacheNode = pGfxBlock->pCacheNode;
    pCacheNode->dwFlags |= 0x02;
    pJob->pDecodedBuffer = CODEC_DecodeDccDirections(pInput, sgtGfxCache.pPool, dwInputBytes, pHeaderData, pGfxBlock, pJob->nStartDirection, pJob->nDirections, &pJob->dwDecodedBytes, pHeaderData->szKey);
    pCacheNode->dwFlags &= ~0x02u;
    pGfxBlock->bFlags &= ~0x04;
    SPRITECACHE_SpriteDataCache_AddNodeBytes(&sgtGfxCache, pCacheNode, pJob->dwDecodedBytes, 0);
    if (!pJob->pDecodedBuffer)
    {
        pHeaderData->bFlags |= 0x01;
    }
    return 1;
}


//  The cel kinds (a context's unit type) that the path and key builders switch on: 0 to 4, and 6.
constexpr int32_t SPRITE_KIND_PLAYER = 0;
constexpr int32_t SPRITE_KIND_MONSTER = 1;
constexpr int32_t SPRITE_KIND_OBJECT = 2;
constexpr int32_t SPRITE_KIND_MISSILE = 3;
constexpr int32_t SPRITE_KIND_ITEM = 4;
constexpr int32_t SPRITE_KIND_6 = 6;

//  The bounds the loaders assert, named after their assert texts: OFFSET_ARRAYSIZE, the frame-offset table's 0x4000 dwords, and MAX_DIROFFSETS,
//  the 33 direction offsets of a header.
constexpr uint32_t OFFSET_ARRAYSIZE = 0x4000;
constexpr uint32_t MAX_DIROFFSETS = 33;

static_assert(sizeof(gSpriteHeaderFileOffsets) == OFFSET_ARRAYSIZE * 4, "the table the bound guards: 65536 bytes, the .i64's item");

static_assert(offsetof(D2GfxDataStrc, fFlags) == 0x18, "the DCC flag: mov eax, [esi+18h] / or al, 1 (0x6FDFC374)");
static_assert(offsetof(D2GfxDataStrc, nOverlay) == 0x2C, "cmp eax, 7Ah with eax = [esi+2Ch] (0x6FDFC3B2)");
static_assert(offsetof(D2GfxDataStrc, szComponent) == 0x34, "mov al, [esi+34h] (0x6FDFC325)");
static_assert(offsetof(D2CmpSpriteHeaderData, nDirectionOffsets) == 0x1C, "lea ecx, [esi+1Ch] (0x6FDFC741)");
static_assert(sizeof(D2CmpSpriteHeaderData) == 0xC0, "the header's LRU block: mov edx, 0C0h (0x6FDFC5D5)");


//1.10f: D2CMP.0x6FDFC200
//  Builds the file a context's sprite loads from, into szPath, or an empty path and 0. Nothing for an empty key. Missiles and kind 6 (with fState
//  bit 1) take a flat name under their root, the missile's pName and kind 6's key: "%s\%s.dcc" while compressed data is enabled (and then fFlags
//  bit 0, the DCC flag, is set) or "%s\%s.dc6"; overlay 122 is DC6 always. Items need pName, and only modes 4 and 3 (IMODE_ONCURSOR, IMODE_ONGROUND)
//  have a file, "DATA\GLOBAL\items\<key>.dc6". Players, monsters and objects: "<root>\<token>\<component>\<key>.dcc" or ".dc6", the token's and
//  component's first three characters each with a space made the end of the string; DC6 when compression is off, for the key "OYTRlitTNhth"
//  (compared case-insensitively; the DCC flag is then cleared), for objects 342 and 563 (OBJECT_HELLGATE_PORTAL,
//  OBJECT_BAALS_PORTAL_WORLDSTONECHAMBER) and for the monster classes BuildHeaderKey spares too: 242, 251, 367 and 521 always (MEPHISTO, TYRAEL1,
//  TYRAEL2, TYRAEL3) and 243, 284, 333, 544, 559 and 570 in mode 0 (DIABLO, MAGGOTQUEEN1, DIABLOCLONE, BAALCRAB, BAALCRABSTAIRS, BAALCLONE). The
//  key test runs, and can clear the flag, for every kind that reaches it, kind 5 too, before that kind is refused.
int32_t __fastcall SPRITECACHE_BuildFilePath(D2GfxDataStrc *pContext, char *szPath, const char *szKey)
{
    if (!*szKey)
    {
        *szPath = 0;
        return 0;
    }

    const char *szRoot;
    const char *szName;
    switch (pContext->nUnitType)
    {
    case SPRITE_KIND_MISSILE:
        if (!pContext->pName)
        {
            *szPath = 0;
            return 0;
        }
        szRoot = "DATA\\GLOBAL\\MISSILES";
        szName = pContext->pName;
        if (gdwSpriteCompressedDataSetting)
        {
            wsprintfA(szPath, "%s\\%s.dcc", szRoot, szName);
            pContext->fFlags |= 0x01;
            return 1;
        }
        wsprintfA(szPath, "%s\\%s.dc6", szRoot, szName);
        return 1;

    case SPRITE_KIND_ITEM:
        if (!pContext->pName)
        {
            *szPath = 0;
            return 0;
        }
        if (pContext->nMode != 4 && pContext->nMode != 3)
        {
            *szPath = 0;
            return 0;
        }
        wsprintfA(szPath, "%s\\items\\%s.dc6", "DATA\\GLOBAL", szKey);
        return 1;

    case SPRITE_KIND_6:
        if (!(pContext->fState & 0x02))
        {
            *szPath = 0;
            return 0;
        }
        szRoot = "DATA\\GLOBAL\\OVERLAYS";
        szName = szKey;
        if (pContext->nOverlay != 122 && gdwSpriteCompressedDataSetting)
        {
            wsprintfA(szPath, "%s\\%s.dcc", szRoot, szName);
            pContext->fFlags |= 0x01;
            return 1;
        }
        wsprintfA(szPath, "%s\\%s.dc6", szRoot, szName);
        return 1;
    }

    uint32_t bCompressed = gdwSpriteCompressedDataSetting;
    if (bCompressed && !STORM_SStrCmpI(szKey, "OYTRlitTNhth", 0x7FFFFFFF))
    {
        pContext->fFlags &= ~0x01u;
        bCompressed = 0;
    }

    switch (pContext->nUnitType)
    {
    case SPRITE_KIND_PLAYER:
        szRoot = "DATA\\GLOBAL\\CHARS";
        break;

    case SPRITE_KIND_MONSTER:
        szRoot = "DATA\\GLOBAL\\MONSTERS";
        switch (pContext->nUnitIndex)
        {
        case 242:
        case 251:
        case 367:
        case 521:
            bCompressed = 0;
            break;
        case 243:
        case 284:
        case 333:
        case 544:
        case 559:
        case 570:
            if (!pContext->nMode)
            {
                bCompressed = 0;
            }
            break;
        }
        break;

    case SPRITE_KIND_OBJECT:
        if (pContext->nUnitIndex == 342 || pContext->nUnitIndex == 563)
        {
            bCompressed = 0;
        }
        szRoot = "DATA\\GLOBAL\\OBJECTS";
        break;

    default:
        *szPath = 0;
        return 0;
    }

    char szToken[4];
    char szComponent[4];
    for (int32_t i = 0; i < 3; ++i)
    {
        szToken[i] = pContext->szToken[i] == ' ' ? 0 : pContext->szToken[i];
        szComponent[i] = pContext->szComponent[i] == ' ' ? 0 : pContext->szComponent[i];
    }
    szToken[3] = 0;
    szComponent[3] = 0;

    if (bCompressed)
    {
        wsprintfA(szPath, "%s\\%s\\%s\\%s.dcc", szRoot, szToken, szComponent, szKey);
        pContext->fFlags |= 0x01;
    }
    else
    {
        wsprintfA(szPath, "%s\\%s\\%s\\%s.dc6", szRoot, szToken, szComponent, szKey);
    }
    return 1;
}


//1.10f: D2CMP.0x6FDFC530
//  Returns the cel file header for the context ("sValidateCelData" in the original's last assert text): from the header LRU cache when its key is
//  known (the entry is touched), else read now. The key (BuildHeaderKey) and the file name (BuildFilePath) are built first; if either fails it
//  returns NULL with the path left empty. A hash entry and a 0xC0-byte LRU block are made for the header, the key is copied into the cleared block,
//  and the header is filled: for a DCC file by CODEC_ReadDccHeader; for a DC6 file from its 24-byte header, its frame-offset table (read into the
//  module's one buffer gSpriteHeaderFileOffsets) and the file size. A failure of either sets the header's bFlags bit 0 and keeps the entry.
//  The file handle starts as szKey because in the original it shares szKey's argument slot; FOG_FOpenFile (Storm's SFileOpenFileEx) clears it
//  before anything else, so the key is never mistaken for a handle.
//  BUG: the key is copied up to its NUL into the header's 32 bytes, unbounded (0x6FDFC620..0x6FDFC628, an inlined strcpy); CelDataHash.cpp's NewEntry
//  copies the same key the same way. NOT ESTABLISHED: whether a key can exceed 32 bytes is not settled.
D2CmpSpriteHeaderData *__fastcall SPRITECACHE_ResolveHeader(D2GfxDataStrc *pContext, char *szPath, char *szKey)
{
    *szPath = 0;
    if (!SPRITECACHE_BuildHeaderKey(pContext, szKey))
    {
        return nullptr;
    }

    D2CmpSpriteHeaderData *ptCelData;
    D2CmpCelHeaderHashEntry *pFoundEntry = CELDATAHASH_FindEntry(&gCelHeaderHash, szKey);
    if (pFoundEntry)
    {
        LRUCACHE_TouchEntry(&gCelHeaderLruCache, pFoundEntry->lpBlock);
        ptCelData = (D2CmpSpriteHeaderData *)pFoundEntry->lpBlock->pData;
    }
    else
    {
        if (!SPRITECACHE_BuildFilePath(pContext, szPath, szKey))
        {
            return nullptr;
        }

        D2CmpCelHeaderHashEntry *ptHashTableElement = CELDATAHASH_NewEntry(gCelHeaderLruCache.pPool, &gCelHeaderHash, nullptr, szKey);
        if (!ptHashTableElement)
        {
            FOG_DisplayAssert("ptHashTableElement", szSpriteCacheCpp, 1032);
            exit(-1);
        }
        ptHashTableElement->lpBlock = LRUCACHE_AllocEntry(&gCelHeaderLruCache, sizeof(D2CmpSpriteHeaderData), ptHashTableElement);
        if (!ptHashTableElement->lpBlock)
        {
            FOG_DisplayAssert("ptHashTableElement->lpBlock", szSpriteCacheCpp, 1037);
            exit(-1);
        }

        ptCelData = (D2CmpSpriteHeaderData *)ptHashTableElement->lpBlock->pData;
        memset(ptCelData, 0, sizeof(D2CmpSpriteHeaderData));
        strcpy(ptCelData->szKey, szKey);

        if (pContext->fFlags & 0x01)
        {
            if (!CODEC_ReadDccHeader(szPath, ptCelData))
            {
                ptCelData->bFlags |= 0x01;
            }
        }
        else
        {
            void *hFile = szKey;
            FOG_FOpenFile(szPath, &hFile);
            if (!hFile)
            {
                ptCelData->bFlags |= 0x01;
            }
            else
            {
                uint32_t dwFileHeader[7] = {};
                FOG_FReadFile(hFile, dwFileHeader, 24, nullptr, 0, 0, 0);
                const uint32_t dwFileSize = FOG_FGetFileSize(hFile, nullptr);
                ptCelData->nFrames = dwFileHeader[5];
                ptCelData->dwFileFlags = dwFileHeader[1];
                ptCelData->dwVersion = dwFileHeader[0];
                ptCelData->nDirections = dwFileHeader[4];
                ptCelData->dwFormat = dwFileHeader[2];

                const uint32_t dwTotalCels = dwFileHeader[4] * dwFileHeader[5];
                if (dwTotalCels >= OFFSET_ARRAYSIZE)
                {
                    FOG_DisplayAssert("dwTotalCels < OFFSET_ARRAYSIZE", szSpriteCacheCpp, 1080);
                    exit(-1);
                }
                FOG_FReadFile(hFile, gSpriteHeaderFileOffsets, 4 * dwTotalCels, nullptr, 0, 0, 0);
                FOG_FCloseFile(hFile);

                if (ptCelData->nDirections >= MAX_DIROFFSETS)
                {
                    FOG_DisplayAssert("ptCelData->tHeader.dwNumDirections < MAX_DIROFFSETS", szSpriteCacheCpp, 1085);
                    exit(-1);
                }
                for (uint32_t nDirection = 0; nDirection < ptCelData->nDirections; ++nDirection)
                {
                    //  BUG: the bound above tests directions times frames as a 32-bit product (`imul` at 0x6FDFC6B6, then `cmp eax, 4000h` /
                    //  `jb`), so a header whose product wraps passes it, and this loop then reads the table at direction times frames
                    //  (0x6FDFC74A, 0x6FDFC74E), an index that can be 0x4000 or more. NOT ESTABLISHED: whether a stock header wraps the
                    //  product is not settled.
                    //  DIFFERENCE: past the table the original reads whatever memory is there, which a C read past our array cannot
                    //  reproduce; ours answers 0. For a header that does not wrap every index is below the product, inside the table, and
                    //  the answers agree.
                    const uint32_t nOffsetIndex = nDirection * ptCelData->nFrames;
                    ptCelData->nDirectionOffsets[nDirection] = nOffsetIndex < OFFSET_ARRAYSIZE ? gSpriteHeaderFileOffsets[nOffsetIndex] : 0;
                }
                ptCelData->nDirectionOffsets[ptCelData->nDirections] = dwFileSize;
            }
        }
    }

    if (!ptCelData)
    {
        FOG_DisplayAssert("[sValidateCelData] ptCelData was NULL.\n", szSpriteCacheCpp, 1104);
        exit(-1);
    }
    return ptCelData;
}


//1.10f: D2CMP.0x6FDFC780
//  Builds the header cache's key for a context into szKey: for players, monsters and objects the five name parts' first three characters each, a
//  part ending at its first space; for missiles, items and kind 6 (fState bit 1) the context's pName, which must be set. Anything else, or a
//  missing name, gives an empty key and 0. On the way it sets fFlags bit 0, the DCC flag, while compressed data is enabled: for missiles, for kind 6
//  but overlay 122, and for players, monsters and objects but the classes BuildFilePath keeps DC6; never for items. The copies of pName are
//  unbounded.
int32_t __fastcall SPRITECACHE_BuildHeaderKey(D2GfxDataStrc *pContext, char *szKey)
{
    switch (pContext->nUnitType)
    {
    case SPRITE_KIND_PLAYER:
    case SPRITE_KIND_MONSTER:
    case SPRITE_KIND_OBJECT:
    {
        char *pOut = szKey;
        for (int32_t i = 0; i < 5; ++i)
        {
            //  Each part is read once, whole, and its characters are taken from that dword.
            const uint32_t dwPart = pContext->dwName[i];
            if ((char)dwPart == ' ')
            {
                continue;
            }
            *pOut++ = (char)dwPart;
            if ((char)(dwPart >> 8) == ' ')
            {
                continue;
            }
            *pOut++ = (char)(dwPart >> 8);
            if ((char)(dwPart >> 16) == ' ')
            {
                continue;
            }
            *pOut++ = (char)(dwPart >> 16);
        }
        *pOut = 0;

        //  The kind is read again after the stores, as the original does.
        if (pContext->nUnitType == SPRITE_KIND_MONSTER)
        {
            switch (pContext->nUnitIndex)
            {
            case 242:
            case 251:
            case 367:
            case 521:
                return 1;
            case 243:
            case 284:
            case 333:
            case 544:
            case 559:
            case 570:
                if (!pContext->nMode)
                {
                    return 1;
                }
                break;
            }
        }
        else if (pContext->nUnitType == SPRITE_KIND_OBJECT)
        {
            if (pContext->nUnitIndex == 342 || pContext->nUnitIndex == 563)
            {
                return 1;
            }
        }
        if (gdwSpriteCompressedDataSetting)
        {
            pContext->fFlags |= 0x01;
        }
        return 1;
    }

    case SPRITE_KIND_MISSILE:
        if (!pContext->pName)
        {
            break;
        }
        if (gdwSpriteCompressedDataSetting)
        {
            pContext->fFlags |= 0x01;
        }
        strcpy(szKey, pContext->pName);
        return 1;

    case SPRITE_KIND_ITEM:
        if (!pContext->pName)
        {
            break;
        }
        strcpy(szKey, pContext->pName);
        return 1;

    case SPRITE_KIND_6:
        if (!(pContext->fState & 0x02) || !pContext->pName)
        {
            break;
        }
        if (pContext->nOverlay != 122 && gdwSpriteCompressedDataSetting)
        {
            pContext->fFlags |= 0x01;
        }
        strcpy(szKey, pContext->pName);
        return 1;
    }

    *szKey = 0;
    return 0;
}


//  The header's bFlags bit 0: the cel file could not be read (set by ResolveHeader).
constexpr uint8_t CELDATAFLAG_NOFILE = 0x01;

//  The bytes of hardware cel data each frame gets.
constexpr uint32_t SPRITE_HARDWARE_SLOT_BYTES = 44;

static_assert(sizeof(D2CmpSpriteGfxNodePrefix) == 0xB8, "the gfx node's data block: mov edx, 0B8h (0x6FDFCC35)");
static_assert(offsetof(D2CmpSpriteGfxNodePrefix, tGfxBlock) == 0x20, "the block: lea eax, [esi+20h] (0x6FDFCE6A)");
static_assert(offsetof(D2CmpSpriteGfxBlockPrefix, nOffsets) == 0x0C, "mov eax, [ebx+eax*4+0Ch] (0x6FDFD0BE)");
static_assert(offsetof(D2CmpSpriteGfxBlockPrefix, bNumOffsets) == 0x03, "mov dl, [ebx+3] (0x6FDFD0A0)");
static_assert(offsetof(D2CmpSpriteGfxBlockPrefix, bBaseDirection) == 0x02, "mov cl, [esi+22h] (0x6FDFCB54)");
static_assert(offsetof(D2CmpSpriteGfxBlockPrefix, fHardwareCelData) == 0x08, "mov eax, [esi+28h] (0x6FDFCAAC)");
static_assert(offsetof(D2GfxDataStrc, nMaxFrames) == 0x14, "mov [ebp+14h], ecx (0x6FDFCFC2)");


//1.10f: D2CMP.0x6FDFCA30
//  Gives every frame of the direction block a cleared 44-byte hardware slot from one pool block ("sInitializeHardwareCelData" in the original's halt
//  text), walking the frames in the buffer from the base direction's offset and storing each slot's address in the frame's pHardwareData. A frame
//  whose flags are not 0 or whose width or height passes 256 (compared unsigned) halts with the file's path. Then fHardwareCelData is set. void: the
//  original leaves the frame buffer in eax and both callers overwrite it. When the base direction's offset is 0 the pool block's base is the first
//  frame's slot, the pointer GfxHash.cpp's DeleteEntry frees to the pool.
void __fastcall SPRITECACHE_InitializeHardwareSlots(D2CmpSpriteHeaderData *ptCelData, D2GfxDataStrc *ptCelContext, D2CmpSpriteGfxNodePrefix *ptGfxNode)
{
    if (!ptCelContext)
    {
        FOG_DisplayAssert("ptCelContext", szSpriteCacheCpp, 1270);
        exit(-1);
    }
    if (!ptGfxNode)
    {
        FOG_DisplayAssert("ptGfxNode", szSpriteCacheCpp, 1271);
        exit(-1);
    }
    if (ptCelData->bFlags & CELDATAFLAG_NOFILE)
    {
        FOG_DisplayAssert("!(ptCelData->bFlags & CELDATAFLAG_NOFILE)", szSpriteCacheCpp, 1272);
        exit(-1);
    }
    if (ptGfxNode->tGfxBlock.fHardwareCelData)
    {
        FOG_DisplayAssert("!ptGfxBlock->fHardwareCelData", szSpriteCacheCpp, 1275);
        exit(-1);
    }

    const uint32_t dwHardwareBytes = SPRITE_HARDWARE_SLOT_BYTES * ((uint32_t)ptGfxNode->tGfxBlock.bNumCelsPerDir * ptGfxNode->tGfxBlock.bNumDirections);
    uint8_t *pHardwareCursor = (uint8_t *)FOG_AllocPool(sgtGfxCache.pPool, dwHardwareBytes, szSpriteCacheCpp, 1280, 0);
    memset(pHardwareCursor, 0, dwHardwareBytes);

    uint8_t *lpbData = (uint8_t *)FOG_AsyncDataGetBuffer(ptGfxNode->tGfxBlock.lpAsyncData);
    if (!lpbData)
    {
        FOG_DisplayAssert("lpbData", szSpriteCacheCpp, 1293);
        exit(-1);
    }

    int32_t nFrameOffset = ptGfxNode->tGfxBlock.nOffsets[ptGfxNode->tGfxBlock.bBaseDirection];
    for (uint32_t nFrame = 0; nFrame < (uint32_t)ptGfxNode->tGfxBlock.bNumCelsPerDir * ptGfxNode->tGfxBlock.bNumDirections; ++nFrame)
    {
        D2CmpSpriteFrameHeaderPrefix *pFrame = (D2CmpSpriteFrameHeaderPrefix *)(lpbData + nFrameOffset);
        if (pFrame->dwFlags || pFrame->dwWidth > 256 || pFrame->dwHeight > 256)
        {
            char szKey[260];
            char szPath[260];
            char szDiagnostic[256];
            SPRITECACHE_BuildHeaderKey(ptCelContext, szKey);
            SPRITECACHE_BuildFilePath(ptCelContext, szPath, szKey);
            FOG_DisplayHalt(FOG_csprintf(szDiagnostic, "[sInitializeHardwareCelData] There offsets are screwed for the Cel File:\n\n%s\n", szPath), szSpriteCacheCpp, 1418);
            exit(-1);
        }
        const uint32_t dwStreamBytes = pFrame->dwControlBytes + pFrame->dwAuxiliaryBytes;
        pFrame->pHardwareData = pHardwareCursor;
        pHardwareCursor += SPRITE_HARDWARE_SLOT_BYTES;
        nFrameOffset += dwStreamBytes + 35;
    }

    ptGfxNode->tGfxBlock.fHardwareCelData = 1;
}


//1.10f: D2CMP.0x6FDFCC10
//  Loads a direction block now, for the context ("sValidateSynchLoadDirectionBlock" in the original's trace text). A node of 0xB8 bytes comes from
//  sgtGfxCache's queue 0, its gfx node is cleared and keyed by the context's kind and name; when the header has a file (CELDATAFLAG_NOFILE clear)
//  it also gets the unit index and mode, and the block's directions:
//
//      kind 6 (fState bit 1), missile  one direction, the converted one, and only with one direction per block; with more, nothing is loaded
//      object                          direction 0
//      item                            the game direction, unconverted
//      monster, 1 per block            the converted direction
//      the rest                        dwDirBlock times dwDirectionsPerBlock
//
//  A block of 0 bytes (the offsets' difference) traces the mismatch and loads nothing. Otherwise (the async data has just been cleared, so this is
//  always the case) the block's counts and offsets are set and the bytes are loaded through Fog, synchronously, at priority -2: through a decode job
//  whose callback is RunDecodeJob when the header's bFlags bit 1 is set, else raw. Then, file or not, the block is entered in GfxHash.cpp's storage
//  and the two are hooked to each other. With no file the block's bFlags bit 0 is set and 0 is returned; otherwise the hardware slots are made if
//  missing and the context's frame is looked up (FindFrame) into its current cell: 1, or a trace with the context's name and 0.
//  BUG: the first and end directions index the header's 33 offsets before anything bounds them: MAX_DIROFFSETS is asserted only inside the copy loop
//  (0x6FDFCDFA), after the subtraction at 0x6FDFCD26..0x6FDFCD32 has read both. NOT ESTABLISHED: whether an item's game direction or a converted
//  one can reach 33 is not settled.
int32_t __fastcall SPRITECACHE_LoadDirectionBlockSync(D2GfxDataStrc *pContext, D2CmpSpriteHeaderData *pHeader, uint32_t dwDirBlock, uint32_t dwDirectionsPerBlock, const char *szPath)
{
    if (!pHeader)
    {
        return 0;
    }

    D2CmpSpriteDataNode *pCacheNode = SPRITECACHE_SpriteDataCache_AllocateNode(&sgtGfxCache, sizeof(D2CmpSpriteGfxNodePrefix), nullptr, 0);
    if (!pCacheNode)
    {
        return 0;
    }

    D2CmpSpriteGfxNodePrefix *ptGfxNode = (D2CmpSpriteGfxNodePrefix *)pCacheNode->pData;
    memset(ptGfxNode, 0, sizeof(D2CmpSpriteGfxNodePrefix));
    ptGfxNode->nUnitType = pContext->nUnitType;
    ptGfxNode->dwName[0] = pContext->dwName[0];
    ptGfxNode->dwName[3] = pContext->dwName[3];
    ptGfxNode->dwName[1] = pContext->dwName[1];
    ptGfxNode->dwName[2] = pContext->dwName[2];
    ptGfxNode->dwName[4] = pContext->dwName[4];
    ptGfxNode->tGfxBlock.pCacheNode = pCacheNode;

    if (!(pHeader->bFlags & CELDATAFLAG_NOFILE))
    {
        ptGfxNode->nUnitIndex = pContext->nUnitIndex;
        ptGfxNode->nMode = pContext->nMode;

        bool bLoad = true;
        uint32_t dwFirstDirection = 0;
        uint32_t dwEndDirection = 0;
        const int32_t nUnitType = pContext->nUnitType;
        if ((nUnitType == SPRITE_KIND_6 && (pContext->fState & 0x02)) || nUnitType == SPRITE_KIND_MISSILE)
        {
            if (dwDirectionsPerBlock != 1)
            {
                bLoad = false;
            }
            else
            {
                dwFirstDirection = CELCMP_CelGameDirToFileDir(pContext->nDirection, pHeader->nDirections);
                dwEndDirection = dwFirstDirection + 1;
            }
        }
        else if (nUnitType == SPRITE_KIND_OBJECT)
        {
            dwFirstDirection = 0;
            dwEndDirection = 1;
        }
        else if (nUnitType == SPRITE_KIND_ITEM)
        {
            dwFirstDirection = pContext->nDirection;
            dwEndDirection = dwFirstDirection + 1;
        }
        else
        {
            if (nUnitType == SPRITE_KIND_MONSTER && dwDirectionsPerBlock == 1)
            {
                dwFirstDirection = CELCMP_CelGameDirToFileDir(pContext->nDirection, pHeader->nDirections);
            }
            else
            {
                dwFirstDirection = dwDirBlock * dwDirectionsPerBlock;
            }
            dwEndDirection = dwFirstDirection + dwDirectionsPerBlock;
        }

        if (bLoad)
        {
            int32_t *pFirstFileOffset = &pHeader->nDirectionOffsets[dwFirstDirection];
            const int32_t nSourceBytes = pHeader->nDirectionOffsets[dwEndDirection] - *pFirstFileOffset;
            if (!nSourceBytes)
            {
                char szKey[260];
                char szDiagnosticPath[260];
                SPRITECACHE_BuildHeaderKey(pContext, szKey);
                SPRITECACHE_BuildFilePath(pContext, szDiagnosticPath, szKey);
                FOG_Trace("[sValidateSynchLoadDirectionBlock] There are differences between the CEL Data and the Cel File: %s", szDiagnosticPath);
            }
            else if (!ptGfxNode->tGfxBlock.lpAsyncData)
            {
                ptGfxNode->tGfxBlock.bNumDirections = (uint8_t)dwDirectionsPerBlock;
                ptGfxNode->tGfxBlock.bBaseDirection = (uint8_t)(dwDirBlock * dwDirectionsPerBlock);
                ptGfxNode->tGfxBlock.bNumCelsPerDir = (uint8_t)pHeader->nFrames;
                ptGfxNode->tGfxBlock.bNumOffsets = (uint8_t)(pHeader->nDirections + 1);

                const int32_t nBlockFileOffset = *pFirstFileOffset;
                for (uint32_t i = dwFirstDirection; i <= dwEndDirection; ++i)
                {
                    if (i >= MAX_DIROFFSETS)
                    {
                        FOG_DisplayAssert("i < MAX_DIROFFSETS", szSpriteCacheCpp, 841);
                        exit(-1);
                    }
                    ptGfxNode->tGfxBlock.nOffsets[i] = pHeader->nDirectionOffsets[i] - nBlockFileOffset;
                    if (ptGfxNode->tGfxBlock.nOffsets[i] < 0)
                    {
                        FOG_DisplayAssert("ptGfxNode->tGfxBlock.nOffsets[i] >= 0", szSpriteCacheCpp, 843);
                        exit(-1);
                    }
                }

                if (pHeader->bFlags & 0x02)
                {
                    D2CmpSpriteDecodeJobPrefix tDecodeJob = {};
                    tDecodeJob.nDirections = dwDirectionsPerBlock;
                    tDecodeJob.nStartDirection = dwDirBlock * dwDirectionsPerBlock;
                    ptGfxNode->tGfxBlock.bFlags |= 0x04;
                    tDecodeJob.pGfxBlock = &ptGfxNode->tGfxBlock;
                    tDecodeJob.pfnCallback = SPRITECACHE_RunDecodeJob;
                    tDecodeJob.pHeaderData = pHeader;
                    ptGfxNode->tGfxBlock.lpAsyncData = FOG_AsyncDataLoadFileEx(sgtGfxCache.pPool, szPath, 0, *pFirstFileOffset, nSourceBytes, nullptr, (AsyncDataLoadFileCallback *)&tDecodeJob, -2, szSpriteCacheCpp, 868);
                    FOG_AsyncDataGetBufferSize(ptGfxNode->tGfxBlock.lpAsyncData);
                }
                else
                {
                    ptGfxNode->tGfxBlock.lpAsyncData = FOG_AsyncDataLoadFileEx(sgtGfxCache.pPool, szPath, 0, *pFirstFileOffset, nSourceBytes, nullptr, nullptr, -2, szSpriteCacheCpp, 882);
                }
                if (!ptGfxNode->tGfxBlock.lpAsyncData)
                {
                    FOG_DisplayAssert("ptGfxNode->tGfxBlock.lpAsyncData", szSpriteCacheCpp, 886);
                    exit(-1);
                }
            }
        }
    }

    //  The node's data is read back through the cache node; it is the same pointer as ptGfxNode.
    D2CmpSpriteGfxHashEntryPrefix *ptHashTableElement = GFXHASH_NewEntry(sgtGfxCache.pPool, &gSpriteGfxHashStorage, (D2CmpSpriteGfxNodePrefix *)pCacheNode->pData, pContext, pHeader);
    if (!ptHashTableElement)
    {
        FOG_DisplayAssert("ptHashTableElement", szSpriteCacheCpp, 1486);
        exit(-1);
    }
    pCacheNode->pUserData = ptHashTableElement;
    ptHashTableElement->pCacheNode = pCacheNode;

    if (pHeader->bFlags & CELDATAFLAG_NOFILE)
    {
        ptGfxNode->tGfxBlock.bFlags |= 0x01;
        return 0;
    }

    if (!ptGfxNode->tGfxBlock.fHardwareCelData)
    {
        SPRITECACHE_InitializeHardwareSlots(pHeader, pContext, ptGfxNode);
    }

    D2CmpSpriteFrameHeaderPrefix *pFrame = nullptr;
    if (ptGfxNode->tGfxBlock.lpAsyncData)
    {
        uint8_t *pBuffer = (uint8_t *)FOG_AsyncDataGetBuffer(ptGfxNode->tGfxBlock.lpAsyncData);
        if (pBuffer)
        {
            pContext->nMaxFrames = ptGfxNode->tGfxBlock.bNumCelsPerDir;
            pFrame = SPRITECACHE_FindFrame(pContext, &ptGfxNode->tGfxBlock, pBuffer);
        }
    }
    pContext->pCurrentCell = (D2CmpRleCellHeader *)pFrame;
    if (pFrame)
    {
        return 1;
    }
    FOG_Trace("Couldn't get cel handle in for:%s  FILE:%s  LINE:%d", pContext->pName, szSpriteCacheCpp, 1515);
    return 0;
}


//1.10f: D2CMP.0x6FDFD020
//  Returns the context's frame in a loaded direction block, or NULL with a trace naming the failed test ("sFindCel" in the original's trace text);
//  with no buffer or block it returns NULL untraced. The frame must be below the block's cels per direction (compared unsigned). The direction is
//  converted from the game's to the file's, against bNumOffsets - 1 directions, for kind 6 (fState bit 1) and every kind but object (direction 0)
//  and item (the game direction); it must be below bNumOffsets - 1 (compared signed) and its offset not negative. The frames are then walked from
//  that offset by their sizes (35 bytes plus the two counts) up to the context's frame, each with flags 0 and width and height up to 256 and the
//  offset staying non-negative. A negative frame number, which the first test already excludes, would answer the buffer's start.
D2CmpSpriteFrameHeaderPrefix *__fastcall SPRITECACHE_FindFrame(D2GfxDataStrc *ptCelContext, D2CmpSpriteGfxBlockPrefix *ptGfxBlock, uint8_t *pBuffer)
{
    if (!pBuffer || !ptGfxBlock)
    {
        return nullptr;
    }
    if (ptCelContext->nFrame >= ptGfxBlock->bNumCelsPerDir)
    {
        FOG_Trace("(ptCelContext->dwCel < ptGfxBlock->bNumCelsPerDir)");
        return nullptr;
    }

    D2CmpSpriteFrameHeaderPrefix *ptCel = (D2CmpSpriteFrameHeaderPrefix *)pBuffer;
    int32_t nDirection;
    const int32_t nUnitType = ptCelContext->nUnitType;
    if (nUnitType == SPRITE_KIND_6 && (ptCelContext->fState & 0x02))
    {
        nDirection = CELCMP_CelGameDirToFileDir(ptCelContext->nDirection, ptGfxBlock->bNumOffsets - 1);
    }
    else if (nUnitType == SPRITE_KIND_OBJECT)
    {
        nDirection = 0;
    }
    else if (nUnitType == SPRITE_KIND_ITEM)
    {
        nDirection = ptCelContext->nDirection;
    }
    else
    {
        nDirection = CELCMP_CelGameDirToFileDir(ptCelContext->nDirection, ptGfxBlock->bNumOffsets - 1);
    }

    if (nDirection >= (int32_t)ptGfxBlock->bNumOffsets - 1)
    {
        FOG_Trace("SpriteCache.cpp:sFindCel()  (nDirection >= (int)ptGfxBlock->bNumOffsets - 1)");
        return nullptr;
    }
    int32_t nOffset = ptGfxBlock->nOffsets[nDirection];
    if (nOffset < 0)
    {
        FOG_Trace("SpriteCache.cpp:sFindCel()  (nOffset < 0)");
        return nullptr;
    }

    const int32_t nFrame = ptCelContext->nFrame;
    for (int32_t nWalk = 0; nWalk <= nFrame; ++nWalk)
    {
        ptCel = (D2CmpSpriteFrameHeaderPrefix *)(pBuffer + nOffset);
        if (ptCel->dwFlags)
        {
            FOG_Trace("SpriteCache.cpp:sFindCel()  (ptCel->dwFlags != 0)");
            return nullptr;
        }
        if (ptCel->dwWidth > 256 || ptCel->dwHeight > 256)
        {
            FOG_Trace("SpriteCache.cpp:sFindCel()  (ptCel->dwWidth > 256 || ptCel->dwHeight > 256)");
            return nullptr;
        }
        nOffset += ptCel->dwControlBytes + ptCel->dwAuxiliaryBytes + 35;
        if (nOffset < 0)
        {
            FOG_Trace("SpriteCache.cpp:sFindCel():2  (nOffset < 0)");
            return nullptr;
        }
    }
    return ptCel;
}


//1.10f: D2CMP.0x6FDFD160
//  After block 0 of a sprite whose blocks hold more than one direction, loads the next block ahead: the context's direction is moved to the first
//  direction of the block that CELCMP_CelConvertBlock (#10020) picks (its answer shifted by 6 and divided, unsigned, by the file's directions, or 1
//  for a file of one), and when GfxHash.cpp has no entry for that block (FindEntry returns its number, which must be 1, in dwDirBlock and its size,
//  which must be above 1, in dwDirBlockSize) it is loaded, asynchronously or not by bAsyncLoad, the key and path being built first when the
//  caller's are empty. Players prefetch only with fState bit 3 set, outside modes 0 and 17 (PLRMODE_DEATH, PLRMODE_DEAD) and off a low-end machine;
//  every other kind always.
//  BUG: the context's direction is left at the prefetched block's (0x6FDFD1ED), and SpriteValidate, the one caller, does not put it back either.
//  NOT ESTABLISHED: the intent is not known.
void __fastcall SPRITECACHE_PrefetchSecondDirectionBlock(D2GfxDataStrc *pContext, uint32_t dwDirBlock, uint32_t dwDirBlockSize, D2CmpSpriteHeaderData *pHeader, char *szKey, char *szPath, int32_t bAsyncLoad)
{
    if (!pHeader || !pContext)
    {
        return;
    }

    int32_t bPrefetch = 1;
    const int32_t nUnitType = pContext->nUnitType;
    if (!(nUnitType == SPRITE_KIND_6 && (pContext->fState & 0x02)) && nUnitType == SPRITE_KIND_PLAYER)
    {
        if (!(pContext->fState & 0x08) || pContext->nMode == 0 || pContext->nMode == 17 || gbSpriteLowEndMachine)
        {
            bPrefetch = 0;
        }
    }
    if (dwDirBlock || dwDirBlockSize <= 1 || !bPrefetch)
    {
        return;
    }

    const uint32_t nTargetBlock = CELCMP_CelConvertBlock(dwDirBlockSize, 2 * dwDirBlockSize);
    pContext->nDirection = pHeader->nDirections > 1 ? (nTargetBlock << 6) / pHeader->nDirections : 1;
    if (GFXHASH_FindEntry(&gSpriteGfxHashStorage, pContext, &dwDirBlock, &dwDirBlockSize, pHeader))
    {
        return;
    }
    if (dwDirBlock != 1)
    {
        FOG_DisplayAssert("dwDirBlock == 1", szSpriteCacheCpp, 1670);
        exit(-1);
    }
    if (dwDirBlockSize <= 1)
    {
        FOG_DisplayAssert("dwDirBlockSize > 1", szSpriteCacheCpp, 1671);
        exit(-1);
    }

    if (!*szKey && !SPRITECACHE_BuildHeaderKey(pContext, szKey))
    {
        return;
    }
    if (!*szPath && !SPRITECACHE_BuildFilePath(pContext, szPath, szKey))
    {
        return;
    }
    if (bAsyncLoad)
    {
        SPRITECACHE_LoadDirectionBlockAsync(pContext, pHeader, dwDirBlock, dwDirBlockSize, szPath);
    }
    else
    {
        SPRITECACHE_LoadDirectionBlockSync(pContext, pHeader, dwDirBlock, dwDirBlockSize, szPath);
    }
}


//1.10f: D2CMP.0x6FDFD2D0
//  Queues a direction block for Fog's async loader, in LoadDirectionBlockSync's shape with these differences. Nothing happens without sgtGfxCache's
//  critical section. The node comes from queue 1. The gfx node's kind is not copied from the context (no store at +0), so it stays 0. The
//  directions are the block's for players and monsters (a monster with one direction per block in a file of more is refused: its node is evicted
//  and 0 returned) and 0..0 for every other kind, which then fails the size assert ("dwSize"); the async data must not exist yet (asserted). The
//  load is asynchronous (bAllowAsync 1), so the decode job, a local passed by address, is evidently copied by Fog (AsyncData holds an
//  AsyncDataLoadFileCallback by value). A load that fails or a size of -1 evicts the node and returns 0; otherwise the node is grown by the block's
//  bytes (either queue may be evicted for them), entered in GfxHash.cpp's storage, and the block's bFlags bit 1 is set (bit 0 instead, and 0
//  returned, for a header with no file).
//  BUG: the gfx node's nUnitType is left 0, which SpriteFindAsynchSiblings (#10058) compares with its first argument. NOT ESTABLISHED: the intent
//  is not known.
//  BUG: every kind but players and monsters reaches the size assert. NOT ESTABLISHED: whether a caller sends such a kind here is not settled.
int32_t __fastcall SPRITECACHE_LoadDirectionBlockAsync(D2GfxDataStrc *pContext, D2CmpSpriteHeaderData *pHeader, uint32_t dwDirBlock, uint32_t dwDirectionsPerBlock, const char *szPath)
{
    if (!sgtGfxCache.pcs)
    {
        return 0;
    }

    D2CmpSpriteDataNode *pCacheNode = SPRITECACHE_SpriteDataCache_AllocateNode(&sgtGfxCache, sizeof(D2CmpSpriteGfxNodePrefix), nullptr, 1);
    if (!pCacheNode)
    {
        return 0;
    }

    D2CmpSpriteGfxNodePrefix *ptGfxNode = (D2CmpSpriteGfxNodePrefix *)pCacheNode->pData;
    memset(ptGfxNode, 0, sizeof(D2CmpSpriteGfxNodePrefix));
    ptGfxNode->dwName[0] = pContext->dwName[0];
    ptGfxNode->dwName[3] = pContext->dwName[3];
    ptGfxNode->dwName[1] = pContext->dwName[1];
    ptGfxNode->dwName[2] = pContext->dwName[2];
    ptGfxNode->dwName[4] = pContext->dwName[4];
    ptGfxNode->tGfxBlock.pCacheNode = pCacheNode;

    if (!(pHeader->bFlags & CELDATAFLAG_NOFILE))
    {
        ptGfxNode->nUnitIndex = pContext->nUnitIndex;
        ptGfxNode->nMode = pContext->nMode;

        uint32_t dwFirstDirection = 0;
        uint32_t dwEndDirection = 0;
        const int32_t nUnitType = pContext->nUnitType;
        if (nUnitType == SPRITE_KIND_MONSTER && dwDirectionsPerBlock == 1 && pHeader->nDirections != 1)
        {
            SPRITECACHE_SpriteDataCache_EvictNode(&sgtGfxCache, pCacheNode, 1);
            return 0;
        }
        if (nUnitType == SPRITE_KIND_PLAYER || nUnitType == SPRITE_KIND_MONSTER)
        {
            dwFirstDirection = dwDirBlock * dwDirectionsPerBlock;
            dwEndDirection = dwFirstDirection + dwDirectionsPerBlock;
        }

        int32_t *pFirstFileOffset = &pHeader->nDirectionOffsets[dwFirstDirection];
        const uint32_t dwSize = pHeader->nDirectionOffsets[dwEndDirection] - *pFirstFileOffset;
        if (!dwSize)
        {
            FOG_DisplayAssert("dwSize", szSpriteCacheCpp, 930);
            exit(-1);
        }
        if (ptGfxNode->tGfxBlock.lpAsyncData)
        {
            FOG_DisplayAssert("!ptGfxNode->tGfxBlock.lpAsyncData", szSpriteCacheCpp, 931);
            exit(-1);
        }

        ptGfxNode->tGfxBlock.bNumDirections = (uint8_t)dwDirectionsPerBlock;
        ptGfxNode->tGfxBlock.bBaseDirection = (uint8_t)(dwDirBlock * dwDirectionsPerBlock);
        ptGfxNode->tGfxBlock.bNumCelsPerDir = (uint8_t)pHeader->nFrames;
        ptGfxNode->tGfxBlock.bNumOffsets = (uint8_t)(pHeader->nDirections + 1);

        const int32_t nBlockFileOffset = *pFirstFileOffset;
        for (uint32_t i = dwFirstDirection; i <= dwEndDirection; ++i)
        {
            if (i >= MAX_DIROFFSETS)
            {
                FOG_DisplayAssert("i < MAX_DIROFFSETS", szSpriteCacheCpp, 954);
                exit(-1);
            }
            ptGfxNode->tGfxBlock.nOffsets[i] = pHeader->nDirectionOffsets[i] - nBlockFileOffset;
            if (ptGfxNode->tGfxBlock.nOffsets[i] < 0)
            {
                FOG_DisplayAssert("ptGfxNode->tGfxBlock.nOffsets[i] >= 0", szSpriteCacheCpp, 956);
                exit(-1);
            }
        }

        AsyncData *pLoadedFile;
        if (pHeader->bFlags & 0x02)
        {
            D2CmpSpriteDecodeJobPrefix tDecodeJob = {};
            tDecodeJob.nDirections = dwDirectionsPerBlock;
            tDecodeJob.pGfxBlock = &ptGfxNode->tGfxBlock;
            tDecodeJob.nStartDirection = dwDirBlock * dwDirectionsPerBlock;
            ptGfxNode->tGfxBlock.bFlags |= 0x04;
            tDecodeJob.pfnCallback = SPRITECACHE_RunDecodeJob;
            tDecodeJob.pHeaderData = pHeader;
            pLoadedFile = FOG_AsyncDataLoadFileEx(sgtGfxCache.pPool, szPath, 1, *pFirstFileOffset, dwSize, nullptr, (AsyncDataLoadFileCallback *)&tDecodeJob, -2, szSpriteCacheCpp, 977);
        }
        else
        {
            pLoadedFile = FOG_AsyncDataLoadFileEx(sgtGfxCache.pPool, szPath, 1, *pFirstFileOffset, dwSize, nullptr, nullptr, -2, szSpriteCacheCpp, 987);
        }
        ptGfxNode->tGfxBlock.lpAsyncData = pLoadedFile;
        if (!pLoadedFile || dwSize == 0xFFFFFFFF)
        {
            SPRITECACHE_SpriteDataCache_EvictNode(&sgtGfxCache, pCacheNode, 1);
            return 0;
        }
        SPRITECACHE_SpriteDataCache_AddNodeBytes(&sgtGfxCache, pCacheNode, dwSize, 1);
    }

    D2CmpSpriteGfxHashEntryPrefix *ptHashTableElement = GFXHASH_NewEntry(sgtGfxCache.pPool, &gSpriteGfxHashStorage, (D2CmpSpriteGfxNodePrefix *)pCacheNode->pData, pContext, pHeader);
    if (!ptHashTableElement)
    {
        FOG_DisplayAssert("ptHashTableElement", szSpriteCacheCpp, 1584);
        exit(-1);
    }
    pCacheNode->pUserData = ptHashTableElement;
    ptHashTableElement->pCacheNode = pCacheNode;

    if (pHeader->bFlags & CELDATAFLAG_NOFILE)
    {
        ptGfxNode->tGfxBlock.bFlags |= 0x01;
        return 0;
    }
    ptGfxNode->tGfxBlock.bFlags |= 0x02;
    return 1;
}


//1.10f: D2CMP.0x6FDFBD40
//  #10055: makes the context's current cell valid, loading on demand; it is the way in for the renderers, D2Client and D2Win. A context with a cel
//  file of its own is CelCmp.cpp's (#10036): the cell is taken from the file and 1 returned. Otherwise the header is resolved and the direction
//  block looked up; for a later block (block above 0, more than one direction a block) that is missing or still loading and whose header has a
//  file, the context's direction is first moved to one in the first block (CELCMP_CelConvertDir, #10019, with fState bit 2) and that block looked
//  up instead. A found block gives 0 for no node, a no-file block or no async data; with bAsyncWait, whether its load is done; otherwise its cache
//  node is moved to queue 0's tail under the lock, the hardware slots are made if missing and the frame is found into the current cell (if it is
//  missing: a trace the first time ever, gbSpriteHandleErrorReported, and 0), and after block 0 of a several-direction sprite the next block is
//  prefetched. A block not found gives 0 with bAsyncWait; otherwise it is loaded now (LoadDirectionBlockSync, the path built when empty) and, on
//  success, prefetched after.
//  The original inlines FindFrame here.
//  BUG: the header ResolveHeader answers is not tested: NULL (no key or no path, 0x6FDFC54E, 0x6FDFC58F) goes to FindEntry, whose direction-block
//  step reads the header for players, monsters, missiles and kind 6 -- a missile without a name, say. NOT ESTABLISHED: whether such a context
//  reaches #10055 is not settled.
//  BUG: the context's direction is left at the first block's when that one was used (0x6FDFBE4D), as PrefetchSecondDirectionBlock leaves it at the
//  second's. NOT ESTABLISHED: the intent is not known.
extern "C" int32_t __stdcall SPRITECACHE_SpriteValidate(D2GfxDataStrc *pContext, int32_t bAsyncWait, int32_t bAsyncLoad)
{
    if (!pContext)
    {
        return 0;
    }

    D2CmpBuiltCellFile *pCellFile = pContext->pCellFile;
    pContext->pCurrentCell = nullptr;
    if (pCellFile)
    {
        pContext->pCurrentCell = CELCMP_CelGetHandle(pContext);
        return 1;
    }

    char szPath[260];
    char szKey[260];
    D2CmpSpriteHeaderData *pHeader = SPRITECACHE_ResolveHeader(pContext, szPath, szKey);
    uint32_t dwDirBlock = 0;
    uint32_t dwDirectionsPerBlock = 0;
    D2CmpSpriteGfxHashEntryPrefix *pGfxEntry = GFXHASH_FindEntry(&gSpriteGfxHashStorage, pContext, &dwDirBlock, &dwDirectionsPerBlock, pHeader);
    int32_t nValidateResult = 1;
    int32_t bInitialLoaded = 0;
    if (dwDirectionsPerBlock > 1 && dwDirBlock)
    {
        bool bFallBack = true;
        if (pGfxEntry)
        {
            AsyncData *lpAsyncData = pGfxEntry->pGfxNode->tGfxBlock.lpAsyncData;
            if (!lpAsyncData)
            {
                return 0;
            }
            bInitialLoaded = FOG_AsyncDataTestLoaded(lpAsyncData);
            bFallBack = !bInitialLoaded;
        }
        if (bFallBack && !(pHeader->bFlags & CELDATAFLAG_NOFILE))
        {
            const uint32_t nFileDirection = CELCMP_CelConvertDir((pHeader->nDirections * pContext->nDirection) >> 6, pHeader->nDirections, dwDirectionsPerBlock, pContext->fState & 0x04);
            pContext->nDirection = pHeader->nDirections > 1 ? (nFileDirection << 6) / pHeader->nDirections : 1;
            pGfxEntry = GFXHASH_FindEntry(&gSpriteGfxHashStorage, pContext, &dwDirBlock, &dwDirectionsPerBlock, pHeader);
        }
    }

    if (pGfxEntry)
    {
        D2CmpSpriteGfxNodePrefix *pGfxNode = pGfxEntry->pGfxNode;
        if (!pGfxNode || (pGfxNode->tGfxBlock.bFlags & 0x01) || !pGfxNode->tGfxBlock.lpAsyncData)
        {
            return 0;
        }
        if (bAsyncWait)
        {
            return FOG_AsyncDataTestLoaded(pGfxNode->tGfxBlock.lpAsyncData) != 0;
        }

        D2CmpSpriteDataNode *ptBlock = pGfxEntry->pCacheNode;
        if (sgtGfxCache.nActive)
        {
            ++sgtGfxCache.unknown_38;
            EnterCriticalSection(sgtGfxCache.pcs);
            if (!ptBlock)
            {
                FOG_DisplayAssert("ptBlock", szSpriteCacheCpp, 2591);
                exit(-1);
            }
            SPRITECACHE_SpriteDataCache_DetachQueuedNode(&sgtGfxCache, ptBlock);
            ptBlock->dwFlags &= ~0x01u;
            SPRITECACHE_SpriteDataCache_AppendQueuedNode(&sgtGfxCache, ptBlock, 0);
            LeaveCriticalSection(sgtGfxCache.pcs);
        }

        if (!pGfxNode->tGfxBlock.fHardwareCelData)
        {
            SPRITECACHE_InitializeHardwareSlots(pHeader, pContext, pGfxNode);
        }

        D2CmpSpriteFrameHeaderPrefix *pFrame = nullptr;
        if (pGfxNode->tGfxBlock.lpAsyncData)
        {
            uint8_t *pFrameBuffer = (uint8_t *)FOG_AsyncDataGetBuffer(pGfxNode->tGfxBlock.lpAsyncData);
            if (pFrameBuffer)
            {
                pContext->nMaxFrames = pGfxNode->tGfxBlock.bNumCelsPerDir;
                pFrame = SPRITECACHE_FindFrame(pContext, &pGfxNode->tGfxBlock, pFrameBuffer);
            }
        }

        pContext->pCurrentCell = (D2CmpRleCellHeader *)pFrame;
        if (!pFrame)
        {
            if (!gbSpriteHandleErrorReported)
            {
                gbSpriteHandleErrorReported = 1;
                FOG_Trace("Couldn't get cel handle in for:%s  TYPE:%d  CLASS:%d  MODE:%d  FILE:%s  LINE:%d", pContext->pName, pContext->nUnitType, pContext->nUnitIndex, pContext->nMode, szSpriteCacheCpp, 1833);
            }
            return 0;
        }

        if (!bInitialLoaded && !dwDirBlock && dwDirectionsPerBlock > 1)
        {
            if (!szPath[0] && !SPRITECACHE_BuildFilePath(pContext, szPath, szKey))
            {
                return 0;
            }
            SPRITECACHE_PrefetchSecondDirectionBlock(pContext, dwDirBlock, dwDirectionsPerBlock, pHeader, szKey, szPath, bAsyncLoad);
        }
        return nValidateResult;
    }

    if (bAsyncWait)
    {
        return 0;
    }
    if (!szPath[0] && !SPRITECACHE_BuildFilePath(pContext, szPath, szKey))
    {
        return 0;
    }
    nValidateResult = SPRITECACHE_LoadDirectionBlockSync(pContext, pHeader, dwDirBlock, dwDirectionsPerBlock, szPath);
    if (nValidateResult)
    {
        SPRITECACHE_PrefetchSecondDirectionBlock(pContext, dwDirBlock, dwDirectionsPerBlock, pHeader, szKey, szPath, bAsyncLoad);
    }
    return nValidateResult;
}


//1.10f: D2CMP.0x6FDFD600
//  #10056: asks for the context's direction block without waiting. The context must exist (asserted); a cel file of its own is CelCmp.cpp's, and 1
//  is returned. A header that does not resolve gives 0. A block already there gives 1, and the context's fFlags bit 2 is set for a no-file block.
//  Otherwise, a later block of a several-direction sprite with a file first falls back to the first block, which must be there and be block 0
//  (asserted), the direction being moved as SpriteValidate moves it; then the block is queued for the async loader (LoadDirectionBlockAsync, its
//  answer returned), the path being built when empty. Unlike #10055 it does not clear the context's current cell first.
//  BUG: after the fall-back the block queued is block 0, the one just found present, not the later block asked for, so a second entry for block 0
//  goes into GfxHash.cpp's storage (0x6FDFD72C, then 0x6FDFD7B7 with dwDirBlock 0). NOT ESTABLISHED: whether the later block can be absent while
//  block 0 is present depends on the loads' timing.
extern "C" int32_t __stdcall SPRITECACHE_SpriteRequest(D2GfxDataStrc *ptCelContext)
{
    if (!ptCelContext)
    {
        FOG_DisplayAssert("ptCelContext", szSpriteCacheCpp, 1902);
        exit(-1);
    }
    if (ptCelContext->pCellFile)
    {
        ptCelContext->pCurrentCell = CELCMP_CelGetHandle(ptCelContext);
        return 1;
    }

    char szPath[260];
    char szKey[260];
    D2CmpSpriteHeaderData *pHeader = SPRITECACHE_ResolveHeader(ptCelContext, szPath, szKey);
    if (!pHeader)
    {
        return 0;
    }

    uint32_t dwDirBlock = 0;
    uint32_t dwDirectionsPerBlock = 0;
    D2CmpSpriteGfxHashEntryPrefix *pEntry = GFXHASH_FindEntry(&gSpriteGfxHashStorage, ptCelContext, &dwDirBlock, &dwDirectionsPerBlock, pHeader);
    if (pEntry)
    {
        if (pEntry->pGfxNode->tGfxBlock.bFlags & 0x01)
        {
            ptCelContext->fFlags |= 0x04;
        }
        return 1;
    }

    if (dwDirectionsPerBlock > 1 && dwDirBlock && !(pHeader->bFlags & CELDATAFLAG_NOFILE))
    {
        const uint32_t nFileDirection = CELCMP_CelConvertDir((pHeader->nDirections * ptCelContext->nDirection) >> 6, pHeader->nDirections, dwDirectionsPerBlock, ptCelContext->fState & 0x04);
        ptCelContext->nDirection = pHeader->nDirections > 1 ? (nFileDirection << 6) / pHeader->nDirections : 1;
        if (!GFXHASH_FindEntry(&gSpriteGfxHashStorage, ptCelContext, &dwDirBlock, &dwDirectionsPerBlock, pHeader))
        {
            FOG_DisplayAssert("ptHashTableElement", szSpriteCacheCpp, 1961);
            exit(-1);
        }
        if (dwDirBlock)
        {
            FOG_DisplayAssert("dwDirBlock == 0", szSpriteCacheCpp, 1962);
            exit(-1);
        }
    }

    if (!szPath[0] && !SPRITECACHE_BuildFilePath(ptCelContext, szPath, szKey))
    {
        return 0;
    }
    return SPRITECACHE_LoadDirectionBlockAsync(ptCelContext, pHeader, dwDirBlock, dwDirectionsPerBlock, szPath);
}
