#include "D2CMP.h"

#include <intrin.h>

//  CelCmp.cpp -- the cel files of D2CMP.dll 1.10: the compressor that builds a cel file out of images under a handle (#10013..#10016), the
//  direction conversions (#10017..#10022), a cel file's serialisation, normalisation, conversions and release (#10023..#10032), the three draw
//  entries that hand a frame to FastCmp.cpp's decoders (#10033..#10035), the accessors and the renderer's hardware hooks (#10036..#10047), and
//  Storm's TSList, TSHashTable and TSExportTableSyncReuse as the original instantiated them over CELLIST and TCELENTRY for the compressor's
//  builders. The original's source file is C:\projects\D2\head\Diablo2\Source\D2CMP\SRC\CelCmp.cpp (named by its asserts). Address range
//  0x6FDF1000..0x6FDF3D0F, plus CELCMP_FindFirstSetBit1Based (0x6FE00346) among the import thunks. The runtime functions of the range (the
//  builders' table's static initialiser, its atexit destructor and its constructor, four list initialisers, operator delete) are not rewritten.
//
//  Storm's lists: a D2CmpStormList is a link offset and a terminator link, and a node's link sits at that offset inside the node. A link's
//  pNextOrSentinel is the next node or, at the end of the list, the complement of the terminator's address, so a node pointer is taken only when it
//  is positive as an int. When the offset is not known (-1) the next link is computed from the previous link's own next node. The original inlines
//  the templates differently at each site; where an inlined copy is a function's whole body ours calls that function, otherwise it is written out.
//
//  D2MOO BUG: D2MOO's D2GfxCellStrc and D2CellFileStrc declare a pointer at a frame's +0x20 and at a cel file's +0x18; the binary keeps the frame's
//  stream and the file's table of frames inline there (D2CmpRleCellHeader and D2CmpBuiltCellFile).
//  DIFFERENCE: the original's destructors, the table's constructor and CELCMP_CellBuilderTable_Rehash carry compiler exception-handling frames (an
//  fs:0 chain with unwind states, there to finish destroying members if a later step throws), and unwind funclets at the end of .text
//  (0x6FE089EE..0x6FE08B6F) call CELCMP_StormListArray_DestroyWithUnwind. Ours has none of them; nothing here throws a C++ exception.

static const char szCelCmpCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2CMP\\SRC\\CelCmp.cpp";

//  Storm's allocation tags: the RTTI type-descriptor names the templates pass to SMemAlloc and SMemFree as their file argument.
static const char szCellListTag[] = ".?AUCELLIST@@";                                              // 0x6FE0D048
static const char szCellListChunkTag[] = ".?AV?$TSHashObjectChunk@UCELLIST@@VHASHKEY_NONE@@@@";  // 0x6FE0D060
static const char szCelEntryTag[] = ".?AUTCELENTRY@@";                                            // 0x6FE0D188
static const char szCellListBucketsTag[] = ".?AV?$TSExplicitList@UCELLIST@@$0?CCCCCCCD@@@";         // 0x6FE0D478
static const char szDeleteTag[] = "delete";                                                       // 0x6FE0D178

static_assert(sizeof(D2CmpStormList) == 12, "`lea eax, [eax+eax*2]` / `lea eax, [edx+eax*4+4]` (0x6FDF16D2, 0x6FDF16D5)");
static_assert(sizeof(D2CmpCellBuildEntry) == 0x24, "`add ecx, 24h` before SMemAlloc (0x6FDF2862)");
static_assert(sizeof(D2CmpCellBuilder) == 0x38, "`add ecx, 38h` before SMemAlloc (0x6FDF3253)");
static_assert(offsetof(D2CmpCellBuilder, slotLink) == 0x04 && offsetof(D2CmpCellBuilder, fullLink) == 0x0C, "`mov eax, [edi+4]` / `lea ecx, [edi+0Ch]` (0x6FDF186F, 0x6FDF189B)");
static_assert(offsetof(D2CmpCellBuilder, nKey) == 0x14, "`mov [eax+14h], cl` (0x6FDF35FF)");
static_assert(offsetof(D2CmpCellBuilder, dwPixelFormatFlags) == 0x18 && offsetof(D2CmpCellBuilder, nFormat) == 0x1C &&
                  offsetof(D2CmpCellBuilder, dwTransparentColor) == 0x20,
              "`mov dword ptr [edi+18h], 4` / `mov [edi+1Ch], eax` / `mov [edi+20h], ecx` (0x6FDF163C, 0x6FDF161F, 0x6FDF1649)");
static_assert(offsetof(D2CmpCellBuilder, nFrameCount) == 0x24 && offsetof(D2CmpCellBuilder, nDirectionCount) == 0x28, "`mov dword ptr [edi+24h], 0` / `mov [edi+28h], ebx` (0x6FDF1628, 0x6FDF161C)");
static_assert(offsetof(D2CmpCellBuilder, frames) == 0x2C, "`lea ecx, [ebx+2Ch]` (0x6FDF1732)");
static_assert(offsetof(D2CmpCellBuildEntry, nOrientation) == 0x08 && offsetof(D2CmpCellBuildEntry, nPixelWidth) == 0x0C &&
                  offsetof(D2CmpCellBuildEntry, nPixelHeight) == 0x10 && offsetof(D2CmpCellBuildEntry, nPixelCount) == 0x14,
              "`mov [esi+8], edx` / `[esi+0Ch], edi` / `[esi+10h], eax` / `[esi+14h], eax` (0x6FDF1765, 0x6FDF175C, 0x6FDF1747, 0x6FDF1755)");
static_assert(offsetof(D2CmpCellBuildEntry, nOffsetX) == 0x18 && offsetof(D2CmpCellBuildEntry, nOffsetY) == 0x1C &&
                  offsetof(D2CmpCellBuildEntry, pPixels) == 0x20,
              "`mov [esi+18h], eax` / `[esi+1Ch], ecx` / `[esi+20h], eax` (0x6FDF175F, 0x6FDF1762, 0x6FDF1799)");
static_assert(offsetof(D2CmpCellBuilderTableHeader, fullList) == 0x04 && offsetof(D2CmpCellBuilderTableHeader, nFullness) == 0x10, "`lea eax, [edi+4]` / `mov dword ptr [edi+10h], 0` (0x6FDF2B58, 0x6FDF2B63)");
static_assert(offsetof(D2CmpCellBuilderTableHeader, nBucketCapacity) == 0x14 && offsetof(D2CmpCellBuilderTableHeader, nBucketCount) == 0x18 &&
                  offsetof(D2CmpCellBuilderTableHeader, pBuckets) == 0x1C && offsetof(D2CmpCellBuilderTableHeader, dwBucketMask) == 0x24,
              "`lea ecx, [esi+14h]` / `mov eax, [esi+18h]` / `mov ecx, [esi+1Ch]` / `mov eax, [esi+24h]` (0x6FDF2F68, 0x6FDF2F37, 0x6FDF3594, 0x6FDF3509)");
static_assert(offsetof(D2CmpCellBuilderTableHeader, nBucketCapacity) + offsetof(D2CmpStormListArray, dwCount) == offsetof(D2CmpCellBuilderTableHeader, nBucketCount) &&
                  offsetof(D2CmpCellBuilderTableHeader, nBucketCapacity) + offsetof(D2CmpStormListArray, pData) == offsetof(D2CmpCellBuilderTableHeader, pBuckets) &&
                  offsetof(D2CmpCellBuilderTableHeader, nBucketCapacity) + offsetof(D2CmpStormListArray, dwChunkSize) == offsetof(D2CmpCellBuilderTableHeader, nBucketGrowth),
              "the bucket array is a D2CmpStormListArray at +0x14: `lea ecx, [esi+14h]` / `call StormListArray_SetCount` (0x6FDF3A4A)");
static_assert(offsetof(D2CmpCellBuilderTablePrefix, reuseList) == 0x28 && offsetof(D2CmpCellBuilderTablePrefix, dwObjectChunkSize) == 0x34 &&
                  offsetof(D2CmpCellBuilderTablePrefix, chunkList) == 0x38,
              "`lea ebp, [edi+28h]` / `mov dword ptr [esi+34h], 10h` / `lea ebp, [edi+38h]` (0x6FDF10F0, 0x6FDF2A0F, 0x6FDF10A9)");
static_assert(offsetof(D2CmpCellBuilderTablePrefix, nDummyKey) == 0x44 && offsetof(D2CmpCellBuilderTablePrefix, dwHandleSequence) == 0x48 &&
                  offsetof(D2CmpCellBuilderTablePrefix, bHandleSequenceWrapped) == 0x4C && offsetof(D2CmpCellBuilderTablePrefix, criticalSection) == 0x50,
              "gnCellBuilderDummyKey 0x6FE12CEC, `mov [esi+48h], eax` / `mov dword ptr [esi+4Ch], 0` / `lea ecx, [esi+50h]` (0x6FDF2A2E, 0x6FDF2A31, 0x6FDF2A1D)");
static_assert(offsetof(D2CmpCellBuilderObjectChunk, link) == 0x10, "`lea eax, [ebp+10h]` (0x6FDF3723)");
static_assert(offsetof(D2CmpBuiltCellFile, nFormat) == 0x08 && offsetof(D2CmpBuiltCellFile, dwTerminationOrTransparentColor) == 0x0C, "`mov edx, [ebx+8]` / `mov [edi+0Ch], eax` (0x6FDF21B7, 0x6FDF19CE)");
static_assert(offsetof(D2CmpBuiltCellFile, nDirections) == 0x10 && offsetof(D2CmpBuiltCellFile, nFramesPerDirection) == 0x14 &&
                  offsetof(D2CmpBuiltCellFile, pFrames) == 0x18,
              "`mov eax, [edi+10h]` / `imul eax, [edi+14h]` / `mov eax, [esi+ecx*4+18h]` (0x6FDF1A59, 0x6FDF1A5D, 0x6FDF25C9)");
static_assert(offsetof(D2CmpRleCellHeader, unknown14) == 0x14 && offsetof(D2CmpRleCellHeader, dwNextBlockOrCellNode) == 0x18 &&
                  offsetof(D2CmpRleCellHeader, dwLength) == 0x1C && sizeof(D2CmpRleCellHeader) == 0x20,
              "`mov edi, [ecx+14h]` / `mov eax, [eax+18h]` / `mov ebx, [ecx+1Ch]` / `add eax, 20h` (0x6FDF1E4D, 0x6FDF2846, 0x6FDF1DDB, 0x6FDF26C6)");
static_assert(offsetof(D2CmpRgb24FrameHeader, dwWidth) == 0x04 && offsetof(D2CmpRgb24FrameHeader, dwHeight) == 0x08 && sizeof(D2CmpRgb24FrameHeader) == 0x20,
              "`mov eax, [edx+8]` / `imul eax, [edx+4]` / `lea esi, [edx+20h]` (0x6FDF228D, 0x6FDF2293, 0x6FDF2290)");
static_assert(offsetof(D2CmpSerializedCellFileV5Header, frameRefs) == 0x34, "`lea ecx, [ebx+34h]` (0x6FDF1B37)");
static_assert(offsetof(D2CmpSerializedRleCellHeaderV5, dwAuxiliaryBytes) == 0x1C && offsetof(D2CmpSerializedRleCellHeaderV5, dwAuxiliaryOffsetOrPointer) == 0x20 &&
                  offsetof(D2CmpSerializedRleCellHeaderV5, dwMainStreamBytes) == 0x24 && offsetof(D2CmpSerializedRleCellHeaderV5, pMainStream) == 0x28,
              "`cmp [eax+1Ch], ebp` / `add [eax+20h], ebx` / `mov ebp, [eax+24h]` / `add esi, 28h` (0x6FDF1B40, 0x6FDF1B45, 0x6FDF1B7C, 0x6FDF1C37)");
static_assert(offsetof(D2CmpCelHardwareNode, pAuxData) == 0x28 && sizeof(D2CmpCelHardwareNode) == 44, "`mov esi, [esi+28h]` (0x6FDF1E0E); 44 bytes a frame, `lea edi, [eax+ecx*2]` / `shl edi, 2` and `add edx, 2Ch` (0x6FDF1F41, 0x6FDF1F8A)");
static_assert(offsetof(D2GfxDataStrc, pCellFile) == 0x04 && offsetof(D2GfxDataStrc, nFrame) == 0x08 && offsetof(D2GfxDataStrc, nDirection) == 0x0C,
              "`mov ebx, [eax+4]` / `mov eax, [edi+8]` / `mov ebx, [edi+0Ch]` (0x6FDF20A7, 0x6FDF257D, 0x6FDF2575)");

//  The original's BITCOUNT, named by three asserts' texts and inlined at each: the bits of a dword counted by nibbles, the nibble sums added by
//  bytes and summed mod 255, all unsigned. The original has no out-of-line copy, so this helper is ours.
static uint32_t CelCmpBitCount(uint32_t n)
{
    const uint32_t nNibbleSums = n - ((n >> 1) & 0x77777777) - ((n >> 2) & 0x33333333) - ((n >> 3) & 0x11111111);
    return ((nNibbleSums + (nNibbleSums >> 4)) & 0x0F0F0F0F) % 0xFF;
}

//  DIFFERENCE: the original indexes its two direction tables and its four format tables without a bound, and an index past the end reads the .rdata
//  that follows. These helpers answer an empty slot there instead, 0 or NULL. A negative format counts as past the end too (the original reads
//  before the table).
static uint32_t CelCmpDirection32Slot(uint32_t nIndex)
{
    return nIndex < 736 ? gnDirection32ToFrameIndex[nIndex] : 0;
}

static uint32_t CelCmpDirection64Slot(uint32_t nIndex)
{
    return nIndex < 512 ? gnDirection64ToFrameIndex[nIndex] : 0;
}

template <typename T>
static T CelCmpFormatSlot(const T (&apTable)[2], int32_t nFormat)
{
    return (uint32_t)nFormat < 2 ? apTable[nFormat] : NULL;
}


//1.10f: D2CMP.0x6FDF1030
//  ~TSExportTableSyncReuse on the builders' table: deletes the table's critical section, then runs the base's destructor, ExportDestroy. Called
//  from SyncScalarDeletingDestructor (slot 2 of gVtblCellBuilderTableSync) and from the destructor of sgCellBuilderTableLifetime, which stands in
//  for the original's atexit destructor (that one jumps here). The original inlines CELCMP_CellBuilderTable_ExportDestroy here.
void __fastcall CELCMP_CellBuilderTable_SyncDestroy(D2CmpCellBuilderTablePrefix *pTable)
{
    DeleteCriticalSection(&pTable->criticalSection);
    CELCMP_CellBuilderTable_ExportDestroy((D2CmpCellBuilderTableReusePrefix *)pTable);
}

//1.10f: D2CMP.0x6FDF11D0
//  #10017. Converts a 32-direction index to the frame row of a file of nDirectionCount directions: gnDirection32ToFrameIndex[32 * (the 1-based
//  first set bit of the count) + the direction]. Nothing is asserted.
//  DIFFERENCE: the index has no bound; one of 736 or more reads the .rdata after the table in the original, and ours answers 0.
extern "C" uint32_t __stdcall CELCMP_CelNormalDirToFileDir(uint32_t nDirection32, uint32_t nDirectionCount)
{
    return CelCmpDirection32Slot(32 * CELCMP_FindFirstSetBit1Based(nDirectionCount) + nDirection32);
}

