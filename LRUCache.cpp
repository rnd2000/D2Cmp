#include "D2CMP.h"

/*---------------------------------------------------------------------------
 *
 *  LRUCache.cpp -- D2CMP.dll 1.10 (image base 0x6FDF0000), address range 0x6FDF8B30..0x6FDF8F5B.
 *  The original's source file is D2CMP\SRC\LRUCache.cpp (its asserts name it).
 *
 *  A least-recently-used cache of pool allocations: a fixed array of nEntries 20-byte entries, threaded on two doubly
 *  linked lists -- the active list (oldest at the head, the most recently allocated or touched at the tail) and the
 *  free list -- with a byte budget, dwMaxAllocated, over the sum of the active entries' sizes. Allocating past the
 *  budget or past the entry count evicts from the active head.
 *
 *  The original does not check its lists: EvictEntry and TouchEntry unlink a given entry from the active list without
 *  asking whether it is on it, and dereference a neighbour they never test; AllocEntry links a free entry in before it
 *  asserts that there was one. All of it holds while the lists stay consistent -- nActive entries on the active list,
 *  the rest on the free list, dwAllocated their sizes' sum.
 *
 *-------------------------------------------------------------------------*/

//  The path literal pushed by every assert and pool call of the unit (the original's __FILE__).
static const char szLRUCacheCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2CMP\\SRC\\LRUCache.cpp";

//  Member offsets of D2CmpLruCache and D2CmpLruEntry, each with an instruction that uses it.
static_assert(offsetof(D2CmpLruCache, pPool) == 0x00, "D2CmpLruCache.pPool: mov ecx, [esi] (0x6FDF8C11)");
static_assert(offsetof(D2CmpLruCache, dwMaxAllocated) == 0x04, "D2CmpLruCache.dwMaxAllocated: mov eax, [esi+4] (0x6FDF8C38)");
static_assert(offsetof(D2CmpLruCache, dwAllocated) == 0x08, "D2CmpLruCache.dwAllocated: mov edx, [esi+8] (0x6FDF8C72)");
static_assert(offsetof(D2CmpLruCache, nActive) == 0x0C, "D2CmpLruCache.nActive: mov eax, [esi+0Ch] (0x6FDF8BF3)");
static_assert(offsetof(D2CmpLruCache, pEntries) == 0x10, "D2CmpLruCache.pEntries: mov [esi+10h], edi (0x6FDF8B76)");
static_assert(offsetof(D2CmpLruCache, nEntries) == 0x14, "D2CmpLruCache.nEntries: mov [esi+14h], ecx (0x6FDF8B53)");
static_assert(offsetof(D2CmpLruCache, pActiveHead) == 0x18, "D2CmpLruCache.pActiveHead: mov esi, [edi+18h] (0x6FDF8DD6)");
static_assert(offsetof(D2CmpLruCache, pActiveTail) == 0x1C, "D2CmpLruCache.pActiveTail: mov eax, [edi+1Ch] (0x6FDF8E20)");
static_assert(offsetof(D2CmpLruCache, pFreeHead) == 0x20, "D2CmpLruCache.pFreeHead: mov eax, [esi+20h] (0x6FDF8C7B)");
static_assert(offsetof(D2CmpLruCache, pFreeTail) == 0x24, "D2CmpLruCache.pFreeTail: mov eax, [edi+24h] (0x6FDF8E85)");
static_assert(offsetof(D2CmpLruCache, pfnOnEvict) == 0x28, "D2CmpLruCache.pfnOnEvict: call dword ptr [edi+28h] (0x6FDF8E62)");
static_assert(offsetof(D2CmpLruCache, nAllocRequests) == 0x2C, "D2CmpLruCache.nAllocRequests: mov ebp, [esi+2Ch] (0x6FDF8C5A)");
static_assert(offsetof(D2CmpLruCache, nAccessRequests) == 0x30, "D2CmpLruCache.nAccessRequests: mov esi, [ecx+30h] (0x6FDF8EBF)");
static_assert(offsetof(D2CmpLruEntry, pData) == 0x00, "D2CmpLruEntry.pData: mov [edi], eax (0x6FDF8D61)");
static_assert(offsetof(D2CmpLruEntry, dwSize) == 0x04, "D2CmpLruEntry.dwSize: mov [edi+4], ebx (0x6FDF8DA5)");
static_assert(offsetof(D2CmpLruEntry, pUserData) == 0x08, "D2CmpLruEntry.pUserData: mov [edi+8], edx (0x6FDF8DA8)");
static_assert(offsetof(D2CmpLruEntry, pNext) == 0x0C, "D2CmpLruEntry.pNext: mov ecx, [eax+0Ch] (0x6FDF8C88)");
static_assert(offsetof(D2CmpLruEntry, pPrev) == 0x10, "D2CmpLruEntry.pPrev: mov [ecx+10h], ebp (0x6FDF8C92)");


