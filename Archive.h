#pragma once

#include "DLLmain.h"

//  Archive.h -- the functions of Archive.cpp (D2Hell's file helpers compiled into D2CMP), declared. None is exported: all are called inside D2CMP
//  only, with ECX an argument nothing reads and the file handle in EDX.

int32_t __fastcall ARCHIVE_OpenFile(int32_t nUnused, const char *szFileName, void **ppFileHandle, int32_t bSuppressMissing);
void __fastcall ARCHIVE_CloseFile(int32_t nUnused, void *hFile);
void __fastcall ARCHIVE_SeekFile(int32_t nUnused, void *hFile, int32_t lDistanceToMove, int32_t *lpDistanceToMoveHigh, uint32_t dwMoveMethod);
void __fastcall ARCHIVE_ReadFileExact(int32_t nUnused, void *hFile, void *pBuffer, uint32_t dwBytesToRead);
