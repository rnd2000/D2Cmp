#include "D2CMP.h"

//  TileLib.cpp -- the tile library of D2CMP.dll 1.10: an array of MAX_TILEPROJECT (32) slots, each holding the find-tiles hash of one loaded
//  tile project (a .dt1 file). AddProject loads a file into the first empty slot, GetTiles asks every slot in turn for the tiles of one type, style
//  and sequence, and twelve getters read one field of a tile library entry (D2TileLibraryEntryStrc) each.
//  The original source file is TileLib.cpp (C:\projects\D2\head\Diablo2\Source\D2CMP\Src\TileLib.cpp, named by its four asserts and twelve warnings).
//  Fourteen functions over 0x6FDFFDE0..0x6FE00136, all exports, #10077..#10088 and #10100..#10101.
//
//  The twelve getters WARN on a NULL tile and read it anyway: FOG_DisplayWarning is the assert without the exit, and nothing between it and the
//  load tests the pointer again. So a NULL tile faults reading its field -- except CollisionInfo's, which reads nothing and answers 0x28.

//  The path literal of the unit's sixteen reports.
static const char szTileLibCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2CMP\\Src\\TileLib.cpp";

//  Layout checks: the offsets the getters read, each with its instruction.
static_assert(offsetof(D2TileLibraryEntryStrc, nLightDirection) == 0x00, "D2TileLibraryEntryStrc.nLightDirection: mov eax, [esi] (0x6FE0003D)");
static_assert(offsetof(D2TileLibraryEntryStrc, nRoofHeight) == 0x04, "D2TileLibraryEntryStrc.nRoofHeight: mov ax, [esi+4] (0x6FE00100)");
static_assert(offsetof(D2TileLibraryEntryStrc, nFlags) == 0x06, "D2TileLibraryEntryStrc.nFlags: mov ax, [esi+6] (0x6FDFFF7F)");
static_assert(offsetof(D2TileLibraryEntryStrc, nTotalHeight) == 0x08, "D2TileLibraryEntryStrc.nTotalHeight: mov eax, [esi+8] (0x6FE00070)");
static_assert(offsetof(D2TileLibraryEntryStrc, nWidth) == 0x0C, "D2TileLibraryEntryStrc.nWidth: mov eax, [esi+0Ch] (0x6FDFFFAD)");
static_assert(offsetof(D2TileLibraryEntryStrc, nHeightToBottom) == 0x10, "D2TileLibraryEntryStrc.nHeightToBottom: mov eax, [esi+10h] (0x6FE000D0)");
static_assert(offsetof(D2TileLibraryEntryStrc, nType) == 0x14, "D2TileLibraryEntryStrc.nType: mov eax, [esi+14h] (0x6FDFFF1D)");
static_assert(offsetof(D2TileLibraryEntryStrc, nStyle) == 0x18, "D2TileLibraryEntryStrc.nStyle: mov eax, [esi+18h] (0x6FDFFF4D)");
static_assert(offsetof(D2TileLibraryEntryStrc, nSequence) == 0x1C, "D2TileLibraryEntryStrc.nSequence: mov eax, [esi+1Ch] (0x6FE0000D)");
static_assert(offsetof(D2TileLibraryEntryStrc, nRarity_Frame) == 0x20, "D2TileLibraryEntryStrc.nRarity_Frame: mov eax, [esi+20h] (0x6FDFFFDD)");
static_assert(offsetof(D2TileLibraryEntryStrc, nSubtileFlags) == 0x28, "D2TileLibraryEntryStrc.nSubtileFlags: lea eax, [esi+28h] (0x6FE000A0)");
static_assert(offsetof(D2TileLibraryEntryStrc, szFileName) == 0x58, "D2TileLibraryEntryStrc.szFileName: mov eax, [esi+58h] (0x6FE00130)");


//1.10f: D2CMP.0x6FDFFDE0
//  #10087: loads a tile project into the library's first empty slot. Both arguments are asserted, each followed by exit; the walk stops at the
//  first NULL slot and hands its address to LoadProjectRecord, which fills it (TileProjects.cpp). The original tests `ii < MAX_TILEPROJECT` after
//  the call as well (the source's assert after the loop), which a full library reaches straight from the walk's end: "ii < MAX_TILEPROJECT" and
//  exit. `void`: LoadProjectRecord's answer is left in eax and no caller reads it.
void __stdcall TILELIB_TileLibraryAddProject(D2TileLibraryHashStrc **ptTileLibrary, const char *szName)
{
    if (!ptTileLibrary)
    {
        FOG_DisplayAssert("ptTileLibrary", szTileLibCpp, 28);
        exit(-1);
    }
    if (!szName)
    {
        FOG_DisplayAssert("szName", szTileLibCpp, 29);
        exit(-1);
    }

    int32_t ii;
    for (ii = 0; ii < MAX_TILEPROJECT; ++ii)
    {
        if (!ptTileLibrary[ii])
        {
            TILEPROJECTS_LoadProjectRecord(&ptTileLibrary[ii], szName);
            break;
        }
    }
    if (ii >= MAX_TILEPROJECT)
    {
        FOG_DisplayAssert("ii < MAX_TILEPROJECT", szTileLibCpp, 42);
        exit(-1);
    }
}