//1.10f: D2CMP.0x6FDF8B30
//  Sets a cache up: the budget, the eviction callback and the entry count stored, nEntries entries allocated from
//  pPool (LRUCache.cpp:48) and zeroed, every entry threaded onto the free list in array order, the active list and
//  both counters cleared. Returns nothing; the original leaves pEntries in eax, which no caller reads.
//  BUG: the allocation is not checked, so a NULL from the pool would be zeroed and walked. LATENT: Fog's AllocPool
//  reports a failed allocation before it answers NULL.
//  BUG: with nEntries 0 the original stores pEntries - 1 into pFreeTail (`lea edx, [eax+ecx*4-14h]`, 0x6FDF8BDA), and
//  the C below does the same arithmetic. LATENT: both callers pass 4096.
void __fastcall LRUCACHE_Init(D2CmpLruCache *pCache, void *pPool, uint32_t dwMaxAllocated, void (__fastcall *pfnOnEvict)(D2CmpLruEntry *), uint32_t nEntries)
{
    pCache->dwMaxAllocated = dwMaxAllocated;
    pCache->pfnOnEvict = pfnOnEvict;
    pCache->nEntries = nEntries;
    pCache->pPool = pPool;
    pCache->dwAllocated = 0;

    D2CmpLruEntry *pEntries = (D2CmpLruEntry *)FOG_AllocPool(pPool, nEntries * sizeof(D2CmpLruEntry), szLRUCacheCpp, 48, 0);
    pCache->pEntries = pEntries;
    memset(pEntries, 0, nEntries * sizeof(D2CmpLruEntry));

    //  The walk reloads the array and nEntries from the cache on every pass; every test is unsigned.
    for (uint32_t i = 0; i < pCache->nEntries; ++i)
    {
        D2CmpLruEntry *pArray = pCache->pEntries;
        if (i < pCache->nEntries - 1)
            pArray[i].pNext = &pArray[i + 1];
        if (i > 0)
            pArray[i].pPrev = &pCache->pEntries[i - 1];
        pArray[i].pData = nullptr;
        pArray[i].dwSize = 0;
        pArray[i].pUserData = nullptr;
    }

    pCache->nActive = 0;
    pCache->pActiveHead = nullptr;
    pCache->pActiveTail = nullptr;
    pCache->pFreeHead = pCache->pEntries;
    pCache->nAllocRequests = 0;
    pCache->nAccessRequests = 0;
    pCache->pFreeTail = &pCache->pEntries[pCache->nEntries - 1];
}


//1.10f: D2CMP.0x6FDF8BF0
//  Evicts the active head until nActive is 0, then frees the entry array (LRUCache.cpp:99) and clears pEntries. Nothing
//  else is reset: the lists' heads and tails still point into the freed array. Returns nothing; the original leaves
//  FreePool's answer (or 0) in eax, which no caller reads.
void __fastcall LRUCACHE_Destroy(D2CmpLruCache *pCache)
{
    while (pCache->nActive)
        LRUCACHE_EvictEntry(pCache, nullptr);

    if (pCache->pEntries)
    {
        FOG_FreePool(pCache->pPool, pCache->pEntries, szLRUCacheCpp, 99, 0);
        pCache->pEntries = nullptr;
    }
}


