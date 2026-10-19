#pragma once

#include "DLLmain.h"
#include "FastCmp.h"

//  CelCmp.h -- the functions of CelCmp.cpp that are ours, declared, and the unit's globals. 35 of the functions are exports (#10013..#10047),
//  declared extern "C" with the ordinal in the trailing comment; the others are the unit's own functions, Storm's list and hash templates as
//  CelCmp.cpp instantiates them, and CELCMP_FindFirstSetBit1Based. A __thiscall member is written __fastcall with `int32_t nEdxUnused` after `this`
//  when it takes stack arguments, and plain __fastcall when `this` is all it takes. The globals are C++17 `inline` variables, one object for every
//  file that includes this header; the comment line "D2CMP 0x6F..." above a global gives its address in the original. They are the cell builders'
//  table and the three vtables our code installs in it, the four format tables, the direction tables and the hardware-free callback. The class
//  chain of the original's table has a fourth vtable, gVtblCellBuilderTableExport (0x6FE0B930). It is not defined here: the original's constructor
//  installs it between two other stores, and nothing of ours does.

void __fastcall CELCMP_CellBuilderTable_SyncDestroy(D2CmpCellBuilderTablePrefix *pTable);
extern "C" uint32_t __stdcall CELCMP_CelNormalDirToFileDir(uint32_t nDirection32, uint32_t nDirectionCount);                // #10017
extern "C" uint32_t __stdcall CELCMP_CelGameDirToFileDir(uint32_t nDirection64, uint32_t nDirectionCount);                  // #10018
extern "C" uint32_t __stdcall CELCMP_CelConvertDir(uint32_t nDirection, uint32_t nStartDirections, uint32_t nRepresentativeDirections, int32_t bAlternate);   // #10019
extern "C" int32_t __stdcall CELCMP_CelConvertBlock(int32_t nSourceDirections, int32_t nTargetDirections);                                         // #10020
extern "C" int32_t __stdcall CELCMP_CelGameDirToShadowGameDir(uint32_t nGameDirection, uint32_t nLevelOfRotation);                 // #10021
extern "C" int32_t __stdcall CELCMP_CelGameDirToNormalDir(uint32_t nGameDirection, uint32_t nLevelOfRotation);                     // #10022
extern "C" uint32_t __stdcall CELCMP_CelCompressCreate(int32_t nFormat, uint32_t nDirections, const uint32_t *pTransparentColor, int32_t bRGB24);   // #10013
extern "C" void __stdcall CELCMP_CelCompressAddImage(uint32_t hBuilder, const uint8_t *pImage, uint32_t nImageStride, uint32_t nPixelHeight, uint32_t nPixelWidth, int32_t nOffsetX, int32_t nOffsetY, int32_t nOrientation);   // #10015
extern "C" void __stdcall CELCMP_CelCompressDestroy(uint32_t hBuilder);                                                             // #10014
extern "C" uint32_t __stdcall CELCMP_CelCompressCreateCelFile(uint32_t hBuilder, D2CmpBuiltCellFile **ppOutput);               // #10016
extern "C" void __stdcall CELCMP_CelFileHalve(int32_t hFile, int32_t nUnused);                                                                  // #10029
extern "C" int32_t __stdcall CELCMP_CelFileConvert24To8(D2CmpBuiltCellFile *pSrc, D2CmpBuiltCellFile **ppOutFile,                        // #10025
                                                    uint8_t (__stdcall *pfMapRgbToIndex)(uint8_t nRed, uint8_t nGreen, uint8_t nBlue),
                                                    uint32_t dwTransparentColor);