//1.10f: D2CMP.0x6FDF11F0
//  #10018. The same for a 64-direction (game) direction: gnDirection64ToFrameIndex[64 * (the 1-based first set bit of the count) + the direction].
//  DIFFERENCE: an index of 512 or more answers 0 here; the original reads what follows the table.
extern "C" uint32_t __stdcall CELCMP_CelGameDirToFileDir(uint32_t nDirection64, uint32_t nDirectionCount)
{
    return CelCmpDirection64Slot(64 * CELCMP_FindFirstSetBit1Based(nDirectionCount) + nDirection64);
}

//1.10f: D2CMP.0x6FDF1210
//  #10019. Maps a direction of a file of nStartDirections (8 or 16) directions to its block of nRepresentativeDirections (4 or 8), by one of four
//  byte tables; bAlternate selects the alternate pair (SpriteCache.cpp passes the context's state bit 2). The direction must be below
//  nStartDirections (unsigned compare); any pair of counts but 8->4 and 16->8 halts.
extern "C" uint32_t __stdcall CELCMP_CelConvertDir(uint32_t nDirection, uint32_t nStartDirections, uint32_t nRepresentativeDirections, int32_t bAlternate)
{
    if (nDirection >= nStartDirections)
    {
        FOG_DisplayAssert("nDirection < nStartDirs", szCelCmpCpp, 416);
        exit(-1);
    }

    if (nStartDirections == 8)
    {
        if (nRepresentativeDirections == 4)
            return bAlternate ? gCelDirection8To4Alternate[nDirection] : gCelDirection8To4Default[nDirection];
    }
    else if (nStartDirections == 16 && nRepresentativeDirections == 8)
    {
        return bAlternate ? gCelDirection16To8Alternate[nDirection] : gCelDirection16To8Default[nDirection];
    }

    FOG_DisplayHalt("Could not convert direction!", szCelCmpCpp, 441);
    exit(-1);
}

//1.10f: D2CMP.0x6FDF12B0
//  #10020. Which half a block of directions doubles into: 0 for 4->8, 1 for 8->16; any other pair halts. SpriteCache.cpp passes the block size and
//  twice it.
extern "C" int32_t __stdcall CELCMP_CelConvertBlock(int32_t nSourceDirections, int32_t nTargetDirections)
{
    if (nSourceDirections == 4)
    {
        if (nTargetDirections == 8)
            return 0;
    }
    else if (nSourceDirections == 8 && nTargetDirections == 16)
    {
        return 1;
    }

    FOG_DisplayHalt("Could not convert direction!", szCelCmpCpp, 464);
    exit(-1);
}

//1.10f: D2CMP.0x6FDF1300
//  #10021. Rotates a game direction (0..63) to its shadow's: gnCelShadowGameDirection[64 * (the 1-based first set bit of nLevelOfRotation) + the
//  direction]. Asserts that the direction is below 64, that the first set bit is below 8 and that nLevelOfRotation has exactly one bit set
//  (BITCOUNT), which together keep the index below 512. The first set bit is computed twice, for the assert and for the index.
extern "C" int32_t __stdcall CELCMP_CelGameDirToShadowGameDir(uint32_t nGameDirection, uint32_t nLevelOfRotation)
{
    if (nGameDirection >= 64)
    {
        FOG_DisplayAssert("nGameDirection < CELFILE_NUMGAMEDIRECTIONS", szCelCmpCpp, 477);
        exit(-1);
    }

    if (CELCMP_FindFirstSetBit1Based(nLevelOfRotation) >= 8)
    {
        FOG_DisplayAssert("GetFirstBit( nLevelOfRotation ) < 8", szCelCmpCpp, 478);
        exit(-1);
    }

    if (CelCmpBitCount(nLevelOfRotation) != 1)
    {
        FOG_DisplayAssert("BITCOUNT( nLevelOfRotation ) == 1", szCelCmpCpp, 479);
        exit(-1);
    }

    return gnCelShadowGameDirection[64 * CELCMP_FindFirstSetBit1Based(nLevelOfRotation) + nGameDirection];
}

//1.10f: D2CMP.0x6FDF1400
//  #10022. The twin of CelGameDirToShadowGameDir over gnCelNormalDirection, with the same asserts.
extern "C" int32_t __stdcall CELCMP_CelGameDirToNormalDir(uint32_t nGameDirection, uint32_t nLevelOfRotation)
{
    if (nGameDirection >= 64)
    {
        FOG_DisplayAssert("nGameDirection < CELFILE_NUMGAMEDIRECTIONS", szCelCmpCpp, 492);
        exit(-1);
    }

    if (CELCMP_FindFirstSetBit1Based(nLevelOfRotation) >= 8)
    {
        FOG_DisplayAssert("GetFirstBit( nLevelOfRotation ) < 8", szCelCmpCpp, 493);
        exit(-1);
    }

    if (CelCmpBitCount(nLevelOfRotation) != 1)
    {
        FOG_DisplayAssert("BITCOUNT( nLevelOfRotation ) == 1", szCelCmpCpp, 494);
        exit(-1);
    }

    return gnCelNormalDirection[64 * CELCMP_FindFirstSetBit1Based(nLevelOfRotation) + nGameDirection];
}

//1.10f: D2CMP.0x6FDF14E0
//  #10013. Creates a cel builder under a fresh handle and returns the handle. nDirections must be a single bit and at most 32 (unsigned compare)
//  and nFormat below 2 (signed compare), all asserted. Under the table's lock the handle sequence steps past 0 (recording that it wrapped), and
//  once it has wrapped also past every handle still in the table. The loop that then drains the new builder's frame list finds nothing, as the list
//  was just made empty; the original has it. The pixel format flag 4 means RGB24. The original inlines the table's NewLock here.
extern "C" uint32_t __stdcall CELCMP_CelCompressCreate(int32_t nFormat, uint32_t nDirections, const uint32_t *pTransparentColor, int32_t bRGB24)
{
    if (CelCmpBitCount(nDirections) != 1)
    {
        FOG_DisplayAssert("BITCOUNT( dwNumDirections ) == 1", szCelCmpCpp, 511);
        exit(-1);
    }

    if (nDirections > 32)
    {
        FOG_DisplayAssert("dwNumDirections <= 32", szCelCmpCpp, 512);
        exit(-1);
    }

    if (nFormat >= 2)
    {
        FOG_DisplayAssert("format < NUM_CELFORMATS", szCelCmpCpp, 513);
        exit(-1);
    }

    EnterCriticalSection(&gCellBuilderTable.criticalSection);
    do
    {
        while (++gCellBuilderTable.dwHandleSequence == 0)
            gCellBuilderTable.bHandleSequenceWrapped = 1;
    } while (gCellBuilderTable.bHandleSequenceWrapped &&
             CELCMP_CellBuilderTable_Find(&gCellBuilderTable.header, 0, gCellBuilderTable.dwHandleSequence, &gCellBuilderTable.nDummyKey));

    const uint32_t hNewBuilder = gCellBuilderTable.dwHandleSequence;
    D2CmpCellBuilder *pBuilder = CELCMP_CellBuilderTable_New(&gCellBuilderTable.header, 0, hNewBuilder, &gCellBuilderTable.nDummyKey, 0, 0);

    D2CmpCellBuildEntry *pStaleEntry;
    while ((intptr_t)(pStaleEntry = (D2CmpCellBuildEntry *)pBuilder->frames.terminator.pNextOrSentinel) > 0)
    {
        CELCMP_StormLink_Unlink(&pStaleEntry->link);
        STORM_SMemFree(pStaleEntry, szCelEntryTag, -2, 0);
    }

    pBuilder->nDirectionCount = nDirections;
    pBuilder->nFormat = nFormat;
    pBuilder->nFrameCount = 0;
    pBuilder->dwPixelFormatFlags = 0;
    if (bRGB24)
    {
        pBuilder->dwPixelFormatFlags = 4;
        if (pTransparentColor)
            pBuilder->dwTransparentColor = *pTransparentColor;
    }

    LeaveCriticalSection(&gCellBuilderTable.criticalSection);
    return hNewBuilder;
}

//1.10f: D2CMP.0x6FDF1670
//  #10015. Adds one image to a builder as a new entry at the tail of its frame list. nPixelHeight and nPixelWidth must be nonzero and nImageStride
//  at least nPixelWidth (asserted). The entry takes the size, the pixel count (height * width), the offsets and the orientation, and a copy of the
//  image from FOG_Alloc: a row of nPixelWidth bytes per line, three bytes a pixel for an RGB24 builder. The original inlines the table's Lock and
//  CELCMP_CellBuilderTable_Find here.
//  BUG: a handle not in the table leaves pBuilder NULL with the lock let go, and the function goes on: CELCMP_CellEntryList_NewNode is handed the
//  address 0x2C and faults reading 0x30 for the tail (0x6FDF28D3), as ours does. UNREACHABLE: the stock game does not reach this function.
//  BUG: FOG_Alloc's answer is used untested (0x6FDF1786, then SHARED_CopyImageRows). UNREACHABLE: the stock game does not reach this function.
extern "C" void __stdcall CELCMP_CelCompressAddImage(uint32_t hBuilder, const uint8_t *pImage, uint32_t nImageStride, uint32_t nPixelHeight, uint32_t nPixelWidth, int32_t nOffsetX, int32_t nOffsetY, int32_t nOrientation)
{
    if (!nPixelHeight || !nPixelWidth)
    {
        FOG_DisplayAssert("dwPixelHeight && dwPixelWidth", szCelCmpCpp, 553);
        exit(-1);
    }

    if (nImageStride < nPixelWidth)
    {
        FOG_DisplayAssert("dwImageByteStride >= dwPixelWidth", szCelCmpCpp, 554);
        exit(-1);
    }

    EnterCriticalSection(&gCellBuilderTable.criticalSection);
    int32_t nLockHeld = -1;

    void *pBucketHead = gCellBuilderTable.header.pBuckets[hBuilder & gCellBuilderTable.header.dwBucketMask].terminator.pNextOrSentinel;
    D2CmpCellBuilder *pCandidate = (intptr_t)pBucketHead <= 0 ? nullptr : (D2CmpCellBuilder *)pBucketHead;
    D2CmpCellBuilder *pBuilder = nullptr;
    while ((intptr_t)pCandidate > 0)
    {
        if (pCandidate->dwHandleHash == hBuilder)
        {
            pBuilder = pCandidate;
            break;
        }

        pCandidate = (D2CmpCellBuilder *)CELCMP_StormList_GetLink(&gCellBuilderTable.header.pBuckets[hBuilder & gCellBuilderTable.header.dwBucketMask], 0, pCandidate)->pNextOrSentinel;
    }

    if (!pBuilder)
    {
        LeaveCriticalSection(&gCellBuilderTable.criticalSection);
        nLockHeld = 0;
    }

    D2CmpCellBuildEntry *pEntry = CELCMP_CellEntryList_NewNode(&pBuilder->frames, 0, 2, 0, 0);
    ++pBuilder->nFrameCount;
    pEntry->nPixelHeight = nPixelHeight;
    pEntry->nPixelCount = nPixelHeight * nPixelWidth;
    pEntry->nPixelWidth = nPixelWidth;
    pEntry->nOffsetX = nOffsetX;
    pEntry->nOffsetY = nOffsetY;
    pEntry->nOrientation = nOrientation;

    uint32_t nRowBytes = nPixelWidth;
    if (pBuilder->dwPixelFormatFlags & 4)
        nRowBytes = 3 * nPixelWidth;

    uint8_t *pPixels = (uint8_t *)FOG_Alloc((int32_t)(nRowBytes * nPixelHeight), szCelCmpCpp, 569, 0);
    pEntry->pPixels = pPixels;
    SHARED_CopyImageRows(pPixels, pImage, (int32_t)nRowBytes, (int32_t)nImageStride, nPixelHeight, nRowBytes);

    if (nLockHeld)
        LeaveCriticalSection(&gCellBuilderTable.criticalSection);
}

//1.10f: D2CMP.0x6FDF17E0
//  #10014. Destroys a builder by handle under the table's lock: frees every entry's pixels, unlinks every entry and returns it to Storm, unlinks
//  the builder from its bucket and from the full list when it is still in the bucket, and hands it to the vtable's slot 0, InternalDelete, which
//  destroys and frees it. The original inlines the table's Lock, the table's Delete and CELCMP_StormLink_Unlink here.
//  BUG: a handle not in the table leaves pBuilder NULL with the lock let go, and the function goes on to read 0x34 (0x6FDF181B) and faults, as ours
//  does. UNREACHABLE: the stock game does not reach this function.
extern "C" void __stdcall CELCMP_CelCompressDestroy(uint32_t hBuilder)
{
    EnterCriticalSection(&gCellBuilderTable.criticalSection);
    int32_t nLockHeld = -1;
    D2CmpCellBuilder *pBuilder = CELCMP_CellBuilderTable_Find(&gCellBuilderTable.header, 0, hBuilder, &gCellBuilderTable.nDummyKey);
    if (!pBuilder)
    {
        LeaveCriticalSection(&gCellBuilderTable.criticalSection);
        nLockHeld = 0;
    }

    for (D2CmpCellBuildEntry *pEntry = (intptr_t)pBuilder->frames.terminator.pNextOrSentinel <= 0 ? nullptr : (D2CmpCellBuildEntry *)pBuilder->frames.terminator.pNextOrSentinel;
         (intptr_t)pEntry > 0; pEntry = (D2CmpCellBuildEntry *)pEntry->link.pNextOrSentinel)
    {
        FOG_Free(pEntry->pPixels, szCelCmpCpp, 588, 0);
    }

    D2CmpCellBuildEntry *pEntryToFree;
    while ((intptr_t)(pEntryToFree = (D2CmpCellBuildEntry *)pBuilder->frames.terminator.pNextOrSentinel) > 0)
    {
        CELCMP_StormLink_Unlink(&pEntryToFree->link);
        STORM_SMemFree(pEntryToFree, szCelEntryTag, -2, 0);
    }

    if (pBuilder->slotLink.pNextOrSentinel)
    {
        if (pBuilder->slotLink.pPrevLink)
        {
            CELCMP_StormLink_GetNextLink(&pBuilder->slotLink, 0, -1)->pPrevLink = pBuilder->slotLink.pPrevLink;
            pBuilder->slotLink.pPrevLink->pNextOrSentinel = pBuilder->slotLink.pNextOrSentinel;
            pBuilder->slotLink.pPrevLink = nullptr;
            pBuilder->slotLink.pNextOrSentinel = nullptr;
        }

        CELCMP_StormLink_Unlink(&pBuilder->fullLink);
    }

    gCellBuilderTable.header.pVtable->FreeBuilder(&gCellBuilderTable.header, 0, pBuilder);

    if (nLockHeld)
        LeaveCriticalSection(&gCellBuilderTable.criticalSection);
}

