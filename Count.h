#pragma once

#include "DLLmain.h"

/*---------------------------------------------------------------------------
 *
 *  Count.h -- the functions of Count.cpp, declared. None of the three is exported.
 *
 *-------------------------------------------------------------------------*/

int32_t __fastcall COUNT_CountConsecutiveFirst(const uint8_t *pSrc, const uint8_t *pEnd, int32_t nMaxCount, int32_t *pbCountEqualsDist, int32_t *pbIsRemainingSame);
int32_t __fastcall COUNT_CountConsecutive(uint8_t bValue, const uint8_t *pSrc, const uint8_t *pEnd, int32_t nMaxCount, int32_t *pbCountEqualsDist, int32_t *pbIsRemainingSame);
int32_t __fastcall COUNT_CountConsecutiveDiff(const uint8_t *pSrc, const uint8_t *pEnd, uint8_t bValue, int32_t nMaxCount, int32_t *pbCountEqualsDist, int32_t *pbIsRemainingSame);