extern "C" void __stdcall CELCMP_CelFileConvertOldToNew(D2CmpSerializedCellFileV5Header *pOldFile, D2CmpBuiltCellFile **ppNewFile, int32_t *pNewFileSize);   // #10030
extern "C" void __stdcall CELCMP_CelFileIteratePixels(D2CmpRgb24RuntimeFilePrefix *pFile, D2CmpPackedColorCallback pVisitColor);       // #10026
extern "C" int32_t __stdcall CELCMP_CelFileSerialize(D2CmpBuiltCellFile *pFile, D2CmpBuiltCellFile **ppSerializedFile, int32_t *pSerializedSize);   // #10023
extern "C" void __stdcall CELCMP_CelFileNormalize(D2CmpBuiltCellFile *pFile, D2CmpBuiltCellFile **ppOutFile, const char *szFile, int32_t nLine, int32_t nSpecVersion, int32_t nUnused);   // #10024
extern "C" void __stdcall CELCMP_CelFileFreeSerialized(void *pSerializedFile);                                                          // #10028
extern "C" void __stdcall CELCMP_CelFileFree(D2CmpBuiltCellFile *pFile);                                                                // #10027
extern "C" void __stdcall CELCMP_CelFileTransform(D2CmpBuiltCellFile *pFile, const uint8_t *pPaletteMap);                      // #10031
extern "C" void __stdcall CELCMP_CelDraw(D2GfxDataStrc *pContext, int32_t nX, int32_t nY, int32_t nClipLeft, int32_t nClipRight,                       // #10033
                                         uint8_t *pDestSurface, int32_t nSurfaceBottom, uint32_t dwStride,
                                         const uint8_t *pPaletteMap, const uint8_t *pBlendTable, const uint8_t *pPreMap);
extern "C" int32_t __stdcall CELCMP_CelGetRawData(D2CmpBuiltCellFile *pFile, uint32_t nDirection64, uint32_t nFrame, uint8_t *pBuffer);   // #10042
extern "C" void __stdcall CELCMP_CelDrawEx(D2GfxDataStrc *pContext, int32_t nX, int32_t nY, int32_t nClipLeft, int32_t nClipRight,                     // #10034
                                           uint8_t *pDestSurface, int32_t nSurfaceBottom, uint32_t dwStride,
                                           const uint8_t *pPaletteMap, const uint8_t *pBlendTable, int32_t nLinesToSkip, int32_t nLinesToDraw,
                                           const uint8_t *pPreMap);
extern "C" void __stdcall CELCMP_CelDrawShadow(D2GfxDataStrc *pContext, int32_t nX, int32_t nY, int32_t nClipLeft, int32_t nClipRight,                 // #10035
                                               uint8_t *pDestSurface, int32_t nSurfaceBottom, uint32_t dwStride, const uint8_t *pBlendTable,
                                               uint8_t nShadowColor);
