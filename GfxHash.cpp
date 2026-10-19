#include "D2CMP.h"

/*---------------------------------------------------------------------------
 *
 *  GfxHash.cpp -- D2CMP.dll 1.10 (image base 0x6FDF0000), address range 0x6FDF8690..0x6FDF8B26.
 *  The original's source file is D2CMP\Src\GfxHash.cpp (its asserts name it).
 *
 *  The sprite cache's hash of loaded direction blocks: 8192 bucket heads in the upper half of a 64 KB storage, each a
 *  doubly linked chain of 48-byte entries keyed by the cel context's five name dwords, a special key (the overlay or
 *  the unit index, for some kinds) and the direction block.
 *
 *-------------------------------------------------------------------------*/

//  The path literal of the unit's asserts and pool calls (the original's __FILE__).
static const char szGfxHashCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2CMP\\Src\\GfxHash.cpp";

//  The cel kinds the hash tests: 0..4 are the unit types player, monster, object, missile and item. Kind 6 is a cel kind of its own: it keys
//  like a missile or an item only while fState bit 1 is set, and like an object without it.
constexpr int32_t GFXHASH_KIND_PLAYER = 0;
constexpr int32_t GFXHASH_KIND_MONSTER = 1;
constexpr int32_t GFXHASH_KIND_OBJECT = 2;
constexpr int32_t GFXHASH_KIND_MISSILE = 3;
constexpr int32_t GFXHASH_KIND_ITEM = 4;
constexpr int32_t GFXHASH_KIND_6 = 6;

static_assert(offsetof(D2CmpSpriteGfxHashStorage, pBuckets) == 0x8000, "the bucket heads: [edi+eax*4+8000h] (0x6FDF86BD)");
static_assert(offsetof(D2GfxDataStrc, nUnitType) == 0x20, "D2GfxDataStrc.nUnitType: [esi+20h] (0x6FDF86D5)");
static_assert(offsetof(D2GfxDataStrc, fState) == 0x1C, "D2GfxDataStrc.fState: test byte ptr [esi+1Ch], 2 (0x6FDF86DD)");
static_assert(offsetof(D2GfxDataStrc, pName) == 0x44, "D2GfxDataStrc.pName: [esi+44h] (0x6FDF87C5)");
static_assert(offsetof(D2CmpSpriteGfxHashEntryPrefix, pNext) == 0x2C, "the entry's pNext: [eax+2Ch] (0x6FDF875E)");
static_assert(offsetof(D2CmpSpriteGfxNodePrefix, tGfxBlock.lpAsyncData) == 0xB0, "the node's async data: [ebx+0B0h] (0x6FDF8956)");
static_assert(offsetof(D2CmpSpriteGfxNodePrefix, tGfxBlock.bNumCelsPerDir) == 0x24, "mov al, [ebx+24h] (0x6FDF89A8)");
static_assert(offsetof(D2CmpSpriteFrameHeaderPrefix, pHardwareData) == 0x18, "a frame's hardware pointer: [esi+edi+18h] (0x6FDF89C1)");


//1.10f: D2CMP.0x6FDF8690
//  Clears the whole storage, 4000h dwords (`rep stosd`); the lower half, 8000h bytes the hash never reads, is cleared too.
void __fastcall GFXHASH_ClearStorage(D2CmpSpriteGfxHashStorage *pHashStorage)
{
    memset(pHashStorage, 0, sizeof(D2CmpSpriteGfxHashStorage));
}