//1.10f: D2CMP.0x6FDF8C30
//  Takes an entry for dwSize bytes of pUserData's: asserts the size is below the whole budget (LRUCache.cpp:118),
//  counts the call in nAllocRequests and nAccessRequests, evicts from the active head while every entry is active or
//  the size would cross the budget, takes the free list's head onto the active tail, and allocates dwSize bytes for
//  it from the pool (:132), which must not fail (:133).
//  The original inlines an unnamed LRUCache.cpp function here (the take-and-append, whose assert is line 159), and its
//  compiler emits that code twice, after the loop's first test and after the loop; the loop is written once here.
//  Both budget tests are unsigned (`ja`, 0x6FDF8C79, 0x6FDF8CF7) on a sum that wraps at 32 bits.
D2CmpLruEntry *__fastcall LRUCACHE_AllocEntry(D2CmpLruCache *pCache, uint32_t dwSize, void *pUserData)
{
    if (dwSize >= pCache->dwMaxAllocated)
    {
        FOG_DisplayAssert("dwSize < ptCache->dwMaxAllocated", szLRUCacheCpp, 118);
        exit(-1);
    }

    ++pCache->nAllocRequests;
    ++pCache->nAccessRequests;

    while (pCache->nActive == pCache->nEntries || pCache->dwAllocated + dwSize > pCache->dwMaxAllocated)
        LRUCACHE_EvictEntry(pCache, nullptr);

    D2CmpLruEntry *pEntry = pCache->pFreeHead;
    if (pEntry)
    {
        D2CmpLruEntry *pNext = pEntry->pNext;
        pCache->pFreeHead = pNext;
        if (pNext)
            pNext->pPrev = nullptr;
        else
            pCache->pFreeTail = nullptr;
        pEntry->pNext = nullptr;
        pEntry->pPrev = nullptr;
    }

    //  BUG: with an empty free list pEntry is NULL, and with an active tail `mov [eax+10h], ecx` (0x6FDF8CB2,
    //  0x6FDF8D30) stores at address 0x10 before the "ptBlock" assert below. LATENT: after the loop nActive < nEntries,
    //  so a consistent cache holds a free entry, and every writer of the lists keeps them consistent.
    if (pCache->pActiveTail)
    {
        pCache->pActiveTail->pNext = pEntry;
        pEntry->pPrev = pCache->pActiveTail;
    }
    else
    {
        pCache->pActiveHead = pEntry;
    }
    pCache->pActiveTail = pEntry;

    if (!pEntry)
    {
        FOG_DisplayAssert("ptBlock", szLRUCacheCpp, 159);
        exit(-1);
    }

    //  Counted before the allocation, so a failure would leave both counts up -- but a failure exits.
    ++pCache->nActive;
    pCache->dwAllocated += dwSize;

    pEntry->pData = FOG_AllocPool(pCache->pPool, dwSize, szLRUCacheCpp, 132, 0);
    if (!pEntry->pData)
    {
        FOG_DisplayAssert("ptBlock->lpData", szLRUCacheCpp, 133);
        exit(-1);
    }

    pEntry->dwSize = dwSize;
    pEntry->pUserData = pUserData;
    return pEntry;
}