//1.10f: D2CMP.0x6FDF18D0
//  #10016. Builds a builder's frames into a cel file, *ppOutput (cleared first). Returns 0, 0x85200001 for a builder with no frames, or 0x85200002
//  when the frames do not divide evenly among the directions (unsigned divide). The file is a FOG_Alloc block of 24 bytes and a slot per frame,
//  filled by Raw.cpp's RGB24 encoder (which also takes the transparent colour) or by the encoder of the builder's format (gpCellBuilderEncoders).
//  The original inlines the table's Lock and CELCMP_CellBuilderTable_Find here.
//  BUG: a handle not in the table leaves pBuilder NULL with the lock let go, and the function goes on to read the frame count at 0x24 (0x6FDF1951)
//  and faults, as ours does. UNREACHABLE: the stock game does not reach this function.
//  BUG: both error returns leave the lock held when a builder was found (0x6FDF1958, 0x6FDF1972): the critical section stays entered. UNREACHABLE:
//  the stock game does not reach this function.
//  BUG: a builder of format 1 (CELCMP_CelCompressCreate lets 0 and 1 through) calls slot 1 of gpCellBuilderEncoders, which is NULL (0x6FDF19D6).
//  UNREACHABLE: the stock game does not reach this function.
//  BUG: FOG_Alloc's answer is used untested (0x6FDF1991..0x6FDF1998). UNREACHABLE: the stock game does not reach this function.
extern "C" uint32_t __stdcall CELCMP_CelCompressCreateCelFile(uint32_t hBuilder, D2CmpBuiltCellFile **ppOutput)
{
    EnterCriticalSection(&gCellBuilderTable.criticalSection);
    int32_t nLockHeld = -1;

    void *pBucketHead = gCellBuilderTable.header.pBuckets[hBuilder & gCellBuilderTable.header.dwBucketMask].terminator.pNextOrSentinel;
    D2CmpCellBuilder *pCandidate = (intptr_t)pBucketHead <= 0 ? nullptr : (D2CmpCellBuilder *)pBucketHead;
    D2CmpCellBuilder *pBuilder = nullptr;
    while ((intptr_t)pCandidate > 0)
    {
        if (pCandidate->dwHandleHash == hBuilder)
        {
            pBuilder = pCandidate;
            break;
        }

        pCandidate = (D2CmpCellBuilder *)CELCMP_StormList_GetLink(&gCellBuilderTable.header.pBuckets[hBuilder & gCellBuilderTable.header.dwBucketMask], 0, pCandidate)->pNextOrSentinel;
    }

    if (!pBuilder)
    {
        LeaveCriticalSection(&gCellBuilderTable.criticalSection);
        nLockHeld = 0;
    }

    *ppOutput = nullptr;
    const uint32_t nFrameCount = pBuilder->nFrameCount;
    if (!nFrameCount)
        return 0x85200001;

    if (nFrameCount % pBuilder->nDirectionCount)
        return 0x85200002;

    D2CmpBuiltCellFile *pOutput = (D2CmpBuiltCellFile *)FOG_Alloc((int32_t)(4 * nFrameCount + 24), szCelCmpCpp, 615, 0);
    pOutput->dwVersion = 6;
    pOutput->dwFlags = pBuilder->dwPixelFormatFlags;
    pOutput->nFormat = pBuilder->nFormat;
    pOutput->nDirections = pBuilder->nDirectionCount;
    pOutput->nFramesPerDirection = pBuilder->nFrameCount / pBuilder->nDirectionCount;
    if (pBuilder->dwPixelFormatFlags & 4)
    {
        RAW_EncodeRGB24CellFrames(pBuilder, pOutput);
        pOutput->dwTerminationOrTransparentColor = pBuilder->dwTransparentColor;
    }
    else
    {
        CelCmpFormatSlot(gpCellBuilderEncoders, pBuilder->nFormat)(pBuilder, pOutput);
    }

    if (nLockHeld)
        LeaveCriticalSection(&gCellBuilderTable.criticalSection);

    *ppOutput = pOutput;
    return 0;
}

//1.10f: D2CMP.0x6FDF1A00
//  #10029. Asserts that hFile is nonzero and does nothing else; it takes a second argument (retn 8) that it never reads.
//  void: no module of the stock install imports this export, so nothing reads the original's eax.
extern "C" void __stdcall CELCMP_CelFileHalve(int32_t hFile, int32_t nUnused)
{
    if (!hFile)
    {
        FOG_DisplayAssert("hFile", szCelCmpCpp, 668);
        exit(-1);
    }
}

//1.10f: D2CMP.0x6FDF1A30
//  #10025. Converts an RGB24 cel file (asserted) to an indexed one, *ppOutFile, and returns 1. The new file is a FOG_Alloc block of 24 bytes and a
//  slot per frame: version 6, the source's flags less their low three bits, its directions and frames per direction, format 0. It is filled by the
//  source format's converter (gpfnCelConvert24To8, FastCmp.cpp's for format 0) with the colour mapper and the transparent colour. The field at
//  +0x0C is left as FOG_Alloc leaves it.
//  BUG: FOG_Alloc's answer is used untested (0x6FDF1A74). UNREACHABLE: the stock game does not reach this function.
//  BUG: a source of format 1 calls slot 1 of gpfnCelConvert24To8, which is NULL (0x6FDF1AAD). UNREACHABLE: the stock game does not reach this
//  function.
//  DIFFERENCE: the source's format is not bounded; past slot 1 the original reads the table that follows, ours finds NULL.
extern "C" int32_t __stdcall CELCMP_CelFileConvert24To8(D2CmpBuiltCellFile *pSrc, D2CmpBuiltCellFile **ppOutFile,
                                                    uint8_t (__stdcall *pfMapRgbToIndex)(uint8_t nRed, uint8_t nGreen, uint8_t nBlue),
                                                    uint32_t dwTransparentColor)
{
    if (!(pSrc->dwFlags & 4))
    {
        FOG_DisplayAssert("pFile->dwFlags & CELFILE_24BIT", szCelCmpCpp, 685);
        exit(-1);
    }

    D2CmpBuiltCellFile *pOutFile = (D2CmpBuiltCellFile *)FOG_Alloc((int32_t)(4 * (pSrc->nDirections * pSrc->nFramesPerDirection) + 24), szCelCmpCpp, 689, 0);
    pOutFile->dwVersion = 6;
    pOutFile->dwFlags = pSrc->dwFlags & 0xFFFFFFF8;
    pOutFile->nDirections = pSrc->nDirections;
    pOutFile->nFramesPerDirection = pSrc->nFramesPerDirection;
    pOutFile->nFormat = 0;
    CelCmpFormatSlot(gpfnCelConvert24To8, pSrc->nFormat)(pSrc, pOutFile, pfMapRgbToIndex, dwTransparentColor);
    *ppOutFile = pOutFile;
    return 1;
}

//1.10f: D2CMP.0x6FDF1AD0
//  #10030. Rewrites a serialized version-5 cel file (flag 1 and version 5, asserted) as a serialized version-6 one. The old file is first changed
//  in place: flag 2 is set, every frame offset becomes a pointer, and each frame's auxiliary offset becomes a pointer, or 0 when the frame has no
//  auxiliary bytes. The new file is a FOG_Alloc block of 24 bytes, a slot per frame and, per frame, a 32-byte header, the main stream, 3 bytes and
//  the auxiliary bytes; its flags are 5 for an RGB24 source and 1 otherwise. Each frame's slot takes the frame's offset in the new file.
//  void: no module of the stock install imports this export, so nothing reads the original's eax.
//  BUG: the auxiliary bytes are copied from the address of the old frame's pointer field, not from where it points (lea esi, [ecx+20h],
//  0x6FDF1C52), so the copy takes the field, the stream length and the first bytes of the stream. Reproduced. UNREACHABLE: the stock game does not
//  reach this function.
//  BUG: FOG_Alloc's answer is used untested (0x6FDF1B9C). UNREACHABLE: the stock game does not reach this function.
extern "C" void __stdcall CELCMP_CelFileConvertOldToNew(D2CmpSerializedCellFileV5Header *pOldFile, D2CmpBuiltCellFile **ppNewFile, int32_t *pNewFileSize)
{
    const uint32_t dwOldFlags = pOldFile->dwFlags;
    if (!(dwOldFlags & 1))
    {
        FOG_DisplayAssert("pOldFile->dwFlags & CELFILE_SERIALIZED", szCelCmpCpp, 715);
        exit(-1);
    }

    if (pOldFile->dwVersion != 5)
    {
        FOG_DisplayAssert("pOldFile->dwVersion == ( CELFILE_VERSION - 1 )", szCelCmpCpp, 716);
        exit(-1);
    }

    pOldFile->dwFlags = dwOldFlags | 2;
    for (uint32_t iFrame = 0; iFrame < pOldFile->nFramesPerDirection * pOldFile->nDirections; ++iFrame)
    {
        D2CmpSerializedRleCellHeaderV5 *pOldFrame = (D2CmpSerializedRleCellHeaderV5 *)((char *)pOldFile + pOldFile->frameRefs[iFrame]);
        pOldFile->frameRefs[iFrame] = (uint32_t)(uintptr_t)pOldFrame;
        if (pOldFrame->dwAuxiliaryBytes)
            pOldFrame->dwAuxiliaryOffsetOrPointer += (uint32_t)(uintptr_t)pOldFile;
        else
            pOldFrame->dwAuxiliaryOffsetOrPointer = 0;
    }

    const uint32_t nFrames = pOldFile->nFramesPerDirection * pOldFile->nDirections;
    int32_t nSize = (int32_t)(4 * nFrames + 24);
    for (uint32_t iFrame = 0; iFrame < nFrames; ++iFrame)
    {
        const D2CmpSerializedRleCellHeaderV5 *pOldFrame = (const D2CmpSerializedRleCellHeaderV5 *)(uintptr_t)pOldFile->frameRefs[iFrame];
        nSize += (int32_t)(pOldFrame->dwMainStreamBytes + pOldFrame->dwAuxiliaryBytes + 35);
    }

    D2CmpBuiltCellFile *pNewFile = (D2CmpBuiltCellFile *)FOG_Alloc(nSize, szCelCmpCpp, 739, 0);
    pNewFile->dwVersion = 6;
    pNewFile->dwFlags = (pOldFile->dwFlags & 4) ? 5 : 1;
    pNewFile->nFormat = 0;
    pNewFile->dwTerminationOrTransparentColor = 0;
    pNewFile->nDirections = pOldFile->nDirections;
    pNewFile->nFramesPerDirection = pOldFile->nFramesPerDirection;

    uint8_t *pCursor = (uint8_t *)&pNewFile->pFrames[nFrames];
    for (uint32_t iFrame = 0; iFrame < nFrames; ++iFrame)
    {
        const D2CmpSerializedRleCellHeaderV5 *pOldFrame = (const D2CmpSerializedRleCellHeaderV5 *)(uintptr_t)pOldFile->frameRefs[iFrame];
        D2CmpRleCellHeader *pFrame = (D2CmpRleCellHeader *)pCursor;
        pNewFile->pFrames[iFrame] = (D2CmpRleCellHeader *)(uintptr_t)(pCursor - (uint8_t *)pNewFile);
        pFrame->dwOrientationFlags = pOldFrame->dwOrientationFlags;
        pFrame->dwWidth = pOldFrame->dwWidth;
        pFrame->dwHeight = pOldFrame->dwHeight;
        pFrame->nXOffset = pOldFrame->nXOffset;
        pFrame->nYOffset = pOldFrame->nYOffset;
        pFrame->unknown14 = pOldFrame->dwAuxiliaryBytes;
        pFrame->dwLength = pOldFrame->dwMainStreamBytes;
        memcpy(pCursor + sizeof(D2CmpRleCellHeader), pOldFrame->pMainStream, pFrame->dwLength);
        pCursor += pFrame->dwLength + 35;

        if (pOldFrame->dwAuxiliaryOffsetOrPointer)
        {
            memcpy(pCursor, &pOldFrame->dwAuxiliaryOffsetOrPointer, pOldFrame->dwAuxiliaryBytes);
            pFrame->dwNextBlockOrCellNode = (uint32_t)(pCursor - (uint8_t *)pNewFile);
            pCursor += pOldFrame->dwAuxiliaryBytes;
        }
        else
        {
            pFrame->dwNextBlockOrCellNode = 0;
        }
    }

    *ppNewFile = pNewFile;
    *pNewFileSize = nSize;
}

//1.10f: D2CMP.0x6FDF1CC0
//  #10026. Hands every non-transparent colour of an RGB24 cel file (asserted) to pVisitColor, frame by frame and direction by direction, through
//  Raw.cpp's visitor; both counts are re-read each pass. void: no module of the stock install imports this export, so nothing reads the original's
//  eax.
extern "C" void __stdcall CELCMP_CelFileIteratePixels(D2CmpRgb24RuntimeFilePrefix *pFile, D2CmpPackedColorCallback pVisitColor)
{
    if (!(pFile->dwPackedFileFlags & 4))
    {
        FOG_DisplayAssert("pFile->dwFlags & CELFILE_24BIT", szCelCmpCpp, 802);
        exit(-1);
    }

    for (uint32_t iDirection = 0; iDirection < pFile->dwDirections; ++iDirection)
    {
        for (uint32_t iFrame = 0; iFrame < pFile->dwFramesPerDirection; ++iFrame)
            RAW_Rgb24Frame_VisitNonTransparentColors(pFile->pFrames[pFile->dwFramesPerDirection * iDirection + iFrame], pVisitColor, pFile->dwTransparentColor);
    }
}

//1.10f: D2CMP.0x6FDF1D30
//  #10023. Serializes a runtime cel file into one FOG_Alloc block, *ppSerializedFile, and returns 1. The first 28 bytes are copied (the header and
//  the first slot, which the loop overwrites), flag 2 is cleared and flag 1 set. Per frame, its slot takes the frame's offset in the block and the
//  frame is copied whole with 3 bytes past its stream (dwLength + 35); when the frame's hardware node carries auxiliary data, unknown14 bytes of it
//  follow, and the copy's +0x18 takes their offset whether or not they were copied. Then the nodes are freed as frame 0's node, the one block that
//  CELCMP_CelFileNormalize allocated for all of them.
//  BUG: the nodes' block is freed only when frame 0's node carries auxiliary data (0x6FDF1E26..0x6FDF1E32); otherwise it is kept. UNREACHABLE: the
//  stock game does not reach this function.
//  BUG: when the block is freed, the source file's frames still point into it: their +0x18 is not cleared. UNREACHABLE: the stock game does not
//  reach this function.
//  BUG: FOG_Alloc's answer is used untested (0x6FDF1D86, then the copy). UNREACHABLE: the stock game does not reach this function.
extern "C" int32_t __stdcall CELCMP_CelFileSerialize(D2CmpBuiltCellFile *pFile, D2CmpBuiltCellFile **ppSerializedFile, int32_t *pSerializedSize)
{
    const uint32_t nFrames = pFile->nFramesPerDirection * pFile->nDirections;
    int32_t nSize = (int32_t)(4 * nFrames + 24);
    for (uint32_t iFrame = 0; iFrame < nFrames; ++iFrame)
        nSize += (int32_t)(pFile->pFrames[iFrame]->dwLength + pFile->pFrames[iFrame]->unknown14 + 35);

    D2CmpBuiltCellFile *pSerialized = (D2CmpBuiltCellFile *)FOG_Alloc(nSize, szCelCmpCpp, 837, 0);
    memcpy(pSerialized, pFile, 28);
    uint8_t *pCursor = (uint8_t *)&pSerialized->pFrames[nFrames];
    pSerialized->dwFlags = (pSerialized->dwFlags & 0xFFFFFFFD) | 1;

    void *pFree = nullptr;
    for (uint32_t iFrame = 0; iFrame < nFrames; ++iFrame)
    {
        D2CmpRleCellHeader *pCopy = (D2CmpRleCellHeader *)pCursor;
        const uint32_t dwBytes = pFile->pFrames[iFrame]->dwLength + 35;
        pSerialized->pFrames[iFrame] = (D2CmpRleCellHeader *)(uintptr_t)(pCursor - (uint8_t *)pSerialized);
        memcpy(pCursor, pFile->pFrames[iFrame], dwBytes);
        pCursor += dwBytes;

        const D2CmpRleCellHeader *pFrame = pFile->pFrames[iFrame];
        const D2CmpCelHardwareNode *pNode = (const D2CmpCelHardwareNode *)(uintptr_t)pFrame->dwNextBlockOrCellNode;
        if (pNode && pNode->pAuxData)
        {
            memcpy(pCursor, pNode->pAuxData, pFrame->unknown14);
            if (!iFrame)
                pFree = (void *)(uintptr_t)pFile->pFrames[0]->dwNextBlockOrCellNode;
        }

        pCopy->dwNextBlockOrCellNode = (uint32_t)(pCursor - (uint8_t *)pSerialized);
        pCursor += pFile->pFrames[iFrame]->unknown14;
    }

    if (pFree)
        FOG_Free(pFree, szCelCmpCpp, 885, 0);

    *ppSerializedFile = pSerialized;
    *pSerializedSize = nSize;
    return 1;
}

