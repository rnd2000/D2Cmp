#include "D2CMP.h"

//  TileProjects.cpp -- the loaded tile projects (.dt1 files) of D2CMP.dll 1.10 (image base 0x6FDF0000), kept in a list headed by
//  gpTileProjectRecords (TileProjects.h), one D2TileRecordStrc (0x110 bytes) each: the file name, the normalized project, its find-tiles hash
//  and the previous record. LoadProjectRecord finds a project by its file name or loads, normalizes and hashes it and records it;
//  DestroyTileHeaders destroys every project and record. The original's source file is D2CMP\SRC\TileProjects.cpp (named by the asserts
//  "ptData" and "ptRecord"). Address range 0x6FE00140..0x6FE00331.

//  The path literal of the unit's Fog calls (__FILE__ in the original).
static const char szTileProjectsCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2CMP\\SRC\\TileProjects.cpp";

//  The record's layout, each offset at an instruction that reads it; its size, 0x110, is asserted with the struct.
static_assert(offsetof(D2TileRecordStrc, szLibraryName) == 0x000, "D2TileRecordStrc.szLibraryName: mov dl, [eax] over the record (0x6FE001C4)");
static_assert(offsetof(D2TileRecordStrc, pLibrary) == 0x104, "D2TileRecordStrc.pLibrary: mov eax, [esi+104h] (0x6FE0014F)");
static_assert(offsetof(D2TileRecordStrc, pHashBlock) == 0x108, "D2TileRecordStrc.pHashBlock: mov ecx, [esi+108h] (0x6FE00162)");
static_assert(offsetof(D2TileRecordStrc, pPrev) == 0x10C, "D2TileRecordStrc.pPrev: mov edi, [esi+10Ch] (0x6FE00155)");


//1.10f: D2CMP.0x6FE00140
//  #10094: destroys every loaded project and its record, newest first. For each record the previous one is taken first, the project is destroyed
//  (TILECMP_TileProjectDestroy with nKeepProject 0, which also takes its tiles out of the tile cache) and its pointer cleared, the hash is freed
//  and its pointer cleared, and the record is freed; then the head is cleared, also when the list was empty. Void: no argument, a bare `retn`.
void __cdecl TILEPROJECTS_DestroyTileHeaders()
{
    D2TileRecordStrc *pRecord = gpTileProjectRecords;
    while (pRecord)
    {
        D2TileRecordStrc *pPrev = pRecord->pPrev;
        TILECMP_TileProjectDestroy(pRecord->pLibrary, 0);
        pRecord->pLibrary = nullptr;
        FINDTILES_FreeHash(pRecord->pHashBlock);
        pRecord->pHashBlock = nullptr;
        FOG_Free(pRecord, szTileProjectsCpp, 42, 0);
        pRecord = pPrev;
    }
    gpTileProjectRecords = nullptr;
}


//1.10f: D2CMP.0x6FE001A0
//  #10095: the project loaded from szFileName, and its hash in *ppHash. The records are searched newest first by an exact, case-sensitive
//  compare of the name (the original's inline strcmp); a hit answers the record's project and hash. Otherwise the whole file is read into a
//  zeroed block of the size TILECACHE_GetProjectAllocationSize gives, normalized into pProject (TILECMP_NormalizeProject), hashed
//  (TILECMP_BuildProjectHash), and a zeroed record is put at the head of the list with the project, the hash and the name. ppHash is never
//  tested. No caller reads the answer; it is kept because both exits load it into eax.
//  BUG: the result of ARCHIVE_OpenFile is not tested. A file that does not open is sized 0 by GetProjectAllocationSize, and the read is made
//  through whatever handle the failed open left (0x6FE00274, then the read at 0x6FE00281). NOT ESTABLISHED: left as in the original, not shown
//  to be a defect.
//  BUG: the name is copied into the record's 260-byte szLibraryName with no bound (an inline strcpy, 0x6FE0030E..0x6FE00316). LATENT: its one
//  importer, D2Client, passes constants of 40 and 41 characters and LvlTypes.txt names of at most 59.
D2CmpNormalizedTileProject *__stdcall TILEPROJECTS_LoadProjectRecord(D2TileLibraryHashStrc **ppHash, const char *szFileName)
{
    D2CmpNormalizedTileProject *pProject = nullptr;
    void *hFile = nullptr;

    D2TileRecordStrc *pRecord;
    for (pRecord = gpTileProjectRecords; pRecord; pRecord = pRecord->pPrev)
    {
        if (!strcmp(pRecord->szLibraryName, szFileName))
            break;
    }
    if (pRecord)
    {
        *ppHash = pRecord->pHashBlock;
        return pRecord->pLibrary;
    }

    uint32_t dwSize = TILECACHE_GetProjectAllocationSize(szFileName);
    D2TileLibraryHeaderStrc *ptData = (D2TileLibraryHeaderStrc *)FOG_Alloc(dwSize, szTileProjectsCpp, 107, 0);
    if (!ptData)
    {
        FOG_DisplayAssert("ptData", szTileProjectsCpp, 108);
        exit(-1);
    }
    memset(ptData, 0, dwSize);
    ARCHIVE_OpenFile(0, szFileName, &hFile, 0);
    ARCHIVE_ReadFileExact(0, hFile, ptData, dwSize);
    ARCHIVE_CloseFile(0, hFile);
    TILECMP_NormalizeProject(ptData, &pProject, szFileName);
    D2TileLibraryHashStrc *pHash = TILECMP_BuildProjectHash(pProject);

    D2TileRecordStrc *ptRecord = (D2TileRecordStrc *)FOG_Alloc(sizeof(D2TileRecordStrc), szTileProjectsCpp, 123, 0);
    if (!ptRecord)
    {
        FOG_DisplayAssert("ptRecord", szTileProjectsCpp, 124);
        exit(-1);
    }
    memset(ptRecord, 0, sizeof(D2TileRecordStrc));
    ptRecord->pPrev = gpTileProjectRecords;
    gpTileProjectRecords = ptRecord;
    ptRecord->pHashBlock = pHash;
    ptRecord->pLibrary = pProject;
    strcpy(ptRecord->szLibraryName, szFileName);
    *ppHash = ptRecord->pHashBlock;
    return pProject;
}
