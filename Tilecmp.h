#pragma once

#include "DLLmain.h"

//  Tilecmp.h -- the functions of Tilecmp.cpp declared, and the unit's constants and global. Eleven are exports (#10065..#10075), declared
//  extern "C" for D2CMP.def; NormalizeProject, BuildProjectHash and RebaseComponentDataPointers are called inside D2CMP only.

//  The tile project's version and the names of the original's own flags, out of its assert texts. The values are the bits the asserting
//  instructions test. The other bits the unit sets and tests have no name of the original's and are written in hex where they are used.
constexpr uint32_t TILEPROJECT_VERSION = 7;

//  D2CMP 0x6FE0F7E8: the pixel origin (x, y) of each of the 25 32x16 pieces a floor tile's 160x80 diamond is cut into, row by row of a 5x5 grid
//  (read by TILECMP_TileAddAddFloorTile). Not `const`: the original keeps it in .data, writable.
inline int32_t gaFloorComponentOffsets[25][2] = {
    {64, 0},  {80, 8},  {96, 16}, {112, 24}, {128, 32},
    {48, 8},  {64, 16}, {80, 24}, {96, 32},  {112, 40},
    {32, 16}, {48, 24}, {64, 32}, {80, 40},  {96, 48},
    {16, 24}, {32, 32}, {48, 40}, {64, 48},  {80, 56},
    {0, 32},  {16, 40}, {32, 48}, {48, 56},  {64, 64},
};

extern "C" D2CmpTileList *__stdcall TILECMP_TileListCreate(int32_t b24Bit, uint32_t dwTransparentRGB24);   // #10065
extern "C" void __stdcall TILECMP_TileListDestroy(D2CmpTileList *pList);                                       // #10066
extern "C" D2CmpTileBuilder *__stdcall TILECMP_TileListAddTile(D2CmpTileList *pList, uint16_t nFlags, uint16_t nRoofHeight,
                                                               int32_t nType, int32_t nStyle, int32_t nSequence, int32_t nRarity_Frame, int32_t nLightDirection,
                                                               const uint8_t *pSubtileFlags);              // #10067
extern "C" void __stdcall TILECMP_TileAddAddSubTile(D2CmpTileBuilder *pTile, const uint8_t *pImage, int32_t nImageStride, int32_t nOffsetX, int32_t nOffsetY);                                                   // #10068
extern "C" void __stdcall TILECMP_TileAddAddFloorTile(D2CmpTileBuilder *pTile, const uint8_t *pImage);   // #10069
extern "C" int32_t __stdcall TILECMP_TileListCreateProject(D2CmpTileList *pTileList, D2CmpNormalizedTileProject **ppProject);   // #10070
extern "C" void __stdcall TILECMP_TileProjectDestroy(D2CmpNormalizedTileProject *pProject, int32_t nKeepProject);   // #10073
extern "C" void __stdcall TILECMP_TileProjectSerialize(D2CmpNormalizedTileProject *pProject, D2TileLibraryHeaderStrc **pptData, uint32_t *pnSize);                                        // #10071
extern "C" void __stdcall TILECMP_TileProjectNormalize(D2TileLibraryHeaderStrc *pSerializedProject, D2CmpNormalizedTileProject **ppProject, const char *szFileName);                                      // #10072
void __stdcall TILECMP_NormalizeProject(D2TileLibraryHeaderStrc *pSerializedProject, D2CmpNormalizedTileProject **ppProject, const char *szFileName);
D2TileLibraryHashStrc *__fastcall TILECMP_BuildProjectHash(D2CmpNormalizedTileProject *pProject);
void __stdcall TILECMP_RebaseComponentDataPointers(D2TileLibraryEntryStrc *pTile);
extern "C" void __stdcall TILECMP_TileProjectIteratePixels(D2CmpNormalizedTileProject *pProject, void (__stdcall *pfnVisit)(uint32_t dwRGB24));      // #10074
extern "C" void __stdcall TILECMP_TileProjectConvert24To8(D2CmpNormalizedTileProject *pProject24, D2CmpNormalizedTileProject **ppProject8,
                                                          uint8_t (__stdcall *pfnMapColor)(uint8_t nByte0, uint8_t nByte1,
                                                                                                   uint8_t nByte2));   // #10075
