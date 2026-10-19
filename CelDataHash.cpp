#include "D2CMP.h"

/*---------------------------------------------------------------------------
 *
 *  CelDataHash.cpp -- D2CMP.dll 1.10 (image base 0x6FDF0000), address range 0x6FDF3D10..0x6FDF3ED0.
 *  The original's source file is D2CMP\Src\CelDataHash.cpp (its asserts name it).
 *
 *  The sprite cache's hash of cel file headers: 512 bucket heads in the upper half of a 4 KB storage, each a doubly
 *  linked chain of 48-byte entries keyed by a string of up to 31 characters, each entry pointing at its LRU cache entry.
 *  The last function, CELDATAHASH_10106, is an empty export.
 *
 *-------------------------------------------------------------------------*/

//  The path literal of the unit's assert and pool calls (the original's __FILE__).
static const char szCelDataHashCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2CMP\\Src\\CelDataHash.cpp";

static_assert(offsetof(D2CmpCelHeaderHash, pBuckets) == 0x800, "the bucket heads: [esi+ecx*4+800h] (0x6FDF3D5E)");
static_assert(offsetof(D2CmpCelHeaderHashEntry, szKey) == 4, "the entry's key: lea esi, [edi+4] (0x6FDF3D69)");
static_assert(offsetof(D2CmpCelHeaderHashEntry, lpBlock) == 0x24, "mov [esi+24h], eax (0x6FDF3DEB)");
static_assert(offsetof(D2CmpCelHeaderHashEntry, pNext) == 0x2C, "the entry's pNext: [edi+2Ch] (0x6FDF3D9F)");


//1.10f: D2CMP.0x6FDF3D10
//  Clears the whole storage, 400h dwords (`rep stosd`); the lower 800h bytes, which the hash never reads, too.
void __fastcall CELDATAHASH_Clear(D2CmpCelHeaderHash *pHash)
{
    memset(pHash, 0, sizeof(D2CmpCelHeaderHash));
}


//1.10f: D2CMP.0x6FDF3D20
//  The entry whose key equals szKey, or NULL; NULL at once for an empty key. The key's Storm hash (SStrHash, flags 1, seed 0) is folded to
//  9 bits by three logical shifts (GfxHash.cpp's fold with 9 bits for 13). The chain is walked through pNext and compared with an inlined
//  strcmp, two bytes a pass, whose result is tested for zero alone.
D2CmpCelHeaderHashEntry *__fastcall CELDATAHASH_FindEntry(D2CmpCelHeaderHash *pHash, const char *szKey)
{
    if (!*szKey)
    {
        return nullptr;
    }

    const uint32_t dwHash = STORM_SStrHash(szKey, 1, 0);
    //  The fold is written out again in NewEntry; the original has it in both.
    const uint32_t dwBucket = ((((dwHash & 0x1FF0000) ^ (dwHash >> 8)) >> 8 ^ (dwHash & 0x1FF00)) >> 8) ^ (dwHash & 0x1FF);

    for (D2CmpCelHeaderHashEntry *pEntry = pHash->pBuckets[dwBucket]; pEntry; pEntry = pEntry->pNext)
    {
        if (!strcmp(szKey, pEntry->szKey))
        {
            return pEntry;
        }
    }

    return nullptr;
}


//1.10f: D2CMP.0x6FDF3DC0
//  A new entry for szKey and pLruEntry, at the head of its bucket's chain; returned. Nothing looks for a duplicate first, and the key is
//  not tested for empty. The entry comes from FOG_AllocPool (:136).
//  BUG: the pool's answer is not tested (`mov esi, eax`, 0x6FDF3DDC). LATENT: a failed pool allocation is reported by Fog first.
//  BUG: the key is copied byte by byte up to and including its NUL (0x6FDF3E3A..0x6FDF3E42, an inlined strcpy) into the entry's 32 bytes,
//  with no bound; a key of 32 characters or more writes past the entry. NOT ESTABLISHED: D2Client's names fit with a margin of 0,
//  and D2Win's call was not read.
D2CmpCelHeaderHashEntry *__fastcall CELDATAHASH_NewEntry(void *pPool, D2CmpCelHeaderHash *pHash, D2CmpLruEntry *pLruEntry, const char *szKey)
{
    D2CmpCelHeaderHashEntry *pNewEntry = (D2CmpCelHeaderHashEntry *)FOG_AllocPool(pPool, sizeof(D2CmpCelHeaderHashEntry), szCelDataHashCpp, 136, 0);
    pNewEntry->lpBlock = pLruEntry;

    const uint32_t dwHash = STORM_SStrHash(szKey, 1, 0);
    pNewEntry->pPrev = nullptr;
    //  The same fold as FindEntry's.
    const uint32_t dwBucket = ((((dwHash & 0x1FF0000) ^ (dwHash >> 8)) >> 8 ^ (dwHash & 0x1FF00)) >> 8) ^ (dwHash & 0x1FF);

    D2CmpCelHeaderHashEntry *pOldHead = pHash->pBuckets[dwBucket];
    pNewEntry->pNext = pOldHead;
    if (pOldHead)
    {
        pOldHead->pPrev = pNewEntry;
    }
    pNewEntry->dwBucket = dwBucket;
    strcpy(pNewEntry->szKey, szKey);
    pHash->pBuckets[dwBucket] = pNewEntry;
    return pNewEntry;
}


//1.10f: D2CMP.0x6FDF3E60
//  Unlinks ptElement (assert :177, and exit, when NULL) and frees it to the pool (:58). The original inlines the pool free here.
void __fastcall CELDATAHASH_DeleteEntry(void *pPool, D2CmpCelHeaderHash *pHash, D2CmpCelHeaderHashEntry *ptElement)
{
    if (!ptElement)
    {
        FOG_DisplayAssert("ptElement", szCelDataHashCpp, 177);
        exit(-1);
    }

    if (ptElement->pPrev)
    {
        ptElement->pPrev->pNext = ptElement->pNext;
    }
    if (ptElement->pNext)
    {
        ptElement->pNext->pPrev = ptElement->pPrev;
    }
    if (pHash->pBuckets[ptElement->dwBucket] == ptElement)
    {
        pHash->pBuckets[ptElement->dwBucket] = ptElement->pNext;
    }

    FOG_FreePool(pPool, ptElement, szCelDataHashCpp, 58, 0);
}


//1.10f: D2CMP.0x6FDF3ED0
//  Exported as #10103, #10104 and #10106 (three ordinals on one body): a single `ret`. Nothing in it reads ECX, but its two callers, the
//  hash-table statistics dumps #10059 and #10062, load ECX with a hash storage before their tail jump, so it takes one pointer, unread.
extern "C" void __fastcall CELDATAHASH_10106(void *pHash)
{
}