//1.10f: D2CMP.0x6FDF8DC0
//  Evicts pEntry, or the active head when pEntry is NULL: unlinks it from the active list, takes its size off
//  dwAllocated, hands the whole entry to the cache's callback when it carries user data (ECX, `call dword ptr [edi+28h]`,
//  0x6FDF8E62; the pointer is not tested), frees its block (LRUCache.cpp:200) and clears it, appends it to the free
//  tail, and decrements nActive last, after the callback. Returns nothing; no caller reads the original's eax.
//  Arms: with nActive 0 nothing happens at all, a given entry included; with no entry and no active head only nActive
//  is decremented; an entry given while the active head is NULL skips the unlink; the block's fields are cleared only
//  when it had a block, otherwise dwSize and pUserData keep their values.
//  The `if (pEntry)` before the cleanup is the original's own test, on a pointer every path to it has already tested.
void __fastcall LRUCACHE_EvictEntry(D2CmpLruCache *pCache, D2CmpLruEntry *pEntry)
{
    if (!pCache->nActive)
        return;

    if (!pEntry)
    {
        pEntry = pCache->pActiveHead;
        if (!pEntry)
        {
            --pCache->nActive;
            return;
        }

        D2CmpLruEntry *pNext = pEntry->pNext;
        pCache->pActiveHead = pNext;
        if (pNext)
            pNext->pPrev = nullptr;
        else
            pCache->pActiveTail = nullptr;
        pEntry->pNext = nullptr;
        pEntry->pPrev = nullptr;
    }
    else if (pCache->pActiveHead)
    {
        D2CmpLruEntry *pHead = pCache->pActiveHead;
        if (pEntry == pHead)
        {
            D2CmpLruEntry *pNext = pHead->pNext;
            pCache->pActiveHead = pNext;
            if (pNext)
                pNext->pPrev = nullptr;
            else
                pCache->pActiveTail = nullptr;
            pHead->pNext = nullptr;
            pHead->pPrev = nullptr;
        }
        else
        {
            //  BUG: neither neighbour is tested, and nothing asks whether the entry is on the active list. LATENT: the
            //  one caller that passes an entry passes a tile's ptLRUCache, which the eviction callback clears.
            if (pEntry == pCache->pActiveTail)
            {
                D2CmpLruEntry *pPrev = pCache->pActiveTail->pPrev;
                pCache->pActiveTail = pPrev;
                pPrev->pNext = nullptr;
            }
            else
            {
                pEntry->pPrev->pNext = pEntry->pNext;
                pEntry->pNext->pPrev = pEntry->pPrev;
            }
            pEntry->pNext = nullptr;
            pEntry->pPrev = nullptr;
        }
    }

    if (pEntry)
    {
        pCache->dwAllocated -= pEntry->dwSize;
        if (pEntry->pUserData)
            pCache->pfnOnEvict(pEntry);

        if (pEntry->pData)
        {
            FOG_FreePool(pCache->pPool, pEntry->pData, szLRUCacheCpp, 200, 0);
            pEntry->pData = nullptr;
            pEntry->dwSize = 0;
            pEntry->pUserData = nullptr;
        }

        if (pCache->pFreeTail)
        {
            pCache->pFreeTail->pNext = pEntry;
            pEntry->pPrev = pCache->pFreeTail;
        }
        else
        {
            pCache->pFreeHead = pEntry;
        }
        pCache->pFreeTail = pEntry;
    }

    --pCache->nActive;
}


//1.10f: D2CMP.0x6FDF8EB0
//  Marks pEntry used: with nActive 0 it does nothing, a NULL entry included; otherwise it counts the access, asserts
//  the entry (LRUCache.cpp:233), unlinks it from the active list -- the head, the tail or the middle, skipped when the
//  active head is NULL -- and appends it to the active tail. Touching the tail unlinks and relinks it. Returns nothing;
//  both callers discard the original's eax.
void __fastcall LRUCACHE_TouchEntry(D2CmpLruCache *pCache, D2CmpLruEntry *pEntry)
{
    if (!pCache->nActive)
        return;

    ++pCache->nAccessRequests;
    if (!pEntry)
    {
        FOG_DisplayAssert("ptBlock", szLRUCacheCpp, 233);
        exit(-1);
    }

    D2CmpLruEntry *pHead = pCache->pActiveHead;
    if (pHead)
    {
        if (pEntry == pHead)
        {
            D2CmpLruEntry *pNext = pHead->pNext;
            pCache->pActiveHead = pNext;
            if (pNext)
                pNext->pPrev = nullptr;
            else
                pCache->pActiveTail = nullptr;
            pHead->pNext = nullptr;
            pHead->pPrev = nullptr;
        }
        else
        {
            //  BUG: neither neighbour is tested, and nothing asks whether the entry is on the active list, as in
            //  EvictEntry. LATENT: both callers pass a holder's back-pointer, which the eviction callbacks clear.
            if (pEntry == pCache->pActiveTail)
            {
                D2CmpLruEntry *pPrev = pCache->pActiveTail->pPrev;
                pCache->pActiveTail = pPrev;
                pPrev->pNext = nullptr;
            }
            else
            {
                pEntry->pPrev->pNext = pEntry->pNext;
                pEntry->pNext->pPrev = pEntry->pPrev;
            }
            pEntry->pNext = nullptr;
            pEntry->pPrev = nullptr;
        }
    }

    if (pCache->pActiveTail)
    {
        pCache->pActiveTail->pNext = pEntry;
        pEntry->pPrev = pCache->pActiveTail;
    }
    else
    {
        pCache->pActiveHead = pEntry;
    }
    pCache->pActiveTail = pEntry;
}