//1.10f: D2CMP.0x6FDF1EA0
//  #10024. Makes a serialized cel file (flag 1, asserted) a runtime one, in place; NULL is a no-op. The version must be 6 when nSpecVersion is -1
//  and nSpecVersion otherwise (asserted). A file that is already normalized (flag 2) is left as it is. Otherwise flag 2 is set, one zeroed block of
//  a 44-byte hardware node per frame comes from FOG_Alloc, every frame offset becomes a pointer, each frame's +0x18 takes its node, and *ppOutFile
//  takes the file. szFile and nLine (the caller's, by their names) and nUnused are never read.
//  BUG: *ppOutFile is written only on that last path; a NULL file or one already normalized leaves it as the caller had it (0x6FDF1EA7,
//  0x6FDF1F22). NOT ESTABLISHED: not certainly a defect, so left as the original has it.
//  BUG: FOG_Alloc's answer is used untested (0x6FDF1F49, then the zeroing). LATENT: only a failed allocation reaches it.
extern "C" void __stdcall CELCMP_CelFileNormalize(D2CmpBuiltCellFile *pFile, D2CmpBuiltCellFile **ppOutFile, const char *szFile, int32_t nLine, int32_t nSpecVersion, int32_t nUnused)
{
    if (!pFile)
        return;

    const uint32_t dwFlags = pFile->dwFlags;
    if (!(dwFlags & 1))
    {
        FOG_DisplayAssert("pFile->dwFlags & CELFILE_SERIALIZED", szCelCmpCpp, 918);
        exit(-1);
    }

    if (nSpecVersion == -1)
    {
        if (pFile->dwVersion != 6)
        {
            FOG_DisplayAssert("pFile->dwVersion == CELFILE_VERSION", szCelCmpCpp, 921);
            exit(-1);
        }
    }
    else if (pFile->dwVersion != (uint32_t)nSpecVersion)
    {
        FOG_DisplayAssert("pFile->dwVersion == (DWORD)nSpecVersion", szCelCmpCpp, 923);
        exit(-1);
    }

    if (dwFlags & 2)
        return;

    pFile->dwFlags = dwFlags | 2;
    const uint32_t nNodeBytes = 44 * (pFile->nFramesPerDirection * pFile->nDirections);
    D2CmpCelHardwareNode *pNode = (D2CmpCelHardwareNode *)FOG_Alloc((int32_t)nNodeBytes, szCelCmpCpp, 934, 0);
    memset(pNode, 0, nNodeBytes);
    for (uint32_t iFrame = 0; iFrame < pFile->nFramesPerDirection * pFile->nDirections; ++iFrame)
    {
        D2CmpRleCellHeader *pFrame = (D2CmpRleCellHeader *)((char *)pFile + (uintptr_t)pFile->pFrames[iFrame]);
        pFile->pFrames[iFrame] = pFrame;
        pFrame->dwNextBlockOrCellNode = (uint32_t)(uintptr_t)pNode;
        ++pNode;
    }

    *ppOutFile = pFile;
}

//1.10f: D2CMP.0x6FDF1FA0
//  #10028. Frees a serialized cel file block; a NULL block asserts.
extern "C" void __stdcall CELCMP_CelFileFreeSerialized(void *pSerializedFile)
{
    if (!pSerializedFile)
    {
        FOG_DisplayAssert("pSerializedFile", szCelCmpCpp, 1033);
        exit(-1);
    }

    FOG_Free(pSerializedFile, szCelCmpCpp, 1034, 0);
}

//1.10f: D2CMP.0x6FDF1FE0
//  #10027. Releases a cel file (a NULL file asserts). Only a file this module owns, flag 1 or 2, is freed; a normalized one (flag 2) has its
//  hardware nodes released first by CELCMP_CelFileFreeHardware. The flags are read once. void: no module of the stock install imports this export,
//  so nothing reads the original's eax.
extern "C" void __stdcall CELCMP_CelFileFree(D2CmpBuiltCellFile *pFile)
{
    if (!pFile)
    {
        FOG_DisplayAssert("pCelFile", szCelCmpCpp, 1047);
        exit(-1);
    }

    const uint32_t dwFlags = pFile->dwFlags;
    if (dwFlags & 3)
    {
        if (dwFlags & 2)
            CELCMP_CelFileFreeHardware(pFile);

        FOG_Free(pFile, szCelCmpCpp, 1054, 0);
    }
}

//1.10f: D2CMP.0x6FDF2030
//  #10031. Passes every frame of a cel file (a NULL file asserts) through its format's palette transform, gpfnCelTransform (FastCmp.cpp's for
//  format 0), with pPaletteMap; the frame count and the format are re-read each pass. void: no module of the stock install imports this export, so
//  nothing reads the original's eax.
//  BUG: a file of format 1 calls slot 1 of gpfnCelTransform, which is NULL (0x6FDF2075). UNREACHABLE: the stock game does not reach this function.
//  DIFFERENCE: the file's format is not bounded; past slot 1 the original reads the table that follows, ours finds NULL.
extern "C" void __stdcall CELCMP_CelFileTransform(D2CmpBuiltCellFile *pFile, const uint8_t *pPaletteMap)
{
    if (!pFile)
    {
        FOG_DisplayAssert("pCelFile", szCelCmpCpp, 1069);
        exit(-1);
    }

    for (uint32_t iFrame = 0; iFrame < pFile->nDirections * pFile->nFramesPerDirection; ++iFrame)
        CelCmpFormatSlot(gpfnCelTransform, pFile->nFormat)(pFile->pFrames[iFrame], pPaletteMap);
}

//1.10f: D2CMP.0x6FDF20A0
//  #10033. Draws the context's current frame with its bottom row at nY plus the frame's y offset. A cel file, when there is one, must be version 6
//  and indexed, and the surface and the frame must be present (all asserted). The height is cut to nRow + 1 when nRow - nHeight is negative (the
//  difference wraps, as the original's sub / jns, and only its sign is tested) and the frame is clipped to the surface's bottom (signed), counting
//  the lines dropped; with nClipLeft and nClipRight both 0 a negative left or bottom row draws nothing. The decoder is the file's format's (format
//  0 when there is no file) and must exist.
//  void: its callers, the four renderers, read no eax after it.
//  DIFFERENCE: the file's format is not bounded; past slot 1 of sgpfnDecodeFunc ours finds NULL and the assert on the decoder fires, where the
//  original calls whatever follows the table.
extern "C" void __stdcall CELCMP_CelDraw(D2GfxDataStrc *pContext, int32_t nX, int32_t nY, int32_t nClipLeft, int32_t nClipRight,
                                         uint8_t *pDestSurface, int32_t nSurfaceBottom, uint32_t dwStride,
                                         const uint8_t *pPaletteMap, const uint8_t *pBlendTable,
                                         const uint8_t *pPreMap)
{
    const D2CmpBuiltCellFile *pCellFile = pContext->pCellFile;
    if (pCellFile)
    {
        if (pCellFile->dwVersion != 6)
        {
            FOG_DisplayAssert("pCelFile->dwVersion == CELFILE_VERSION", szCelCmpCpp, 1102);
            exit(-1);
        }

        if (pCellFile->dwFlags & 4)
        {
            FOG_DisplayAssert("!( pCelFile->dwFlags & CELFILE_24BIT )", szCelCmpCpp, 1103);
            exit(-1);
        }
    }

    if (!pDestSurface)
    {
        FOG_DisplayAssert("pDestSurface", szCelCmpCpp, 1106);
        exit(-1);
    }

    const D2CmpRleCellHeader *pCel = pContext->pCurrentCell;
    if (!pCel)
    {
        FOG_DisplayAssert("pCel", szCelCmpCpp, 1109);
        exit(-1);
    }

    int32_t nRow = pCel->nYOffset + nY;
    const int32_t nLeft = pCel->nXOffset + nX;
    int32_t nHeight = (int32_t)pCel->dwHeight;
    int32_t nLinesToSkip = 0;
    if ((int32_t)((uint32_t)nRow - (uint32_t)nHeight) < 0)
        nHeight = nRow + 1;

    if (nHeight <= 0)
        return;

    if (nRow >= nSurfaceBottom)
    {
        nLinesToSkip = nRow - nSurfaceBottom + 1;
        nHeight -= nLinesToSkip;
        if (nHeight <= 0)
            return;

        nRow = nSurfaceBottom - 1;
    }

    if (!nClipLeft && !nClipRight && (nLeft < 0 || nRow < 0))
        return;

    uint8_t *pDst = pDestSurface + (uint32_t)(nRow * dwStride + nLeft);
    const D2CmpCelDecodeFunc pfnDecode = CelCmpFormatSlot(sgpfnDecodeFunc, pCellFile ? pCellFile->nFormat : 0);
    if (!pfnDecode)
    {
        FOG_DisplayAssert("sgpfnDecodeFunc[eFormat]", szCelCmpCpp, 1149);
        exit(-1);
    }

    pfnDecode(pCel, pDst, nLeft, nClipLeft, nClipRight, dwStride, nLinesToSkip, nHeight, pPaletteMap, pBlendTable, pPreMap);
}

//1.10f: D2CMP.0x6FDF2210
//  #10042. Copies the pixels of an RGB24 frame to pBuffer and returns 1: width * height pixels of three bytes, taken from after the frame's 32-byte
//  header. The file must be version 6 and RGB24 (asserted). The frame is at the 64-direction's row (the table, as #10018) times the frames per
//  direction, plus nFrame.
//  DIFFERENCE: as #10018, an index of 512 or more takes row 0 here.
extern "C" int32_t __stdcall CELCMP_CelGetRawData(D2CmpBuiltCellFile *pFile, uint32_t nDirection64, uint32_t nFrame, uint8_t *pBuffer)
{
    if (pFile->dwVersion != 6)
    {
        FOG_DisplayAssert("pCelFile->dwVersion == CELFILE_VERSION", szCelCmpCpp, 1180);
        exit(-1);
    }

    if (!(pFile->dwFlags & 4))
    {
        FOG_DisplayAssert("pCelFile->dwFlags & CELFILE_24BIT", szCelCmpCpp, 1181);
        exit(-1);
    }

    const uint32_t nRow = CelCmpDirection64Slot(64 * CELCMP_FindFirstSetBit1Based(pFile->nDirections) + nDirection64);
    const D2CmpRgb24FrameHeader *pFrame = (const D2CmpRgb24FrameHeader *)pFile->pFrames[pFile->nFramesPerDirection * nRow + nFrame];
    memcpy(pBuffer, pFrame + 1, 3 * (pFrame->dwHeight * pFrame->dwWidth));
    return 1;
}

//1.10f: D2CMP.0x6FDF22C0
//  #10034. CelDraw for a band of the frame: the caller gives the lines to skip from the bottom and the lines to draw, and the checks are CelDraw's.
//  The band's bottom is the frame's bottom less the lines skipped, its top that less the lines drawn. Nothing is drawn when the bottom is above the
//  surface or the top is at or past nSurfaceBottom (signed); the top is clamped to 0, the bottom to the last line, and an empty band draws nothing.
//  The decoder must exist and is told how far the bottom moved. void: its callers, the renderers, read no eax after it.
//  BUG: the decoder is taken from the cel file without the NULL test CelDraw makes; a context with no file reads its format at 0x08 (0x6FDF23A2)
//  and faults, as ours does. NOT ESTABLISHED: whether a context with no file reaches it is not settled.
//  DIFFERENCE: as in CelDraw, the file's format is not bounded; past slot 1 of sgpfnDecodeFunc ours finds NULL.
extern "C" void __stdcall CELCMP_CelDrawEx(D2GfxDataStrc *pContext, int32_t nX, int32_t nY, int32_t nClipLeft, int32_t nClipRight,
                                           uint8_t *pDestSurface, int32_t nSurfaceBottom, uint32_t dwStride,
                                           const uint8_t *pPaletteMap, const uint8_t *pBlendTable,
                                           int32_t nLinesToSkip, int32_t nLinesToDraw, const uint8_t *pPreMap)
{
    const D2CmpBuiltCellFile *pCellFile = pContext->pCellFile;
    if (pCellFile)
    {
        if (pCellFile->dwVersion != 6)
        {
            FOG_DisplayAssert("pCelFile->dwVersion == CELFILE_VERSION", szCelCmpCpp, 1218);
            exit(-1);
        }

        if (pCellFile->dwFlags & 4)
        {
            FOG_DisplayAssert("!( pCelFile->dwFlags & CELFILE_24BIT )", szCelCmpCpp, 1219);
            exit(-1);
        }
    }

    if (!pDestSurface)
    {
        FOG_DisplayAssert("pDestSurface", szCelCmpCpp, 1222);
        exit(-1);
    }

    const D2CmpRleCellHeader *pCel = pContext->pCurrentCell;
    if (!pCel)
    {
        FOG_DisplayAssert("pCel", szCelCmpCpp, 1225);
        exit(-1);
    }

    const int32_t nLeft = pCel->nXOffset + nX;
    const int32_t nFrameBottom = pCel->nYOffset + nY;
    int32_t nBottom = nFrameBottom - nLinesToSkip;
    int32_t nTop = nBottom - nLinesToDraw;
    if (nBottom < 0 || nTop >= nSurfaceBottom)
        return;

    if (nTop < 0)
        nTop = 0;

    if (nBottom >= nSurfaceBottom)
        nBottom = nSurfaceBottom - 1;

    const int32_t nLines = nBottom - nTop;
    if (!nLines)
        return;

    const D2CmpCelDecodeFunc pfnDecode = CelCmpFormatSlot(sgpfnDecodeFunc, pCellFile->nFormat);
    if (!pfnDecode)
    {
        FOG_DisplayAssert("sgpfnDecodeFunc[pCelFile->format]", szCelCmpCpp, 1253);
        exit(-1);
    }

    pfnDecode(pCel, pDestSurface + (uint32_t)(nBottom * dwStride + nLeft), nLeft, nClipLeft, nClipRight, dwStride, nFrameBottom - nBottom, nLines, pPaletteMap, pBlendTable, pPreMap);
}

