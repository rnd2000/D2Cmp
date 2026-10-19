#include "D2CMP.h"

//  PalShift.cpp -- the item palette shifts of D2CMP.dll 1.10 (image base 0x6FDF0000), address range 0x6FDFB1B0..0x6FDFB7A0: eight .dat files of
//  palette-index maps are loaded into gPalShiftTransforms at start-up and handed out one map at a time, by an item's Transform / InvTrans column and
//  its colour. The original's source file is D2CMP\SRC\PalShift.cpp (the warnings in this unit name it).

//  The path literal of the warnings (the original's __FILE__).
static const char szPalShiftCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2CMP\\SRC\\PalShift.cpp";

static_assert(sizeof(gPalShiftTransforms[0]) == 0x6900, "the group stride: imul edi, 6900h (0x6FDFB6E1)");
static_assert(sizeof(gPalShiftTransforms) == 0x6FED0D18 - 0x6FE95C18, "nine groups, up to gCelHeaderHash");


//1.10f: D2CMP.0x6FDFB1B0
//  #10096: loads the eight palette-shift files, "grey", "grey2", "gold", "brown", "greybrown", "invgrey", "invgrey2" and "invgreybrown", into groups
//  1 to 8 of gPalShiftTransforms; group 0 is not loaded. The original inlines PALSHIFT_LoadItemPaletteGroup here for the first six files and calls it
//  for the last two. void: its one importer, D2Client, does not read the eax the original leaves.
extern "C" void __stdcall PALSHIFT_PalShiftInit()
{
    PALSHIFT_LoadItemPaletteGroup("grey", 1);
    PALSHIFT_LoadItemPaletteGroup("grey2", 2);
    PALSHIFT_LoadItemPaletteGroup("gold", 3);
    PALSHIFT_LoadItemPaletteGroup("brown", 4);
    PALSHIFT_LoadItemPaletteGroup("greybrown", 5);
    PALSHIFT_LoadItemPaletteGroup("invgrey", 6);
    PALSHIFT_LoadItemPaletteGroup("invgrey2", 7);
    PALSHIFT_LoadItemPaletteGroup("invgreybrown", 8);
}


//1.10f: D2CMP.0x6FDFB5C0
//  Loads one palette-shift file, "Data\Global\Items\Palette\<szBaseName>.dat": 1500h bytes of it into group nGroupIndex of gPalShiftTransforms. The
//  open's last argument, 1, asks ARCHIVE_OpenFile not to report a missing file; when the file cannot be opened, a warning ("0", line 57: the
//  original's warning macro over a constant false) is raised and the group's 1500h bytes are cleared. The file name is built in a 256-byte buffer
//  with no bound. void: the eax the original leaves (ARCHIVE_CloseFile's leftover, or the clear's 0) is read by neither caller.
//  BUG: nGroupIndex is not bounded. LATENT: PalShiftInit passes only the constants 1 to 8, inside the nine groups (the original's two calls load 7
//  and 8 into edx at 0x6FDFB599 and 0x6FDFB5A8).
void __fastcall PALSHIFT_LoadItemPaletteGroup(const char *szBaseName, int32_t nGroupIndex)
{
    char szFileName[256];
    strcpy(szFileName, "Data\\Global\\Items\\Palette\\");
    strcat(szFileName, szBaseName);
    strcat(szFileName, ".dat");

    const uint32_t dwBytesToRead = 0x1500;
    void *hFile;
    if (!ARCHIVE_OpenFile(0, szFileName, &hFile, 1))
    {
        FOG_DisplayWarning("0", szPalShiftCpp, 57);
        memset(gPalShiftTransforms[nGroupIndex], 0, dwBytesToRead);
        return;
    }
    ARCHIVE_ReadFileExact(0, hFile, gPalShiftTransforms[nGroupIndex], dwBytesToRead);
    ARCHIVE_CloseFile(0, hFile);
}


//1.10f: D2CMP.0x6FDFB740
//  #10097: sets gnPalShiftAct to nAct when it is 1 to 5 (signed tests), otherwise does nothing; nothing in the module reads the act it stores. void:
//  the eax the original leaves is the argument, and none of D2Client's call sites reads it.
extern "C" void __stdcall PALSHIFT_PalShiftSetAct(int32_t nAct)
{
    if (nAct > 0 && nAct <= 5)
    {
        gnPalShiftAct = nAct;
    }
}


//1.10f: D2CMP.0x6FDFB760
//  #10098: returns row nColor of group nTransform of gPalShiftTransforms, a 256-byte palette-index map: NULL for a group of 9 or more, a colour of 21
//  or more, group 0, and groups 3 and 4 ("gold" and "brown", which PalShiftInit loads all the same). The tests are signed and the act is not read:
//  the row is 105 * nTransform + nColor whatever gnPalShiftAct holds.
//  D2MOO BUG: D2MOO's MixPalette declares nTrans uint8_t; the binary loads the whole stack slot and compares it signed, so it is an int.
//  BUG: a negative nColor passes (only nColor >= 21 is tested) and so does a negative nTransform (the signed tests refuse only 0, 3, 4 and 9 or
//  more); the answer then points before the group's first row, below the table itself for a large negative value. NOT ESTABLISHED: D2Common passes
//  the transform zero-extended and sign-extends the colour at some of its call sites, so only a colour byte of 0x80 or more can arrive negative;
//  whether any item has one is unknown.
//  The address is computed on the integer (uintptr_t), not by indexing the array: the original's arithmetic has no bound below, and an
//  index before the array's first row would be undefined in C.
extern "C" uint8_t *__stdcall PALSHIFT_PalShiftGetTransform(int32_t nTransform, int32_t nColor)
{
    if (nTransform >= 9 || nColor >= 21 || nTransform == 0 || (nTransform > 2 && nTransform <= 4))
    {
        return nullptr;
    }
    const uint32_t dwRow = (uint32_t)(105 * nTransform + nColor);
    return (uint8_t *)((uintptr_t)gPalShiftTransforms + (dwRow << 8));
}