//1.10f: D2CMP.0x6FDFFE70
//  #10088: collects into pTileList the tiles of one type, style and sequence from each project of the library in turn, until a NULL slot, the
//  thirty-second slot, or a full list. Each project's hash is asked for at most the room left (FINDTILES_CollectTiles, which answers how many it
//  wrote) at the next free place of the list. The answer is nCapacity when the list filled and nCapacity less the room left otherwise. The library
//  is asserted, then exit. The slot index lives in a stack slot and the library pointer is re-read from the argument each pass; the compare is
//  signed for the index and a zero test for the room.
uint32_t __stdcall TILELIB_TileLibraryGetTiles(D2TileLibraryHashStrc **ptTileLibrary, int32_t nType, int32_t nStyle, int32_t nSequence, D2TileLibraryEntryStrc **pTileList, uint32_t nCapacity)
{
    uint32_t nWritten = 0;
    if (!ptTileLibrary)
    {
        FOG_DisplayAssert("ptTileLibrary", szTileLibCpp, 93);
        exit(-1);
    }

    uint32_t nRemaining = nCapacity;
    for (int32_t ii = 0; ii < MAX_TILEPROJECT; ++ii)
    {
        D2TileLibraryHashStrc *pHash = ptTileLibrary[ii];
        if (!pHash)
            break;

        uint32_t nFound = FINDTILES_CollectTiles(pHash, &pTileList[nWritten], nRemaining, nType, nStyle, nSequence);
        nRemaining -= nFound;
        nWritten += nFound;
        if (!nRemaining)
            return nCapacity;
    }
    return nCapacity - nRemaining;
}


//1.10f: D2CMP.0x6FDFFF00
//  #10077: the tile's type (+0x14).
//  BUG: a NULL tile is warned about and then read, in this getter and the eleven after it: FOG_DisplayWarning logs and returns, and the load
//  follows with no second test, faulting at the field's offset (CollisionInfo excepted, which loads nothing). NOT ESTABLISHED: left in the C, not
//  certainly a defect. Reproduced.
//  D2MOO BUG: D2CMP.h gives #10077 the offset 0xFFF0, which is #10082's RVA (0x6FDFFFF0); the binary has #10077 at 0xFF00.
int32_t __stdcall TILELIB_TileGetType(D2TileLibraryEntryStrc *hTile)
{
    if (!hTile)
        FOG_DisplayWarning("hTile", szTileLibCpp, 121);
    return hTile->nType;
}


//1.10f: D2CMP.0x6FDFFF30
//  #10078: the tile's style (+0x18).
//  BUG: a NULL tile is read after the warning, as in TileGetType. NOT ESTABLISHED: left in the C, not certainly a defect.
int32_t __stdcall TILELIB_TileGetStyle(D2TileLibraryEntryStrc *hTile)
{
    if (!hTile)
        FOG_DisplayWarning("hTile", szTileLibCpp, 122);
    return hTile->nStyle;
}


//1.10f: D2CMP.0x6FDFFF60
//  #10079: the tile's flags, the word at +0x06 zero-extended (`xor eax, eax` then `mov ax, [esi+6]`, 0x6FDFFF7D, 0x6FDFFF7F), so the high half is
//  zero. The C's `int` return of the `unsigned __int16` field is that widening.
//  BUG: a NULL tile is read after the warning, as in TileGetType. NOT ESTABLISHED: left in the C, not certainly a defect.
int32_t __stdcall TILELIB_TileGetFlags(D2TileLibraryEntryStrc *hTile)
{
    if (!hTile)
        FOG_DisplayWarning("hTile", szTileLibCpp, 123);
    return hTile->nFlags;
}


//1.10f: D2CMP.0x6FDFFF90
//  #10080: the tile's width (+0x0C).
//  BUG: a NULL tile is read after the warning, as in TileGetType. UNREACHABLE: no stock module imports #10080.
int32_t __stdcall TILELIB_TileGetWidth(D2TileLibraryEntryStrc *hTile)
{
    if (!hTile)
        FOG_DisplayWarning("hTile", szTileLibCpp, 124);
    return hTile->nWidth;
}


