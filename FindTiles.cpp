#include "D2CMP.h"

/*---------------------------------------------------------------------------
 *
 *  FindTiles.cpp -- D2CMP.dll 1.10 (image base 0x6FDF0000), address range 0x6FDF82A0..0x6FDF8685.
 *  The original's source file is D2CMP\SRC\FindTiles.cpp (its asserts name it).
 *
 *  The find-tiles hash of a tile project (D2TileLibraryHashStrc): 128 buckets of nodes, one node per (style, sequence,
 *  type), each holding a list of references to the project's tiles of that kind. TILECMP_BuildProjectHash builds it with
 *  AllocHash and AddTile, TILELIB_TileLibraryGetTiles reads it with CollectTiles, TILEPROJECTS_DestroyTileHeaders frees it.
 *
 *  The key is the low byte of 2 * sequence - type + style, reduced modulo 128: tiles of different kinds share buckets,
 *  and the nodes are told apart by all three fields. The key's assert cannot fire: a remainder of 128 is below 128.
 *
 *-------------------------------------------------------------------------*/

//  The path literal of the unit's Fog calls (the original's __FILE__).
static const char szFindTilesCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2CMP\\SRC\\FindTiles.cpp";

//  The node's and the reference's members, each at an instruction that uses it (node 14h bytes, reference 8).
static_assert(offsetof(D2TileLibraryHashNodeStrc, nStyle) == 0x00, "D2TileLibraryHashNodeStrc.nStyle: mov ecx, [esi] (0x6FDF843B)");
static_assert(offsetof(D2TileLibraryHashNodeStrc, nSequence) == 0x04, "D2TileLibraryHashNodeStrc.nSequence: cmp [esi+4], edx (0x6FDF8433)");
static_assert(offsetof(D2TileLibraryHashNodeStrc, nType) == 0x08, "D2TileLibraryHashNodeStrc.nType: mov ecx, [esi+8] (0x6FDF8444)");
static_assert(offsetof(D2TileLibraryHashNodeStrc, pRef) == 0x0C, "D2TileLibraryHashNodeStrc.pRef: mov eax, [esi+0Ch] (0x6FDF850E)");
static_assert(offsetof(D2TileLibraryHashNodeStrc, pPrev) == 0x10, "D2TileLibraryHashNodeStrc.pPrev: mov esi, [esi+10h] (0x6FDF844F)");
static_assert(offsetof(D2TileLibraryHashRefStrc, pTile) == 0x00 && offsetof(D2TileLibraryHashRefStrc, pPrev) == 0x04, "D2TileLibraryHashRefStrc: cmp [eax], edi / mov eax, [eax+4] (0x6FDF8515, 0x6FDF8519)");


//1.10f: D2CMP.0x6FDF82A0
//  Returns a zeroed hash (FOG_Alloc 200h, :42, asserted :43 and exit; `rep stosd` 80h dwords). __cdecl with nothing to take.
D2TileLibraryHashStrc *__cdecl FINDTILES_AllocHash()
{
    D2TileLibraryHashStrc *ptHash = (D2TileLibraryHashStrc *)FOG_Alloc(sizeof(D2TileLibraryHashStrc), szFindTilesCpp, 42, 0);
    if (!ptHash)
    {
        FOG_DisplayAssert("ptHash", szFindTilesCpp, 43);
        exit(-1);
    }
    memset(ptHash, 0, sizeof(D2TileLibraryHashStrc));
    return ptHash;
}


//1.10f: D2CMP.0x6FDF82F0
//  Frees a hash: nothing for NULL; for each of the 128 buckets, each node -- its next taken first -- has its references freed (:76), its
//  reference head cleared and itself freed (:80); the bucket is cleared; then the hash is freed (:87). The bucket index is a signed compare.
void __fastcall FINDTILES_FreeHash(D2TileLibraryHashStrc *pHash)
{
    if (!pHash)
        return;

    for (int32_t i = 0; i < FIND_TILE_HASH_SIZE; ++i)
    {
        D2TileLibraryHashNodeStrc *pNode = pHash->pNodes[i];
        while (pNode)
        {
            D2TileLibraryHashNodeStrc *pPrevNode = pNode->pPrev;
            D2TileLibraryHashRefStrc *pRef = pNode->pRef;
            while (pRef)
            {
                D2TileLibraryHashRefStrc *pPrevRef = pRef->pPrev;
                FOG_Free(pRef, szFindTilesCpp, 76, 0);
                pRef = pPrevRef;
            }
            pNode->pRef = nullptr;
            FOG_Free(pNode, szFindTilesCpp, 80, 0);
            pNode = pPrevNode;
        }
        pHash->pNodes[i] = nullptr;
    }
    FOG_Free(pHash, szFindTilesCpp, 87, 0);
}


