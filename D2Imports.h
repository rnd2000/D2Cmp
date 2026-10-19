#pragma once

/*  GENERATED from the reverse-engineering database of D2CMP.dll 1.10 (image base 0x6FDF0000).
 *
 *  The original's game imports (Fog, Storm), declared. Each binds BY ORDINAL against
 *  implib/named/<Module>.lib, as the original binds them; a call site names the DLL in
 *  capitals, then the import's own name. Windows imports are called directly.
 *
 *  A WRONG SIGNATURE HERE IS A LINK ERROR, not a corrupted stack: the argument-byte
 *  count is part of the symbol.
 */

/*  The import libraries, relative to the project directory.
 */
#pragma comment(lib, "implib/named/Fog.lib")
#pragma comment(lib, "implib/named/Storm.lib")

extern "C" {

/*  ---- Fog ---- */
const char * __cdecl FOG_csprintf(char *szDest, const char *szFormat, ...);   /* #10018 */
D2CmpFogSystemInfoPrefix * __cdecl FOG_GetSystemInfo(void);   /* #10022 */
void __cdecl FOG_DisplayAssert(const char *szMsg, const char *szFile, int nLine);   /* #10023 */
void __cdecl FOG_DisplayHalt(const char *szMsg, const char *szFile, int nLine);   /* #10024 */
void __cdecl FOG_DisplayWarning(const char *szMsg, const char *szFile, int nLine);   /* #10025 */
void __cdecl FOG_DisplayError(int nCategory, const char *szMsg, const char *szFile, int nLine);   /* #10026 */
int __cdecl FOG_Trace(const char *, ...);   /* #10029 */
void __cdecl FOG_TraceF(const char *szFileSubName, const char *szFormat, ...);   /* #10030 */
void * __fastcall FOG_Alloc(int nSize, const char *szFile, int nLine, int n0);   /* #10042 */
void __fastcall FOG_Free(void *pFree, const char *szFile, int nLine, int n0);   /* #10043 */
void * __fastcall FOG_AllocPool(void *pPool, unsigned int dwSize, const char *szFile, int nLine, int nFlags);   /* #10045 */
int __fastcall FOG_FreePool(void *pPool, void *pData, const char *szFile, int nLine, int nFlags);   /* #10046 */
AsyncData * __fastcall FOG_AsyncDataLoadFileEx(void *pMemPool, const char *szFileName, int bAllowAsync, int nFileOffset, int nFileSize, void *pBuffer, AsyncDataLoadFileCallback *pCallback, int nPriority, const char *szSourceFile, int nSourceLine);   /* #10091 */
int __fastcall FOG_AsyncDataTestLoaded(void *pAsyncData);   /* #10092 */
void * __fastcall FOG_AsyncDataGetBuffer(void *pAsyncData);   /* #10094 */
unsigned int __fastcall FOG_AsyncDataGetBufferSize(AsyncData *pAsyncData);   /* #10095 */
void __fastcall FOG_AsyncDataFree(void *pAsyncData);   /* #10097 */
int __fastcall FOG_FOpenFile(const char *szFile, void **ppFileHandle);   /* #10102 */
int __fastcall FOG_FCloseFile(void *hFile);   /* #10103 */
int __fastcall FOG_FReadFile(void *hFile, void *pBuffer, unsigned int dwBytes, unsigned int *pBytesRead, unsigned int nUnknown0, unsigned int nUnknown1, unsigned int nUnknown2);   /* #10104 */
unsigned int __fastcall FOG_FGetFileSize(void *hFile, unsigned int *pFileSizeHigh);   /* #10105 */
unsigned int __fastcall FOG_FSetFilePointer(void *hFile, int nDistance, int *pDistanceHigh, unsigned int nMoveMethod);   /* #10106 */

/*  ---- Storm ---- */
int __stdcall STORM_SFileGetFileName(void *hFile, char *szBuffer, int nBufferBytes);   /* #276 */
void * __stdcall STORM_SMemAlloc(unsigned int, const char *, int, unsigned int);   /* #401 */
int __stdcall STORM_SMemFree(void *, const char *, int, unsigned int);   /* #403 */
void * __stdcall STORM_SMemReAlloc(void *, unsigned int, const char *, int, unsigned int);   /* #405 */
int __stdcall STORM_SRegLoadValue(const char *szKeyName, const char *szValueName, unsigned int dwFlags, unsigned int *pdwValue);   /* #423 */
unsigned int __stdcall STORM_SStrHash(const char *szString, unsigned int dwFlags, unsigned int dwSeed);   /* #502 */
int __stdcall STORM_SStrCmpI(const char *szLeft, const char *szRight, unsigned int dwMaxCount);   /* #509 */

}  /* extern "C" */


/*  CALL-SITE NAMES. Each says which DLL the call leaves for --
 *  D2WIN_ARCHIVE_UnloadExpansionArchives rather than D2CMP_...,
 *  because ARCHIVE_UnloadExpansionArchives is D2Win's.
 *
 *  The prefix is never DOUBLED. Where the name already carries the
 *  module it is REPLACED, case-insensitively:
 *
 *      FOG_CreateFileA  ->  FOG_CreateFileA      already right
 *      D2Win_10028      ->  D2WIN_10028          replaced, not stacked
 *      ARCHIVE_Unload.. ->  D2WIN_ARCHIVE_Unload..  nothing to replace
 *
 *  Where the new name IS the declared name there is nothing to
 *  define and the call site simply uses it.
 */