extern "C" D2CmpRleCellHeader *__stdcall CELCMP_CelGetHandle(D2GfxDataStrc *pContext);                                                  // #10036
extern "C" int32_t __stdcall CELCMP_CelGetWidth(const D2CmpRleCellHeader *pCel);                                                            // #10037
extern "C" int32_t __stdcall CELCMP_CelGetHeight(const D2CmpRleCellHeader *pCel);                                                           // #10038
extern "C" int32_t __stdcall CELCMP_CelGetOffsetX(const D2CmpRleCellHeader *pCel);                                                          // #10039
extern "C" int32_t __stdcall CELCMP_CelGetOffsetY(const D2CmpRleCellHeader *pCel);                                                          // #10040
extern "C" uint8_t *__stdcall CELCMP_CelGetControlData(D2CmpRleCellHeader *pCel);                                               // #10041
extern "C" int32_t __stdcall CELCMP_CelFileGetNumDirections(D2CmpBuiltCellFile *pFile);                                                     // #10047
extern "C" int32_t __stdcall CELCMP_CelFileGetCelsPerDirection(D2CmpBuiltCellFile *pCelFile);                                               // #10046
extern "C" D2CmpCelHardwareFreeCallback __stdcall CELCMP_CelSetHardwareFreeCallback(D2CmpCelHardwareFreeCallback pfnHardwareFree);      // #10044
extern "C" D2CmpCelHardwareFreeCallback __cdecl CELCMP_CelGetHardwareFreeCallback();                                                    // #10045
extern "C" BOOL __stdcall CELCMP_CelFileFreeHardware(D2CmpBuiltCellFile *pFile);                                                        // #10032
extern "C" void *__stdcall CELCMP_CelGetHardwareData(const D2CmpRleCellHeader *pCel);                                                   // #10043
D2CmpCellBuildEntry *__fastcall CELCMP_CellEntryList_NewNode(D2CmpStormList *pList, int32_t nEdxUnused, uint32_t nLocation, uint32_t nExtraBytes, uint32_t dwAllocFlags);
void __fastcall CELCMP_CellBuilderTable_ClearObjects(D2CmpCellBuilderTableHeader *pTable);
void __fastcall CELCMP_CellBuilderTable_InternalDelete(D2CmpCellBuilderTableHeader *pUnusedTable, int32_t nEdxUnused, D2CmpCellBuilder *pBuilder);
D2CmpCellBuilder *__fastcall CELCMP_CellBuilderTable_InternalNew(D2CmpCellBuilderTableHeader *pUnusedTable, int32_t nEdxUnused, D2CmpStormList *pBucket, uint32_t nExtraBytes, uint32_t dwFlags);
void __fastcall CELCMP_CellBuilderTable_BaseDestroy(D2CmpCellBuilderTableHeader *pTable);
void __fastcall CELCMP_CellBuilderTable_ResetAndReleaseBuckets(D2CmpCellBuilderTableHeader *pTable);
D2CmpCellBuilderTableHeader *__fastcall CELCMP_CellBuilderTable_SyncScalarDeletingDestructor(D2CmpCellBuilderTableHeader *pTable, int32_t nEdxUnused, uint8_t nFlags);
D2CmpCellBuilder *__fastcall CELCMP_CellBuilder_ScalarDeletingDestructor(D2CmpCellBuilder *pBuilder, int32_t nEdxUnused, uint8_t nFlags);
D2CmpCellBuilderTableHeader *__fastcall CELCMP_CellBuilderTable_ReuseScalarDeletingDestructor(D2CmpCellBuilderTableHeader *pTable, int32_t nEdxUnused, uint8_t nFlags);
void __fastcall CELCMP_CellBuilderTable_ReuseDestroy(D2CmpCellBuilderTableReusePrefix *pTable);
void __fastcall CELCMP_StormList_UnlinkAllAndTerminator(D2CmpStormList *pList);
D2CmpCellBuilderTableHeader *__fastcall CELCMP_CellBuilderTable_BaseScalarDeletingDestructor(D2CmpCellBuilderTableHeader *pTable, int32_t nEdxUnused, uint8_t nFlags);
void __fastcall CELCMP_StormListArray_DestroyWithUnwind(D2CmpStormListArray *pArray);
void __fastcall CELCMP_StormLink_Unlink(D2CmpStormLink *pLink);
void __fastcall CELCMP_CellBuilder_Destroy(D2CmpCellBuilder *pBuilder);
D2CmpCellBuilder *__fastcall CELCMP_CellBuilderList_NewNode(D2CmpStormList *pList, int32_t nEdxUnused, uint32_t dwLocation, uint32_t nExtraBytes, uint32_t dwFlags);
void __fastcall CELCMP_StormListArray_Destroy(D2CmpStormListArray *pArray);
D2CmpCellBuilderTableHeader *__fastcall CELCMP_CellBuilderTable_ExportScalarDeletingDestructor(D2CmpCellBuilderTableHeader *pTable, int32_t nEdxUnused, uint8_t nFlags);
void __fastcall CELCMP_CellBuilderTable_ExportDestroy(D2CmpCellBuilderTableReusePrefix *pTable);
D2CmpCellBuilder *__fastcall CELCMP_CellBuilderTable_New(D2CmpCellBuilderTableHeader *pTable, int32_t nEdxUnused, uint32_t dwHandleHash, const uint8_t *pKey, uint32_t nExtraBytes, uint32_t dwAllocFlags);
D2CmpCellBuilder *__fastcall CELCMP_CellBuilderTable_Find(D2CmpCellBuilderTableHeader *pTable, int32_t nEdxUnused, uint32_t dwHandleHash, const uint8_t *pUnusedKey);
D2CmpStormLink *__fastcall CELCMP_StormList_GetLink(D2CmpStormList *pList, int32_t nEdxUnused, const void *pNode);
void __fastcall CELCMP_StormList_UnlinkAll(D2CmpStormList *pList);
D2CmpStormList *__fastcall CELCMP_StormList_ScalarDeletingDestructor(D2CmpStormList *pList, int32_t nEdxUnused, uint8_t bDeleteFlags);
void __fastcall CELCMP_CellBuilderObjectChunk_Destroy(D2CmpCellBuilderObjectChunk *pChunk);
void __fastcall CELCMP_CellBuilder_UnlinkHashLinks(D2CmpCellBuilder *pBuilder);
void *__fastcall CELCMP_StormLink_GetNext(D2CmpStormLink *pLink);
D2CmpStormLink *__fastcall CELCMP_StormList_GetEmbeddedLink(D2CmpStormList *pList, int32_t nEdxUnused, void *pNode);
D2CmpStormLink *__fastcall CELCMP_StormLink_GetNextLink(D2CmpStormLink *pLink, int32_t nEdxUnused, int32_t nLinkOffset);
D2CmpStormLink *__fastcall CELCMP_StormExplicitList_GetLink(const void *pNode, int32_t nLinkOffset);
void __fastcall CELCMP_CellBuilderTable_Rehash(D2CmpCellBuilderTableHeader *pTable);
void __fastcall CELCMP_StormList_InitializeTerminator(D2CmpStormList *pList);
void __fastcall CELCMP_StormListArray_SetCount(D2CmpStormListArray *pArray, int32_t nEdxUnused, uint32_t dwNewCount);
void __fastcall CELCMP_StormList_InitializePoisonedOffsetIfNonNull(D2CmpStormList *pList);
void __fastcall CELCMP_StormList_InitializeOffsetFromSourceIfNonNull(D2CmpStormList *pList, const D2CmpStormList *pSource);
uint32_t __cdecl CELCMP_FindFirstSetBit1Based(uint32_t dwMask);