//1.10f: D2CMP.0x6FDF86A0
//  The entry for the context's direction block, or NULL. GetBucket writes *pDirBlock and *pDirectionsPerBlock on the way, whatever is found.
//  The chain is walked through pNext with no other test; an entry matches on the five name dwords, then -- for kind 6 with fState bit 1 the
//  overlay, for a missile or an item the unit index -- the special key, then the direction block. The original compares the five names in
//  each of three arms; one comparison here reads the same fields.
D2CmpSpriteGfxHashEntryPrefix *__fastcall GFXHASH_FindEntry(D2CmpSpriteGfxHashStorage *pHash, D2GfxDataStrc *ptCelContext, uint32_t *pDirBlock, uint32_t *pDirectionsPerBlock, D2CmpSpriteHeaderData *pHeader)
{
    D2CmpSpriteGfxHashEntryPrefix *pEntry = pHash->pBuckets[GFXHASH_GetBucket(ptCelContext, pHeader, pDirBlock, pDirectionsPerBlock)];

    for (; pEntry; pEntry = pEntry->pNext)
    {
        if (ptCelContext->dwName[0] != pEntry->dwName[0] || ptCelContext->dwName[1] != pEntry->dwName[1] || ptCelContext->dwName[2] != pEntry->dwName[2] || ptCelContext->dwName[3] != pEntry->dwName[3] || ptCelContext->dwName[4] != pEntry->dwName[4])
        {
            continue;
        }

        const int32_t nUnitType = ptCelContext->nUnitType;
        if (nUnitType == GFXHASH_KIND_6 && (ptCelContext->fState & 0x02))
        {
            if ((uint32_t)ptCelContext->nOverlay != pEntry->dwSpecialKey)
            {
                continue;
            }
        }
        else if (nUnitType == GFXHASH_KIND_ITEM || nUnitType == GFXHASH_KIND_MISSILE)
        {
            if ((uint32_t)ptCelContext->nUnitIndex != pEntry->dwSpecialKey)
            {
                continue;
            }
        }

        if (*pDirBlock == pEntry->dwDirBlock)
        {
            return pEntry;
        }
    }

    return nullptr;
}


//1.10f: D2CMP.0x6FDF8780
//  The bucket of the context's direction block, 0..8191; the block itself goes to *pDirBlock first. The key is the name's Storm hash
//  (SStrHash, flags 1, seed 0) for kind 6 with fState bit 1, a missile or an item -- pName must be set (assert :44, and exit) -- and the sum
//  of the five name dwords otherwise. The block is added, then the sum is folded to 13 bits by three logical shifts.
uint32_t __fastcall GFXHASH_GetBucket(D2GfxDataStrc *ptCelContext, D2CmpSpriteHeaderData *pHeader, uint32_t *pDirBlock, uint32_t *pDirectionsPerBlock)
{
    uint32_t dwDirBlock = GFXHASH_GetDirectionBlock(ptCelContext, pDirectionsPerBlock, pHeader);
    *pDirBlock = dwDirBlock;

    uint32_t dwKeyHash;
    const int32_t nUnitType = ptCelContext->nUnitType;
    if ((nUnitType == GFXHASH_KIND_6 && (ptCelContext->fState & 0x02)) || nUnitType == GFXHASH_KIND_MISSILE || nUnitType == GFXHASH_KIND_ITEM)
    {
        if (!ptCelContext->pName)
        {
            FOG_DisplayAssert("ptCelContext->tUnitInfo.pszFileName", szGfxHashCpp, 44);
            exit(-1);
        }

        dwKeyHash = STORM_SStrHash(ptCelContext->pName, 1, 0);
        //  The original re-reads *pDirBlock after the call (`mov eax, [edi]`, 0x6FDF87F3); it is the value just stored.
        dwDirBlock = *pDirBlock;
    }
    else
    {
        dwKeyHash = ptCelContext->dwName[0] + ptCelContext->dwName[1] + ptCelContext->dwName[2] + ptCelContext->dwName[3] + ptCelContext->dwName[4];
    }

    const uint32_t dwHash = dwKeyHash + dwDirBlock;
    return ((((dwHash & 0x1FFF0000) ^ (dwHash >> 8)) >> 8 ^ (dwHash & 0x1FFF00)) >> 8) ^ (dwHash & 0x1FFF);
}