//1.10f: D2CMP.0x6FDF2410
//  #10035. Draws the current frame as a shadow, squashed to half its height and sheared; the checks are CelDraw's. The frame's y offset halved (a
//  signed divide, toward zero) moves both the bottom row and the left. The height is halved with an unsigned shift but clipped against the frame's
//  top with the signed half (dwHeight / 2); the frame is clipped to the surface's bottom as in CelDraw, and the left is moved back by the lines
//  dropped. FASTCMP_DrawDC6Shadow draws it.
extern "C" void __stdcall CELCMP_CelDrawShadow(D2GfxDataStrc *pContext, int32_t nX, int32_t nY, int32_t nClipLeft, int32_t nClipRight,
                                               uint8_t *pDestSurface, int32_t nSurfaceBottom,
                                               uint32_t dwStride, const uint8_t *pBlendTable,
                                               uint8_t nShadowColor)
{
    const D2CmpBuiltCellFile *pCellFile = pContext->pCellFile;
    if (pCellFile)
    {
        if (pCellFile->dwVersion != 6)
        {
            FOG_DisplayAssert("pCelFile->dwVersion == CELFILE_VERSION", szCelCmpCpp, 1289);
            exit(-1);
        }

        if (pCellFile->dwFlags & 4)
        {
            FOG_DisplayAssert("!( pCelFile->dwFlags & CELFILE_24BIT )", szCelCmpCpp, 1290);
            exit(-1);
        }
    }

    if (!pDestSurface)
    {
        FOG_DisplayAssert("pDestSurface", szCelCmpCpp, 1293);
        exit(-1);
    }

    const D2CmpRleCellHeader *pCel = pContext->pCurrentCell;
    if (!pCel)
    {
        FOG_DisplayAssert("pCel", szCelCmpCpp, 1296);
        exit(-1);
    }

    const int32_t nHalfOffsetY = pCel->nYOffset / 2;
    int32_t nRow = nY + nHalfOffsetY;
    int32_t nLeft = nX + (pCel->nXOffset + nHalfOffsetY);
    int32_t nHeight = (int32_t)(pCel->dwHeight >> 1);
    int32_t nLinesToSkip = 0;
    if ((int32_t)((uint32_t)nRow - (uint32_t)((int32_t)pCel->dwHeight / 2)) < 0)
        nHeight = nRow + 1;

    if (nHeight <= 0)
        return;

    if (nRow >= nSurfaceBottom)
    {
        nLinesToSkip = nRow - nSurfaceBottom + 1;
        nHeight -= nLinesToSkip;
        if (nHeight <= 0)
            return;

        nRow = nSurfaceBottom - 1;
    }

    nLeft -= nLinesToSkip;
    FASTCMP_DrawDC6Shadow(pCel, pDestSurface + (uint32_t)(nRow * dwStride + nLeft), nLeft, nClipLeft, nClipRight, dwStride, nLinesToSkip, nHeight, pBlendTable, nShadowColor);
}

//1.10f: D2CMP.0x6FDF2540
//  #10036. Returns the context's frame. The context must be present and valid: a present version-6 file, a direction below 64 and a frame not above
//  the frames per direction (all asserted). The frame is the direction's row (the table, as #10018) times the frames per direction, plus the frame.
//  BUG: the validity test lets a frame EQUAL to the frames per direction through, which reads the next row's first frame or past the slots. NOT
//  ESTABLISHED: whether any caller passes such a frame is not settled.
//  DIFFERENCE: as #10018, an index of 512 or more takes row 0.
extern "C" D2CmpRleCellHeader *__stdcall CELCMP_CelGetHandle(D2GfxDataStrc *pContext)
{
    if (!pContext)
    {
        FOG_DisplayAssert("pCelContext", szCelCmpCpp, 1378);
        exit(-1);
    }

    D2CmpBuiltCellFile *pCellFile = pContext->pCellFile;
    if (!pCellFile || pCellFile->dwVersion != 6 || pContext->nDirection >= 64 || pContext->nFrame > pCellFile->nFramesPerDirection)
    {
        FOG_DisplayAssert("ValidCelContext( pCelContext )", szCelCmpCpp, 1379);
        exit(-1);
    }

    const uint32_t nRow = CelCmpDirection64Slot(64 * CELCMP_FindFirstSetBit1Based(pCellFile->nDirections) + pContext->nDirection);
    return pCellFile->pFrames[pCellFile->nFramesPerDirection * nRow + pContext->nFrame];
}

//1.10f: D2CMP.0x6FDF25E0
//  #10037. A frame's width; a NULL frame asserts.
extern "C" int32_t __stdcall CELCMP_CelGetWidth(const D2CmpRleCellHeader *pCel)
{
    if (!pCel)
    {
        FOG_DisplayAssert("pCel", szCelCmpCpp, 1399);
        exit(-1);
    }

    return (int32_t)pCel->dwWidth;
}

//1.10f: D2CMP.0x6FDF2610
//  #10038. A frame's height; a NULL frame asserts.
extern "C" int32_t __stdcall CELCMP_CelGetHeight(const D2CmpRleCellHeader *pCel)
{
    if (!pCel)
    {
        FOG_DisplayAssert("pCel", szCelCmpCpp, 1413);
        exit(-1);
    }

    return (int32_t)pCel->dwHeight;
}

//1.10f: D2CMP.0x6FDF2640
//  #10039. A frame's x offset; a NULL frame asserts.
extern "C" int32_t __stdcall CELCMP_CelGetOffsetX(const D2CmpRleCellHeader *pCel)
{
    if (!pCel)
    {
        FOG_DisplayAssert("pCel", szCelCmpCpp, 1427);
        exit(-1);
    }

    return pCel->nXOffset;
}

//1.10f: D2CMP.0x6FDF2670
//  #10040. A frame's y offset; a NULL frame asserts.
extern "C" int32_t __stdcall CELCMP_CelGetOffsetY(const D2CmpRleCellHeader *pCel)
{
    if (!pCel)
    {
        FOG_DisplayAssert("pCel", szCelCmpCpp, 1441);
        exit(-1);
    }

    return pCel->nYOffset;
}

//1.10f: D2CMP.0x6FDF26A0
//  #10041. The frame's encoded stream, which follows its 32-byte header; a NULL frame asserts.
extern "C" uint8_t *__stdcall CELCMP_CelGetControlData(D2CmpRleCellHeader *pCel)
{
    if (!pCel)
    {
        FOG_DisplayAssert("ptCel", szCelCmpCpp, 1456);
        exit(-1);
    }

    return (uint8_t *)(pCel + 1);
}

//1.10f: D2CMP.0x6FDF26D0
//  #10047. A cel file's direction count; asserts version 6.
extern "C" int32_t __stdcall CELCMP_CelFileGetNumDirections(D2CmpBuiltCellFile *pFile)
{
    if (pFile->dwVersion != 6)
    {
        FOG_DisplayAssert("pFile->dwVersion == CELFILE_VERSION", szCelCmpCpp, 1470);
        exit(-1);
    }

    return (int32_t)pFile->nDirections;
}

//1.10f: D2CMP.0x6FDF2700
//  #10046. A cel file's frames per direction; asserts version 6.
extern "C" int32_t __stdcall CELCMP_CelFileGetCelsPerDirection(D2CmpBuiltCellFile *pCelFile)
{
    if (pCelFile->dwVersion != 6)
    {
        FOG_DisplayAssert("pFile->dwVersion == CELFILE_VERSION", szCelCmpCpp, 1485);
        exit(-1);
    }

    return (int32_t)pCelFile->nFramesPerDirection;
}

//1.10f: D2CMP.0x6FDF2730
//  #10044. Stores the renderer's hook for freeing a frame's hardware node, and returns it.
extern "C" D2CmpCelHardwareFreeCallback __stdcall CELCMP_CelSetHardwareFreeCallback(D2CmpCelHardwareFreeCallback pfnHardwareFree)
{
    gCelHardwareFreeCallback = pfnHardwareFree;
    return pfnHardwareFree;
}

//1.10f: D2CMP.0x6FDF2740
//  #10045. Returns the hook stored by #10044.
extern "C" D2CmpCelHardwareFreeCallback __cdecl CELCMP_CelGetHardwareFreeCallback()
{
    return gCelHardwareFreeCallback;
}

//1.10f: D2CMP.0x6FDF2750
//  #10032. Releases a normalized file's hardware nodes: returns 0 for NULL, else 1. The file must be version 6 and frame 0's node (the start of the
//  block CELCMP_CelFileNormalize allocated for all of them) must be there, both asserted. Every frame with a node hands it to the hook when one is
//  set and then clears it, re-reading the frame count each pass; finally the block is freed.
//  BUG: frame 0 is read without a NULL test (0x6FDF2784..0x6FDF278A), where the loop tests every frame (0x6FDF27C3). NOT ESTABLISHED: a normalized
//  file's frame 0 is never NULL, so only a file of no frames would read past its table, and whether one reaches this function is not settled.
extern "C" BOOL __stdcall CELCMP_CelFileFreeHardware(D2CmpBuiltCellFile *pFile)
{
    if (!pFile)
        return FALSE;

    if (pFile->dwVersion != 6)
    {
        FOG_DisplayAssert("ptFile->dwVersion == CELFILE_VERSION", szCelCmpCpp, 1517);
        exit(-1);
    }

    void *lpParent = (void *)(uintptr_t)pFile->pFrames[0]->dwNextBlockOrCellNode;
    if (!lpParent)
    {
        FOG_DisplayAssert("lpParent", szCelCmpCpp, 1521);
        exit(-1);
    }

    for (uint32_t iFrame = 0; iFrame < pFile->nFramesPerDirection * pFile->nDirections; ++iFrame)
    {
        D2CmpRleCellHeader *pFrame = pFile->pFrames[iFrame];
        if (pFrame && pFrame->dwNextBlockOrCellNode)
        {
            if (gCelHardwareFreeCallback)
                gCelHardwareFreeCallback((void *)(uintptr_t)pFrame->dwNextBlockOrCellNode);

            pFile->pFrames[iFrame]->dwNextBlockOrCellNode = 0;
        }
    }

    FOG_Free(lpParent, szCelCmpCpp, 1561, 0);
    return TRUE;
}

//1.10f: D2CMP.0x6FDF2820
//  #10043. A frame's hardware node (its +0x18); a NULL frame asserts.
extern "C" void *__stdcall CELCMP_CelGetHardwareData(const D2CmpRleCellHeader *pCel)
{
    if (!pCel)
    {
        FOG_DisplayAssert("ptCel", szCelCmpCpp, 1574);
        exit(-1);
    }

    return (void *)(uintptr_t)pCel->dwNextBlockOrCellNode;
}

//1.10f: D2CMP.0x6FDF2850
//  TSList<TCELENTRY>::NewNode. Allocates an entry of 36 bytes plus nExtraBytes from Storm (the flags or'ed with 8) with its link cleared. A nonzero
//  nLocation unlinks the entry if it is linked and puts it at the head of the list (1) or at the tail (any other value); 0 only allocates. At the
//  head, the old first node's link is found from the list's offset or, when that is negative, from the terminator's previous link. The original
//  inlines CELCMP_StormList_GetEmbeddedLink and CELCMP_StormLink_Unlink here.
D2CmpCellBuildEntry *__fastcall CELCMP_CellEntryList_NewNode(D2CmpStormList *pList, int32_t nEdxUnused, uint32_t nLocation, uint32_t nExtraBytes, uint32_t dwAllocFlags)
{
    D2CmpCellBuildEntry *pNewEntry = (D2CmpCellBuildEntry *)STORM_SMemAlloc(nExtraBytes + sizeof(D2CmpCellBuildEntry), szCelEntryTag, -2, dwAllocFlags | 8);
    if (pNewEntry)
    {
        pNewEntry->link.pPrevLink = nullptr;
        pNewEntry->link.pNextOrSentinel = nullptr;
    }

    if (nLocation)
    {
        D2CmpStormLink *pNodeLink = CELCMP_StormList_GetEmbeddedLink(pList, 0, pNewEntry);
        CELCMP_StormLink_Unlink(pNodeLink);
        D2CmpStormLink *pTerminator = &pList->terminator;
        if (nLocation == 1)
        {
            pNodeLink->pPrevLink = pTerminator;
            pNodeLink->pNextOrSentinel = pTerminator->pNextOrSentinel;
            const intptr_t nHead = (intptr_t)pTerminator->pNextOrSentinel;
            intptr_t nLinkOffset = pList->nLinkOffset;
            if (nHead < 0)
            {
                ((D2CmpStormLink *)~nHead)->pPrevLink = pNodeLink;
            }
            else
            {
                if (nLinkOffset < 0)
                    nLinkOffset = (intptr_t)((uintptr_t)pTerminator - (uintptr_t)pTerminator->pPrevLink->pNextOrSentinel);

                ((D2CmpStormLink *)(nHead + nLinkOffset))->pPrevLink = pNodeLink;
            }

            pTerminator->pNextOrSentinel = pNewEntry;
        }
        else
        {
            D2CmpStormLink *pTailLink = pTerminator->pPrevLink;
            pNodeLink->pPrevLink = pTailLink;
            pNodeLink->pNextOrSentinel = pTailLink->pNextOrSentinel;
            pTailLink->pNextOrSentinel = pNewEntry;
            pTerminator->pPrevLink = pNodeLink;
        }
    }

    return pNewEntry;
}

//1.10f: D2CMP.0x6FDF2A60
//  TSHashTableReuse::Clear, slot 3 of gVtblCellBuilderTableReuse and gVtblCellBuilderTableSync: sets the fullness to 0, unlinks every node of the
//  full list, hands every builder of every bucket to the vtable's InternalDelete, destroys every object chunk and returns it to Storm, and destroys
//  every builder on the reuse list and returns it.
void __fastcall CELCMP_CellBuilderTable_ClearObjects(D2CmpCellBuilderTableHeader *pTable)
{
    D2CmpCellBuilderTableReusePrefix *pReuseTable = (D2CmpCellBuilderTableReusePrefix *)pTable;
    pTable->nFullness = 0;
    while ((intptr_t)pTable->fullList.terminator.pNextOrSentinel > 0)
        CELCMP_StormLink_Unlink((D2CmpStormLink *)((char *)pTable->fullList.terminator.pNextOrSentinel + pTable->fullList.nLinkOffset));

    for (uint32_t iBucket = 0; iBucket < pTable->nBucketCount; ++iBucket)
    {
        while ((intptr_t)pTable->pBuckets[iBucket].terminator.pNextOrSentinel > 0)
            pTable->pVtable->FreeBuilder(pTable, 0, (D2CmpCellBuilder *)pTable->pBuckets[iBucket].terminator.pNextOrSentinel);
    }

    D2CmpCellBuilderObjectChunk *pChunk;
    while ((intptr_t)(pChunk = (D2CmpCellBuilderObjectChunk *)pReuseTable->chunkList.terminator.pNextOrSentinel) > 0)
    {
        CELCMP_CellBuilderObjectChunk_Destroy(pChunk);
        STORM_SMemFree(pChunk, szCellListChunkTag, -2, 0);
    }

    D2CmpCellBuilder *pReuseBuilder;
    while ((intptr_t)(pReuseBuilder = (D2CmpCellBuilder *)pReuseTable->reuseList.terminator.pNextOrSentinel) > 0)
    {
        CELCMP_CellBuilder_Destroy(pReuseBuilder);
        STORM_SMemFree(pReuseBuilder, szCellListTag, -2, 0);
    }
}

//1.10f: D2CMP.0x6FDF2AF0
//  Slot 0 of the four vtables: destroys a builder and returns it to Storm. The table (pUnusedTable) is not read.
void __fastcall CELCMP_CellBuilderTable_InternalDelete(D2CmpCellBuilderTableHeader *pUnusedTable, int32_t nEdxUnused, D2CmpCellBuilder *pBuilder)
{
    CELCMP_CellBuilder_Destroy(pBuilder);
    STORM_SMemFree(pBuilder, szCellListTag, -2, 0);
}

