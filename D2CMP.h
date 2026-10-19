#pragma once

#include "DLLmain.h"

//  D2CMP.h -- every D2CMP function of ours, declared: it includes the generated import declarations (after DLLmain.h, whose D2Structs.h declares
//  the types they name) and every module header.
#include "D2Imports.h"

//  Fog's data export #10259, imported by name as the original imports it: `1 << i` for i = 0..31.
extern "C" __declspec(dllimport) const uint32_t gdwBitMasks[32];        // Fog #10259

//  The module headers, one per original source file, in address order.
#include "CelCmp.h"
#include "CelDataHash.h"
#include "Codec.h"
#include "Count.h"
#include "FastCmp.h"
#include "FindTiles.h"
#include "GfxHash.h"
#include "LRUCache.h"
#include "Palette.h"
#include "PalShift.h"
#include "Raw.h"
#include "SpriteCache.h"
#include "SubTile.h"
#include "TileCache.h"
#include "Tilecmp.h"
#include "TileLib.h"
#include "TileProjects.h"
#include "Archive.h"

//  Shared.cpp's, which is not one of the original's files.
#include "Shared.h"
