#pragma once

#include "DLLmain.h"

//  Shared.h -- SHARED_CopyImageRows, the one function of Shared.cpp, declared.

void __fastcall SHARED_CopyImageRows(uint8_t *pDst, const uint8_t *pSrc, int32_t nDstStride, int32_t nSrcStride, uint32_t nRows, uint32_t nRowBytes);