//1.10f: D2CMP.0x6FDFFFC0
//  #10081: the tile's rarity (+0x20) -- for an animated tile its frame.
//  BUG: a NULL tile is read after the warning, as in TileGetType. NOT ESTABLISHED: left in the C, not certainly a defect.
int32_t __stdcall TILELIB_TileGetRarity(D2TileLibraryEntryStrc *hTile)
{
    if (!hTile)
        FOG_DisplayWarning("hTile", szTileLibCpp, 125);
    return hTile->nRarity_Frame;
}


//1.10f: D2CMP.0x6FDFFFF0
//  #10082: the tile's sequence (+0x1C). `int`, the field's; one caller compares it unsigned, which is the caller's sign, not the getter's.
//  BUG: a NULL tile is read after the warning, as in TileGetType. NOT ESTABLISHED: left in the C, not certainly a defect.
int32_t __stdcall TILELIB_TileGetSequence(D2TileLibraryEntryStrc *hTile)
{
    if (!hTile)
        FOG_DisplayWarning("hTile", szTileLibCpp, 126);
    return hTile->nSequence;
}


//1.10f: D2CMP.0x6FE00020
//  #10083: the tile's light direction (+0x00).
//  BUG: a NULL tile is read after the warning, as in TileGetType; this one faults at 0. UNREACHABLE: no stock module imports #10083.
int32_t __stdcall TILELIB_TileGetTileLight(D2TileLibraryEntryStrc *hTile)
{
    if (!hTile)
        FOG_DisplayWarning("hTile", szTileLibCpp, 127);
    return hTile->nLightDirection;
}


//1.10f: D2CMP.0x6FE00050
//  #10084: the tile's total height (+0x08).
//  BUG: a NULL tile is read after the warning, as in TileGetType. NOT ESTABLISHED: left in the C, not certainly a defect.
int32_t __stdcall TILELIB_TileGetTotalHeight(D2TileLibraryEntryStrc *hTile)
{
    if (!hTile)
        FOG_DisplayWarning("hTile", szTileLibCpp, 128);
    return hTile->nTotalHeight;
}


//1.10f: D2CMP.0x6FE00080
//  #10085: the ADDRESS of the tile's subtile flags (+0x28, `lea eax, [esi+28h]`, 0x6FE000A0), read by the collision code. Nothing is loaded, so a
//  NULL tile is warned about and answers 0x28. The C adds the offset to the pointer's integer value: the same answer, where
//  `&hTile->nSubtileFlags` on a NULL tile would be undefined.
//  D2MOO BUG: D2TileLibraryEntryStrc declares +0x28 as dwTileFlags[4]; the binary has 25 subtile flags there (nSubtileFlags[25]).
uint8_t *__stdcall TILELIB_TileGetCollisionInfo(D2TileLibraryEntryStrc *hTile)
{
    if (!hTile)
        FOG_DisplayWarning("hTile", szTileLibCpp, 129);
    return (uint8_t *)((uintptr_t)hTile + offsetof(D2TileLibraryEntryStrc, nSubtileFlags));
}


//1.10f: D2CMP.0x6FE000B0
//  #10086: the tile's height to its bottom (+0x10).
//  BUG: a NULL tile is read after the warning, as in TileGetType. UNREACHABLE: no stock module imports #10086.
int32_t __stdcall TILELIB_TileGetHeightToBottom(D2TileLibraryEntryStrc *hTile)
{
    if (!hTile)
        FOG_DisplayWarning("hTile", szTileLibCpp, 130);
    return hTile->nHeightToBottom;
}


//1.10f: D2CMP.0x6FE000E0
//  #10100: the tile's roof height, the word at +0x04 and nothing above it (`mov ax, [esi+4]`; no `xor eax, eax` here, unlike TileGetFlags).
//  BUG: a NULL tile is read after the warning, as in TileGetType. UNREACHABLE: no stock module imports #10100.
uint16_t __stdcall TILELIB_TileGetRoofHeight(D2TileLibraryEntryStrc *hTile)
{
    if (!hTile)
        FOG_DisplayWarning("hTile", szTileLibCpp, 131);
    return hTile->nRoofHeight;
}


//1.10f: D2CMP.0x6FE00110
//  #10101: the name of the file the tile came from (+0x58, the pointer the tile cache opens).
//  BUG: a NULL tile is read after the warning, as in TileGetType. UNREACHABLE: no stock module imports #10101.
char *__stdcall TILELIB_TileGetFilename(D2TileLibraryEntryStrc *hTile)
{
    if (!hTile)
        FOG_DisplayWarning("hTile", szTileLibCpp, 132);
    return hTile->szFileName;
}
