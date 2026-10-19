#pragma once

#include "DLLmain.h"

/*---------------------------------------------------------------------------
 *
 *  CelDataHash.h -- the functions of CelDataHash.cpp, declared. The first four are not exported; the fifth is extern "C" for D2CMP.def.
 *
 *-------------------------------------------------------------------------*/

void __fastcall CELDATAHASH_Clear(D2CmpCelHeaderHash *pHash);
D2CmpCelHeaderHashEntry *__fastcall CELDATAHASH_FindEntry(D2CmpCelHeaderHash *pHash, const char *szKey);
D2CmpCelHeaderHashEntry *__fastcall CELDATAHASH_NewEntry(void *pPool, D2CmpCelHeaderHash *pHash, D2CmpLruEntry *pLruEntry, const char *szKey);
void __fastcall CELDATAHASH_DeleteEntry(void *pPool, D2CmpCelHeaderHash *pHash, D2CmpCelHeaderHashEntry *ptElement);
extern "C" void __fastcall CELDATAHASH_10106(void *pHash);   // #10103, #10104, #10106