//  D2CMP 0x6FE12CA8
//  The cell builders' table, CELLIST objects under handles: Storm's TSExportTableSyncReuse as the RTTI tags name it. Zero at load, as any object at
//  namespace scope; built by CelCmp.cpp's sgCellBuilderTableLifetime and torn down by the same object.
inline D2CmpCellBuilderTablePrefix gCellBuilderTable = {};

//  D2CMP 0x6FE0B900
//  The table's class chain has four vtables, each {InternalDelete, InternalNew, the scalar deleting destructor, Clear/Destroy}; slots 0 and 1 are
//  the same two functions in all four. This one, the hash table's base, is stored by BaseDestroy and by the derived destructors as they unwind to
//  it.
inline const D2CmpCellBuilderTableVtable gVtblCellBuilderTableBase = {
    &CELCMP_CellBuilderTable_InternalDelete,
    &CELCMP_CellBuilderTable_InternalNew,
    &CELCMP_CellBuilderTable_BaseScalarDeletingDestructor,
    &CELCMP_CellBuilderTable_ResetAndReleaseBuckets,
};

//  D2CMP 0x6FE0B910
//  TSHashTableReuse's vtable: stored by the destructors that unwind through it.
inline const D2CmpCellBuilderTableVtable gVtblCellBuilderTableReuse = {
    &CELCMP_CellBuilderTable_InternalDelete,
    &CELCMP_CellBuilderTable_InternalNew,
    &CELCMP_CellBuilderTable_ReuseScalarDeletingDestructor,
    &CELCMP_CellBuilderTable_ClearObjects,
};

//  D2CMP 0x6FE0B920
//  TSExportTableSyncReuse's vtable, the live table's: the last of the original constructor's four stores, and the one our construction installs.
inline const D2CmpCellBuilderTableVtable gVtblCellBuilderTableSync = {
    &CELCMP_CellBuilderTable_InternalDelete,
    &CELCMP_CellBuilderTable_InternalNew,
    &CELCMP_CellBuilderTable_SyncScalarDeletingDestructor,
    &CELCMP_CellBuilderTable_ClearObjects,
};

