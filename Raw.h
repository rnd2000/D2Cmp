#pragma once

#include "DLLmain.h"

//  Raw.h -- declarations for Raw.cpp, the RGB24 ("raw") cel frame encoder and pixel visitor. Neither function is exported: CelCmp.cpp's
//  CELCMP_CelCompressCreateCelFile and CELCMP_CelFileIteratePixels call them, and the stock game reaches neither.

void __fastcall RAW_EncodeRGB24CellFrames(D2CmpCellBuilder *pBuilder, D2CmpBuiltCellFile *pOutput);
void __fastcall RAW_Rgb24Frame_VisitNonTransparentColors(const D2CmpRgb24FrameHeader *pFrame, D2CmpPackedColorCallback pVisitColor, uint32_t dwTransparentColor);