//1.10f: D2CMP.0x6FDF2B10
//  Slot 1 of the four vtables: a new builder at the head of its bucket. The table (pUnusedTable) is not read.
D2CmpCellBuilder *__fastcall CELCMP_CellBuilderTable_InternalNew(D2CmpCellBuilderTableHeader *pUnusedTable, int32_t nEdxUnused, D2CmpStormList *pBucket, uint32_t nExtraBytes, uint32_t dwFlags)
{
    return CELCMP_CellBuilderList_NewNode(pBucket, 0, 1, nExtraBytes, dwFlags);
}

//1.10f: D2CMP.0x6FDF2B30
//  ~TSHashTable, reached through CELCMP_CellBuilderTable_BaseScalarDeletingDestructor: sets the base vtable and the fullness to 0, unlinks the full
//  list's nodes and every bucket's nodes, destroys the bucket array, unlinks the full list's nodes again (none are left) and then its terminator.
//  The original inlines CELCMP_StormLink_Unlink and CELCMP_StormListArray_Destroy here.
void __fastcall CELCMP_CellBuilderTable_BaseDestroy(D2CmpCellBuilderTableHeader *pTable)
{
    pTable->pVtable = &gVtblCellBuilderTableBase;
    pTable->nFullness = 0;
    D2CmpStormList *pFullList = &pTable->fullList;
    while ((intptr_t)pFullList->terminator.pNextOrSentinel > 0)
        CELCMP_StormLink_Unlink(CELCMP_StormList_GetLink(pFullList, 0, pFullList->terminator.pNextOrSentinel));

    for (uint32_t iBucket = 0; iBucket < pTable->nBucketCount; ++iBucket)
    {
        D2CmpStormList *pBucket;
        while ((intptr_t)(pBucket = &pTable->pBuckets[iBucket])->terminator.pNextOrSentinel > 0)
            CELCMP_StormLink_Unlink((D2CmpStormLink *)((char *)pBucket->terminator.pNextOrSentinel + pBucket->nLinkOffset));
    }

    CELCMP_StormListArray_Destroy((D2CmpStormListArray *)&pTable->nBucketCapacity);

    while ((intptr_t)pFullList->terminator.pNextOrSentinel > 0)
    {
        D2CmpStormLink *pLink = (D2CmpStormLink *)((char *)pFullList->terminator.pNextOrSentinel + pFullList->nLinkOffset);
        if (pLink->pPrevLink)
        {
            CELCMP_StormLink_GetNextLink(pLink, 0, -1)->pPrevLink = pLink->pPrevLink;
            pLink->pPrevLink->pNextOrSentinel = pLink->pNextOrSentinel;
            pLink->pPrevLink = nullptr;
            pLink->pNextOrSentinel = nullptr;
        }
    }

    CELCMP_StormLink_Unlink(&pFullList->terminator);
}

//1.10f: D2CMP.0x6FDF2CB0
//  TSHashTable::Clear-and-release, slot 3 of gVtblCellBuilderTableBase: unlinks the full list's nodes and every bucket's nodes, sets the fullness
//  and the mask to 0, destroys every bucket list (its scalar deleting destructor, flags 0), returns the buckets' storage to Storm and empties the
//  array. The original inlines CELCMP_StormLink_Unlink here.
void __fastcall CELCMP_CellBuilderTable_ResetAndReleaseBuckets(D2CmpCellBuilderTableHeader *pTable)
{
    pTable->nFullness = 0;
    while ((intptr_t)pTable->fullList.terminator.pNextOrSentinel > 0)
        CELCMP_StormLink_Unlink((D2CmpStormLink *)((char *)pTable->fullList.terminator.pNextOrSentinel + pTable->fullList.nLinkOffset));

    for (uint32_t iBucket = 0; iBucket < pTable->nBucketCount; ++iBucket)
    {
        D2CmpStormList *pBucket;
        while ((intptr_t)(pBucket = &pTable->pBuckets[iBucket])->terminator.pNextOrSentinel > 0)
            CELCMP_StormLink_Unlink((D2CmpStormLink *)((char *)pBucket->terminator.pNextOrSentinel + pBucket->nLinkOffset));
    }

    pTable->nFullness = 0;
    pTable->dwBucketMask = 0;
    for (uint32_t iBucket = 0; iBucket < pTable->nBucketCount; ++iBucket)
        CELCMP_StormList_ScalarDeletingDestructor(&pTable->pBuckets[iBucket], 0, 0);

    if (pTable->pBuckets)
        STORM_SMemFree(pTable->pBuckets, szCellListBucketsTag, -2, 0);

    pTable->nBucketCapacity = 0;
    pTable->nBucketCount = 0;
    pTable->pBuckets = nullptr;
}

//1.10f: D2CMP.0x6FDF2D90
//  Slot 2 of gVtblCellBuilderTableSync: SyncDestroy, then operator delete when bit 0 of nFlags is set. The original inlines operator delete here.
D2CmpCellBuilderTableHeader *__fastcall CELCMP_CellBuilderTable_SyncScalarDeletingDestructor(D2CmpCellBuilderTableHeader *pTable, int32_t nEdxUnused, uint8_t nFlags)
{
    CELCMP_CellBuilderTable_SyncDestroy((D2CmpCellBuilderTablePrefix *)pTable);
    if ((nFlags & 1) && pTable)
        STORM_SMemFree(pTable, szDeleteTag, -1, 0);

    return pTable;
}

//1.10f: D2CMP.0x6FDF2DC0
//  ~CELLIST's scalar deleting destructor: CellBuilder_Destroy, then operator delete when bit 0 of nFlags is set. ExportDestroy calls it with flags
//  0, so there it only destroys. The original inlines operator delete here.
D2CmpCellBuilder *__fastcall CELCMP_CellBuilder_ScalarDeletingDestructor(D2CmpCellBuilder *pBuilder, int32_t nEdxUnused, uint8_t nFlags)
{
    CELCMP_CellBuilder_Destroy(pBuilder);
    if ((nFlags & 1) && pBuilder)
        STORM_SMemFree(pBuilder, szDeleteTag, -1, 0);

    return pBuilder;
}

//1.10f: D2CMP.0x6FDF2DF0
//  Slot 2 of gVtblCellBuilderTableReuse: ReuseDestroy, then operator delete when bit 0 of nFlags is set. The original inlines operator delete here.
D2CmpCellBuilderTableHeader *__fastcall CELCMP_CellBuilderTable_ReuseScalarDeletingDestructor(D2CmpCellBuilderTableHeader *pTable, int32_t nEdxUnused, uint8_t nFlags)
{
    CELCMP_CellBuilderTable_ReuseDestroy((D2CmpCellBuilderTableReusePrefix *)pTable);
    if ((nFlags & 1) && pTable)
        STORM_SMemFree(pTable, szDeleteTag, -1, 0);

    return pTable;
}

//1.10f: D2CMP.0x6FDF2E20
//  ~TSHashTableReuse: sets its own vtable, destroys every object chunk and returns it to Storm, destroys every reuse-list builder and returns it,
//  unlinks the chunk list's and the reuse list's nodes and terminators, then runs ~TSHashTable (BaseDestroy) in place. The original inlines
//  CELCMP_StormLink_Unlink and CELCMP_CellBuilderTable_BaseDestroy here.
void __fastcall CELCMP_CellBuilderTable_ReuseDestroy(D2CmpCellBuilderTableReusePrefix *pTable)
{
    pTable->header.pVtable = &gVtblCellBuilderTableReuse;

    D2CmpCellBuilderObjectChunk *pChunk;
    while ((intptr_t)(pChunk = (D2CmpCellBuilderObjectChunk *)pTable->chunkList.terminator.pNextOrSentinel) > 0)
    {
        CELCMP_CellBuilderObjectChunk_Destroy(pChunk);
        STORM_SMemFree(pChunk, szCellListChunkTag, -2, 0);
    }

    D2CmpCellBuilder *pReuseBuilder;
    while ((intptr_t)(pReuseBuilder = (D2CmpCellBuilder *)pTable->reuseList.terminator.pNextOrSentinel) > 0)
    {
        CELCMP_CellBuilder_Destroy(pReuseBuilder);
        STORM_SMemFree(pReuseBuilder, szCellListTag, -2, 0);
    }

    while ((intptr_t)pTable->chunkList.terminator.pNextOrSentinel > 0)
        CELCMP_StormLink_Unlink((D2CmpStormLink *)((char *)pTable->chunkList.terminator.pNextOrSentinel + pTable->chunkList.nLinkOffset));

    if (pTable->chunkList.terminator.pPrevLink)
    {
        CELCMP_StormLink_GetNextLink(&pTable->chunkList.terminator, 0, -1)->pPrevLink = pTable->chunkList.terminator.pPrevLink;
        pTable->chunkList.terminator.pPrevLink->pNextOrSentinel = pTable->chunkList.terminator.pNextOrSentinel;
        pTable->chunkList.terminator.pPrevLink = nullptr;
        pTable->chunkList.terminator.pNextOrSentinel = nullptr;
    }

    while ((intptr_t)pTable->reuseList.terminator.pNextOrSentinel > 0)
        CELCMP_StormLink_Unlink((D2CmpStormLink *)((char *)pTable->reuseList.terminator.pNextOrSentinel + pTable->reuseList.nLinkOffset));

    if (pTable->reuseList.terminator.pPrevLink)
    {
        CELCMP_StormLink_GetNextLink(&pTable->reuseList.terminator, 0, -1)->pPrevLink = pTable->reuseList.terminator.pPrevLink;
        pTable->reuseList.terminator.pPrevLink->pNextOrSentinel = pTable->reuseList.terminator.pNextOrSentinel;
        pTable->reuseList.terminator.pPrevLink = nullptr;
        pTable->reuseList.terminator.pNextOrSentinel = nullptr;
    }

    pTable->header.pVtable = &gVtblCellBuilderTableBase;
    pTable->header.nFullness = 0;
    CELCMP_StormList_UnlinkAll(&pTable->header.fullList);
    for (uint32_t iBucket = 0; iBucket < pTable->header.nBucketCount; ++iBucket)
    {
        D2CmpStormList *pBucket;
        while ((intptr_t)(pBucket = &pTable->header.pBuckets[iBucket])->terminator.pNextOrSentinel > 0)
            CELCMP_StormLink_Unlink(CELCMP_StormList_GetLink(pBucket, 0, pBucket->terminator.pNextOrSentinel));
    }

    CELCMP_StormListArray_Destroy((D2CmpStormListArray *)&pTable->header.nBucketCapacity);
    CELCMP_StormList_UnlinkAll(&pTable->header.fullList);
    CELCMP_StormLink_Unlink(&pTable->header.fullList.terminator);
}

//1.10f: D2CMP.0x6FDF2FB0
//  ~TSList: unlinks every node, then the terminator. The original inlines CELCMP_StormList_UnlinkAll and CELCMP_StormLink_Unlink here.
void __fastcall CELCMP_StormList_UnlinkAllAndTerminator(D2CmpStormList *pList)
{
    CELCMP_StormList_UnlinkAll(pList);
    CELCMP_StormLink_Unlink(&pList->terminator);
}

//1.10f: D2CMP.0x6FDF3030
//  Slot 2 of gVtblCellBuilderTableBase: BaseDestroy, then operator delete when bit 0 of nFlags is set. The original inlines operator delete here.
D2CmpCellBuilderTableHeader *__fastcall CELCMP_CellBuilderTable_BaseScalarDeletingDestructor(D2CmpCellBuilderTableHeader *pTable, int32_t nEdxUnused, uint8_t nFlags)
{
    CELCMP_CellBuilderTable_BaseDestroy(pTable);
    if ((nFlags & 1) && pTable)
        STORM_SMemFree(pTable, szDeleteTag, -1, 0);

    return pTable;
}

//1.10f: D2CMP.0x6FDF3060
//  Destroys an array of lists: each list's nodes are unlinked (through the explicit list's link, node + offset) and then its terminator, and the
//  storage is returned to Storm. Its callers are the compiler's unwind funclets at the end of .text; nothing of ours calls it.
void __fastcall CELCMP_StormListArray_DestroyWithUnwind(D2CmpStormListArray *pArray)
{
    for (uint32_t iList = 0; iList < pArray->dwCount; ++iList)
    {
        D2CmpStormList *pList = &pArray->pData[iList];
        while ((intptr_t)pList->terminator.pNextOrSentinel > 0)
            CELCMP_StormLink_Unlink(CELCMP_StormExplicitList_GetLink(pList->terminator.pNextOrSentinel, pList->nLinkOffset));

        CELCMP_StormLink_Unlink(&pList->terminator);
    }

    if (pArray->pData)
        STORM_SMemFree(pArray->pData, szCellListBucketsTag, -2, 0);
}

//1.10f: D2CMP.0x6FDF3110
//  TSLink::Unlink. Takes a linked link (previous link set) out of its list and clears it. The next link is the complement of pNextOrSentinel when
//  that is negative (the terminator), else this link's address less the previous link's next node plus this link's next node (a signed test).
void __fastcall CELCMP_StormLink_Unlink(D2CmpStormLink *pLink)
{
    D2CmpStormLink *pPrevLink = pLink->pPrevLink;
    if (pPrevLink)
    {
        const intptr_t nNext = (intptr_t)pLink->pNextOrSentinel;
        D2CmpStormLink *pNextLink = nNext < 0 ? (D2CmpStormLink *)~nNext
                                              : (D2CmpStormLink *)((uintptr_t)pLink - (uintptr_t)pPrevLink->pNextOrSentinel + (uintptr_t)nNext);
        pNextLink->pPrevLink = pPrevLink;
        pLink->pPrevLink->pNextOrSentinel = pLink->pNextOrSentinel;
        pLink->pPrevLink = nullptr;
        pLink->pNextOrSentinel = nullptr;
    }
}

//1.10f: D2CMP.0x6FDF3150
//  ~CELLIST: unlinks the frame list's nodes and terminator, then the builder's full and slot links. The original inlines
//  CELCMP_StormList_UnlinkAllAndTerminator, CELCMP_StormLink_Unlink and CELCMP_CellBuilder_UnlinkHashLinks here.
void __fastcall CELCMP_CellBuilder_Destroy(D2CmpCellBuilder *pBuilder)
{
    while ((intptr_t)pBuilder->frames.terminator.pNextOrSentinel > 0)
        CELCMP_StormLink_Unlink((D2CmpStormLink *)pBuilder->frames.terminator.pNextOrSentinel);

    CELCMP_StormLink_Unlink(&pBuilder->frames.terminator);
    CELCMP_CellBuilder_UnlinkHashLinks(pBuilder);
}

