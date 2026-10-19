#pragma once

#include "DLLmain.h"

//  FastCmp.h -- the declarations of FastCmp.cpp's 21 functions and the unit's globals: the three lens tables and the generic decoder's flag. Each
//  global is a C++17 `inline` variable, one object for every file that includes this header. The comment line "D2CMP 0x6F..." above a global gives
//  its address in the original. FASTCMP_CmpDoLensEffect (#10048) and FASTCMP_ToggleCelDrawMode (#10049) are the unit's exports, declared
//  extern "C". FASTCMP_EncodeIndexedCellFrames, FASTCMP_DrawDC6, FASTCMP_ConvertRGB24CellFramesToIndexed and FASTCMP_TransformDC6PaletteInPlace are
//  the DC6 entries (slot 0) of the cel format tables in CelCmp.h (gpCellBuilderEncoders, sgpfnDecodeFunc, gpfnCelConvert24To8, gpfnCelTransform),
//  and FASTCMP_DrawDC6Shadow is called by CELCMP_CelDrawShadow (#10035). FASTCMP_EncodeDC6RlePixels, FASTCMP_DC6CopyClipped and the twelve DC6
//  leaf drawers are called inside the unit only.

void __fastcall FASTCMP_EncodeIndexedCellFrames(D2CmpCellBuilder *pBuilder, D2CmpBuiltCellFile *pOutput);
void __fastcall FASTCMP_EncodeDC6RlePixels(const uint8_t *pPixels, int32_t nPixelWidth, uint32_t nPixelHeight, uint8_t nOrientationFlags, uint8_t **ppEncoded, uint32_t *pnEncodedBytes);
void __fastcall FASTCMP_TransformDC6PaletteInPlace(D2CmpRleCellHeader *pCell, const uint8_t *pPaletteMap);
void __fastcall FASTCMP_ConvertRGB24CellFramesToIndexed(const D2CmpBuiltCellFile *pSrc, D2CmpBuiltCellFile *pDst,
                                                        uint8_t (__stdcall *pfMapRgbToIndex)(uint8_t nRed,
                                                                                                      uint8_t nGreen,
                                                                                                      uint8_t nBlue),
                                                        uint32_t dwTransparentColor);
void __fastcall FASTCMP_DC6CopyClipped(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight);
void __fastcall FASTCMP_DC6MapBlend(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pPaletteMap, const uint8_t *pBlendTable);
void __fastcall FASTCMP_DC6MapBlendClipped(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pPaletteMap, const uint8_t *pBlendTable);
void __fastcall FASTCMP_DC6PreMapMap(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pPaletteMap, const uint8_t *pPreMap);
void __fastcall FASTCMP_DC6PreMapMapClipped(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pPaletteMap, const uint8_t *pPreMap);
void __fastcall FASTCMP_DC6Map(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pPaletteMap);
void __fastcall FASTCMP_DC6MapClipped(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pPaletteMap);
void __fastcall FASTCMP_DC6PreMapBlend(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pPreMap, const uint8_t *pBlendTable);
void __fastcall FASTCMP_DC6PreMapBlendClipped(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pPreMap, const uint8_t *pBlendTable);
void __fastcall FASTCMP_DC6PreMap(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pPreMap);
void __fastcall FASTCMP_DC6PreMapClipped(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pPreMap);
void __fastcall FASTCMP_DC6Blend(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, const uint8_t *pBlendTable);
void __fastcall FASTCMP_DC6BlendClipped(const uint8_t *pRle, uint8_t *pDst, int32_t nLinesToDraw, int32_t nRowStep, int32_t nX, int32_t nClipLeft, int32_t nClipRight, const uint8_t *pBlendTable);
extern "C" int32_t __cdecl FASTCMP_ToggleCelDrawMode();                                                  // #10049
void __fastcall FASTCMP_DrawDC6(const D2CmpRleCellHeader *pCel, uint8_t *pDst, int32_t nX, int32_t nClipLeft, int32_t nClipRight,
                                uint32_t dwStride, int32_t nLinesToSkip, int32_t nLinesToDraw, const uint8_t *pPaletteMap,
                                const uint8_t *pBlendTable, const uint8_t *pPreMap);
extern "C" void __stdcall FASTCMP_CmpDoLensEffect(uint8_t *pPixels, int32_t nPitch);              // #10048
void __fastcall FASTCMP_DrawDC6Shadow(const D2CmpRleCellHeader *pCel, uint8_t *pDst, int32_t nX, int32_t nClipLeft, int32_t nClipRight,
                                      uint32_t dwStride, int32_t nLinesToSkip, int32_t nLinesToDraw, const uint8_t *pBlendTable,
                                      uint8_t nShadowColor);

//  The lens tables of FASTCMP_CmpDoLensEffect. Nothing in the module writes them (they are .bss), so they stay zero.

//  D2CMP 0x6FE8E570
//  The first column of each of the 80 rows the left half warps; compared signed.
inline int32_t gaLensLeftBoundary[80] = {};

//  D2CMP 0x6FE8E6B0
//  The last column of each row, the right half's start.
inline int32_t gaLensRightBoundary[80] = {};

//  D2CMP 0x6FE8E7F0
//  Per pixel of the 80x80 lens (row * 80 + column): the signed byte distance of the pixel it takes its colour from.
inline int32_t gaLensSourceOffset[6400] = {};

//  D2CMP 0x6FE94BF0
//  The generic decoder's flag: FASTCMP_ToggleCelDrawMode (#10049) flips it and FASTCMP_DrawDC6 reads it; zero at load (past .data's raw bytes).
inline int32_t gbFastCmpUseGenericDecoder = 0;