//  D2CMP 0x6FE091B0
//  The four format tables (this one, sgpfnDecodeFunc, gpfnCelConvert24To8 and gpfnCelTransform) hold FastCmp.cpp's function for the DC6 format in
//  slot 0 and nothing in slot 1; the format a cel file or builder names indexes them. This one encodes a builder's frames
//  (CelCompressCreateCelFile).
inline const D2CmpCellBuilderEncoder gpCellBuilderEncoders[2] = { &FASTCMP_EncodeIndexedCellFrames, NULL };

//  D2CMP 0x6FE091B8
//  The decoders by format; the name is the original's own, from CelDraw's assert "sgpfnDecodeFunc[eFormat]".
inline const D2CmpCelDecodeFunc sgpfnDecodeFunc[2] = { &FASTCMP_DrawDC6, NULL };

//  D2CMP 0x6FE091C0
//  The RGB24-to-indexed converters by format (CelFileConvert24To8).
inline const D2CmpCelConvert24To8Func gpfnCelConvert24To8[2] = { &FASTCMP_ConvertRGB24CellFramesToIndexed, NULL };

//  D2CMP 0x6FE091C8
//  The palette transforms by format (CelFileTransform).
inline const D2CmpCelTransformFunc gpfnCelTransform[2] = { &FASTCMP_TransformDC6PaletteInPlace, NULL };

//  D2CMP 0x6FE09550
//  CelNormalDirToFileDir's table, indexed [32 * (the 1-based first set bit of the direction count) + the direction]. Its own rows are 0..6 (the
//  counts 1..32); rows 7..22 are a second table at 0x6FE098D0 that no instruction reads, eight rows of 64 shaped like the tables that follow (64
//  directions to 1..64). Both are one array here, the span up to the next table, so that a count above 32 (the function asserts nothing) reads what
//  the original reads instead of past the end of ours. Const, in .rdata, as every table below.
inline const uint32_t gnDirection32ToFrameIndex[736] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 5, 6, 7, 0, 2, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 5, 6, 7, 0, 2, 1, 3, 8, 9, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 5, 6, 7, 0, 2, 1, 3, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 1, 1,
    1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 3, 3, 3, 3, 3, 3, 3, 3, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0,
    0, 0, 8, 8, 8, 8, 4, 4, 4, 4, 9, 9, 9, 9, 2, 2, 2, 2, 10, 10, 10, 10, 5, 5, 5, 5, 11, 11, 11, 11, 1, 1,
    1, 1, 12, 12, 12, 12, 6, 6, 6, 6, 13, 13, 13, 13, 3, 3, 3, 3, 14, 14, 14, 14, 7, 7, 7, 7, 15, 15, 15, 15, 0, 0,
    0, 16, 16, 8, 8, 17, 17, 4, 4, 18, 18, 9, 9, 19, 19, 2, 2, 20, 20, 10, 10, 21, 21, 5, 5, 22, 22, 11, 11, 23, 23, 1,
    1, 24, 24, 12, 12, 25, 25, 6, 6, 26, 26, 13, 13, 27, 27, 3, 3, 28, 28, 14, 14, 29, 29, 7, 7, 30, 30, 15, 15, 31, 31, 0,
    0, 32, 16, 33, 8, 34, 17, 35, 4, 36, 18, 37, 9, 38, 19, 39, 2, 40, 20, 41, 10, 42, 21, 43, 5, 44, 22, 45, 11, 46, 23, 47,
    1, 48, 24, 49, 12, 50, 25, 51, 6, 52, 26, 53, 13, 54, 27, 55, 3, 56, 28, 57, 14, 58, 29, 59, 7, 60, 30, 61, 15, 62, 31, 63,
};