//1.10f: D2CMP.0x6FDF3240
//  TSList<CELLIST>::NewNode, for a bucket: allocates a builder of 56 bytes plus nExtraBytes from Storm (the flags or'ed with 8), clears its two
//  links, makes its frame list empty with offset 0, then places it as CELCMP_CellEntryList_NewNode places an entry, with its link at the list's
//  offset inside it. The original inlines CELCMP_StormList_GetLink and CELCMP_StormLink_Unlink here.
D2CmpCellBuilder *__fastcall CELCMP_CellBuilderList_NewNode(D2CmpStormList *pList, int32_t nEdxUnused, uint32_t dwLocation, uint32_t nExtraBytes, uint32_t dwFlags)
{
    D2CmpCellBuilder *pBuilder = (D2CmpCellBuilder *)STORM_SMemAlloc(nExtraBytes + sizeof(D2CmpCellBuilder), szCellListTag, -2, dwFlags | 8);
    if (pBuilder)
    {
        pBuilder->slotLink.pPrevLink = nullptr;
        pBuilder->slotLink.pNextOrSentinel = nullptr;
        pBuilder->fullLink.pPrevLink = nullptr;
        pBuilder->fullLink.pNextOrSentinel = nullptr;
        pBuilder->frames.terminator.pPrevLink = nullptr;
        pBuilder->frames.terminator.pNextOrSentinel = nullptr;
        pBuilder->frames.terminator.pPrevLink = &pBuilder->frames.terminator;
        pBuilder->frames.nLinkOffset = 0;
        pBuilder->frames.terminator.pNextOrSentinel = (void *)~(uintptr_t)&pBuilder->frames.terminator;
    }

    if (dwLocation)
    {
        D2CmpStormLink *pNodeLink = CELCMP_StormList_GetLink(pList, 0, pBuilder);
        CELCMP_StormLink_Unlink(pNodeLink);
        D2CmpStormLink *pTerminator = &pList->terminator;
        if (dwLocation == 1)
        {
            pNodeLink->pPrevLink = pTerminator;
            pNodeLink->pNextOrSentinel = pTerminator->pNextOrSentinel;
            const intptr_t nHead = (intptr_t)pTerminator->pNextOrSentinel;
            intptr_t nLinkOffset = pList->nLinkOffset;
            if (nHead < 0)
            {
                ((D2CmpStormLink *)~nHead)->pPrevLink = pNodeLink;
            }
            else
            {
                if (nLinkOffset < 0)
                    nLinkOffset = (intptr_t)((uintptr_t)pTerminator - (uintptr_t)pTerminator->pPrevLink->pNextOrSentinel);

                ((D2CmpStormLink *)(nHead + nLinkOffset))->pPrevLink = pNodeLink;
            }

            pTerminator->pNextOrSentinel = pBuilder;
        }
        else
        {
            D2CmpStormLink *pTailLink = pTerminator->pPrevLink;
            pNodeLink->pPrevLink = pTailLink;
            pNodeLink->pNextOrSentinel = pTailLink->pNextOrSentinel;
            pTailLink->pNextOrSentinel = pBuilder;
            pTerminator->pPrevLink = pNodeLink;
        }
    }

    return pBuilder;
}

//1.10f: D2CMP.0x6FDF3330
//  Destroys an array of lists: each list's ~TSList, then the storage is returned to Storm; the count is re-read each pass.
void __fastcall CELCMP_StormListArray_Destroy(D2CmpStormListArray *pArray)
{
    for (uint32_t iList = 0; iList < pArray->dwCount; ++iList)
        CELCMP_StormList_UnlinkAllAndTerminator(&pArray->pData[iList]);

    if (pArray->pData)
        STORM_SMemFree(pArray->pData, szCellListBucketsTag, -2, 0);
}

//1.10f: D2CMP.0x6FDF3370
//  Slot 2 of gVtblCellBuilderTableExport, the original's: ExportDestroy, then operator delete when bit 0 of nFlags is set. Our table never carries
//  that vtable (CelCmp.h). The original inlines operator delete here.
D2CmpCellBuilderTableHeader *__fastcall CELCMP_CellBuilderTable_ExportScalarDeletingDestructor(D2CmpCellBuilderTableHeader *pTable, int32_t nEdxUnused, uint8_t nFlags)
{
    CELCMP_CellBuilderTable_ExportDestroy((D2CmpCellBuilderTableReusePrefix *)pTable);
    if ((nFlags & 1) && pTable)
        STORM_SMemFree(pTable, szDeleteTag, -1, 0);

    return pTable;
}

//1.10f: D2CMP.0x6FDF33A0
//  The export table's destructor: ReuseDestroy's work, except that every reuse-list builder is destroyed through its scalar deleting destructor
//  with flags 0 (ReuseDestroy calls CellBuilder_Destroy; it is the same call, as flags 0 free nothing). The original inlines
//  CELCMP_StormLink_Unlink and CELCMP_CellBuilderTable_BaseDestroy here.
void __fastcall CELCMP_CellBuilderTable_ExportDestroy(D2CmpCellBuilderTableReusePrefix *pTable)
{
    pTable->header.pVtable = &gVtblCellBuilderTableReuse;

    D2CmpCellBuilderObjectChunk *pChunk;
    while ((intptr_t)(pChunk = (D2CmpCellBuilderObjectChunk *)pTable->chunkList.terminator.pNextOrSentinel) > 0)
    {
        CELCMP_CellBuilderObjectChunk_Destroy(pChunk);
        STORM_SMemFree(pChunk, szCellListChunkTag, -2, 0);
    }

    D2CmpCellBuilder *pReuseBuilder;
    while ((intptr_t)(pReuseBuilder = (D2CmpCellBuilder *)pTable->reuseList.terminator.pNextOrSentinel) > 0)
    {
        CELCMP_CellBuilder_ScalarDeletingDestructor(pReuseBuilder, 0, 0);
        STORM_SMemFree(pReuseBuilder, szCellListTag, -2, 0);
    }

    while ((intptr_t)pTable->chunkList.terminator.pNextOrSentinel > 0)
        CELCMP_StormLink_Unlink((D2CmpStormLink *)((char *)pTable->chunkList.terminator.pNextOrSentinel + pTable->chunkList.nLinkOffset));

    if (pTable->chunkList.terminator.pPrevLink)
    {
        CELCMP_StormLink_GetNextLink(&pTable->chunkList.terminator, 0, -1)->pPrevLink = pTable->chunkList.terminator.pPrevLink;
        pTable->chunkList.terminator.pPrevLink->pNextOrSentinel = pTable->chunkList.terminator.pNextOrSentinel;
        pTable->chunkList.terminator.pPrevLink = nullptr;
        pTable->chunkList.terminator.pNextOrSentinel = nullptr;
    }

    while ((intptr_t)pTable->reuseList.terminator.pNextOrSentinel > 0)
        CELCMP_StormLink_Unlink((D2CmpStormLink *)((char *)pTable->reuseList.terminator.pNextOrSentinel + pTable->reuseList.nLinkOffset));

    if (pTable->reuseList.terminator.pPrevLink)
    {
        CELCMP_StormLink_GetNextLink(&pTable->reuseList.terminator, 0, -1)->pPrevLink = pTable->reuseList.terminator.pPrevLink;
        pTable->reuseList.terminator.pPrevLink->pNextOrSentinel = pTable->reuseList.terminator.pNextOrSentinel;
        pTable->reuseList.terminator.pPrevLink = nullptr;
        pTable->reuseList.terminator.pNextOrSentinel = nullptr;
    }

    pTable->header.pVtable = &gVtblCellBuilderTableBase;
    pTable->header.nFullness = 0;
    CELCMP_StormList_UnlinkAll(&pTable->header.fullList);
    for (uint32_t iBucket = 0; iBucket < pTable->header.nBucketCount; ++iBucket)
    {
        D2CmpStormList *pBucket;
        while ((intptr_t)(pBucket = &pTable->header.pBuckets[iBucket])->terminator.pNextOrSentinel > 0)
            CELCMP_StormLink_Unlink(CELCMP_StormList_GetLink(pBucket, 0, pBucket->terminator.pNextOrSentinel));
    }

    CELCMP_StormListArray_Destroy((D2CmpStormListArray *)&pTable->header.nBucketCapacity);
    CELCMP_StormList_UnlinkAll(&pTable->header.fullList);
    CELCMP_StormLink_Unlink(&pTable->header.fullList.terminator);
}

//1.10f: D2CMP.0x6FDF3500
//  TSHashTable::New: a builder under a handle hash. While the table has fewer than 1024 buckets (mask below 0x3FF), the fullness falls by 3 (to no
//  less than 0) and rises by one per builder already in the bucket; past 13 it is reset to 0 and the table rehashed, and the bucket is taken again
//  under the new mask. The builder comes from the vtable's InternalNew, goes to the full list's tail, and takes the hash and the key. The original
//  inlines CELCMP_StormList_GetLink and CELCMP_StormLink_Unlink here.
//  BUG: InternalNew's answer is used untested: a NULL from Storm sends the full list's terminator through the unlink and the link (0x6FDF35AE) and
//  faults storing the hash at address 0 (0x6FDF35FA), as ours does. UNREACHABLE: the stock game does not reach this function.
D2CmpCellBuilder *__fastcall CELCMP_CellBuilderTable_New(D2CmpCellBuilderTableHeader *pTable, int32_t nEdxUnused, uint32_t dwHandleHash, const uint8_t *pKey, uint32_t nExtraBytes, uint32_t dwAllocFlags)
{
    uint32_t nBucketIndex = dwHandleHash & pTable->dwBucketMask;
    if (pTable->dwBucketMask < 0x3FF)
    {
        pTable->nFullness = pTable->nFullness <= 3 ? 0 : pTable->nFullness - 3;
        void *pBucketHead = pTable->pBuckets[nBucketIndex].terminator.pNextOrSentinel;
        D2CmpCellBuilder *pProbe = (intptr_t)pBucketHead <= 0 ? nullptr : (D2CmpCellBuilder *)pBucketHead;
        while ((intptr_t)pProbe > 0)
        {
            if (++pTable->nFullness > 13)
            {
                pTable->nFullness = 0;
                CELCMP_CellBuilderTable_Rehash(pTable);
                nBucketIndex = dwHandleHash & pTable->dwBucketMask;
                break;
            }

            pProbe = (D2CmpCellBuilder *)CELCMP_StormList_GetLink(&pTable->pBuckets[nBucketIndex], 0, pProbe)->pNextOrSentinel;
        }
    }

    D2CmpCellBuilder *pBuilder = pTable->pVtable->NewBuilder(pTable, 0, &pTable->pBuckets[nBucketIndex], nExtraBytes, dwAllocFlags);
    D2CmpStormLink *pFullLink = CELCMP_StormList_GetLink(&pTable->fullList, 0, pBuilder);
    CELCMP_StormLink_Unlink(pFullLink);
    D2CmpStormLink *pTailLink = pTable->fullList.terminator.pPrevLink;
    pFullLink->pPrevLink = pTailLink;
    pFullLink->pNextOrSentinel = pTailLink->pNextOrSentinel;
    pTailLink->pNextOrSentinel = pBuilder;
    pTable->fullList.terminator.pPrevLink = pFullLink;
    pBuilder->dwHandleHash = dwHandleHash;
    pBuilder->nKey = *pKey;
    return pBuilder;
}

//1.10f: D2CMP.0x6FDF3610
//  TSHashTable::Ptr: the builder under dwHandleHash, or NULL; the key is not read. The mask and the buckets are re-read each pass. The next node is
//  the candidate plus the bucket's link offset, with GetLink's NULL arm dropped as the candidate is known positive.
D2CmpCellBuilder *__fastcall CELCMP_CellBuilderTable_Find(D2CmpCellBuilderTableHeader *pTable, int32_t nEdxUnused, uint32_t dwHandleHash, const uint8_t *pUnusedKey)
{
    void *pBucketHead = pTable->pBuckets[dwHandleHash & pTable->dwBucketMask].terminator.pNextOrSentinel;
    D2CmpCellBuilder *pCandidate = (intptr_t)pBucketHead <= 0 ? nullptr : (D2CmpCellBuilder *)pBucketHead;
    while ((intptr_t)pCandidate > 0)
    {
        if (pCandidate->dwHandleHash == dwHandleHash)
            return pCandidate;

        pCandidate = (D2CmpCellBuilder *)((D2CmpStormLink *)((char *)pCandidate + pTable->pBuckets[dwHandleHash & pTable->dwBucketMask].nLinkOffset))->pNextOrSentinel;
    }

    return nullptr;
}

//1.10f: D2CMP.0x6FDF3660
//  TSList::Link: a node's link, at the list's offset inside it, or the terminator for NULL.
D2CmpStormLink *__fastcall CELCMP_StormList_GetLink(D2CmpStormList *pList, int32_t nEdxUnused, const void *pNode)
{
    if (pNode)
        return (D2CmpStormLink *)((uintptr_t)pNode + pList->nLinkOffset);

    return &pList->terminator;
}

//1.10f: D2CMP.0x6FDF3680
//  TSList::UnlinkAll: unlinks every node, re-reading the head each pass. The original inlines CELCMP_StormLink_Unlink here.
void __fastcall CELCMP_StormList_UnlinkAll(D2CmpStormList *pList)
{
    while ((intptr_t)pList->terminator.pNextOrSentinel > 0)
        CELCMP_StormLink_Unlink((D2CmpStormLink *)((char *)pList->terminator.pNextOrSentinel + pList->nLinkOffset));
}

//1.10f: D2CMP.0x6FDF36D0
//  ~TSList's scalar deleting destructor, as the bucket arrays call it with flags 0. The original inlines operator delete here.
D2CmpStormList *__fastcall CELCMP_StormList_ScalarDeletingDestructor(D2CmpStormList *pList, int32_t nEdxUnused, uint8_t bDeleteFlags)
{
    CELCMP_StormList_UnlinkAllAndTerminator(pList);
    if ((bDeleteFlags & 1) && pList)
        STORM_SMemFree(pList, szDeleteTag, -1, 0);

    return pList;
}

//1.10f: D2CMP.0x6FDF3700
//  ~TSHashObjectChunk: unlinks the chunk from the chunk list; for each of its builders empties the frame list (GetNext / GetEmbeddedLink / Unlink
//  per node), unlinks the list's terminator and the builder's hash links; then returns the builders' storage to Storm. The builder count is re-read
//  each pass. The original inlines CELCMP_StormLink_Unlink here.
void __fastcall CELCMP_CellBuilderObjectChunk_Destroy(D2CmpCellBuilderObjectChunk *pChunk)
{
    CELCMP_StormLink_Unlink(&pChunk->link);
    for (uint32_t iBuilder = 0; iBuilder < pChunk->array.dwCount; ++iBuilder)
    {
        D2CmpCellBuilder *pBuilder = &pChunk->array.pData[iBuilder];
        for (void *pFrameNode = CELCMP_StormLink_GetNext(&pBuilder->frames.terminator); pFrameNode; pFrameNode = CELCMP_StormLink_GetNext(&pBuilder->frames.terminator))
            CELCMP_StormLink_Unlink(CELCMP_StormList_GetEmbeddedLink(&pBuilder->frames, 0, pFrameNode));

        CELCMP_StormLink_Unlink(&pBuilder->frames.terminator);
        CELCMP_CellBuilder_UnlinkHashLinks(pBuilder);
    }

    if (pChunk->array.pData)
        STORM_SMemFree(pChunk->array.pData, szCellListTag, -2, 0);
}

//1.10f: D2CMP.0x6FDF3800
//  ~TSHashObject: unlinks the builder's full link, then its slot link. The original inlines CELCMP_StormLink_Unlink here.
void __fastcall CELCMP_CellBuilder_UnlinkHashLinks(D2CmpCellBuilder *pBuilder)
{
    CELCMP_StormLink_Unlink(&pBuilder->fullLink);
    CELCMP_StormLink_Unlink(&pBuilder->slotLink);
}

//1.10f: D2CMP.0x6FDF3870
//  TSLink::Next: the next node, or NULL at the terminator.
void *__fastcall CELCMP_StormLink_GetNext(D2CmpStormLink *pLink)
{
    return (intptr_t)pLink->pNextOrSentinel <= 0 ? nullptr : pLink->pNextOrSentinel;
}