//1.10f: D2CMP.0x6FDF8830
//  A new entry for pGfxNode's direction block, at the head of its bucket's chain; returned. Nothing looks for a duplicate first.
//  The entry comes from FOG_AllocPool (:239).
//  BUG: the pool's answer is not tested (`mov esi, eax`, 0x6FDF884C). LATENT: a failed pool allocation is reported by Fog first.
//  BUG: the special key (+4) is written only for kind 6 with fState bit 1 and for an item or a missile (0x6FDF88A4, 0x6FDF88B6); for every
//  other kind it keeps what the pool's block held, and pCacheNode (+24h) is not written either. NOT ESTABLISHED: FindEntry never reads the
//  key for those kinds, and whether FOG_AllocPool clears a block is not known.
D2CmpSpriteGfxHashEntryPrefix *__fastcall GFXHASH_NewEntry(void *pPool, D2CmpSpriteGfxHashStorage *pHash, D2CmpSpriteGfxNodePrefix *pGfxNode, D2GfxDataStrc *ptCelContext, D2CmpSpriteHeaderData *pHeader)
{
    D2CmpSpriteGfxHashEntryPrefix *pNewEntry = (D2CmpSpriteGfxHashEntryPrefix *)FOG_AllocPool(pPool, sizeof(D2CmpSpriteGfxHashEntryPrefix), szGfxHashCpp, 239, 0);
    pNewEntry->pGfxNode = pGfxNode;

    //  Written by GetDirectionBlock and never read here.
    uint32_t dwDirectionsPerBlock = 0;
    const uint32_t dwBucket = GFXHASH_GetBucket(ptCelContext, pHeader, &pNewEntry->dwDirBlock, &dwDirectionsPerBlock);

    for (int32_t i = 0; i < 5; ++i)
    {
        pNewEntry->dwName[i] = ptCelContext->dwName[i];
    }

    const int32_t nUnitType = ptCelContext->nUnitType;
    if (nUnitType == GFXHASH_KIND_6 && (ptCelContext->fState & 0x02))
    {
        pNewEntry->dwSpecialKey = ptCelContext->nOverlay;
    }
    else if (nUnitType == GFXHASH_KIND_ITEM || nUnitType == GFXHASH_KIND_MISSILE)
    {
        pNewEntry->dwSpecialKey = ptCelContext->nUnitIndex;
    }

    pNewEntry->pPrev = nullptr;
    D2CmpSpriteGfxHashEntryPrefix *pOldHead = pHash->pBuckets[dwBucket];
    pNewEntry->pNext = pOldHead;
    if (pOldHead)
    {
        pOldHead->pPrev = pNewEntry;
    }
    pNewEntry->dwBucket = dwBucket;
    pHash->pBuckets[dwBucket] = pNewEntry;
    return pNewEntry;
}