//  D2CMP 0x6FE0A0D0
//  CelGameDirToFileDir's (#10018), CelGetRawData's and CelGetHandle's table, indexed [64 * (the 1-based first set bit of the count) + the
//  64-direction]. Rows 0..2 are all zero.
inline const uint32_t gnDirection64ToFrameIndex[512] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6,
    6, 6, 6, 6, 2, 2, 2, 2, 2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 7, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4,
    4, 4, 8, 8, 8, 8, 0, 0, 0, 0, 9, 9, 9, 9, 5, 5, 5, 5, 10, 10, 10, 10, 1, 1, 1, 1, 11, 11, 11, 11, 6, 6,
    6, 6, 12, 12, 12, 12, 2, 2, 2, 2, 13, 13, 13, 13, 7, 7, 7, 7, 14, 14, 14, 14, 3, 3, 3, 3, 15, 15, 15, 15, 4, 4,
    4, 16, 16, 8, 8, 17, 17, 0, 0, 18, 18, 9, 9, 19, 19, 5, 5, 20, 20, 10, 10, 21, 21, 1, 1, 22, 22, 11, 11, 23, 23, 6,
    6, 24, 24, 12, 12, 25, 25, 2, 2, 26, 26, 13, 13, 27, 27, 7, 7, 28, 28, 14, 14, 29, 29, 3, 3, 30, 30, 15, 15, 31, 31, 4,
    4, 32, 16, 33, 8, 34, 17, 35, 0, 36, 18, 37, 9, 38, 19, 39, 5, 40, 20, 41, 10, 42, 21, 43, 1, 44, 22, 45, 11, 46, 23, 47,
    6, 48, 24, 49, 12, 50, 25, 51, 2, 52, 26, 53, 13, 54, 27, 55, 7, 56, 28, 57, 14, 58, 29, 59, 3, 60, 30, 61, 15, 62, 31, 63,
};

//  D2CMP 0x6FE0A8D0
//  CelGameDirToShadowGameDir's table (#10021), indexed [64 * (the 1-based first set bit of the rotation) + the game direction].
inline const int32_t gnCelShadowGameDirection[512] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 32, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 16, 16, 16, 16, 16, 16, 16, 16, 24, 24, 24, 24, 24, 24, 24, 24, 32, 32, 32, 32,
    32, 32, 32, 32, 40, 40, 40, 40, 40, 40, 40, 40, 48, 48, 48, 48, 48, 48, 48, 48, 56, 56, 56, 56, 56, 56, 56, 56, 0, 0, 0, 0,
    4, 4, 8, 8, 8, 8, 12, 12, 12, 12, 16, 16, 16, 16, 20, 20, 20, 20, 24, 24, 24, 24, 28, 28, 28, 28, 32, 32, 32, 32, 36, 36,
    36, 36, 40, 40, 40, 40, 44, 44, 44, 44, 48, 48, 48, 48, 52, 52, 52, 52, 56, 56, 56, 56, 60, 60, 60, 60, 0, 0, 0, 0, 4, 4,
    2, 4, 4, 6, 6, 8, 8, 10, 10, 12, 12, 14, 14, 16, 16, 18, 18, 20, 20, 22, 22, 24, 24, 26, 26, 28, 28, 30, 30, 32, 32, 34,
    34, 36, 36, 38, 38, 40, 40, 42, 42, 44, 44, 46, 46, 48, 48, 50, 50, 52, 52, 54, 54, 56, 56, 58, 58, 60, 60, 62, 62, 0, 0, 2,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

//  D2CMP 0x6FE0B0D0
//  CelGameDirToNormalDir's table (#10022), of the same shape.
inline const int32_t gnCelNormalDirection[512] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4,
    4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8,
    8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 0, 0,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16,
    16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
};

//  D2CMP 0x6FE0B8D0..0x6FE0B8FF
//  CelConvertDir's (#10019) four byte tables, 8 directions to 4 and 16 to 8, the default and the alternate.
inline const uint8_t gCelDirection8To4Default[8] = { 1, 1, 3, 3, 5, 5, 7, 7 };
inline const uint8_t gCelDirection16To8Default[16] = { 0, 0, 2, 2, 4, 4, 6, 6, 8, 8, 10, 10, 12, 12, 14, 14 };
inline const uint8_t gCelDirection8To4Alternate[8] = { 3, 3, 5, 5, 7, 7, 1, 1 };
inline const uint8_t gCelDirection16To8Alternate[16] = { 2, 2, 4, 4, 6, 6, 8, 8, 10, 10, 12, 12, 14, 14, 0, 0 };

//  D2CMP 0x6FE12D10
//  Set by #10044, read by #10045 and CelFileFreeHardware; zero at load.
inline D2CmpCelHardwareFreeCallback gCelHardwareFreeCallback = NULL;
