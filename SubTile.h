#pragma once

#include "DLLmain.h"

//  SubTile.h -- the functions of SubTile.cpp, declared. None is exported: Tilecmp.cpp calls three, CompressIsometric the other two.

uint8_t *__fastcall SUBTILE_EncodeSquareRle(const uint8_t *pPixels, uint32_t *pEncodedBytes);
void __fastcall SUBTILE_ConvertRGB24ToIndexed(D2CmpTileRuntimeComponent *ptComp8, const D2CmpTileRuntimeComponent *ptComp24,
                                              uint32_t dwTransparentColor,
                                              uint8_t (__stdcall *pfnMapColor)(uint8_t nByte0, uint8_t nByte1,
                                                                                       uint8_t nByte2));
void __fastcall SUBTILE_CompressIsometric(D2CmpTileRuntimeComponent *pComponent);
void __fastcall SUBTILE_EncodeIsometricRle(D2CmpTileRuntimeComponent *pComponent, const uint8_t *pPixels);
void __fastcall SUBTILE_PackOpaqueIsometric(D2CmpTileRuntimeComponent *pComponent, const uint8_t *pPixels);
