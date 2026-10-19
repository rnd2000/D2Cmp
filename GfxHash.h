#pragma once

#include "DLLmain.h"

/*---------------------------------------------------------------------------
 *
 *  GfxHash.h -- the functions of GfxHash.cpp, declared. None of the six is exported.
 *
 *-------------------------------------------------------------------------*/

void __fastcall GFXHASH_ClearStorage(D2CmpSpriteGfxHashStorage *pHashStorage);
D2CmpSpriteGfxHashEntryPrefix *__fastcall GFXHASH_FindEntry(D2CmpSpriteGfxHashStorage *pHash, D2GfxDataStrc *ptCelContext, uint32_t *pDirBlock, uint32_t *pDirectionsPerBlock, D2CmpSpriteHeaderData *pHeader);
uint32_t __fastcall GFXHASH_GetBucket(D2GfxDataStrc *ptCelContext, D2CmpSpriteHeaderData *pHeader, uint32_t *pDirBlock, uint32_t *pDirectionsPerBlock);
D2CmpSpriteGfxHashEntryPrefix *__fastcall GFXHASH_NewEntry(void *pPool, D2CmpSpriteGfxHashStorage *pHash, D2CmpSpriteGfxNodePrefix *pGfxNode, D2GfxDataStrc *ptCelContext, D2CmpSpriteHeaderData *pHeader);
void __fastcall GFXHASH_DeleteEntry(void *pPool, D2CmpSpriteGfxHashStorage *pHash, D2CmpSpriteGfxHashEntryPrefix *ptElement);
uint32_t __fastcall GFXHASH_GetDirectionBlock(D2GfxDataStrc *ptCelContext, uint32_t *pDirectionsPerBlock, D2CmpSpriteHeaderData *pHeader);