//1.10f: D2CMP.0x6FDF8380
//  Adds a tile to the hash: both pointers asserted (:165, :166, and exit); the bucket is the key's; a node of the tile's style, sequence
//  and type is looked for -- the walk keeps a flag, 1 until a match, as the source's loop did -- and, found, gets a reference to the tile
//  unless it holds one already; not found, a new node with the tile's three fields and one reference goes in at the bucket's head.
//  The original inlines the hash key, a new key node with its first reference, and a further reference here.
void __fastcall FINDTILES_AddTile(D2TileLibraryHashStrc *ptHash, D2TileLibraryEntryStrc *ptTile)
{
    if (!ptHash)
    {
        FOG_DisplayAssert("ptHash", szFindTilesCpp, 165);
        exit(-1);
    }
    if (!ptTile)
    {
        FOG_DisplayAssert("ptTile", szFindTilesCpp, 166);
        exit(-1);
    }

    uint8_t tKey = (uint8_t)((uint8_t)((uint8_t)ptTile->nSequence * 2 - (uint8_t)ptTile->nType + (uint8_t)ptTile->nStyle) % FIND_TILE_HASH_SIZE);
    if (tKey >= FIND_TILE_HASH_SIZE)
    {
        FOG_DisplayAssert("tKey < FIND_TILE_HASH_SIZE", szFindTilesCpp, 30);
        exit(-1);
    }

    D2TileLibraryHashNodeStrc **ppBucket = &ptHash->pNodes[tKey];
    D2TileLibraryHashNodeStrc *pNode = *ppBucket;
    int32_t bNotFound = 1;
    while (pNode && bNotFound)
    {
        if (pNode->nSequence == ptTile->nSequence && pNode->nStyle == ptTile->nStyle && pNode->nType == ptTile->nType)
            bNotFound = 0;
        else
            pNode = pNode->pPrev;
    }

    if (bNotFound)
    {
        D2TileLibraryHashNodeStrc *ptKeyNode = (D2TileLibraryHashNodeStrc *)FOG_Alloc(sizeof(D2TileLibraryHashNodeStrc), szFindTilesCpp, 130, 0);
        if (!ptKeyNode)
        {
            FOG_DisplayAssert("ptKeyNode", szFindTilesCpp, 131);
            exit(-1);
        }
        memset(ptKeyNode, 0, sizeof(D2TileLibraryHashNodeStrc));
        ptKeyNode->nSequence = ptTile->nSequence;
        ptKeyNode->nStyle = ptTile->nStyle;
        ptKeyNode->nType = ptTile->nType;
        ptKeyNode->pPrev = *ppBucket;
        *ppBucket = ptKeyNode;

        D2TileLibraryHashRefStrc *ptTileRef = (D2TileLibraryHashRefStrc *)FOG_Alloc(sizeof(D2TileLibraryHashRefStrc), szFindTilesCpp, 142, 0);
        if (!ptTileRef)
        {
            FOG_DisplayAssert("ptTileRef", szFindTilesCpp, 143);
            exit(-1);
        }
        memset(ptTileRef, 0, sizeof(D2TileLibraryHashRefStrc));
        ptTileRef->pTile = ptTile;
        ptKeyNode->pRef = ptTileRef;
        return;
    }

    for (D2TileLibraryHashRefStrc *pRef = pNode->pRef; pRef; pRef = pRef->pPrev)
    {
        if (pRef->pTile == ptTile)
            return;
    }
    D2TileLibraryHashRefStrc *ptRef = (D2TileLibraryHashRefStrc *)FOG_Alloc(sizeof(D2TileLibraryHashRefStrc), szFindTilesCpp, 110, 0);
    if (!ptRef)
    {
        FOG_DisplayAssert("ptRef", szFindTilesCpp, 111);
        exit(-1);
    }
    memset(ptRef, 0, sizeof(D2TileLibraryHashRefStrc));
    ptRef->pTile = ptTile;
    ptRef->pPrev = pNode->pRef;
    pNode->pRef = ptRef;
}


//1.10f: D2CMP.0x6FDF8570
//  Collects into hTileList, up to nCapacity, the tiles of one style, sequence and type: both pointers asserted (:207, :208, and exit),
//  0 for a zero capacity; the key's bucket walked with AddTile's flag for the node; its references copied in list order while the
//  count is below the capacity (unsigned compare); returns the count. The original keeps the count twice (the list index and the
//  answer), equal at every step. The original inlines the hash key here.
uint32_t __fastcall FINDTILES_CollectTiles(D2TileLibraryHashStrc *ptHash, D2TileLibraryEntryStrc **hTileList, uint32_t nCapacity, int32_t nType, int32_t nStyle, int32_t nSequence)
{
    uint32_t nFound = 0;
    if (!ptHash)
    {
        FOG_DisplayAssert("ptHash", szFindTilesCpp, 207);
        exit(-1);
    }
    if (!hTileList)
    {
        FOG_DisplayAssert("hTileList", szFindTilesCpp, 208);
        exit(-1);
    }
    if (!nCapacity)
        return 0;

    uint8_t tKey = (uint8_t)((uint8_t)((uint8_t)nSequence * 2 - (uint8_t)nType + (uint8_t)nStyle) % FIND_TILE_HASH_SIZE);
    if (tKey >= FIND_TILE_HASH_SIZE)
    {
        FOG_DisplayAssert("tKey < FIND_TILE_HASH_SIZE", szFindTilesCpp, 30);
        exit(-1);
    }

    D2TileLibraryHashNodeStrc *pNode = ptHash->pNodes[tKey];
    int32_t bFound = 0;
    while (pNode && !bFound)
    {
        if (pNode->nSequence == nSequence && pNode->nStyle == nStyle && pNode->nType == nType)
            bFound = 1;
        else
            pNode = pNode->pPrev;
    }
    if (!bFound)
        return nFound;

    for (D2TileLibraryHashRefStrc *pRef = pNode->pRef; pRef && nFound < nCapacity; pRef = pRef->pPrev)
        hTileList[nFound++] = pRef->pTile;
    return nFound;
}
