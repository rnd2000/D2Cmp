#pragma once

/****************************************************************************
*                                                                           *
*   D2Ptrs.h                                                                *
*                                                                           *
*   Licensed under the Apache License, Version 2.0 (the "License");         *
*   you may not use this file except in compliance with the License.        *
*   You may obtain a copy of the License at                                 *
*                                                                           *
*   http://www.apache.org/licenses/LICENSE-2.0                              *
*                                                                           *
*   Unless required by applicable law or agreed to in writing, software     *
*   distributed under the License is distributed on an "AS IS" BASIS,       *
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
*   See the License for the specific language governing permissions and     *
*   limitations under the License.                                          *
*                                                                           *
*---------------------------------------------------------------------------*
*                                                                           *
*   https://github.com/olivier-verville/D2Template                          *
*                                                                           *
*   This file is used to declare pointers, be it function or variable       *
*   pointers, from the game's libraries.                                    *
*                                                                           *
*   It is recommended that you keep this file organized by which dll the    *
*   pointer is imported from, what type of pointer it is, etc. Ordering     *
*   them by address can also end up being very useful                       *
*                                                                           *
*****************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  These are the macros used by the template core to declare                                                                                                                                                                   ///
//  pointers. Do not touch unless you know what you're doing                                                                                                                                                                    ///
//                                                                                                                                                                                                                              ///
#ifdef _MSC_VER // MS Compiler                                                                                                                                                                                                  ///
#define D2FUNC(DLL, NAME, RETURN, CONV, ARGS, OFFSET) typedef RETURN (CONV* DLL##_##NAME##_t) ARGS; inline DLL##_##NAME##_t DLL##_##NAME = (DLL##_##NAME##_t)GetDllOffset(#DLL, DLLBASE_##DLL, OFFSET);   ///
#define D2VAR(DLL, NAME, TYPE, OFFSET) typedef TYPE DLL##_##NAME##_vt; inline DLL##_##NAME##_vt * DLL##_##NAME = (DLL##_##NAME##_vt *)GetDllOffset(#DLL, DLLBASE_##DLL, OFFSET);                          ///
#define D2PTR(DLL, NAME, OFFSET) inline DWORD NAME = GetDllOffset(#DLL, DLLBASE_##DLL, OFFSET);                                                                                                           ///
#else // GCC Compiler                                                                                                                                                                                                           ///
#define D2FUNC(DLL, NAME, RETURN, CONV, ARGS, OFFSET) typedef RETURN (CONV* DLL##_##NAME##_t) ARGS; DLL##_##NAME##_t DLL##_##NAME __attribute__((weak)) = (DLL##_##NAME##_t)GetDllOffset(#DLL, DLLBASE_##DLL, OFFSET);          ///
#define D2VAR(DLL, NAME, TYPE, OFFSET) typedef TYPE DLL##_##NAME##_vt; DLL##_##NAME##_vt * DLL##_##NAME __attribute__((weak)) = (DLL##_##NAME##_vt *)GetDllOffset(#DLL, DLLBASE_##DLL, OFFSET);                                 ///
#define D2PTR(DLL, NAME, OFFSET) DWORD NAME __attribute__((weak)) = GetDllOffset(#DLL, DLLBASE_##DLL, OFFSET);                                                                                                              ///
#endif 

extern DWORD GetDllOffset(const char* ModuleName, DWORD BaseAddress, int Offset); 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
//  GENERATED from the reverse-engineering database of D2CMP.dll 1.10 (image base 0x6FDF0000).
//
//  Every game function of the original, at its RVA, or by its export ordinal where the last argument
//  is negative (GetDllOffset resolves it through GetProcAddress). Every line is commented out: this
//  source implements each function, and calls Fog and Storm through D2Imports.h and Windows directly.
//  The VC6 runtime the original was linked with is not listed. Ordered by address.
//
///////////////////////////////////////////////////////////////////////////////

//D2FUNC(D2CMP, CELCMP_CellBuilderTable_SyncDestroy, void, __thiscall, (D2CmpCellBuilderTablePrefix *pTable), 0x1030)
//D2FUNC(D2CMP, CELCMP_CelNormalDirToFileDir, uint32_t, __stdcall, (uint32_t nDirection32, uint32_t nDirectionCount), -10017)
//D2FUNC(D2CMP, CELCMP_CelGameDirToFileDir, uint32_t, __stdcall, (uint32_t nDirection64, uint32_t nDirectionCount), -10018)
//D2FUNC(D2CMP, CELCMP_CelConvertDir, uint32_t, __stdcall, (uint32_t nDirection, uint32_t nStartDirections, uint32_t nRepresentativeDirections, int32_t bAlternate), -10019)
//D2FUNC(D2CMP, CELCMP_CelConvertBlock, int32_t, __stdcall, (int32_t nSourceDirections, int32_t nTargetDirections), -10020)
//D2FUNC(D2CMP, CELCMP_CelGameDirToShadowGameDir, int32_t, __stdcall, (uint32_t nGameDirection, uint32_t nLevelOfRotation), -10021)
//D2FUNC(D2CMP, CELCMP_CelGameDirToNormalDir, int32_t, __stdcall, (uint32_t nGameDirection, uint32_t nLevelOfRotation), -10022)
//D2FUNC(D2CMP, CELCMP_CelCompressCreate, uint32_t, __stdcall, (int32_t nFormat, uint32_t nDirections, const uint32_t *pTransparentColor, int32_t bRGB24), -10013)
//D2FUNC(D2CMP, CELCMP_CelCompressAddImage, void, __stdcall, (uint32_t hBuilder, const uint8_t *pImage, uint32_t nImageStride, uint32_t nPixelHeight, uint32_t nPixelWidth, int32_t nOffsetX, int32_t nOffsetY, int32_t nOrientation), -10015)
//D2FUNC(D2CMP, CELCMP_CelCompressDestroy, void, __stdcall, (uint32_t hBuilder), -10014)
//D2FUNC(D2CMP, CELCMP_CelCompressCreateCelFile, uint32_t, __stdcall, (uint32_t hBuilder, D2CmpBuiltCellFile **ppOutput), -10016)
//D2FUNC(D2CMP, CELCMP_CelFileHalve, void, __stdcall, (int32_t hFile, int32_t nUnused), -10029)
//D2FUNC(D2CMP, CELCMP_CelFileConvert24To8, int32_t, __stdcall, (D2CmpBuiltCellFile *pSrc, D2CmpBuiltCellFile **ppOutFile, uint8_t (__stdcall *pfMapRgbToIndex)(uint8_t nRed, uint8_t nGreen, uint8_t nBlue), uint32_t dwTransparentColor), -10025)
//D2FUNC(D2CMP, CELCMP_CelFileConvertOldToNew, void, __stdcall, (D2CmpSerializedCellFileV5Header *pOldFile, D2CmpBuiltCellFile **ppNewFile, int32_t *pNewFileSize), -10030)
//D2FUNC(D2CMP, CELCMP_CelFileIteratePixels, void, __stdcall, (D2CmpRgb24RuntimeFilePrefix *pFile, D2CmpPackedColorCallback pVisitColor), -10026)
//D2FUNC(D2CMP, CELCMP_CelFileSerialize, int32_t, __stdcall, (D2CmpBuiltCellFile *pFile, D2CmpBuiltCellFile **ppSerializedFile, int32_t *pSerializedSize), -10023)
//D2FUNC(D2CMP, CELCMP_CelFileNormalize, void, __stdcall, (D2CmpBuiltCellFile *pFile, D2CmpBuiltCellFile **ppOutFile, const char *szFile, int32_t nLine, int32_t nSpecVersion, int32_t nUnused), -10024)
//D2FUNC(D2CMP, CELCMP_CelFileFreeSerialized, void, __stdcall, (void *pSerializedFile), -10028)
//D2FUNC(D2CMP, CELCMP_CelFileFree, void, __stdcall, (D2CmpBuiltCellFile *pFile), -10027)
//D2FUNC(D2CMP, CELCMP_CelFileTransform, void, __stdcall, (D2CmpBuiltCellFile *pFile, const uint8_t *pPaletteMap), -10031)
//D2FUNC(D2CMP, CELCMP_CelDraw, void, __stdcall, (D2GfxDataStrc *pContext, int32_t nX, int32_t nY, int32_t nClipLeft, int32_t nClipRight, uint8_t *pDestSurface, int32_t nSurfaceBottom, uint32_t dwStride, const uint8_t *pPaletteMap, const uint8_t *pBlendTable, const uint8_t *pPreMap), -10033)
//D2FUNC(D2CMP, CELCMP_CelGetRawData, int32_t, __stdcall, (D2CmpBuiltCellFile *pFile, uint32_t nDirection64, uint32_t nFrame, uint8_t *pBuffer), -10042)
//D2FUNC(D2CMP, CELCMP_CelDrawEx, void, __stdcall, (D2GfxDataStrc *pContext, int32_t nX, int32_t nY, int32_t nClipLeft, int32_t nClipRight, uint8_t *pDestSurface, int32_t nSurfaceBottom, uint32_t dwStride, const uint8_t *pPaletteMap, const uint8_t *pBlendTable, int32_t nLinesToSkip, int32_t nLinesToDraw, const uint8_t *pPreMap), -10034)
//D2FUNC(D2CMP, CELCMP_CelDrawShadow, void, __stdcall, (D2GfxDataStrc *pContext, int32_t nX, int32_t nY, int32_t nClipLeft, int32_t nClipRight, uint8_t *pDestSurface, int32_t nSurfaceBottom, uint32_t dwStride, const uint8_t *pBlendTable, uint8_t nShadowColor), -10035)
//D2FUNC(D2CMP, CELCMP_CelGetHandle, D2CmpRleCellHeader *, __stdcall, (D2GfxDataStrc *pContext), -10036)
//D2FUNC(D2CMP, CELCMP_CelGetWidth, int32_t, __stdcall, (const D2CmpRleCellHeader *pCel), -10037)
//D2FUNC(D2CMP, CELCMP_CelGetHeight, int32_t, __stdcall, (const D2CmpRleCellHeader *pCel), -10038)
//D2FUNC(D2CMP, CELCMP_CelGetOffsetX, int32_t, __stdcall, (const D2CmpRleCellHeader *pCel), -10039)
//D2FUNC(D2CMP, CELCMP_CelGetOffsetY, int32_t, __stdcall, (const D2CmpRleCellHeader *pCel), -10040)
//D2FUNC(D2CMP, CELCMP_CelGetControlData, uint8_t *, __stdcall, (D2CmpRleCellHeader *pCel), -10041)
//D2FUNC(D2CMP, CELCMP_CelFileGetNumDirections, int32_t, __stdcall, (D2CmpBuiltCellFile *pFile), -10047)
//D2FUNC(D2CMP, CELCMP_CelFileGetCelsPerDirection, int32_t, __stdcall, (D2CmpBuiltCellFile *pCelFile), -10046)
//D2FUNC(D2CMP, CELCMP_CelSetHardwareFreeCallback, D2CmpCelHardwareFreeCallback, __stdcall, (D2CmpCelHardwareFreeCallback pfnHardwareFree), -10044)
//D2FUNC(D2CMP, CELCMP_CelGetHardwareFreeCallback, D2CmpCelHardwareFreeCallback, __cdecl, (), -10045)
//D2FUNC(D2CMP, CELCMP_CelFileFreeHardware, BOOL, __stdcall, (D2CmpBuiltCellFile *pFile), -10032)
//D2FUNC(D2CMP, CELCMP_CelGetHardwareData, void *, __stdcall, (const D2CmpRleCellHeader *pCel), -10043)
//D2FUNC(D2CMP, CELCMP_CellEntryList_NewNode, D2CmpCellBuildEntry *, __thiscall, (D2CmpStormList *pList, uint32_t nLocation, uint32_t nExtraBytes, uint32_t dwAllocFlags), 0x2850)
//D2FUNC(D2CMP, CELCMP_CellBuilderTable_ClearObjects, void, __thiscall, (D2CmpCellBuilderTableHeader *pTable), 0x2A60)
//D2FUNC(D2CMP, CELCMP_CellBuilderTable_InternalDelete, void, __thiscall, (D2CmpCellBuilderTableHeader *pUnusedTable, D2CmpCellBuilder *pBuilder), 0x2AF0)
//D2FUNC(D2CMP, CELCMP_CellBuilderTable_InternalNew, D2CmpCellBuilder *, __thiscall, (D2CmpCellBuilderTableHeader *pUnusedTable, D2CmpStormList *pBucket, uint32_t nExtraBytes, uint32_t dwFlags), 0x2B10)
//D2FUNC(D2CMP, CELCMP_CellBuilderTable_BaseDestroy, void, __thiscall, (D2CmpCellBuilderTableHeader *pTable), 0x2B30)
//D2FUNC(D2CMP, CELCMP_CellBuilderTable_ResetAndReleaseBuckets, void, __thiscall, (D2CmpCellBuilderTableHeader *pTable), 0x2CB0)
//D2FUNC(D2CMP, CELCMP_CellBuilderTable_SyncScalarDeletingDestructor, D2CmpCellBuilderTableHeader *, __thiscall, (D2CmpCellBuilderTableHeader *pTable, uint8_t nFlags), 0x2D90)
//D2FUNC(D2CMP, CELCMP_CellBuilder_ScalarDeletingDestructor, D2CmpCellBuilder *, __thiscall, (D2CmpCellBuilder *pBuilder, uint8_t nFlags), 0x2DC0)
//D2FUNC(D2CMP, CELCMP_CellBuilderTable_ReuseScalarDeletingDestructor, D2CmpCellBuilderTableHeader *, __thiscall, (D2CmpCellBuilderTableHeader *pTable, uint8_t nFlags), 0x2DF0)
//D2FUNC(D2CMP, CELCMP_CellBuilderTable_ReuseDestroy, void, __thiscall, (D2CmpCellBuilderTableReusePrefix *pTable), 0x2E20)
//D2FUNC(D2CMP, CELCMP_StormList_UnlinkAllAndTerminator, void, __thiscall, (D2CmpStormList *pList), 0x2FB0)
//D2FUNC(D2CMP, CELCMP_CellBuilderTable_BaseScalarDeletingDestructor, D2CmpCellBuilderTableHeader *, __thiscall, (D2CmpCellBuilderTableHeader *pTable, uint8_t nFlags), 0x3030)
//D2FUNC(D2CMP, CELCMP_StormListArray_DestroyWithUnwind, void, __thiscall, (D2CmpStormListArray *pArray), 0x3060)
//D2FUNC(D2CMP, CELCMP_StormLink_Unlink, void, __thiscall, (D2CmpStormLink *pLink), 0x3110)
//D2FUNC(D2CMP, CELCMP_CellBuilder_Destroy, void, __thiscall, (D2CmpCellBuilder *pBuilder), 0x3150)
//D2FUNC(D2CMP, CELCMP_CellBuilderList_NewNode, D2CmpCellBuilder *, __thiscall, (D2CmpStormList *pList, uint32_t dwLocation, uint32_t nExtraBytes, uint32_t dwFlags), 0x3240)
//D2FUNC(D2CMP, CELCMP_StormListArray_Destroy, void, __thiscall, (D2CmpStormListArray *pArray), 0x3330)
//D2FUNC(D2CMP, CELCMP_CellBuilderTable_ExportScalarDeletingDestructor, D2CmpCellBuilderTableHeader *, __thiscall, (D2CmpCellBuilderTableHeader *pTable, uint8_t nFlags), 0x3370)
//D2FUNC(D2CMP, CELCMP_CellBuilderTable_ExportDestroy, void, __thiscall, (D2CmpCellBuilderTableReusePrefix *pTable), 0x33A0)
//D2FUNC(D2CMP, CELCMP_CellBuilderTable_New, D2CmpCellBuilder *, __thiscall, (D2CmpCellBuilderTableHeader *pTable, uint32_t dwHandleHash, const uint8_t *pKey, uint32_t nExtraBytes, uint32_t dwAllocFlags), 0x3500)
//D2FUNC(D2CMP, CELCMP_CellBuilderTable_Find, D2CmpCellBuilder *, __thiscall, (D2CmpCellBuilderTableHeader *pTable, uint32_t dwHandleHash, const uint8_t *pUnusedKey), 0x3610)
//D2FUNC(D2CMP, CELCMP_StormList_GetLink, D2CmpStormLink *, __thiscall, (D2CmpStormList *pList, const void *pNode), 0x3660)
//D2FUNC(D2CMP, CELCMP_StormList_UnlinkAll, void, __thiscall, (D2CmpStormList *pList), 0x3680)
//D2FUNC(D2CMP, CELCMP_StormList_ScalarDeletingDestructor, D2CmpStormList *, __thiscall, (D2CmpStormList *pList, uint8_t bDeleteFlags), 0x36D0)
//D2FUNC(D2CMP, CELCMP_CellBuilderObjectChunk_Destroy, void, __thiscall, (D2CmpCellBuilderObjectChunk *pChunk), 0x3700)
//D2FUNC(D2CMP, CELCMP_CellBuilder_UnlinkHashLinks, void, __thiscall, (D2CmpCellBuilder *pBuilder), 0x3800)
//D2FUNC(D2CMP, CELCMP_StormLink_GetNext, void *, __thiscall, (D2CmpStormLink *pLink), 0x3870)
//D2FUNC(D2CMP, CELCMP_StormList_GetEmbeddedLink, D2CmpStormLink *, __thiscall, (D2CmpStormList *pList, void *pNode), 0x3880)
//D2FUNC(D2CMP, CELCMP_StormLink_GetNextLink, D2CmpStormLink *, __thiscall, (D2CmpStormLink *pLink, int32_t nLinkOffset), 0x3910)
//D2FUNC(D2CMP, CELCMP_StormExplicitList_GetLink, D2CmpStormLink *, __fastcall, (const void *pNode, int32_t nLinkOffset), 0x3940)
//D2FUNC(D2CMP, CELCMP_CellBuilderTable_Rehash, void, __thiscall, (D2CmpCellBuilderTableHeader *pTable), 0x3950)
//D2FUNC(D2CMP, CELCMP_StormList_InitializeTerminator, void, __thiscall, (D2CmpStormList *pList), 0x3B40)
//D2FUNC(D2CMP, CELCMP_StormListArray_SetCount, void, __thiscall, (D2CmpStormListArray *pArray, uint32_t dwNewCount), 0x3B50)
//D2FUNC(D2CMP, CELCMP_StormList_InitializePoisonedOffsetIfNonNull, void, __fastcall, (D2CmpStormList *pList), 0x3CC0)
//D2FUNC(D2CMP, CELCMP_StormList_InitializeOffsetFromSourceIfNonNull, void, __fastcall, (D2CmpStormList *pList, const D2CmpStormList *pSource), 0x3CF0)
//D2FUNC(D2CMP, CELDATAHASH_Clear, void, __fastcall, (D2CmpCelHeaderHash *pHash), 0x3D10)
//D2FUNC(D2CMP, CELDATAHASH_FindEntry, D2CmpCelHeaderHashEntry *, __fastcall, (D2CmpCelHeaderHash *pHash, const char *szKey), 0x3D20)
//D2FUNC(D2CMP, CELDATAHASH_NewEntry, D2CmpCelHeaderHashEntry *, __fastcall, (void *pPool, D2CmpCelHeaderHash *pHash, D2CmpLruEntry *pLruEntry, const char *szKey), 0x3DC0)
//D2FUNC(D2CMP, CELDATAHASH_DeleteEntry, void, __fastcall, (void *pPool, D2CmpCelHeaderHash *pHash, D2CmpCelHeaderHashEntry *ptElement), 0x3E60)
//D2FUNC(D2CMP, CELDATAHASH_10106, void, __fastcall, (void *pHash), -10103)
//D2FUNC(D2CMP, CODEC_EncodeDccFrameRle, int32_t, __fastcall, (D2CmpDccFrameState *pFrame, D2CmpSpriteFrameHeaderPrefix *pOutputFrame, uint8_t *pDestination), 0x3EE0)
//D2FUNC(D2CMP, CODEC_ReadDccHeader, int32_t, __fastcall, (const char *szPath, D2CmpSpriteHeaderData *pHeader), 0x4050)
//D2FUNC(D2CMP, CODEC_DecodeDccDirections, void *, __fastcall, (void *pInput, void *pPool, int32_t nInputBytes, D2CmpSpriteHeaderData *pHeader, D2CmpSpriteGfxBlockPrefix *pGfxBlock, int32_t nStartDirection, int32_t nDirections, uint32_t *pDecodedBytes, const char *szKey), 0x4160)
//D2FUNC(D2CMP, CODEC_SkipBits, void, __fastcall, (D2CmpCodecBitReader *pReader, int32_t nBits), 0x4F10)
//D2FUNC(D2CMP, CODEC_AlignReaderToByte, void, __fastcall, (D2CmpCodecBitReader *pReader), 0x4F40)
//D2FUNC(D2CMP, CODEC_ReadBits, uint32_t, __fastcall, (D2CmpCodecBitReader *pReader, int32_t nBits), 0x4F60)
//D2FUNC(D2CMP, CODEC_AdvanceDccCell, int32_t, __fastcall, (D2CmpDccDirectionContext *pDirection, D2CmpDccCellCursor *pCursor), 0x4FE0)
//D2FUNC(D2CMP, CODEC_StartDccCells, int32_t, __fastcall, (D2CmpDccDirectionContext *pDirection, D2CmpDccCellCursor *pCursor), 0x51B0)
//D2FUNC(D2CMP, CODEC_BuildDccPaletteMaps, void, __fastcall, (const uint8_t *pPresence, D2CmpDccPaletteMap *pForward, D2CmpDccPaletteMap *pInverse), 0x51E0)
//D2FUNC(D2CMP, CODEC_RasterizeDccCell, void, __fastcall, (D2CmpDccCellState *pCurrent, D2CmpCodecBitReader *pPixels, D2CmpDccCellState *pPrevious, D2CmpCodecBitReader *pEqualCells, uint8_t **ppPaletteCursor), 0x5220)
//D2FUNC(D2CMP, CODEC_DecodeDccCodebooks, void, __fastcall, (D2CmpDccDirectionContext *pDirection, D2CmpDccCellState *pHistory, D2CmpCodecBitReader *pEqualCells, D2CmpCodecBitReader *pMasks, D2CmpCodecBitReader *pEncodingTypes, D2CmpCodecBitReader *pRawCodes, D2CmpCodecBitReader *pPixelCodes, D2CmpCodecByteSpan *pCodebooks, int32_t *pConsumedPixelBits), 0x5520)
//D2FUNC(D2CMP, CODEC_RemapDccCodebooks, void, __fastcall, (D2CmpCodecByteSpan *pCodebooks, const uint8_t *pInverseMap), 0x59E0)
//D2FUNC(D2CMP, CODEC_InitCriticalSection, void, __cdecl, (), 0x5A10)
//D2FUNC(D2CMP, CODEC_ShutdownCriticalSection, void, __cdecl, (), 0x5A30)
//D2FUNC(D2CMP, COUNT_CountConsecutiveFirst, int32_t, __fastcall, (const uint8_t *pSrc, const uint8_t *pEnd, int32_t nMaxCount, int32_t *pbCountEqualsDist, int32_t *pbIsRemainingSame), 0x5A60)
//D2FUNC(D2CMP, COUNT_CountConsecutive, int32_t, __fastcall, (uint8_t bValue, const uint8_t *pSrc, const uint8_t *pEnd, int32_t nMaxCount, int32_t *pbCountEqualsDist, int32_t *pbIsRemainingSame), 0x5AF0)
//D2FUNC(D2CMP, COUNT_CountConsecutiveDiff, int32_t, __fastcall, (const uint8_t *pSrc, const uint8_t *pEnd, uint8_t bValue, int32_t nMaxCount, int32_t *pbCountEqualsDist, int32_t *pbIsRemainingSame), 0x5B70)
//D2FUNC(D2CMP, FASTCMP_EncodeIndexedCellFrames, void, __fastcall, (D2CmpCellBuilder *pBuilder, D2CmpBuiltCellFile *pOutput), 0x5C10)
//D2FUNC(D2CMP, FASTCMP_EncodeDC6RlePixels, void, __fastcall, (const uint8_t *pPixels, int32_t nPixelWidth, uint32_t nPixelHeight, uint8_t nOrientationFlags, uint8_t **ppEncoded, uint32_t *pnEncodedBytes), 0x5D60)
//D2FUNC(D2CMP, FASTCMP_TransformDC6PaletteInPlace, void, __fastcall, (D2CmpRleCellHeader *pCell, const uint8_t *pPaletteMap), 0x5E60)
//D2FUNC(D2CMP, FASTCMP_ConvertRGB24CellFramesToIndexed, void, __fastcall, (const D2CmpBuiltCellFile *pSrc, D2CmpBuiltCellFile *pDst, uint8_t (__stdcall *pfMapRgbToIndex)(uint8_t nRed, uint8_t nGreen, uint8_t nBlue), uint32_t dwTransparentColor), 0x5EA0)
//D2FUNC(D2CMP, FASTCMP_DC6CopyClipped, void, __fastcall, (const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight), 0x6080)
//D2FUNC(D2CMP, FASTCMP_DC6MapBlend, void, __fastcall, (const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pPaletteMap, const uint8_t *pBlendTable), 0x6130)
//D2FUNC(D2CMP, FASTCMP_DC6MapBlendClipped, void, __fastcall, (const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pPaletteMap, const uint8_t *pBlendTable), 0x6390)
//D2FUNC(D2CMP, FASTCMP_DC6PreMapMap, void, __fastcall, (const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pPaletteMap, const uint8_t *pPreMap), 0x6690)
//D2FUNC(D2CMP, FASTCMP_DC6PreMapMapClipped, void, __fastcall, (const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pPaletteMap, const uint8_t *pPreMap), 0x6830)
//D2FUNC(D2CMP, FASTCMP_DC6Map, void, __fastcall, (const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pPaletteMap), 0x6A60)
//D2FUNC(D2CMP, FASTCMP_DC6MapClipped, void, __fastcall, (const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pPaletteMap), 0x6BA0)
//D2FUNC(D2CMP, FASTCMP_DC6PreMapBlend, void, __fastcall, (const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pPreMap, const uint8_t *pBlendTable), 0x6D70)
//D2FUNC(D2CMP, FASTCMP_DC6PreMapBlendClipped, void, __fastcall, (const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pPreMap, const uint8_t *pBlendTable), 0x6FD0)
//D2FUNC(D2CMP, FASTCMP_DC6PreMap, void, __fastcall, (const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pPreMap), 0x72D0)
//D2FUNC(D2CMP, FASTCMP_DC6PreMapClipped, void, __fastcall, (const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pPreMap), 0x7410)
//D2FUNC(D2CMP, FASTCMP_DC6Blend, void, __fastcall, (const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pBlendTable), 0x75E0)
//D2FUNC(D2CMP, FASTCMP_DC6BlendClipped, void, __fastcall, (const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pBlendTable), 0x77E0)
//D2FUNC(D2CMP, FASTCMP_ToggleCelDrawMode, int32_t, __cdecl, (), -10049)
//D2FUNC(D2CMP, FASTCMP_DrawDC6, void, __fastcall, (const D2CmpRleCellHeader *pCel, uint8_t *pDst, int32_t nX, int32_t nClipLeft, int32_t nClipRight, uint32_t dwStride, int32_t nLinesToSkip, int32_t nLinesToDraw, const uint8_t *pPaletteMap, const uint8_t *pBlendTable, const uint8_t *pPreMap), 0x7AA0)
//D2FUNC(D2CMP, FASTCMP_CmpDoLensEffect, void, __stdcall, (uint8_t *pPixels, int32_t nPitch), -10048)
//D2FUNC(D2CMP, FASTCMP_DrawDC6Shadow, void, __fastcall, (const D2CmpRleCellHeader *pCel, uint8_t *pDst, int32_t nX, int32_t nClipLeft, int32_t nClipRight, uint32_t dwStride, int32_t nLinesToSkip, int32_t nLinesToDraw, const uint8_t *pBlendTable, uint8_t nShadowColor), 0x8100)
//D2FUNC(D2CMP, FINDTILES_AllocHash, D2TileLibraryHashStrc *, __cdecl, (), 0x82A0)
//D2FUNC(D2CMP, FINDTILES_FreeHash, void, __fastcall, (D2TileLibraryHashStrc *pHash), 0x82F0)
//D2FUNC(D2CMP, FINDTILES_AddTile, void, __fastcall, (D2TileLibraryHashStrc *ptHash, D2TileLibraryEntryStrc *ptTile), 0x8380)
//D2FUNC(D2CMP, FINDTILES_CollectTiles, uint32_t, __fastcall, (D2TileLibraryHashStrc *ptHash, D2TileLibraryEntryStrc **hTileList, uint32_t nCapacity, int32_t nType, int32_t nStyle, int32_t nSequence), 0x8570)
//D2FUNC(D2CMP, GFXHASH_ClearStorage, void, __fastcall, (D2CmpSpriteGfxHashStorage *pHashStorage), 0x8690)
//D2FUNC(D2CMP, GFXHASH_FindEntry, D2CmpSpriteGfxHashEntryPrefix *, __fastcall, (D2CmpSpriteGfxHashStorage *pHash, D2GfxDataStrc *ptCelContext, uint32_t *pDirBlock, uint32_t *pDirectionsPerBlock, D2CmpSpriteHeaderData *pHeader), 0x86A0)
//D2FUNC(D2CMP, GFXHASH_GetBucket, uint32_t, __fastcall, (D2GfxDataStrc *ptCelContext, D2CmpSpriteHeaderData *pHeader, uint32_t *pDirBlock, uint32_t *pDirectionsPerBlock), 0x8780)
//D2FUNC(D2CMP, GFXHASH_NewEntry, D2CmpSpriteGfxHashEntryPrefix *, __fastcall, (void *pPool, D2CmpSpriteGfxHashStorage *pHash, D2CmpSpriteGfxNodePrefix *pGfxNode, D2GfxDataStrc *ptCelContext, D2CmpSpriteHeaderData *pHeader), 0x8830)
//D2FUNC(D2CMP, GFXHASH_DeleteEntry, void, __fastcall, (void *pPool, D2CmpSpriteGfxHashStorage *pHash, D2CmpSpriteGfxHashEntryPrefix *ptElement), 0x88F0)
//D2FUNC(D2CMP, GFXHASH_GetDirectionBlock, uint32_t, __fastcall, (D2GfxDataStrc *ptCelContext, uint32_t *pDirectionsPerBlock, D2CmpSpriteHeaderData *pHeader), 0x8A60)
//D2FUNC(D2CMP, LRUCACHE_Init, void, __fastcall, (D2CmpLruCache *pCache, void *pPool, uint32_t dwMaxAllocated, void (__fastcall *pfnOnEvict)(D2CmpLruEntry *), uint32_t nEntries), 0x8B30)
//D2FUNC(D2CMP, LRUCACHE_Destroy, void, __fastcall, (D2CmpLruCache *pCache), 0x8BF0)
//D2FUNC(D2CMP, LRUCACHE_AllocEntry, D2CmpLruEntry *, __fastcall, (D2CmpLruCache *pCache, uint32_t dwSize, void *pUserData), 0x8C30)
//D2FUNC(D2CMP, LRUCACHE_EvictEntry, void, __fastcall, (D2CmpLruCache *pCache, D2CmpLruEntry *pEntry), 0x8DC0)
//D2FUNC(D2CMP, LRUCACHE_TouchEntry, void, __fastcall, (D2CmpLruCache *pCache, D2CmpLruEntry *pEntry), 0x8EB0)
//D2FUNC(D2CMP, PALETTE_UpdateD2TransparencyTable, void, __stdcall, (D2CmpPaletteTables *pTables, int32_t nUnused), -10003)
//D2FUNC(D2CMP, PALETTE_BuildAdditiveTable, void, __fastcall, (D2CmpPaletteTables *pTables), 0x8F70)
//D2FUNC(D2CMP, PALETTE_CreateD2Palette, D2CmpPaletteTables *, __stdcall, (const D2CmpPaletteEntry *pPalette), -10000)
//D2FUNC(D2CMP, PALETTE_BuildMultiplyTable, void, __fastcall, (D2CmpPaletteTables *pTables), 0x95C0)
//D2FUNC(D2CMP, PALETTE_BuildDarkenAndDarkBlend, void, __fastcall, (D2CmpPaletteTables *pTables), 0x9740)
//D2FUNC(D2CMP, PALETTE_BuildStandardShifts, void, __fastcall, (D2CmpPaletteTables *pTables), 0x9A60)
//D2FUNC(D2CMP, PALETTE_BuildBrightenTables, void, __fastcall, (D2CmpPaletteTables *pTables), 0x9BD0)
//D2FUNC(D2CMP, PALETTE_BuildColorShiftTables, void, __fastcall, (D2CmpPaletteTables *pTables, const D2CmpHSL *pSourceHSL), 0x9D20)
//D2FUNC(D2CMP, PALETTE_HSLToRGB256, void, __fastcall, (const D2CmpHSL *pHSL, D2CmpPaletteEntry *pRGB), 0xA770)
//D2FUNC(D2CMP, PALETTE_BuildLuminanceTable, void, __fastcall, (D2CmpPaletteTables *pTables, D2CmpHSL *pHSL), 0xA9E0)
//D2FUNC(D2CMP, PALETTE_CreateD2PaletteByteStream, D2CmpPaletteTables *, __stdcall, (const D2CmpPaletteEntry *pPalette, uint32_t *pdwSize), -10001)
//D2FUNC(D2CMP, PALETTE_DestroyD2Palette, void, __stdcall, (D2CmpPaletteTables *pTables), -10002)
//D2FUNC(D2CMP, PALETTE_GetClosestColor, uint8_t, __stdcall, (const D2CmpPaletteEntry *pPalette, uint32_t nColors, int32_t nRed, int32_t nGreen, int32_t nBlue), -10004)
//D2FUNC(D2CMP, PALETTE_GetFurthestColor, uint8_t, __stdcall, (const D2CmpPaletteEntry *pPalette, uint32_t nColors, int32_t nRed, int32_t nGreen, int32_t nBlue), -10005)
//D2FUNC(D2CMP, PALETTE_PaletteSetTransparency, void, __stdcall, (uint32_t dwColor), -10009)
//D2FUNC(D2CMP, PALETTE_PaletteInit, int32_t, __stdcall, (const char *pszPalette, int32_t nPaletteStartEntry, int32_t nPaletteEndEntry), -10006)
//D2FUNC(D2CMP, PALETTE_PaletteTestBackground, void, __stdcall, (int32_t bTestBackground), -10010)
//D2FUNC(D2CMP, PALETTE_Palette24BitTo8BitPixel, uint8_t, __stdcall, (uint8_t nBlue, uint8_t nGreen, uint8_t nRed), -10007)
//D2FUNC(D2CMP, PALETTE_Palette24BitTo8BitImage, int32_t, __stdcall, (const uint8_t *pSource, uint8_t *pDest, uint32_t nPixels, uint8_t (__stdcall *pfnConvert)(uint8_t, uint8_t, uint8_t)), -10008)
//D2FUNC(D2CMP, PALETTE_PaletteFixBlack, void, __stdcall, (int32_t bFixBlack), -10011)
//D2FUNC(D2CMP, PALETTE_PaletteSetMask, void, __stdcall, (const uint8_t *pMask), -10012)
//D2FUNC(D2CMP, PALSHIFT_PalShiftInit, void, __stdcall, (), -10096)
//D2FUNC(D2CMP, PALSHIFT_LoadItemPaletteGroup, void, __fastcall, (const char *szBaseName, int32_t nGroupIndex), 0xB5C0)
//D2FUNC(D2CMP, PALSHIFT_PalShiftSetAct, void, __stdcall, (int32_t nAct), -10097)
//D2FUNC(D2CMP, PALSHIFT_PalShiftGetTransform, uint8_t *, __stdcall, (int32_t nTransform, int32_t nColor), -10098)
//D2FUNC(D2CMP, RAW_EncodeRGB24CellFrames, void, __fastcall, (D2CmpCellBuilder *pBuilder, D2CmpBuiltCellFile *pOutput), 0xB7B0)
//D2FUNC(D2CMP, RAW_Rgb24Frame_VisitNonTransparentColors, void, __fastcall, (const D2CmpRgb24FrameHeader *pFrame, D2CmpPackedColorCallback pVisitColor, uint32_t dwTransparentColor), 0xB8B0)
//D2FUNC(D2CMP, SPRITECACHE_GetGfxFileExtension, const char *, __stdcall, (int32_t bAllowCompressed), -10051)
//D2FUNC(D2CMP, SPRITECACHE_BuildFileName, void, __stdcall, (char *Buffer, int32_t bAllowCompressed, const char *Format, va_list ArgList), -10050)
//D2FUNC(D2CMP, SPRITECACHE_SetNoCompress, void, __stdcall, (int32_t bAllowCompressedMode), -10054)
//D2FUNC(D2CMP, SPRITECACHE_SpriteCacheInit, void, __stdcall, (void *pPool, int32_t nSpriteCacheBytes, int32_t nCelHeaderCacheBytes, uint32_t dwMemoryOverride), -10052)
//D2FUNC(D2CMP, SPRITECACHE_OnDataEvict, void, __fastcall, (D2CmpSpriteGfxHashEntryPrefix *pElement), 0xBB70)
//D2FUNC(D2CMP, SPRITECACHE_OnHeaderEvict, void, __fastcall, (D2CmpLruEntry *pEntry), 0xBBD0)
//D2FUNC(D2CMP, SPRITECACHE_SpriteCacheDestroy, void, __stdcall, (int32_t bFreeCaches), -10053)
//D2FUNC(D2CMP, SPRITECACHE_RunDecodeJob, uint32_t, __stdcall, (void *pInput, uint32_t dwInputBytes, D2CmpSpriteDecodeJobPrefix *pJob), 0xBCB0)
//D2FUNC(D2CMP, SPRITECACHE_SpriteValidate, int32_t, __stdcall, (D2GfxDataStrc *pContext, int32_t bAsyncWait, int32_t bAsyncLoad), -10055)
//D2FUNC(D2CMP, SPRITECACHE_BuildFilePath, int32_t, __fastcall, (D2GfxDataStrc *pContext, char *szPath, const char *szKey), 0xC200)
//D2FUNC(D2CMP, SPRITECACHE_ResolveHeader, D2CmpSpriteHeaderData *, __fastcall, (D2GfxDataStrc *pContext, char *szPath, char *szKey), 0xC530)
//D2FUNC(D2CMP, SPRITECACHE_BuildHeaderKey, int32_t, __fastcall, (D2GfxDataStrc *pContext, char *szKey), 0xC780)
//D2FUNC(D2CMP, SPRITECACHE_InitializeHardwareSlots, void, __fastcall, (D2CmpSpriteHeaderData *ptCelData, D2GfxDataStrc *ptCelContext, D2CmpSpriteGfxNodePrefix *ptGfxNode), 0xCA30)
//D2FUNC(D2CMP, SPRITECACHE_LoadDirectionBlockSync, int32_t, __fastcall, (D2GfxDataStrc *pContext, D2CmpSpriteHeaderData *pHeader, uint32_t dwDirBlock, uint32_t dwDirectionsPerBlock, const char *szPath), 0xCC10)
//D2FUNC(D2CMP, SPRITECACHE_FindFrame, D2CmpSpriteFrameHeaderPrefix *, __fastcall, (D2GfxDataStrc *ptCelContext, D2CmpSpriteGfxBlockPrefix *ptGfxBlock, uint8_t *pBuffer), 0xD020)
//D2FUNC(D2CMP, SPRITECACHE_PrefetchSecondDirectionBlock, void, __fastcall, (D2GfxDataStrc *pContext, uint32_t dwDirBlock, uint32_t dwDirBlockSize, D2CmpSpriteHeaderData *pHeader, char *szKey, char *szPath, int32_t bAsyncLoad), 0xD160)
//D2FUNC(D2CMP, SPRITECACHE_LoadDirectionBlockAsync, int32_t, __fastcall, (D2GfxDataStrc *pContext, D2CmpSpriteHeaderData *pHeader, uint32_t dwDirBlock, uint32_t dwDirectionsPerBlock, const char *szPath), 0xD2D0)
//D2FUNC(D2CMP, SPRITECACHE_SpriteRequest, int32_t, __stdcall, (D2GfxDataStrc *ptCelContext), -10056)
//D2FUNC(D2CMP, SPRITECACHE_SpriteCacheDumpHashTableStats, void, __cdecl, (), -10059)
//D2FUNC(D2CMP, SPRITECACHE_SpriteCacheGetUsedMemory, uint32_t, __cdecl, (), -10060)
//D2FUNC(D2CMP, SPRITECACHE_SpriteCacheGetMaxMemory, uint32_t, __cdecl, (), -10061)
//D2FUNC(D2CMP, SPRITECACHE_CelDataCacheDumpHashTableStats, void, __cdecl, (), -10062)
//D2FUNC(D2CMP, SPRITECACHE_CelDataCacheGetUsedMemory, uint32_t, __cdecl, (), -10063)
//D2FUNC(D2CMP, SPRITECACHE_CelDataCacheGetMaxMemory, uint32_t, __cdecl, (), -10064)
//D2FUNC(D2CMP, SPRITECACHE_SpriteDataCache_Init, void, __fastcall, (D2CmpSpriteDataCache *pCache, void *pPool, int32_t nCacheBytes, void (__fastcall *pfnOnEvict)(void *)), 0xD870)
//D2FUNC(D2CMP, SPRITECACHE_SpriteDataCache_AllocateNode, D2CmpSpriteDataNode *, __fastcall, (D2CmpSpriteDataCache *ptCache, uint32_t dwSize, void *pUserData, int32_t bQueue1), 0xD940)
//D2FUNC(D2CMP, SPRITECACHE_SpriteDataCache_EvictNode, D2CmpSpriteDataNode *, __fastcall, (D2CmpSpriteDataCache *pCache, D2CmpSpriteDataNode *pNode, int32_t bAllowQueue1), 0xDBA0)
//D2FUNC(D2CMP, SPRITECACHE_SpriteDataCache_AddNodeBytes, void, __fastcall, (D2CmpSpriteDataCache *pCache, D2CmpSpriteDataNode *ptBlock, uint32_t dwDeltaBytes, int32_t bAllowQueue1), 0xDCA0)
//D2FUNC(D2CMP, SPRITECACHE_SpriteDataCache_AppendQueuedNode, void, __fastcall, (D2CmpSpriteDataCache *pCache, D2CmpSpriteDataNode *pNode, int32_t bQueue1), 0xDD60)
//D2FUNC(D2CMP, SPRITECACHE_SpriteDataCache_PopQueueHead, D2CmpSpriteDataNode *, __fastcall, (D2CmpSpriteDataCache *pCache, int32_t bQueue1), 0xDDE0)
//D2FUNC(D2CMP, SPRITECACHE_SpriteDataCache_DetachQueuedNode, void, __fastcall, (D2CmpSpriteDataCache *pCache, D2CmpSpriteDataNode *ptBlock), 0xDE60)
//D2FUNC(D2CMP, SPRITECACHE_SpriteNumAsyncLoads, int32_t, __cdecl, (), -10057)
//D2FUNC(D2CMP, SPRITECACHE_SpriteFindAsynchSiblings, int32_t, __stdcall, (int32_t nUnitType, int32_t nMode, int32_t nUnitIndex), -10058)
//D2FUNC(D2CMP, SPRITECACHE_SpriteFreeAsyncLoads, void, __stdcall, (), -10099)
//D2FUNC(D2CMP, SUBTILE_EncodeSquareRle, uint8_t *, __fastcall, (const uint8_t *pPixels, uint32_t *pEncodedBytes), 0xE050)
//D2FUNC(D2CMP, SUBTILE_ConvertRGB24ToIndexed, void, __fastcall, (D2CmpTileRuntimeComponent *ptComp8, const D2CmpTileRuntimeComponent *ptComp24, uint32_t dwTransparentColor, uint8_t (__stdcall *pfnMapColor)(uint8_t nByte0, uint8_t nByte1, uint8_t nByte2)), 0xE130)
//D2FUNC(D2CMP, SUBTILE_CompressIsometric, void, __fastcall, (D2CmpTileRuntimeComponent *pComponent), 0xE2F0)
//D2FUNC(D2CMP, SUBTILE_EncodeIsometricRle, void, __fastcall, (D2CmpTileRuntimeComponent *pComponent, const uint8_t *pPixels), 0xE370)
//D2FUNC(D2CMP, SUBTILE_PackOpaqueIsometric, void, __fastcall, (D2CmpTileRuntimeComponent *pComponent, const uint8_t *pPixels), 0xE530)
//D2FUNC(D2CMP, TILECACHE_TileCacheInit, void, __stdcall, (void *pPool, uint32_t dwCacheBytes), -10089)
//D2FUNC(D2CMP, TILECACHE_TileCacheDestroy, void, __stdcall, (int32_t bFreeCache), -10090)
//D2FUNC(D2CMP, TILECACHE_OnEvict, void, __fastcall, (D2CmpLruEntry *pEntry), 0xE680)
//D2FUNC(D2CMP, TILECACHE_TileValidate, int32_t, __stdcall, (D2TileLibraryEntryStrc *pTile, int32_t nUnused0, int32_t nUnused1), -10091)
//D2FUNC(D2CMP, TILECACHE_GetProjectAllocationSize, uint32_t, __fastcall, (const char *szFileName), 0xE780)
//D2FUNC(D2CMP, TILECACHE_SetHardwareFreeCallback, void, __fastcall, (D2CmpTileHardwareFreeCallback pfnCallback), 0xE7E0)
//D2FUNC(D2CMP, TILECACHE_GetTileCacheHits, void, __stdcall, (uint32_t *pConfiguredBytes, uint32_t *pAllocRequests, uint32_t *pAccessRequests, uint32_t *pPercentAllocated), -10092)
//D2FUNC(D2CMP, TILECACHE_ResetTileCacheHits, void, __cdecl, (), -10093)
//D2FUNC(D2CMP, TILECACHE_TileFreeAsyncLoads, void, __stdcall, (), -10102)
//D2FUNC(D2CMP, TILECACHE_DetachProjectTiles, void, __fastcall, (D2CmpNormalizedTileProject *pProject), 0xE870)
//D2FUNC(D2CMP, TILECACHE_10105, void, __stdcall, (int32_t nUnused1, int32_t nUnused2), -10105)
//D2FUNC(D2CMP, TILECMP_TileListCreate, D2CmpTileList *, __stdcall, (int32_t b24Bit, uint32_t dwTransparentRGB24), -10065)
//D2FUNC(D2CMP, TILECMP_TileListDestroy, void, __stdcall, (D2CmpTileList *pList), -10066)
//D2FUNC(D2CMP, TILECMP_TileListAddTile, D2CmpTileBuilder *, __stdcall, (D2CmpTileList *pList, uint16_t nFlags, uint16_t nRoofHeight, int32_t nType, int32_t nStyle, int32_t nSequence, int32_t nRarity_Frame, int32_t nLightDirection, const uint8_t *pSubtileFlags), -10067)
//D2FUNC(D2CMP, TILECMP_TileAddAddSubTile, void, __stdcall, (D2CmpTileBuilder *pTile, const uint8_t *pImage, int32_t nImageStride, int32_t nOffsetX, int32_t nOffsetY), -10068)
//D2FUNC(D2CMP, TILECMP_TileAddAddFloorTile, void, __stdcall, (D2CmpTileBuilder *pTile, const uint8_t *pImage), -10069)
//D2FUNC(D2CMP, TILECMP_TileListCreateProject, int32_t, __stdcall, (D2CmpTileList *pTileList, D2CmpNormalizedTileProject **ppProject), -10070)
//D2FUNC(D2CMP, TILECMP_TileProjectDestroy, void, __stdcall, (D2CmpNormalizedTileProject *pProject, int32_t nKeepProject), -10073)
//D2FUNC(D2CMP, TILECMP_TileProjectSerialize, void, __stdcall, (D2CmpNormalizedTileProject *pProject, D2TileLibraryHeaderStrc **pptData, uint32_t *pnSize), -10071)
//D2FUNC(D2CMP, TILECMP_TileProjectNormalize, void, __stdcall, (D2TileLibraryHeaderStrc *pSerializedProject, D2CmpNormalizedTileProject **ppProject, const char *szFileName), -10072)
//D2FUNC(D2CMP, TILECMP_NormalizeProject, void, __stdcall, (D2TileLibraryHeaderStrc *pSerializedProject, D2CmpNormalizedTileProject **ppProject, const char *szFileName), 0xF780)
//D2FUNC(D2CMP, TILECMP_BuildProjectHash, D2TileLibraryHashStrc *, __fastcall, (D2CmpNormalizedTileProject *pProject), 0xF860)
//D2FUNC(D2CMP, TILECMP_RebaseComponentDataPointers, void, __stdcall, (D2TileLibraryEntryStrc *pTile), 0xF8F0)
//D2FUNC(D2CMP, TILECMP_TileProjectIteratePixels, void, __stdcall, (D2CmpNormalizedTileProject *pProject, void (__stdcall *pfnVisit)(uint32_t dwRGB24)), -10074)
//D2FUNC(D2CMP, TILECMP_TileProjectConvert24To8, void, __stdcall, (D2CmpNormalizedTileProject *pProject24, D2CmpNormalizedTileProject **ppProject8, uint8_t (__stdcall *pfnMapColor)(uint8_t nByte0, uint8_t nByte1, uint8_t nByte2)), -10075)
//D2FUNC(D2CMP, TILECACHE_CallSetHardwareFreeCallback, void, __stdcall, (D2CmpTileHardwareFreeCallback pfnCallback), -10076)
//D2FUNC(D2CMP, TILELIB_TileLibraryAddProject, void, __stdcall, (D2TileLibraryHashStrc **ptTileLibrary, const char *szName), -10087)
//D2FUNC(D2CMP, TILELIB_TileLibraryGetTiles, uint32_t, __stdcall, (D2TileLibraryHashStrc **ptTileLibrary, int32_t nType, int32_t nStyle, int32_t nSequence, D2TileLibraryEntryStrc **pTileList, uint32_t nCapacity), -10088)
//D2FUNC(D2CMP, TILELIB_TileGetType, int32_t, __stdcall, (D2TileLibraryEntryStrc *hTile), -10077)
//D2FUNC(D2CMP, TILELIB_TileGetStyle, int32_t, __stdcall, (D2TileLibraryEntryStrc *hTile), -10078)
//D2FUNC(D2CMP, TILELIB_TileGetFlags, int32_t, __stdcall, (D2TileLibraryEntryStrc *hTile), -10079)
//D2FUNC(D2CMP, TILELIB_TileGetWidth, int32_t, __stdcall, (D2TileLibraryEntryStrc *hTile), -10080)
//D2FUNC(D2CMP, TILELIB_TileGetRarity, int32_t, __stdcall, (D2TileLibraryEntryStrc *hTile), -10081)
//D2FUNC(D2CMP, TILELIB_TileGetSequence, int32_t, __stdcall, (D2TileLibraryEntryStrc *hTile), -10082)
//D2FUNC(D2CMP, TILELIB_TileGetTileLight, int32_t, __stdcall, (D2TileLibraryEntryStrc *hTile), -10083)
//D2FUNC(D2CMP, TILELIB_TileGetTotalHeight, int32_t, __stdcall, (D2TileLibraryEntryStrc *hTile), -10084)
//D2FUNC(D2CMP, TILELIB_TileGetCollisionInfo, uint8_t *, __stdcall, (D2TileLibraryEntryStrc *hTile), -10085)
//D2FUNC(D2CMP, TILELIB_TileGetHeightToBottom, int32_t, __stdcall, (D2TileLibraryEntryStrc *hTile), -10086)
//D2FUNC(D2CMP, TILELIB_TileGetRoofHeight, uint16_t, __stdcall, (D2TileLibraryEntryStrc *hTile), -10100)
//D2FUNC(D2CMP, TILELIB_TileGetFilename, char *, __stdcall, (D2TileLibraryEntryStrc *hTile), -10101)
//D2FUNC(D2CMP, TILEPROJECTS_DestroyTileHeaders, void, __cdecl, (), -10094)
//D2FUNC(D2CMP, TILEPROJECTS_LoadProjectRecord, D2CmpNormalizedTileProject *, __stdcall, (D2TileLibraryHashStrc **ppHash, const char *szFileName), -10095)

//  D2FUNC(D2CMP, STORM_SMemFree, int, __stdcall, (void *, const char *, int, unsigned int), 0x10340)   -- now STORM_SMemFree

//D2FUNC(D2CMP, CELCMP_FindFirstSetBit1Based, uint32_t, __cdecl, (uint32_t dwMask), 0x10346)

//  D2FUNC(D2CMP, STORM_SMemAlloc, void *, __stdcall, (unsigned int, const char *, int, unsigned int), 0x10362)   -- now STORM_SMemAlloc

//D2FUNC(D2CMP, PALETTE_VirtualAllocRWX, void *, __fastcall, (uint32_t dwSize), 0x10368)
//D2FUNC(D2CMP, PALETTE_VirtualRelease, BOOL, __fastcall, (void *pAllocation), 0x10379)
//D2FUNC(D2CMP, SHARED_CopyImageRows, void, __fastcall, (uint8_t *pDst, const uint8_t *pSrc, int32_t nDstStride, int32_t nSrcStride, uint32_t nRows, uint32_t nRowBytes), 0x10388)

//  D2FUNC(D2CMP, STORM_SMemReAlloc, void *, __stdcall, (void *, unsigned int, const char *, int, unsigned int), 0x1040C)   -- now STORM_SMemReAlloc
//  D2FUNC(D2CMP, STORM_SStrHash, unsigned int, __stdcall, (const char *szString, unsigned int dwFlags, unsigned int dwSeed), 0x10412)   -- now STORM_SStrHash

//D2FUNC(D2CMP, ARCHIVE_OpenFile, int32_t, __fastcall, (int32_t nUnused, const char *szFileName, void **ppFileHandle, int32_t bSuppressMissing), 0x10418)
//D2FUNC(D2CMP, ARCHIVE_CloseFile, void, __fastcall, (int32_t nUnused, void *hFile), 0x10454)
//D2FUNC(D2CMP, ARCHIVE_SeekFile, void, __fastcall, (int32_t nUnused, void *hFile, int32_t lDistanceToMove, int32_t *lpDistanceToMoveHigh, uint32_t dwMoveMethod), 0x1047A)
//D2FUNC(D2CMP, ARCHIVE_ReadFileExact, void, __fastcall, (int32_t nUnused, void *hFile, void *pBuffer, uint32_t dwBytesToRead), 0x104F0)

//  D2FUNC(D2CMP, STORM_SRegLoadValue, int, __stdcall, (const char *szKeyName, const char *szValueName, unsigned int dwFlags, unsigned int *pdwValue), 0x10598)   -- now STORM_SRegLoadValue
//  D2FUNC(D2CMP, STORM_SStrCmpI, int, __stdcall, (const char *szLeft, const char *szRight, unsigned int dwMaxCount), 0x1059E)   -- now STORM_SStrCmpI
//  D2FUNC(D2CMP, STORM_SFileGetFileName, int, __stdcall, (void *hFile, char *szBuffer, int nBufferBytes), 0x105A4)   -- now STORM_SFileGetFileName
//  D2FUNC(D2CMP, FOG_DisplayHalt, void, __cdecl, (const char *szMsg, const char *szFile, int nLine), 0x105AA)   -- now FOG_DisplayHalt
//  D2FUNC(D2CMP, FOG_DisplayAssert, void, __cdecl, (const char *szMsg, const char *szFile, int nLine), 0x105B0)   -- now FOG_DisplayAssert
//  D2FUNC(D2CMP, FOG_Alloc, void *, __fastcall, (int nSize, const char *szFile, int nLine, int n0), 0x105B6)   -- now FOG_Alloc
//  D2FUNC(D2CMP, FOG_Free, void, __fastcall, (void *pFree, const char *szFile, int nLine, int n0), 0x105BC)   -- now FOG_Free
//  D2FUNC(D2CMP, FOG_FreePool, int, __fastcall, (void *pPool, void *pData, const char *szFile, int nLine, int nFlags), 0x105C2)   -- now FOG_FreePool
//  D2FUNC(D2CMP, FOG_AllocPool, void *, __fastcall, (void *pPool, unsigned int dwSize, const char *szFile, int nLine, int nFlags), 0x105C8)   -- now FOG_AllocPool
//  D2FUNC(D2CMP, FOG_Trace, int, __cdecl, (const char *, ...), 0x105CE)   -- now FOG_Trace
//  D2FUNC(D2CMP, FOG_FGetFileSize, unsigned int, __fastcall, (void *hFile, unsigned int *pFileSizeHigh), 0x105D4)   -- now FOG_FGetFileSize
//  D2FUNC(D2CMP, FOG_FCloseFile, int, __fastcall, (void *hFile), 0x105DA)   -- now FOG_FCloseFile
//  D2FUNC(D2CMP, FOG_FReadFile, int, __fastcall, (void *hFile, void *pBuffer, unsigned int dwBytes, unsigned int *pBytesRead, unsigned int nUnknown0, unsigned int nUnknown1, unsigned int nUnknown2), 0x105E0)   -- now FOG_FReadFile
//  D2FUNC(D2CMP, FOG_FOpenFile, int, __fastcall, (const char *szFile, void **ppFileHandle), 0x105E6)   -- now FOG_FOpenFile
//  D2FUNC(D2CMP, FOG_AsyncDataFree, void, __fastcall, (void *pAsyncData), 0x105EC)   -- now FOG_AsyncDataFree
//  D2FUNC(D2CMP, FOG_AsyncDataGetBuffer, void *, __fastcall, (void *pAsyncData), 0x105F2)   -- now FOG_AsyncDataGetBuffer
//  D2FUNC(D2CMP, FOG_DisplayWarning, void, __cdecl, (const char *szMsg, const char *szFile, int nLine), 0x105F8)   -- now FOG_DisplayWarning
//  D2FUNC(D2CMP, FOG_GetSystemInfo, D2CmpFogSystemInfoPrefix *, __cdecl, (), 0x105FE)   -- now FOG_GetSystemInfo
//  D2FUNC(D2CMP, FOG_AsyncDataTestLoaded, int, __fastcall, (void *pAsyncData), 0x10604)   -- now FOG_AsyncDataTestLoaded
//  D2FUNC(D2CMP, FOG_csprintf, const char *, __cdecl, (char *szDest, const char *szFormat, ...), 0x1060A)   -- now FOG_csprintf
//  D2FUNC(D2CMP, FOG_AsyncDataGetBufferSize, unsigned int, __fastcall, (AsyncData *pAsyncData), 0x10610)   -- now FOG_AsyncDataGetBufferSize
//  D2FUNC(D2CMP, FOG_AsyncDataLoadFileEx, AsyncData *, __fastcall, (void *pMemPool, const char *szFileName, int bAllowAsync, int nFileOffset, int nFileSize, void *pBuffer, AsyncDataLoadFileCallback *pCallback, int nPriority, const char *szSourceFile, int nSourceLine), 0x10616)   -- now FOG_AsyncDataLoadFileEx
//  D2FUNC(D2CMP, FOG_TraceF, void, __cdecl, (const char *szFileSubName, const char *szFormat, ...), 0x1061C)   -- now FOG_TraceF
//  D2FUNC(D2CMP, FOG_DisplayError, void, __cdecl, (int nCategory, const char *szMsg, const char *szFile, int nLine), 0x10622)   -- now FOG_DisplayError
//  D2FUNC(D2CMP, FOG_FSetFilePointer, unsigned int, __fastcall, (void *hFile, int nDistance, int *pDistanceHigh, unsigned int nMoveMethod), 0x10628)   -- now FOG_FSetFilePointer
//  D2FUNC(D2CMP, KERNEL32_RtlUnwind, void, __stdcall, (PVOID TargetFrame, PVOID TargetIp, PEXCEPTION_RECORD ExceptionRecord, PVOID ReturnValue), 0x189D4)   -- KERNEL32, called directly

///////////////////////////////////////////////////////////////////////////////
//
//  NOT EXPRESSIBLE THROUGH D2FUNC -- listed, not dropped.
//
//  The reason each cannot be a D2FUNC is on its line.
//
///////////////////////////////////////////////////////////////////////////////

// 0x5C00     _DllMain@12                                  the name is not a C identifier