//1.10f: D2CMP.0x6FDF88F0
//  Unlinks ptElement (assert :321, and exit, when NULL) and frees it (:142). On the way, when its node holds async data: for hardware cel
//  data, every frame's hardware pointer goes to the cel module's free callback and is cleared, and the async data is freed and its pointer
//  cleared either way. The frames are walked by their own sizes (35 bytes plus the counts at +14h and +1Ch) for the node's directions times
//  its cels per direction, two bytes zero-extended, compared unsigned. The node itself is not tested. The original's asserts and frees
//  (:95, :130, :142, :321) come from different source lines, so it inlines helper functions here.
//  BUG: frame 0's hardware pointer is taken before the walk (0x6FDF89B1), handed to the callback and cleared in the walk's first pass,
//  and then -- the saved copy -- freed to the pool (:130, 0x6FDF8A14). NOT ESTABLISHED: whether that is a double release depends on the
//  renderer's callback, which was not read.
void __fastcall GFXHASH_DeleteEntry(void *pPool, D2CmpSpriteGfxHashStorage *pHash, D2CmpSpriteGfxHashEntryPrefix *ptElement)
{
    if (!ptElement)
    {
        FOG_DisplayAssert("ptElement", szGfxHashCpp, 321);
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

    D2CmpSpriteGfxNodePrefix *pGfxNode = ptElement->pGfxNode;
    AsyncData *pAsyncData = pGfxNode->tGfxBlock.lpAsyncData;
    if (pAsyncData)
    {
        if (pGfxNode->tGfxBlock.fHardwareCelData)
        {
            const D2CmpCelHardwareFreeCallback pfnHardwareFree = CELCMP_CelGetHardwareFreeCallback();
            uint8_t *lpbData = (uint8_t *)FOG_AsyncDataGetBuffer(pAsyncData);
            if (!lpbData)
            {
                FOG_DisplayAssert("lpbData", szGfxHashCpp, 95);
                exit(-1);
            }

            void *pSavedFirstHardwareNode = ((D2CmpSpriteFrameHeaderPrefix *)lpbData)->pHardwareData;
            uint32_t dwFrameOffset = 0;
            uint32_t nFrameIndex = 0;
            if ((uint32_t)pGfxNode->tGfxBlock.bNumCelsPerDir * pGfxNode->tGfxBlock.bNumDirections)
            {
                do
                {
                    D2CmpSpriteFrameHeaderPrefix *pFrame = (D2CmpSpriteFrameHeaderPrefix *)(lpbData + dwFrameOffset);
                    if (pFrame->pHardwareData)
                    {
                        if (pfnHardwareFree)
                        {
                            pfnHardwareFree(pFrame->pHardwareData);
                        }
                        pFrame->pHardwareData = nullptr;
                    }
                    ++nFrameIndex;
                    dwFrameOffset += pFrame->dwControlBytes + pFrame->dwAuxiliaryBytes + 35;
                } while (nFrameIndex < (uint32_t)pGfxNode->tGfxBlock.bNumCelsPerDir * pGfxNode->tGfxBlock.bNumDirections);
            }

            if (pSavedFirstHardwareNode)
            {
                FOG_FreePool(pPool, pSavedFirstHardwareNode, szGfxHashCpp, 130, 0);
            }
        }

        FOG_AsyncDataFree(pAsyncData);
        pGfxNode->tGfxBlock.lpAsyncData = nullptr;
    }

    FOG_FreePool(pPool, ptElement, szGfxHashCpp, 142, 0);
}


//1.10f: D2CMP.0x6FDF8A60
//  The direction block of the context's direction, and in *pDirectionsPerBlock how many directions a block holds. The file direction is
//  the cel module's conversion of the game direction unless the header's bit 0 is set, when it stays 0. By kind:
//      player    8 per block, 1 in mode 0 or 17 or with fFlags bit 1
//      monster   bit 0 set: 0, and *pDirectionsPerBlock stays 0; else 4 per block for an 8-direction file, 1 otherwise
//      object    1, block 0
//      item      1, the game direction, unconverted, with no header test
//      missile, kind 6 with fState bit 1
//                1, the converted direction (block 0 with bit 0 set)
//      the rest  1, block 0
//  The converted paths test the divisor for 0 before `div` (0x6FDF8B18); the direct ones do not. The test never fires, since 1, 4 or 8 is
//  stored first on every path that reaches it. Every division is `div`, unsigned.
uint32_t __fastcall GFXHASH_GetDirectionBlock(D2GfxDataStrc *ptCelContext, uint32_t *pDirectionsPerBlock, D2CmpSpriteHeaderData *pHeader)
{
    uint32_t nFileDirection = 0;
    *pDirectionsPerBlock = 0;

    switch (ptCelContext->nUnitType)
    {
    case GFXHASH_KIND_PLAYER:
        *pDirectionsPerBlock = 8;
        if (ptCelContext->nMode == 0 || ptCelContext->nMode == 17 || (ptCelContext->fFlags & 0x02))
        {
            *pDirectionsPerBlock = 1;
        }
        if (!(pHeader->bFlags & 0x01))
        {
            nFileDirection = CELCMP_CelGameDirToFileDir(ptCelContext->nDirection, pHeader->nDirections);
        }
        break;

    case GFXHASH_KIND_MONSTER:
        if (pHeader->bFlags & 0x01)
        {
            return 0;
        }
        *pDirectionsPerBlock = pHeader->nDirections == 8 ? 4 : 1;
        nFileDirection = CELCMP_CelGameDirToFileDir(ptCelContext->nDirection, pHeader->nDirections);
        break;

    default:
    {
        *pDirectionsPerBlock = 1;
        //  The original re-reads nUnitType after the store (0x6FDF8A77).
        const int32_t nUnitType = ptCelContext->nUnitType;
        if (nUnitType == GFXHASH_KIND_OBJECT)
        {
            return nFileDirection / *pDirectionsPerBlock;
        }
        if (!(nUnitType == GFXHASH_KIND_6 && (ptCelContext->fState & 0x02)))
        {
            if (nUnitType == GFXHASH_KIND_ITEM)
            {
                return ptCelContext->nDirection / *pDirectionsPerBlock;
            }
            if (nUnitType != GFXHASH_KIND_MISSILE)
            {
                return nFileDirection / *pDirectionsPerBlock;
            }
        }
        if (pHeader->bFlags & 0x01)
        {
            return nFileDirection / *pDirectionsPerBlock;
        }
        nFileDirection = CELCMP_CelGameDirToFileDir(ptCelContext->nDirection, pHeader->nDirections);
        break;
    }
    }

    if (*pDirectionsPerBlock == 0)
    {
        return 0;
    }
    return nFileDirection / *pDirectionsPerBlock;
}