//1.10f: D2CMP.0x6FDF3880
//  TSList::Link for a list of link offset 0: the node is its own link, or the terminator for NULL.
D2CmpStormLink *__fastcall CELCMP_StormList_GetEmbeddedLink(D2CmpStormList *pList, int32_t nEdxUnused, void *pNode)
{
    return pNode ? (D2CmpStormLink *)pNode : &pList->terminator;
}

//1.10f: D2CMP.0x6FDF3910
//  TSLink::NextLink: the next link. At the terminator it is the complement of the next node; otherwise it is the next node plus nLinkOffset or, for
//  a negative offset, plus the offset this link sits at (its address less its previous link's next node).
D2CmpStormLink *__fastcall CELCMP_StormLink_GetNextLink(D2CmpStormLink *pLink, int32_t nEdxUnused, int32_t nLinkOffset)
{
    const intptr_t nNext = (intptr_t)pLink->pNextOrSentinel;
    if (nNext < 0)
        return (D2CmpStormLink *)~nNext;

    intptr_t nEffectiveLinkOffset = nLinkOffset;
    if (nLinkOffset < 0)
        nEffectiveLinkOffset = (intptr_t)((uintptr_t)pLink - (uintptr_t)pLink->pPrevLink->pNextOrSentinel);

    return (D2CmpStormLink *)(nEffectiveLinkOffset + nNext);
}

//1.10f: D2CMP.0x6FDF3940
//  TSExplicitList::Link: node + offset. A genuine __fastcall of two registers.
D2CmpStormLink *__fastcall CELCMP_StormExplicitList_GetLink(const void *pNode, int32_t nLinkOffset)
{
    return (D2CmpStormLink *)((uintptr_t)pNode + nLinkOffset);
}

//1.10f: D2CMP.0x6FDF3950
//  TSHashTable::Grow: doubles the buckets. Every builder is moved off the old buckets onto a temporary list on the stack (its link found at the
//  temporary list's offset, 4, as the buckets'), the array is grown to twice the count, every bucket is given link offset 4, the mask is set, and
//  every builder is moved to the tail of the bucket its hash names under the new mask; the temporary's terminator is unlinked last. The old count
//  is held, not re-read. The original inlines the runtime functions CRT_CELCMP_StormList_InitializeOffsetZero and
//  CRT_CELCMP_StormList_ChangeLinkOffset here.
//  DIFFERENCE: the original has a loop draining the temporary list (0x6FDF3AD9..0x6FDF3AF2) that only its own back edge reaches; it is not written.
void __fastcall CELCMP_CellBuilderTable_Rehash(D2CmpCellBuilderTableHeader *pTable)
{
    const uint32_t dwOldBucketCount = pTable->dwBucketMask + 1;
    const uint32_t dwNewBucketCount = 2 * dwOldBucketCount;

    D2CmpStormList tTemporary;
    tTemporary.terminator.pPrevLink = nullptr;
    tTemporary.terminator.pNextOrSentinel = nullptr;
    tTemporary.nLinkOffset = 0;
    CELCMP_StormList_InitializeTerminator(&tTemporary);
    tTemporary.nLinkOffset = (int32_t)0xDDDDDDDD;
    CELCMP_StormList_InitializeTerminator(&tTemporary);

    if (tTemporary.nLinkOffset != 4)
    {
        while ((intptr_t)tTemporary.terminator.pNextOrSentinel > 0)
            CELCMP_StormLink_Unlink(CELCMP_StormList_GetLink(&tTemporary, 0, tTemporary.terminator.pNextOrSentinel));

        tTemporary.nLinkOffset = 4;
        CELCMP_StormList_InitializeTerminator(&tTemporary);
    }

    D2CmpStormLink *pTemporaryTail = tTemporary.terminator.pPrevLink;
    for (uint32_t iOldBucket = 0; iOldBucket < dwOldBucketCount; ++iOldBucket)
    {
        void *pNode;
        while ((intptr_t)(pNode = pTable->pBuckets[iOldBucket].terminator.pNextOrSentinel) > 0)
        {
            D2CmpStormLink *pMovingLink = (D2CmpStormLink *)((char *)pNode + tTemporary.nLinkOffset);
            if (pMovingLink->pPrevLink)
            {
                CELCMP_StormLink_Unlink(pMovingLink);
                pTemporaryTail = tTemporary.terminator.pPrevLink;
            }

            pMovingLink->pPrevLink = pTemporaryTail;
            pMovingLink->pNextOrSentinel = pTemporaryTail->pNextOrSentinel;
            pTemporaryTail->pNextOrSentinel = pNode;
            pTemporaryTail = pMovingLink;
            tTemporary.terminator.pPrevLink = pTemporaryTail;
        }
    }

    CELCMP_StormListArray_SetCount((D2CmpStormListArray *)&pTable->nBucketCapacity, 0, dwNewBucketCount);
    for (uint32_t iNewBucket = 0; iNewBucket < dwNewBucketCount; ++iNewBucket)
    {
        D2CmpStormList *pNewBucket = &pTable->pBuckets[iNewBucket];
        if (pNewBucket->nLinkOffset != 4)
        {
            while ((intptr_t)pNewBucket->terminator.pNextOrSentinel > 0)
                CELCMP_StormLink_Unlink(CELCMP_StormList_GetLink(pNewBucket, 0, pNewBucket->terminator.pNextOrSentinel));

            pNewBucket->nLinkOffset = 4;
            CELCMP_StormList_InitializeTerminator(pNewBucket);
        }
    }

    pTable->dwBucketMask = dwNewBucketCount - 1;
    while ((intptr_t)tTemporary.terminator.pNextOrSentinel > 0)
    {
        void *pNode = tTemporary.terminator.pNextOrSentinel;
        D2CmpStormList *pDestination = &pTable->pBuckets[pTable->dwBucketMask & ((const D2CmpCellBuilder *)pNode)->dwHandleHash];
        D2CmpStormLink *pDestinationLink = (D2CmpStormLink *)((char *)pNode + pDestination->nLinkOffset);
        if (pDestinationLink->pPrevLink)
            CELCMP_StormLink_Unlink(pDestinationLink);

        D2CmpStormLink *pDestinationTail = pDestination->terminator.pPrevLink;
        pDestinationLink->pPrevLink = pDestinationTail;
        pDestinationLink->pNextOrSentinel = pDestinationTail->pNextOrSentinel;
        pDestinationTail->pNextOrSentinel = pNode;
        pDestination->terminator.pPrevLink = pDestinationLink;
    }

    CELCMP_StormLink_Unlink(&tTemporary.terminator);
}

//1.10f: D2CMP.0x6FDF3B40
//  Links the terminator to itself: its previous link is its own address and its next is the complement of it.
void __fastcall CELCMP_StormList_InitializeTerminator(D2CmpStormList *pList)
{
    pList->terminator.pPrevLink = &pList->terminator;
    pList->terminator.pNextOrSentinel = (void *)~(uintptr_t)&pList->terminator;
}

//1.10f: D2CMP.0x6FDF3B50
//  TSFixedArray::SetCount over lists. Shrinking empties the lists past the new count (~TSList), re-reading the count each pass. Growing past the
//  allocation: the chunk size is the array's or, when 0, the new count -- 21 from 21 up (stored), else its highest set bit, 1 for none -- and the
//  allocation is the count rounded up to a multiple of it; the storage is re-allocated by SMemReAlloc (flags 0x10) and, only when that answers
//  NULL, allocated anew by SMemAlloc, with the old lists' offsets carried over. The lists added are constructed with link offset 0xDDDDDDDD.
//  BUG: when SMemReAlloc answers NULL, the SMemAlloc that follows is used untested: the lists are written into its answer. LATENT: only a failed
//  allocation reaches it; SMemReAlloc's own answer is tested (0x6FDF3C00).
void __fastcall CELCMP_StormListArray_SetCount(D2CmpStormListArray *pArray, int32_t nEdxUnused, uint32_t dwNewCount)
{
    if (dwNewCount <= pArray->dwCount)
    {
        for (uint32_t iList = dwNewCount; iList < pArray->dwCount; ++iList)
            CELCMP_StormList_UnlinkAllAndTerminator(&pArray->pData[iList]);

        pArray->dwCount = dwNewCount;
        return;
    }

    if (dwNewCount > pArray->dwAllocated)
    {
        uint32_t dwChunkSize = pArray->dwChunkSize;
        if (!dwChunkSize)
        {
            dwChunkSize = dwNewCount;
            if (dwNewCount >= 21)
            {
                pArray->dwChunkSize = 21;
                dwChunkSize = 21;
            }
            else
            {
                for (uint32_t dwBits = dwNewCount & (dwNewCount - 1); dwBits; dwBits &= dwBits - 1)
                    dwChunkSize = dwBits;

                if (!dwChunkSize)
                    dwChunkSize = 1;
            }
        }

        uint32_t dwAllocated = dwNewCount;
        if (dwNewCount % dwChunkSize)
            dwAllocated = dwNewCount + dwChunkSize - dwNewCount % dwChunkSize;

        D2CmpStormList *pOldData = pArray->pData;
        for (uint32_t iList = dwAllocated; iList < pArray->dwCount; ++iList)
            CELCMP_StormList_ScalarDeletingDestructor(&pOldData[iList], 0, 0);

        pArray->dwAllocated = dwAllocated;
        pArray->pData = (D2CmpStormList *)STORM_SMemReAlloc(pOldData, 12 * dwAllocated, szCellListBucketsTag, -2, 0x10);
        if (!pArray->pData)
        {
            pArray->pData = (D2CmpStormList *)STORM_SMemAlloc(12 * dwAllocated, szCellListBucketsTag, -2, 0);
            if (pOldData)
            {
                const uint32_t dwCopies = dwAllocated < pArray->dwCount ? dwAllocated : pArray->dwCount;
                for (uint32_t iList = 0; iList < dwCopies; ++iList)
                {
                    CELCMP_StormList_InitializeOffsetFromSourceIfNonNull(&pArray->pData[iList], &pOldData[iList]);
                    CELCMP_StormList_ScalarDeletingDestructor(&pOldData[iList], 0, 0);
                }

                STORM_SMemFree(pOldData, szCellListBucketsTag, -2, 0);
            }
        }
    }

    for (uint32_t iList = pArray->dwCount; iList < dwNewCount; ++iList)
        CELCMP_StormList_InitializePoisonedOffsetIfNonNull(&pArray->pData[iList]);

    pArray->dwCount = dwNewCount;
}

//1.10f: D2CMP.0x6FDF3CC0
//  A list constructed in place when there is a place (a NULL pointer is skipped): emptied, with link offset 0xDDDDDDDD, Storm's template argument
//  for an offset to be set later. The original inlines the runtime function CRT_CELCMP_StormList_InitializePoisonedOffset here.
void __fastcall CELCMP_StormList_InitializePoisonedOffsetIfNonNull(D2CmpStormList *pList)
{
    if (pList)
    {
        pList->terminator.pPrevLink = nullptr;
        pList->terminator.pNextOrSentinel = nullptr;
        pList->terminator.pPrevLink = &pList->terminator;
        pList->nLinkOffset = (int32_t)0xDDDDDDDD;
        pList->terminator.pNextOrSentinel = (void *)~(uintptr_t)&pList->terminator;
    }
}

//1.10f: D2CMP.0x6FDF3CF0
//  A list copy-constructed in place when there is a place (a NULL pointer is skipped): emptied, with the source's link offset.
void __fastcall CELCMP_StormList_InitializeOffsetFromSourceIfNonNull(D2CmpStormList *pList, const D2CmpStormList *pSource)
{
    if (pList)
    {
        pList->terminator.pPrevLink = nullptr;
        pList->terminator.pNextOrSentinel = nullptr;
        pList->terminator.pPrevLink = &pList->terminator;
        pList->nLinkOffset = pSource->nLinkOffset;
        pList->terminator.pNextOrSentinel = (void *)~(uintptr_t)&pList->terminator;
    }
}

//  The builders' table, gCellBuilderTable, is built by a static object of ours, sgCellBuilderTableLifetime, in place of the original's runtime
//  static initialiser (which also registers the table's destructor with atexit). The constructor leaves the table in the state the original's table
//  constructor leaves it in; the destructor is SyncDestroy, as the original's atexit destructor jumps to it. An empty list's terminator links to
//  itself. The bucket array is four lists from SetCount(4) (allocated 4, count 4, chunk size 0); the dummy key is never written and stays zero.
//  DIFFERENCE: the original stores the four vtables of the class chain in turn (Base, Reuse, Export, Sync) with no virtual call between, passes the
//  link offsets through 0xDDDDDDDD and drains lists that are already empty on the way. Nothing reads the object before the constructor returns, so
//  ours writes the end state. The bucket array is the one call kept: SetCount makes the original's Storm calls in the original's order (SMemReAlloc
//  of NULL, then SMemAlloc only if that answers NULL), before InitializeCriticalSection.
static void CelCmpInitEmptyList(D2CmpStormList *pList, int32_t nLinkOffset)
{
    pList->nLinkOffset = nLinkOffset;
    pList->terminator.pPrevLink = &pList->terminator;
    pList->terminator.pNextOrSentinel = (void *)~(uintptr_t)&pList->terminator;
}

static void CelCmpConstructCellBuilderTable(D2CmpCellBuilderTablePrefix *pTable)
{
    CelCmpInitEmptyList(&pTable->header.fullList, 12);
    pTable->header.nFullness = 0;
    pTable->header.dwBucketMask = 3;
    CELCMP_StormListArray_SetCount((D2CmpStormListArray *)&pTable->header.nBucketCapacity, 0, 4);
    for (uint32_t iBucket = 0; iBucket <= pTable->header.dwBucketMask; ++iBucket)
        CelCmpInitEmptyList(&pTable->header.pBuckets[iBucket], 4);

    CelCmpInitEmptyList(&pTable->reuseList, 4);
    pTable->dwObjectChunkSize = 16;
    CelCmpInitEmptyList(&pTable->chunkList, 16);
    pTable->dwHandleSequence = ~(uint32_t)(uintptr_t)pTable & 0x0FFFFFFF;
    pTable->bHandleSequenceWrapped = 0;
    InitializeCriticalSection(&pTable->criticalSection);
    pTable->header.pVtable = &gVtblCellBuilderTableSync;
}

//  The static object whose construction builds gCellBuilderTable and whose destruction tears it down. gCellBuilderTable is zero when it runs: an
//  object at namespace scope is zero-initialised before any dynamic initialisation.
struct CellBuilderTableLifetime
{
    CellBuilderTableLifetime() { CelCmpConstructCellBuilderTable(&gCellBuilderTable); }
    ~CellBuilderTableLifetime() { CELCMP_CellBuilderTable_SyncDestroy(&gCellBuilderTable); }
};
static CellBuilderTableLifetime sgCellBuilderTableLifetime;


//1.10f: D2CMP.0x6FE00346
//  The 1-based index of the lowest set bit, 0 for 0 (bsf, then + 1). __cdecl, one stack argument, a bare retn. It lies among the import thunks at
//  the end of the image, outside this unit's range; every one of its callers is in this file.
//  DIFFERENCE: the original builds an ebp frame and passes the answer through a local (the shape MSVC gives a function with an __asm block); the C
//  computes the same value with _BitScanForward.
uint32_t __cdecl CELCMP_FindFirstSetBit1Based(uint32_t dwMask)
{
    if (!dwMask)
        return 0;

    unsigned long nIndex;
    _BitScanForward(&nIndex, dwMask);
    return nIndex + 1;
}
