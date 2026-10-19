#pragma once

/****************************************************************************
*                                                                           *
*   D2Structs.h                                                             *
*                                                                           *
*   Licensed under the Apache License, Version 2.0 (the "License");         *
*   you may not use this file except in compliance with the License.        *
*   You may obtain a copy of the License at                                 *
*                                                                           *
*   http://www.apache.org/licenses/LICENSE-2.0                              *
*                                                                           *
*   Unless required by applicable law or agreed to in writing, software     *
*   distributed under the License is distributed on an "AS IS" BASIS,       *
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
*   See the License for the specific language governing permissions and     *
*   limitations under the License.                                          *
*                                                                           *
*---------------------------------------------------------------------------*
*                                                                           *
*   https://github.com/olivier-verville/D2Template                          *
*                                                                           *
*   This file is used to declare data structures used by the game to        *
*   represent various entities, such as the data structure representing     *
*   a unit entity, or a game entity                                         *
*                                                                           *
*****************************************************************************/

/*---------------------------------------------------------------------------
 *
 *  GENERATED from the reverse-engineering database of D2CMP.dll 1.10 (image base 0x6FDF0000).
 *
 *  Included by DLLmain.h after <windows.h>, <cstdint> and D2Constants.h. The definitions are
 *  wrapped in #pragma pack(1) and carry an explicit _gapN member for every hole, so each layout
 *  is the binary's; the static_asserts at the end of the file hold every type's size.
 *
 *-------------------------------------------------------------------------*/

/* ---- base types the generated layouts use ---- */
#ifndef _IDA_BASE_TYPES
#define _IDA_BASE_TYPES
typedef unsigned char       _BYTE;
typedef unsigned short      _WORD;
typedef unsigned int        _DWORD;
typedef unsigned __int64    _QWORD;
typedef unsigned char       _UNKNOWN;
typedef unsigned char       _BOOL1;
typedef unsigned short      _BOOL2;
typedef unsigned int        _BOOL4;
#endif

/* ---- enums this database has that D2Constants.h does not ----------------
 * Emitted here because a structure below refers to them and the header that
 * normally carries the enums does not define them. If D2Constants.h is later
 * regenerated and gains one of these, THIS is the copy to delete -- two
 * definitions of the same enum is an error, not a merge.
 *------------------------------------------------------------------------*/
// enum D2C_LaunchType : unsigned __int8
// {
// LAUNCHTYPE_NONE = 0x0,
// LAUNCHTYPE_LOCAL = 0x1,
// LAUNCHTYPE_REALM = 0x2,
// LAUNCHTYPE_TCPIP = 0x3,
// LAUNCHTYPE_OPENBNET = 0x4,
// };

/* ---- forward declarations ---- */
struct AsyncData;
struct AsyncDataEventSlot;
struct AsyncDataLoadFileCallback;
// struct BnClientInterface;
// struct ClientPacketSaveHeaderPart;
// struct D2Act1Quest1Strc;
// struct D2Act1Quest2Strc;
// struct D2Act1Quest3Strc;
// struct D2Act1Quest4Strc;
// struct D2Act1Quest5Strc;
// struct D2Act1Quest6Strc;
// struct D2Act2Quest0Strc;
// struct D2Act2Quest1Strc;
// struct D2Act2Quest2Strc;
// struct D2Act2Quest3Strc;
// struct D2Act2Quest4Strc;
// struct D2Act2Quest5Strc;
// struct D2Act2Quest6Strc;
// struct D2Act2Quest7Strc;
// struct D2Act2Quest8Strc;
// struct D2Act3Quest0Strc;
// struct D2Act3Quest1Strc;
// struct D2Act3Quest2Strc;
// struct D2Act3Quest3Strc;
// struct D2Act3Quest4Strc;
// struct D2Act3Quest5Strc;
// struct D2Act3Quest6Strc;
// struct D2Act3Quest7Strc;
// struct D2Act4Quest1Strc;
// struct D2Act4Quest2Strc;
// struct D2Act4Quest3Strc;
// struct D2Act5Quest1Strc;
// struct D2Act5Quest2Strc;
// struct D2Act5Quest3Strc;
// struct D2Act5Quest4Strc;
// struct D2Act5Quest5Strc;
// struct D2Act5Quest6Strc;
// struct D2ActiveRoomStrc;
// struct D2AiCallback11ArgStrc;
// struct D2AiCallback7ArgStrc;
// struct D2AiCmdStrc;
// struct D2AiControlStrc;
// struct D2AiParamStrc;
// struct D2AiTableStrc;
// struct D2AiTickParamStrc;
// struct D2AltDrawStrc;
// struct D2AnimDataBucketStrc;
// struct D2AnimDataRecordStrc;
// struct D2AnimDataTableStrc;
// struct D2AnimSeqRecordStrc;
// struct D2AnimSeqTxt;
// struct D2AnimatedImageDescriptor;
// struct D2AnvilUIButtonStrc;
// struct D2ArchiveHandleStrc;
// struct D2ArenaStrc;
// struct D2ArenaTxt;
// struct D2ArenaUnitStrc;
// struct D2ArmTypeTxt;
// struct D2AuraCallbackStrc;
// struct D2AutomapCellStrc;
// struct D2AutomapLayerStrc;
// struct D2AutomapRandStrc;
// struct D2AutomapShortStrc;
// struct D2AutomapTxt;
// struct D2BaalThroneAiCallbackArgStrc;
// struct D2BeltsTxt;
// struct D2BinFieldStrc;
// struct D2BinFileStrc;
// struct D2BitBufferStrc;
// struct D2BodyLocsTxt;
// struct D2BookDataTbl;
// struct D2BooksTxt;
// struct D2BoundingBoxStrc;
// struct D2BuySellTabStrc;
// struct D2CalcCallbackInfoStrc;
// struct D2CellFileStrc;
// struct D2CharItemStrc;
// struct D2CharStatsTxt;
// struct D2CharStrc;
// struct D2CharTemplateTxt;
// struct D2CharacterPreviewInfoStrc;
// struct D2ClientInfoStrc;
// struct D2ClientKeySkillStrc;
// struct D2ClientPlayerDataStrc;
// struct D2ClientStrc;
// struct D2ClientUnitUpdateSortStrc;
// struct D2ClientUnitUpdateStrc;
struct D2CmpBuiltCellFile;
struct D2CmpCelHardwareNode;
struct D2CmpCelHeaderHash;
struct D2CmpCelHeaderHashEntry;
struct D2CmpCellBuildEntry;
struct D2CmpCellBuilder;
struct D2CmpCellBuilderArray;
struct D2CmpCellBuilderObjectChunk;
struct D2CmpCellBuilderTableHeader;
struct D2CmpCellBuilderTablePrefix;
struct D2CmpCellBuilderTableReusePrefix;
struct D2CmpCellBuilderTableVtable;
struct D2CmpCodecBitReader;
struct D2CmpCodecByteSpan;
// struct D2CmpCrtLegacyPageEntry;
// struct D2CmpCrtLegacyPagePrefix;
// struct D2CmpCrtLegacyRegion;
struct D2CmpDccCellCursor;
struct D2CmpDccCellState;
// struct D2CmpDccCursorPresenceScratch;
struct D2CmpDccDirectionContext;
struct D2CmpDccFileHeader;
struct D2CmpDccFrameState;
struct D2CmpDccPaletteMap;
struct D2CmpDccSourceInfo;
struct D2CmpFogSystemInfoPrefix;
struct D2CmpHSL;
struct D2CmpLruCache;
struct D2CmpLruEntry;
// struct D2CmpMsvcTypeInfoPrefix;
struct D2CmpNormalizedTileProject;
struct D2CmpPaletteEntry;
struct D2CmpPaletteTables;
// struct D2CmpPeLinkerVersion;
struct D2CmpRGB24;
struct D2CmpRgb24FrameHeader;
struct D2CmpRgb24RuntimeFilePrefix;
struct D2CmpRleCellHeader;
struct D2CmpSerializedCellFileV5Header;
struct D2CmpSerializedRleCellHeaderV5;
struct D2CmpSpriteDataCache;
struct D2CmpSpriteDataNode;
struct D2CmpSpriteDecodeJobPrefix;
struct D2CmpSpriteFrameHeaderPrefix;
struct D2CmpSpriteGfxBlockPrefix;
struct D2CmpSpriteGfxHashEntryPrefix;
struct D2CmpSpriteGfxHashStorage;
struct D2CmpSpriteGfxNodePrefix;
struct D2CmpSpriteHeaderData;
struct D2CmpStormLink;
struct D2CmpStormList;
struct D2CmpStormListArray;
struct D2CmpTileBuildComponent;
struct D2CmpTileBuilder;
struct D2CmpTileList;
struct D2CmpTileRuntimeComponent;
// struct D2CofDataStrc;
// struct D2CofInfoStrc;
// struct D2CofNodeStrc;
// struct D2CollisionFieldStrc;
// struct D2ColorsTxt;
// struct D2CombatStrc;
// struct D2CompCodeTxt;
// struct D2CompositStrc;
// struct D2CompositTxt;
// struct D2CompositeUnitStrc;
// struct D2ConfigStrc;
// struct D2CoordStrc;
// struct D2CorpseStrc;
// struct D2CubeInputItem;
// struct D2CubeItemStrc;
// struct D2CubeMainTxt;
// struct D2CubeOutputItem;
// struct D2CubeOutputParamStrc;
// struct D2CubeTableStrc;
// struct D2CurseStrc;
// struct D2DamageInfoStrc;
// struct D2DamageStatTableStrc;
// struct D2DamageStrc;
// struct D2DataTablesStrc;
// struct D2DifficultyLevelsTxt;
// struct D2DoorObjectAiCallbackArgStrc;
// struct D2DrlgActStrc;
// struct D2DrlgAnimTileGridStrc;
// struct D2DrlgBuildStrc;
// struct D2DrlgCoordStrc;
// struct D2DrlgCoordsStrc;
// struct D2DrlgDeleteStrc;
// struct D2DrlgEnvironmentStrc;
// struct D2DrlgFileStrc;
// struct D2DrlgGridStrc;
// struct D2DrlgLevelLinkDataStrc;
// struct D2DrlgLevelStrc;
// struct D2DrlgLinkStrc;
// struct D2DrlgLinkerParamsStrc;
// struct D2DrlgLogicalRoomInfoStrc;
// struct D2DrlgMapStrc;
// struct D2DrlgOrthStrc;
// struct D2DrlgOutdoorGridStrc;
// struct D2DrlgOutdoorInfoStrc;
// struct D2DrlgOutdoorRoomStrc;
// struct D2DrlgPresetInfoStrc;
// struct D2DrlgPresetRoomStrc;
// struct D2DrlgRGBStrc;
// struct D2DrlgRoomStrc;
// struct D2DrlgRoomTilesStrc;
// struct D2DrlgStrc;
// struct D2DrlgSubstGroupStrc;
// struct D2DrlgTileDataStrc;
// struct D2DrlgTileGridStrc;
// struct D2DrlgTileInfoStrc;
// struct D2DrlgTileLinkStrc;
// struct D2DrlgUnitsStrc;
// struct D2DrlgVertexStrc;
// struct D2DrlgWarpStrc;
// struct D2DynamicPathStrc;
// struct D2EffectStrc;
// struct D2ElemTypesTxt;
// struct D2EnvironmentCycleStrc;
// struct D2EventTimerQueueStrc;
// struct D2EventTimerSlabListStrc;
// struct D2EventTimerStrc;
// struct D2EventsTxt;
// struct D2ExperienceDataTbl;
// struct D2ExperienceTxt;
// struct D2FallenShamanAiCallbackArgStrc;
// struct D2FieldStrc;
// struct D2FontCacheStrc;
// struct D2FontStrc;
// struct D2GSPacketClt01;
// struct D2GSPacketClt02;
// struct D2GSPacketClt03;
// struct D2GSPacketClt04;
// struct D2GSPacketClt05;
// struct D2GSPacketClt06;
// struct D2GSPacketClt07;
// struct D2GSPacketClt08;
// struct D2GSPacketClt09;
// struct D2GSPacketClt0A;
// struct D2GSPacketClt0B;
// struct D2GSPacketClt0C;
// struct D2GSPacketClt0D;
// struct D2GSPacketClt0E;
// struct D2GSPacketClt0F;
// struct D2GSPacketClt10;
// struct D2GSPacketClt11;
// struct D2GSPacketClt12;
// struct D2GSPacketClt13;
// struct D2GSPacketClt14;
// struct D2GSPacketClt15;
// struct D2GSPacketClt16;
// struct D2GSPacketClt17;
// struct D2GSPacketClt18;
// struct D2GSPacketClt19;
// struct D2GSPacketClt20;
// struct D2GSPacketClt23;
// struct D2GSPacketClt26;
// struct D2GSPacketClt27;
// struct D2GSPacketClt29;
// struct D2GSPacketClt2A;
// struct D2GSPacketClt2B;
// struct D2GSPacketClt31;
// struct D2GSPacketClt32;
// struct D2GSPacketClt33;
// struct D2GSPacketClt38;
// struct D2GSPacketClt3A;
// struct D2GSPacketClt3B;
// struct D2GSPacketClt3C;
// struct D2GSPacketClt3D;
// struct D2GSPacketClt41;
// struct D2GSPacketClt42;
// struct D2GSPacketClt44;
// struct D2GSPacketClt45;
// struct D2GSPacketClt49;
// struct D2GSPacketClt4F;
// struct D2GSPacketClt51;
// struct D2GSPacketClt58;
// struct D2GSPacketClt59;
// struct D2GSPacketClt5D;
// struct D2GSPacketClt5E;
// struct D2GSPacketClt5F;
// struct D2GSPacketClt63;
// struct D2GSPacketClt64;
// struct D2GSPacketClt65;
// struct D2GSPacketClt66;
// struct D2GSPacketClt67;
// struct D2GSPacketClt68;
// struct D2GSPacketClt69;
// struct D2GSPacketClt6A;
// struct D2GSPacketClt6B;
// struct D2GSPacketClt6C;
// struct D2GSPacketClt6D;
// struct D2GSPacketClt6F;
// struct D2GSPacketSrv00;
// struct D2GSPacketSrv01;
// struct D2GSPacketSrv02;
// struct D2GSPacketSrv03;
// struct D2GSPacketSrv04;
// struct D2GSPacketSrv05;
// struct D2GSPacketSrv06;
// struct D2GSPacketSrv07;
// struct D2GSPacketSrv08;
// struct D2GSPacketSrv09;
// struct D2GSPacketSrv0A;
// struct D2GSPacketSrv0B;
// struct D2GSPacketSrv0C;
// struct D2GSPacketSrv0D;
// struct D2GSPacketSrv0E;
// struct D2GSPacketSrv0F;
// struct D2GSPacketSrv10;
// struct D2GSPacketSrv11;
// struct D2GSPacketSrv12;
// struct D2GSPacketSrv13;
// struct D2GSPacketSrv14;
// struct D2GSPacketSrv15;
// struct D2GSPacketSrv16;
// struct D2GSPacketSrv17;
// struct D2GSPacketSrv18;
// struct D2GSPacketSrv19;
// struct D2GSPacketSrv1A;
// struct D2GSPacketSrv1B;
// struct D2GSPacketSrv1C;
// struct D2GSPacketSrv20;
// struct D2GSPacketSrv21;
// struct D2GSPacketSrv22;
// struct D2GSPacketSrv23;
// struct D2GSPacketSrv24;
// struct D2GSPacketSrv25;
// struct D2GSPacketSrv26;
// struct D2GSPacketSrv26Args;
// struct D2GSPacketSrv27;
// struct D2GSPacketSrv28;
// struct D2GSPacketSrv29;
// struct D2GSPacketSrv2A;
// struct D2GSPacketSrv2B;
// struct D2GSPacketSrv2C;
// struct D2GSPacketSrv3A;
// struct D2GSPacketSrv3B;
// struct D2GSPacketSrv3E;
// struct D2GSPacketSrv3F;
// struct D2GSPacketSrv40;
// struct D2GSPacketSrv42;
// struct D2GSPacketSrv45;
// struct D2GSPacketSrv47;
// struct D2GSPacketSrv48;
// struct D2GSPacketSrv4C;
// struct D2GSPacketSrv4D;
// struct D2GSPacketSrv4E;
// struct D2GSPacketSrv4F;
// struct D2GSPacketSrv50;
// struct D2GSPacketSrv51;
// struct D2GSPacketSrv52;
// struct D2GSPacketSrv53;
// struct D2GSPacketSrv54;
// struct D2GSPacketSrv55;
// struct D2GSPacketSrv57;
// struct D2GSPacketSrv58;
// struct D2GSPacketSrv59;
// struct D2GSPacketSrv5A;
// struct D2GSPacketSrv5B;
// struct D2GSPacketSrv5C;
// struct D2GSPacketSrv5D;
// struct D2GSPacketSrv5E;
// struct D2GSPacketSrv5F;
// struct D2GSPacketSrv60;
// struct D2GSPacketSrv61;
// struct D2GSPacketSrv62;
// struct D2GSPacketSrv63;
// struct D2GSPacketSrv65;
// struct D2GSPacketSrv66;
// struct D2GSPacketSrv67;
// struct D2GSPacketSrv68;
// struct D2GSPacketSrv69;
// struct D2GSPacketSrv6A;
// struct D2GSPacketSrv6B;
// struct D2GSPacketSrv6C;
// struct D2GSPacketSrv6D;
// struct D2GSPacketSrv6E;
// struct D2GSPacketSrv6F;
// struct D2GSPacketSrv70;
// struct D2GSPacketSrv71;
// struct D2GSPacketSrv72;
// struct D2GSPacketSrv73;
// struct D2GSPacketSrv74;
// struct D2GSPacketSrv75;
// struct D2GSPacketSrv76;
// struct D2GSPacketSrv77;
// struct D2GSPacketSrv78;
// struct D2GSPacketSrv79;
// struct D2GSPacketSrv7A;
// struct D2GSPacketSrv7B;
// struct D2GSPacketSrv7C;
// struct D2GSPacketSrv7D;
// struct D2GSPacketSrv7E;
// struct D2GSPacketSrv7F;
// struct D2GSPacketSrv81;
// struct D2GSPacketSrv82;
// struct D2GSPacketSrv89;
// struct D2GSPacketSrv8A;
// struct D2GSPacketSrv8B;
// struct D2GSPacketSrv8C;
// struct D2GSPacketSrv8D;
// struct D2GSPacketSrv8E;
// struct D2GSPacketSrv8F;
// struct D2GSPacketSrv90;
// struct D2GSPacketSrv91;
// struct D2GSPacketSrv92;
// struct D2GSPacketSrv93;
// struct D2GSPacketSrv94;
// struct D2GSPacketSrv94__SkillInfo;
// struct D2GSPacketSrv95;
// struct D2GSPacketSrv96;
// struct D2GSPacketSrv97;
// struct D2GSPacketSrv98;
// struct D2GSPacketSrv99;
// struct D2GSPacketSrv9A;
// struct D2GSPacketSrv9B;
// struct D2GSPacketSrv9C;
// struct D2GSPacketSrv9D;
// struct D2GSPacketSrvA3;
// struct D2GSPacketSrvA4;
// struct D2GSPacketSrvA5;
// struct D2GSPacketSrvA6;
// struct D2GSPacketSrvA7;
// struct D2GSPacketSrvA8;
// struct D2GSPacketSrvA9;
// struct D2GSPacketSrvAA;
// struct D2GSPacketSrvAB;
// struct D2GSPacketSrvAC;
// struct D2GSPacketSrvAD;
// struct D2GSPacketSrvAE;
// struct D2GSPacketSrvAF;
// struct D2GSPacketSrvB0;
// struct D2GSPacketSrvB1;
// struct D2GSPacketSrvB2;
// struct D2GSPacketSrvB3;
// struct D2GSPacketSrvB4;
// struct D2GSPacketSrvFF01;
// struct D2GSPacketSrvFFFD;
// struct D2GSPacketSrvStat;
// struct D2GSPacketSrvStatAndGuid;
// struct D2GambleDataTbl;
// struct D2GambleTxt;
// struct D2GameDataTableStrc;
// struct D2GameInfoStrc;
// struct D2GameServerInfoExStrc;
// struct D2GameServerInfoStrc;
// struct D2GameStatisticsStrc;
// struct D2GameStrc;
// struct D2GemDataTbl;
// struct D2GemsTxt;
// struct D2GfxCellNodeStrc;
// struct D2GfxCellStrc;
struct D2GfxDataStrc;
// struct D2GfxHelperStrc;
// struct D2GfxInfoStrc;
// struct D2GfxLightExStrc;
// struct D2GfxLightStrc;
// struct D2GfxSettingsStrc;
// struct D2GraphicsInterfaceStrc;
// struct D2GuildInformationStrc;
// struct D2HireDescTxt;
// struct D2HirelingInitStrc;
// struct D2HirelingTxt;
// struct D2HitClassTxt;
// struct D2HoverTextStrc;
// struct D2InactiveItemNodeStrc;
// struct D2InactiveMonsterNodeStrc;
// struct D2InactiveUnitListStrc;
// struct D2InactiveUnitNodeStrc;
// struct D2InteractInfoStrc;
// struct D2InvCompGridStrc;
// struct D2InvRectStrc;
// struct D2InventoryGridInfoStrc;
// struct D2InventoryGridStrc;
// struct D2InventoryNodeStrc;
// struct D2InventoryStrc;
// struct D2InventoryTxt;
// struct D2ItemCacheStrc;
// struct D2ItemCalcStrc;
// struct D2ItemDataStrc;
// struct D2ItemDataTbl;
// struct D2ItemDropStrc;
// struct D2ItemExtraDataStrc;
// struct D2ItemModeArgStrc;
// struct D2ItemRatioDataTbl;
// struct D2ItemRatioTxt;
// struct D2ItemSaveStrc;
// struct D2ItemStatCostDescStrc;
// struct D2ItemStatCostTxt;
// struct D2ItemTradeStrc;
// struct D2ItemTypesTxt;
// struct D2ItemsTxt;
// struct D2JungleStrc;
// struct D2KeyConfigOptionStrc;
// struct D2KeyConfigStrc;
// struct D2LevelDefBin;
// struct D2LevelFileListStrc;
// struct D2LevelsTxt;
// struct D2LinkStrc;
// struct D2LowQualityItemDataTbl;
// struct D2LowQualityItemsTxt;
// struct D2LvlMazeTxt;
// struct D2LvlPrestTxt;
// struct D2LvlSubTxt;
// struct D2LvlTypesTxt;
// struct D2LvlWarpTxt;
// struct D2MagicAffixDataTbl;
// struct D2MagicAffixTxt;
// struct D2MapAIPathPositionStrc;
// struct D2MapAIStrc;
// struct D2MazeLevelIdStrc;
// struct D2MenuEntryStrc;
// struct D2MenuInfoStrc;
// struct D2MenuItemStrc;
// struct D2MenuUIStateSaveStrc;
// struct D2MercDataStrc;
// struct D2MercSaveDataStrc;
// struct D2MessageListStrc;
// struct D2MessageStrc;
// struct D2MinionListStrc;
// struct D2MissCalcTxt;
// struct D2MissileCalcStrc;
// struct D2MissileDamageDataStrc;
// struct D2MissileDataStrc;
// struct D2MissileStrc;
// struct D2MissileStreamStrc;
// struct D2MissileUnitFindArgStrc;
// struct D2MissileUnitFindTableStrc;
// struct D2MissilesTxt;
// struct D2ModStatsArrayStrc;
// struct D2ModeChangeStrc;
// struct D2MonAiTxt;
// struct D2MonEquipTxt;
// struct D2MonItemPercentDataTbl;
// struct D2MonItemPercentTxt;
// struct D2MonLvlTxt;
// struct D2MonModeCallbackTableStrc;
// struct D2MonModeDataTbl;
// struct D2MonModeTxt;
// struct D2MonModeTxtStub;
// struct D2MonPlaceTxt;
// struct D2MonPresetTxt;
// struct D2MonPropTxt;
// struct D2MonRegDataStrc;
// struct D2MonSkillInfoStrc;
// struct D2MonSoundsTxt;
// struct D2MonStats2Txt;
// struct D2MonStatsInitStrc;
// struct D2MonStatsTxt;
// struct D2MonTypeTxt;
// struct D2MonUModTxt;
// struct D2MonsterDataStrc;
// struct D2MonsterInteractStrc;
// struct D2MonsterRegionStrc;
// struct D2NPCMessageListStrc;
// struct D2NPCMessageStrc;
// struct D2NPCMessageTableStrc;
// struct D2NpcControlStrc;
// struct D2NpcEventStrc;
// struct D2NpcGambleStrc;
// struct D2NpcRecordStrc;
// struct D2NpcTradeStrc;
// struct D2NpcTxt;
// struct D2NpcVendorChainStrc;
// struct D2ObeliskPowerUpStrc;
// struct D2ObjGroupTxt;
// struct D2ObjInitFnStrc;
// struct D2ObjModeDataTbl;
// struct D2ObjModeTypeTxt;
// struct D2ObjOperateFnStrc;
// struct D2ObjectControlStrc;
// struct D2ObjectDataStrc;
// struct D2ObjectRegionStrc;
// struct D2ObjectRoomCoordStrc;
// struct D2ObjectsTxt;
// struct D2OpStatDataStrc;
// struct D2OverlayTxt;
// struct D2PacketBufferStrc;
// struct D2PacketDataStrc;
// struct D2PacketListStrc;
// struct D2PacketStrc;
// struct D2PacketTableStrc;
// struct D2PaletteStrc;
// struct D2PaletteTableStrc;
// struct D2PartyControlStrc;
// struct D2PartyNodeStrc;
// struct D2PartyStrc;
// struct D2PathFoWallContextStrc;
// struct D2PathFoWallNodeStrc;
// struct D2PathIDAStarContextStrc;
// struct D2PathIDAStarNodeStrc;
// struct D2PathInfoStrc;
// struct D2PathPointStrc;
// struct D2PetArgStrc;
// struct D2PetDataStrc;
// struct D2PetInfoStrc;
// struct D2PetListStrc;
// struct D2PetTypeTxt;
// struct D2PlayerClassTxt;
// struct D2PlayerCountBonusStrc;
// struct D2PlayerDataStrc;
// struct D2PlayerListStrc;
// struct D2PlayerPetStrc;
// struct D2PlayerTradeStrc;
// struct D2PlayerWeaponSequencesStrc;
// struct D2PlrIntroStrc;
// struct D2PlrModeDataTbl;
// struct D2PlrModeTxtStub;
// struct D2PlrModeTypeTxt;
// struct D2PresetUnitStrc;
// struct D2PrisonOfIceRewardStrc;
// struct D2PropertiesTxt;
// struct D2PropertyAssignStrc;
// struct D2PropertyStrc;
// struct D2QualityItemDataTbl;
// struct D2QualityItemsTxt;
// struct D2QuestArgStrc;
// struct D2QuestChainStrc;
// struct D2QuestDataStrc;
// struct D2QuestDescStrc;
// struct D2QuestDescriptorStrc;
// struct D2QuestGUIDStrc;
// struct D2QuestInfoStrc;
// struct D2QuestInitTableStrc;
// struct D2QuestIntroTableStrc;
// struct D2QuestTimerStrc;
// struct D2QuestUiButtonCoordStrc;
// struct D2QuestUiStrc;
// struct D2QuestUiTabStrc;
// struct D2RareAffixDataTbl;
// struct D2RareAffixTxt;
// struct D2RoomCollisionGridStrc;
// struct D2RoomCoordListStrc;
// struct D2RoomTileStrc;
// struct D2RosterButtonStrc;
// struct D2RosterControlStrc;
// struct D2RosterCorpseStrc;
// struct D2RosterInfoStrc;
// struct D2RosterPetStrc;
// struct D2RosterUnitStrc;
// struct D2RuneDataTbl;
// struct D2RunesTxt;
// struct D2SLayerStatIdStrc;
// struct D2SaveHeaderStrc;
// struct D2SaveSkillKeyStrc;
// struct D2SavedItemStrc;
// struct D2SeedStrc;
// struct D2ServerCallbackFunctions;
// struct D2SetItemsTxt;
// struct D2SetsTxt;
// struct D2ShadowLightGammaPaletteStrc;
// struct D2ShrineDataStrc;
// struct D2ShrineTableStrc;
// struct D2ShrinesTxt;
// struct D2SkillCalcStrc;
// struct D2SkillCalcTxt;
// struct D2SkillDescTxt;
// struct D2SkillListStrc;
// struct D2SkillStrc;
// struct D2SkillTreeChartStrc;
// struct D2SkillsTxt;
// struct D2SoundsTxtStub;
// struct D2SplittedTextStrc;
// struct D2StatListExStrc;
// struct D2StatListStrc;
// struct D2StatStrc;
// struct D2StatesTxt;
// struct D2StaticFieldCallbackArgStrc;
// struct D2StaticPathStrc;
// struct D2StatsArrayStrc;
// struct D2StorePageTxt;
// struct D2SummonArgStrc;
// struct D2SuperUniquesTxt;
// struct D2TCExInfoStrc;
// struct D2TCExShortStrc;
// struct D2TargetNodeStrc;
// struct D2TaskStrc;
// struct D2TblHeaderStrc;
// struct D2TblNodeStrc;
// struct D2TextHeaderStrc;
// struct D2TextMessageStrc;
// struct D2TextNodeStrc;
// struct D2TileLibraryBlockStrc;
struct D2TileLibraryEntryStrc;
struct D2TileLibraryHashNodeStrc;
struct D2TileLibraryHashRefStrc;
struct D2TileLibraryHashStrc;
struct D2TileLibraryHeaderStrc;
struct D2TileRecordStrc;
// struct D2TileStrc;
// struct D2TimerArg2Strc;
// struct D2TimerArgStrc;
// struct D2TreasureClassExTxt;
// struct D2TxtLinkNodeStrc;
// struct D2TxtLinkStrc;
// struct D2TxtLinkTblStrc;
// struct D2UniqueAppellationTxt;
// struct D2UniqueItemsTxt;
// struct D2UniquePrefixTxt;
// struct D2UniqueSuffixTxt;
// struct D2UniqueTitleTxt;
// struct D2UnitDescriptionListStrc;
// struct D2UnitEventStrc;
// struct D2UnitFindArgStrc;
// struct D2UnitFindDataStrc;
// struct D2UnitInfoStrc;
// struct D2UnitPacketListStrc;
// struct D2UnitProxyStrc;
// struct D2UnitStrc;
// struct D2UnkAiCallbackArgStrc;
// struct D2UnkAiCallbackArgStrc2;
// struct D2UnkDrlgLogicStrc;
// struct D2UnkExcelStrc;
// struct D2UnkItemModeStrc;
// struct D2UnkItemModeStrc2;
// struct D2UnkMissileCalcStrc;
// struct D2UnkMonCompStrc;
// struct D2UnkMonCreateStrc;
// struct D2UnkMonCreateStrc2;
// struct D2UnkMonCreateStrc3;
// struct D2UnkMonsterDataStrc;
// struct D2UnkNecSkillStrc;
// struct D2UnkOutdoorStrc;
// struct D2UnkOutdoorStrc2;
// struct D2UnkOutdoorStrc3;
// struct D2UnkPlayerDataStrc;
// struct D2UnkPlrModeStrc2;
// struct D2VileMotherAiCallbackArgStrc;
// struct D2WaypointActTableStrc;
// struct D2WaypointCoordTableStrc;
// struct D2WaypointDataStrc;
// struct D2WaypointFlagTableStrc;
// struct D2WaypointTableStrc;
// struct D2WinAccountListDataStrc;
// struct D2WinAccountListStrc;
// struct D2WinAnimImageStrc;
// struct D2WinButtonStrc;
// struct D2WinCommandRegisterStrc;
// struct D2WinControlStrc;
// struct D2WinEditBoxStrc;
// struct D2WinImage2Strc;
// struct D2WinImageClickRectStrc;
// struct D2WinImageStrc;
// struct D2WinListDataStrc;
// struct D2WinListStrc;
// struct D2WinPopUpLineStrc;
// struct D2WinPopUpStrc;
// struct D2WinProgressBarStrc;
// struct D2WinScrollBarStrc;
// struct D2WinSmackStrc;
// struct D2WinTextBoxLineStrc;
// struct D2WinTextBoxStrc;
// struct D2WinTimerStrc;
// struct D2WindowPlacementStrc;
// struct D2_6FD106F0_Strc;
// struct D2_6FD10880_Strc;
// struct D2pSpellTblStrc;
// struct HSARCHIVE__;
// struct HSFILE__;
// struct SMSGHANDLER_PARAMS;
// struct _OVERLAPPED;
// struct anon_114F3EFF484AED11DD38F3E16D75C987;
// struct anon_14587F979C641006F380FCBF23CEC161;
// struct anon_17EA52B533BA0577390A0A91F2834258;
// struct anon_34227162FB87FBED6A1B09BFAFEE6B72;
// union anon_380F2128073F347D8978CA0677085670;
// struct anon_3F2D4F5E8D1F490FBA7D93D8C6A24CCE;
// union anon_41E2BDCDD1C2585266E3BA819DC7D701;
// struct anon_427EBAF5AD08AB583E1D4DF460AB8325;
// struct anon_50DA7804EBF03AEE90BE8F81913C2806;
// struct anon_527403D9EC2CF491362D3AE59B5ADD67;
// struct anon_55738B68E7BB1318E56D3D4C5D360CAA;
// struct anon_63503F4130976E24F3A9E580D3B242ED;
// union anon_83B6E30208D726A49ED3B6646DA7606D;
// struct anon_8E1104106B2EB30A90505EAA0C598AB1;
// union anon_9061D5506B188DF6886B65C2F499A751;
// union anon_9DEA95786DEDD61FF48E8BC3CF3A06D9;
// struct anon_9E01DBFBEC86134033FD3477EE0CD60D;
// union anon_9E656BAF27D4BA1E170AEB1A92E0D77D;
// struct anon_A0B592BF32EB0681EBBB5D252D2EFAF3;
// struct anon_A3719FF1A699486E8629E16291555876;
// struct anon_A85CD68F813312C5E6BB1494B5EEB9CC;
// struct anon_ABAA6F78D0909A5EC38B8DAC19886F2D;
// struct anon_B40DC28E98FE5F06F50437E23D68FF31;
// union anon_BBC6B91D16152AC7247EC2523B3ED144;
// struct anon_CAAEA91160A63143F2AE058B4EF58ACE;
// struct anon_E8C05EF12DE2F4465A905B87E8CB9726;

/* ---- D2Lang's spelling of D2C_Language ----------------------------------
 * Unicode::Personalize's exported name carries W4ELANGUAGE@@, so this is the
 * type its fifth parameter has to be declared with -- a typedef of
 * D2C_Language mangles to W4D2C_Language@@ and does not link. Opaque on
 * purpose: C++11 makes an enum with a fixed underlying type complete, so it
 * can be passed and cast, and the enumerators are D2Constants.h's.
 *------------------------------------------------------------------------*/
enum ELANGUAGE : int;

/* ---- definitions: pack(1) and the _gapN members reproduce the binary's layouts ---- */
#pragma pack(push, 1)
struct AsyncDataLoadFileCallback
{
uint32_t (__stdcall *callback)(void *pFileData, uint32_t nDataLength, AsyncDataLoadFileCallback *pCallbackData);
uint32_t userData[4];
void *pData;
uint32_t nFileSize;
};

struct AsyncData
{
void *hFile;
void *pMemPool;
OVERLAPPED tOverlapped;
AsyncDataLoadFileCallback pAsyncOpDesc;
AsyncDataEventSlot *pEventSlot;
void *pBuffer;
uint32_t nFileSize;
uint32_t nPriority;
uint32_t nFlags;
uint32_t nPreviousPriority;
AsyncData *pNextPostponedPriorityChange;
};

struct D2CmpBuiltCellFile
{
uint32_t dwVersion;
uint32_t dwFlags;
int32_t nFormat;
uint32_t dwTerminationOrTransparentColor;
uint32_t nDirections;
uint32_t nFramesPerDirection;
D2CmpRleCellHeader *pFrames[];
};

typedef void (__fastcall *D2CmpCelConvert24To8Func)(const D2CmpBuiltCellFile *pSrc, D2CmpBuiltCellFile *pDst, uint8_t (__stdcall *pfMapRgbToIndex)(uint8_t nRed, uint8_t nGreen, uint8_t nBlue), uint32_t dwTransparentColor);

typedef void (__fastcall *D2CmpCelDecodeFunc)(const D2CmpRleCellHeader *pCel, uint8_t *pDst, int32_t nX, int32_t nClipLeft, int32_t nClipRight, uint32_t dwStride, int32_t nLinesToSkip, int32_t nLinesToDraw, const uint8_t *pPaletteMap, const uint8_t *pBlendTable, const uint8_t *pPreMap);

typedef void (__fastcall *D2CmpCelHardwareFreeCallback)(void *pHardwareNode);

struct D2CmpCelHardwareNode
{
uint8_t unknown00[40];
const uint8_t *pAuxData;
};

struct D2CmpCelHeaderHash
{
uint8_t unknown_00[2048];
D2CmpCelHeaderHashEntry *pBuckets[512];
};

struct D2CmpCelHeaderHashEntry
{
uint32_t dwBucket;
char szKey[32];
D2CmpLruEntry *lpBlock;
D2CmpCelHeaderHashEntry *pPrev;
D2CmpCelHeaderHashEntry *pNext;
};

typedef void (__fastcall *D2CmpCelTransformFunc)(D2CmpRleCellHeader *pCell, const uint8_t *pPaletteMap);

struct D2CmpStormLink
{
D2CmpStormLink *pPrevLink;
void *pNextOrSentinel;
};

struct D2CmpCellBuildEntry
{
D2CmpStormLink link;
int32_t nOrientation;
uint32_t nPixelWidth;
uint32_t nPixelHeight;
uint32_t nPixelCount;
int32_t nOffsetX;
int32_t nOffsetY;
uint8_t *pPixels;
};

struct D2CmpStormList
{
int32_t nLinkOffset;
D2CmpStormLink terminator;
};

struct D2CmpCellBuilder
{
uint32_t dwHandleHash;
D2CmpStormLink slotLink;
D2CmpStormLink fullLink;
uint8_t nKey;
uint8_t unknown15[3];
uint32_t dwPixelFormatFlags;
int32_t nFormat;
uint32_t dwTransparentColor;
uint32_t nFrameCount;
uint32_t nDirectionCount;
D2CmpStormList frames;
};

struct D2CmpCellBuilderArray
{
uint32_t dwAllocated;
uint32_t dwCount;
D2CmpCellBuilder *pData;
uint32_t dwChunkSize;
};

typedef void (__fastcall *D2CmpCellBuilderEncoder)(D2CmpCellBuilder *pBuilder, D2CmpBuiltCellFile *pOutput);

struct D2CmpCellBuilderObjectChunk
{
D2CmpCellBuilderArray array;
D2CmpStormLink link;
};

struct D2CmpCellBuilderTableHeader
{
const D2CmpCellBuilderTableVtable *pVtable;
D2CmpStormList fullList;
uint32_t nFullness;
uint32_t nBucketCapacity;
uint32_t nBucketCount;
D2CmpStormList *pBuckets;
uint32_t nBucketGrowth;
uint32_t dwBucketMask;
};

struct D2CmpCellBuilderTablePrefix
{
D2CmpCellBuilderTableHeader header;
D2CmpStormList reuseList;
uint32_t dwObjectChunkSize;
D2CmpStormList chunkList;
uint8_t nDummyKey;
uint8_t unknown45[3];
uint32_t dwHandleSequence;
int32_t bHandleSequenceWrapped;
CRITICAL_SECTION criticalSection;
};

struct D2CmpCellBuilderTableReusePrefix
{
D2CmpCellBuilderTableHeader header;
D2CmpStormList reuseList;
uint32_t dwObjectChunkSize;
D2CmpStormList chunkList;
};

struct D2CmpCellBuilderTableVtable
{
void (__fastcall *FreeBuilder)(D2CmpCellBuilderTableHeader *pUnusedTable, int32_t nEdxUnused, D2CmpCellBuilder *pBuilder);
D2CmpCellBuilder *(__fastcall *NewBuilder)(D2CmpCellBuilderTableHeader *pUnusedTable, int32_t nEdxUnused, D2CmpStormList *pBucket, uint32_t nExtraBytes, uint32_t dwFlags);
D2CmpCellBuilderTableHeader *(__fastcall *pDestructor)(D2CmpCellBuilderTableHeader *pTable, int32_t nEdxUnused, uint8_t nFlags);
void (__fastcall *pClear)(D2CmpCellBuilderTableHeader *pTable);
};

struct D2CmpCodecBitReader
{
const uint32_t *pStart;
const uint32_t *pWord;
int32_t nTotalBits;
int32_t nBitPosition;
};

struct D2CmpCodecByteSpan
{
uint8_t *pData;
int32_t nBytes;
};

struct D2CmpDccCellCursor
{
int32_t nFrame;
int32_t nFrameWidth;
int32_t nFrameHeight;
int32_t nLeft;
int32_t nTop;
uint8_t *pImage;
int32_t nPitch;
int32_t nRowCellIndex;
int32_t nCellIndex;
int32_t nX;
int32_t nY;
int32_t nRowImageOffset;
int32_t nImageOffset;
int32_t nCellWidth;
int32_t nCellHeight;
};

struct D2CmpDccCellState
{
uint8_t *pCodebook;
uint8_t *pImage;
int32_t nPitch;
int32_t nWidth;
int32_t nHeight;
};

struct D2CmpDccDirectionContext
{
D2CmpDccSourceInfo *pSource;
D2CmpDccFrameState *pFrames;
int32_t nMinLeft;
int32_t nMinTop;
int32_t nPixelWidth;
int32_t nPixelHeight;
int32_t nDirection;
int32_t nFrames;
int32_t nVisitedCells;
int32_t nGridCells;
int32_t nGridColumns;
};

struct D2CmpDccFileHeader
{
uint8_t bSignature;
uint8_t bVersion;
uint8_t nDirections;
uint32_t nFramesPerDirection;
uint32_t dwTag;
uint32_t dwUncompressedSize;
};

struct D2CmpDccFrameState
{
uint32_t dwVariable;
int32_t nWidth;
int32_t nHeight;
int32_t nOffsetX;
int32_t nOffsetY;
uint32_t dwExtraBytes;
uint8_t *pExtraData;
uint32_t dwControlBytes;
uint8_t *pImage;
int32_t nPitch;
int32_t bFlip;
};

struct D2CmpDccPaletteMap
{
uint8_t entries[256];
uint32_t nCount;
};

struct D2CmpDccSourceInfo
{
uint32_t dwVersion;
uint32_t dwTag;
uint32_t nDirections;
uint32_t nFrames;
};

struct D2CmpFogSystemInfoPrefix
{
uint32_t bInitialized;
uint8_t unknown_04[8];
uint32_t dwTotalPhysicalBytes;
};

struct D2CmpHSL
{
double hue;
double saturation;
double luminance;
};

struct D2CmpLruCache
{
void *pPool;
uint32_t dwMaxAllocated;
uint32_t dwAllocated;
uint32_t nActive;
D2CmpLruEntry *pEntries;
uint32_t nEntries;
D2CmpLruEntry *pActiveHead;
D2CmpLruEntry *pActiveTail;
D2CmpLruEntry *pFreeHead;
D2CmpLruEntry *pFreeTail;
void (__fastcall *pfnOnEvict)(D2CmpLruEntry *);
uint32_t nAllocRequests;
uint32_t nAccessRequests;
};

struct D2CmpLruEntry
{
void *pData;
uint32_t dwSize;
void *pUserData;
D2CmpLruEntry *pNext;
D2CmpLruEntry *pPrev;
};

struct D2CmpNormalizedTileProject
{
uint32_t dwVersion;
uint32_t dwFlags;
char szLibraryName[260];
int32_t nNumTiles;
D2TileLibraryEntryStrc *ptTileArray;
};

typedef void (__stdcall *D2CmpPackedColorCallback)(uint32_t dwPackedColor);

struct D2CmpPaletteEntry
{
uint8_t red;
uint8_t green;
uint8_t blue;
uint8_t flags;
};

struct D2CmpRGB24
{
uint8_t red;
uint8_t green;
uint8_t blue;
};

struct D2CmpPaletteTables
{
D2CmpPaletteEntry colors[256];
uint8_t light[8192];
uint8_t brighten[4096];
uint8_t luminance[256];
uint8_t transparency[196608];
uint8_t additive[65536];
uint8_t multiply[65536];
uint8_t hueRotate[6144];
uint8_t hueRotateDark[6144];
uint8_t hueRotateLight[6144];
uint8_t grayDark[256];
uint8_t grayLight[256];
uint8_t hueRotateSelective[6144];
uint8_t reserved59700[256];
uint8_t fixedHue[3072];
uint8_t redMagnitude[256];
uint8_t greenMagnitude[256];
uint8_t blueMagnitude[256];
uint8_t reserved5A700[3584];
uint8_t darkBlend[65536];
uint8_t darken[256];
D2CmpRGB24 standardColors[13];
uint8_t reserved6B627[256];
uint8_t standardShifts[3072];
};

struct D2CmpRgb24FrameHeader
{
int32_t nOrientation;
uint32_t dwWidth;
uint32_t dwHeight;
int32_t nOffsetX;
int32_t nOffsetY;
uint32_t dwAuxiliaryBytes;
void *pHardwareData;
uint32_t dwPixelBytes;
};

struct D2CmpRgb24RuntimeFilePrefix
{
uint32_t dwVersion;
uint32_t dwPackedFileFlags;
uint32_t dwFormat;
uint32_t dwTransparentColor;
uint32_t dwDirections;
uint32_t dwFramesPerDirection;
D2CmpRgb24FrameHeader *pFrames[];
};

struct D2CmpRleCellHeader
{
uint32_t dwOrientationFlags;
uint32_t dwWidth;
uint32_t dwHeight;
int32_t nXOffset;
int32_t nYOffset;
uint32_t unknown14;
uint32_t dwNextBlockOrCellNode;
uint32_t dwLength;
};

struct D2CmpSerializedCellFileV5Header
{
uint32_t dwVersion;
uint32_t dwFlags;
uint32_t unknown08;
uint32_t unknown0C;
uint32_t nDirections;
uint32_t nFramesPerDirection;
uint32_t unknown18[7];
uint32_t frameRefs[];
};

struct D2CmpSerializedRleCellHeaderV5
{
uint32_t unknown00;
uint32_t dwOrientationFlags;
uint32_t unknown08;
uint32_t dwWidth;
uint32_t dwHeight;
int32_t nXOffset;
int32_t nYOffset;
uint32_t dwAuxiliaryBytes;
uint32_t dwAuxiliaryOffsetOrPointer;
uint32_t dwMainStreamBytes;
uint8_t pMainStream[];
};

struct D2CmpSpriteDataCache
{
void *pPool;
uint32_t dwMaxAllocated;
uint32_t dwAllocatedBytes;
uint32_t nActive;
D2CmpSpriteDataNode *pNodes;
D2CmpSpriteDataNode *pQueue0Head;
D2CmpSpriteDataNode *pQueue0Tail;
D2CmpSpriteDataNode *pQueue1Head;
D2CmpSpriteDataNode *pQueue1Tail;
D2CmpSpriteDataNode *pFreeHead;
D2CmpSpriteDataNode *pFreeTail;
void (__fastcall *pfnOnEvict)(void *);
CRITICAL_SECTION *pcs;
uint32_t unknown_34;
uint32_t unknown_38;
};

struct D2CmpSpriteDataNode
{
void *pData;
uint32_t dwSize;
void *pUserData;
uint32_t dwFlags;
uint32_t dwQueuedTick;
D2CmpSpriteDataNode *pNext;
D2CmpSpriteDataNode *pPrev;
};

struct D2CmpSpriteDecodeJobPrefix
{
uint32_t (__stdcall *pfnCallback)(void *pInput, uint32_t dwInputBytes, D2CmpSpriteDecodeJobPrefix *pJob);
D2CmpSpriteHeaderData *pHeaderData;
D2CmpSpriteGfxBlockPrefix *pGfxBlock;
int32_t nStartDirection;
int32_t nDirections;
void *pDecodedBuffer;
uint32_t dwDecodedBytes;
};

struct D2CmpSpriteFrameHeaderPrefix
{
uint32_t dwFlags;
uint32_t dwWidth;
uint32_t dwHeight;
int32_t nOffsetX;
int32_t nOffsetY;
uint32_t dwAuxiliaryBytes;
void *pHardwareData;
uint32_t dwControlBytes;
};

struct D2CmpSpriteGfxBlockPrefix
{
uint8_t bFlags;
uint8_t bNumDirections;
uint8_t bBaseDirection;
uint8_t bNumOffsets;
uint8_t bNumCelsPerDir;
uint8_t unknown_05[3];
uint32_t fHardwareCelData;
int32_t nOffsets[33];
AsyncData *lpAsyncData;
D2CmpSpriteDataNode *pCacheNode;
};

struct D2CmpSpriteGfxHashEntryPrefix
{
uint32_t dwBucket;
uint32_t dwSpecialKey;
uint32_t dwName[5];
uint32_t dwDirBlock;
D2CmpSpriteGfxNodePrefix *pGfxNode;
D2CmpSpriteDataNode *pCacheNode;
D2CmpSpriteGfxHashEntryPrefix *pPrev;
D2CmpSpriteGfxHashEntryPrefix *pNext;
};

struct D2CmpSpriteGfxHashStorage
{
uint8_t unknown_00[32768];
D2CmpSpriteGfxHashEntryPrefix *pBuckets[8192];
};

struct D2CmpSpriteGfxNodePrefix
{
int32_t nUnitType;
int32_t nMode;
int32_t nUnitIndex;
uint32_t dwName[5];
D2CmpSpriteGfxBlockPrefix tGfxBlock;
};

typedef void (__fastcall *D2CmpSpriteHardwareFreeCallback)(uint32_t dwArg0, uint32_t dwArg8, uint32_t dwArg4);

struct D2CmpSpriteHeaderData
{
uint8_t bFlags;
uint8_t unknown_01[3];
uint32_t dwVersion;
uint32_t dwFileFlags;
uint32_t dwFormat;
uint32_t unknown_10;
uint32_t nDirections;
uint32_t nFrames;
int32_t nDirectionOffsets[33];
char szKey[32];
};

struct D2CmpStormListArray
{
uint32_t dwAllocated;
uint32_t dwCount;
D2CmpStormList *pData;
uint32_t dwChunkSize;
};

struct D2CmpTileBuildComponent
{
int16_t nOffsetX;
int16_t nOffsetY;
uint8_t nGridX;
uint8_t nGridY;
uint16_t wFlags;
uint16_t nDataLength;
uint16_t unknown0A;
uint8_t *pData;
D2CmpTileBuildComponent *pNext;
};

struct D2CmpTileBuilder
{
int32_t nMinX;
int32_t nMaxY;
int32_t nMaxX;
int32_t nMinY;
int32_t nLightDirection;
uint16_t nRoofHeight;
uint16_t nFlags;
uint8_t unknown18[12];
int32_t nType;
int32_t nStyle;
int32_t nSequence;
int32_t nRarity_Frame;
uint32_t dwTransparentRGB24;
uint8_t nSubtileFlags[25];
uint8_t nPixelFormatFlags;
uint8_t unknown52[2];
uint32_t nComponentCount;
D2CmpTileBuildComponent *pComponents;
D2CmpTileBuilder *pNext;
};

typedef void (__fastcall *D2CmpTileHardwareFreeCallback)(D2TileLibraryEntryStrc *pTile);

struct D2CmpTileList
{
int32_t b24Bit;
uint32_t dwTransparentRGB24;
int32_t nNumTiles;
D2CmpTileBuilder *pTiles;
};

struct D2CmpTileRuntimeComponent
{
int16_t nOffsetX;
int16_t nOffsetY;
int16_t sCacheIndex;
uint8_t nGridX;
uint8_t nGridY;
uint16_t wFlags;
uint16_t wDataBytesLow;
uint8_t unknown0C[4];
uint8_t *pData;
};

struct D2GfxDataStrc
{
D2CmpRleCellHeader *pCurrentCell;
D2CmpBuiltCellFile *pCellFile;
uint32_t nFrame;
uint32_t nDirection;
int32_t nMaxDirections;
int32_t nMaxFrames;
uint32_t fFlags;
uint8_t fState;
union
{
uint8_t nComponent;
uint8_t fItemFlags;
};
uint8_t unknown_1E;
uint8_t unknown_1F;
int32_t nUnitType;
int32_t nUnitIndex;
int32_t nMode;
int32_t nOverlay;
union
{
struct
{
char szToken[4];
char szComponent[4];
char szArmorType[4];
char szMode[4];
char szHitClass[4];
};
uint32_t dwName[5];
};
char *pName;
};

struct D2TileLibraryEntryStrc
{
int32_t nLightDirection;
uint16_t nRoofHeight;
uint16_t nFlags;
int32_t nTotalHeight;
int32_t nWidth;
int32_t nHeightToBottom;
int32_t nType;
int32_t nStyle;
int32_t nSequence;
int32_t nRarity_Frame;
int32_t transparentColorRGB24;
uint8_t nSubtileFlags[25];
uint8_t unknown41;
int16_t sCacheIndex;
uint8_t unknown44[4];
uint32_t dwComponentFileOffset;
uint32_t dwComponentDataBytes;
int32_t nCompCount;
D2CmpTileRuntimeComponent *ptCompArray;
char *szFileName;
D2CmpLruEntry *ptLRUCache;
};

struct D2TileLibraryHashNodeStrc
{
int32_t nStyle;
int32_t nSequence;
int32_t nType;
D2TileLibraryHashRefStrc *pRef;
D2TileLibraryHashNodeStrc *pPrev;
};

struct D2TileLibraryHashRefStrc
{
D2TileLibraryEntryStrc *pTile;
D2TileLibraryHashRefStrc *pPrev;
};

struct D2TileLibraryHashStrc
{
D2TileLibraryHashNodeStrc *pNodes[128];
};

struct D2TileLibraryHeaderStrc
{
uint32_t dwVersion;
uint32_t dwFlags;
char szLibraryName[260];
int32_t nTiles;
uint32_t dwTileStart_pFirst;
};

struct D2TileRecordStrc
{
char szLibraryName[260];
D2CmpNormalizedTileProject *pLibrary;
D2TileLibraryHashStrc *pHashBlock;
D2TileRecordStrc *pPrev;
};

#pragma pack(pop)

/* ---- NOT USED BY THIS SOURCE -- the database's other types, commented out, each with its size ---- */
// struct ClientPacketSaveHeaderPart
// {
// uint8_t nHeaderId;
// uint8_t nPartDataSize;
// uint8_t bFirstPart;
// uint8_t _gap0[1];
// uint32_t nSaveHeaderSize;
// uint8_t aData[256];
// };
// static_assert(sizeof(ClientPacketSaveHeaderPart) == 264, "ClientPacketSaveHeaderPart: layout differs from the database");

// struct D2AltDrawStrc
// {
// int32_t nLeft;
// int32_t nTop;
// int32_t nRight;
// int32_t nBottom;
// D2UnitStrc *pItem;
// wchar_t wszName[128];
// int32_t nRectColor;
// int32_t nRectDrawMode;
// int32_t nColor;
// };
// static_assert(sizeof(D2AltDrawStrc) == 288, "D2AltDrawStrc: layout differs from the database");

// struct D2AnvilUIButtonStrc
// {
// uint16_t nStringId;
// uint8_t _gap0[2];
// int32_t nTextX;
// int32_t nTextY;
// int32_t nCellfileX;
// int32_t nCellfileY;
// int32_t nClickAreaXStart;
// int32_t nClickAreaXEnd;
// int32_t nClickAreaYStart;
// int32_t nClickAreaYEnd;
// int32_t nFrame;
// uint8_t bButtonClicked;
// uint8_t unk0x27;
// uint8_t _gap1[2];
// };
// static_assert(sizeof(D2AnvilUIButtonStrc) == 44, "D2AnvilUIButtonStrc: layout differs from the database");

// struct D2AutomapCellStrc
// {
// uint32_t fSaved;
// uint16_t nCellNo;
// uint16_t xPixel;
// uint16_t yPixel;
// uint16_t wWeight;
// D2AutomapCellStrc *pPrev;
// D2AutomapCellStrc *pNext;
// };
// static_assert(sizeof(D2AutomapCellStrc) == 20, "D2AutomapCellStrc: layout differs from the database");

// struct D2AutomapLayerStrc
// {
// uint32_t nLayerNo;
// uint32_t fSaved;
// D2AutomapCellStrc *pFloors;
// D2AutomapCellStrc *pWalls;
// D2AutomapCellStrc *pObjects;
// D2AutomapCellStrc *pExtras;
// D2AutomapLayerStrc *pNext;
// };
// static_assert(sizeof(D2AutomapLayerStrc) == 28, "D2AutomapLayerStrc: layout differs from the database");

// struct D2BuySellTabStrc
// {
// int32_t nX;
// int32_t nY;
// uint16_t nStringIndex;
// uint8_t _gap0[2];
// BOOL bSelected;
// BOOL bActive;
// };
// static_assert(sizeof(D2BuySellTabStrc) == 20, "D2BuySellTabStrc: layout differs from the database");

// struct D2CharacterPreviewInfoStrc
// {
// uint16_t nVersion;
// uint8_t pComponents[11];
// uint8_t nClass;
// uint8_t pComponentColors[11];
// uint8_t nLevel;
// uint16_t nClientFlags;
// uint16_t nGuildFlags;
// uint8_t nGuildEmblemBgColor;
// uint8_t nGuildEmblemFgColor;
// uint8_t nGuildEmblemType;
// uint8_t _gap0[3];
// uint32_t szGuildTag;
// uint8_t pad0x25;
// uint8_t _gap1[3];
// };
// static_assert(sizeof(D2CharacterPreviewInfoStrc) == 44, "D2CharacterPreviewInfoStrc: layout differs from the database");

// struct D2ClientUnitUpdateStrc
// {
// uint8_t nUnitType;
// uint8_t _gap0[3];
// int32_t nUnitGUID;
// uint16_t nX;
// uint16_t nY;
// };
// static_assert(sizeof(D2ClientUnitUpdateStrc) == 12, "D2ClientUnitUpdateStrc: layout differs from the database");

// struct D2CmpCrtLegacyPageEntry
// {
// uint32_t dwFreeUnits;
// uint32_t dwSearchMarker;
// };
// static_assert(sizeof(D2CmpCrtLegacyPageEntry) == 8, "D2CmpCrtLegacyPageEntry: layout differs from the database");

// struct D2CmpCrtLegacyPagePrefix
// {
// uint8_t *pSearchCursor;
// uint32_t dwCursorRunUnits;
// uint8_t unitCounts[240];
// uint8_t nEndMarker;
// uint8_t unknownF9[7];
// };
// static_assert(sizeof(D2CmpCrtLegacyPagePrefix) == 256, "D2CmpCrtLegacyPagePrefix: layout differs from the database");

// struct D2CmpCrtLegacyRegion
// {
// D2CmpCrtLegacyRegion *pNext;
// D2CmpCrtLegacyRegion *pPrevious;
// D2CmpCrtLegacyPageEntry *pAllocationHint;
// D2CmpCrtLegacyPageEntry *pFirstUncommittedHint;
// uint8_t *pBase;
// uint8_t *pEnd;
// D2CmpCrtLegacyPageEntry pages[1024];
// uint8_t unknown2018[8];
// };
// static_assert(sizeof(D2CmpCrtLegacyRegion) == 8224, "D2CmpCrtLegacyRegion: layout differs from the database");

// struct D2CmpDccCursorPresenceScratch
// {
// D2CmpDccCellCursor tCursor;
// uint32_t dwPresence[8];
// };
// static_assert(sizeof(D2CmpDccCursorPresenceScratch) == 92, "D2CmpDccCursorPresenceScratch: layout differs from the database");

// struct D2CmpMsvcTypeInfoPrefix
// {
// void *pVtable;
// void *pAuxiliaryAllocation;
// };
// static_assert(sizeof(D2CmpMsvcTypeInfoPrefix) == 8, "D2CmpMsvcTypeInfoPrefix: layout differs from the database");

// struct D2CmpPeLinkerVersion
// {
// uint8_t nMajor;
// uint8_t nMinor;
// uint16_t nClearedPadding;
// };
// static_assert(sizeof(D2CmpPeLinkerVersion) == 4, "D2CmpPeLinkerVersion: layout differs from the database");

// struct D2ConfigStrc
// {
// BOOL bIsExpansion;
// uint8_t bWindow;
// uint8_t b3DFX;
// uint8_t bOpenGL;
// uint8_t bRave;
// uint8_t bD3D;
// uint8_t bPerspective;
// uint8_t bQuality;
// uint32_t dwGamma;
// uint8_t bVSync;
// uint32_t dwFramerate;
// uint32_t dwGameType;
// uint16_t wJoinID;
// char szGameName[24];
// char szServerIP[24];
// char szBattleNetIP[24];
// char szMCPIP[24];
// uint8_t _0076[4];
// uint8_t bNoPK;
// uint8_t bOpenC;
// uint8_t bAmazon;
// uint8_t bPaladin;
// uint8_t bSorceress;
// uint8_t bNecromancer;
// uint8_t bBarbarian;
// uint8_t bDruid;
// uint8_t bAssassin;
// uint8_t bInvincible;
// uint8_t _0082[48];
// char szName[24];
// char szRealm[24];
// D2CharacterPreviewInfoStrc tCharPreviewInfo;
// char szUnk[194];
// uint8_t _01D0[24];
// union
// {
// uint32_t dwCTemp;
// struct anon_8E1104106B2EB30A90505EAA0C598AB1 unpackedCTemp;
// };
// uint8_t bNoMonsters;
// uint32_t dwMonsterClass;
// uint8_t bMonsterInfo;
// uint32_t dwMonsterDebug;
// uint8_t bRare;
// uint8_t bUnique;
// uint8_t _01DA[2];
// uint32_t dwAct;
// uint8_t bNoPreload;
// uint8_t bDirect;
// uint8_t bLowEnd;
// uint8_t bNoCompress;
// uint16_t wArena;
// uint32_t nArenaFlags;
// uint8_t nArenaTemplate;
// uint8_t _01E9[2];
// uint8_t nArenaDifficulty;
// BOOL (__stdcall *pAllowExpansionCallback)();
// uint8_t bTxt;
// uint8_t bLog;
// uint8_t bMsgLog;
// uint8_t bSafeMode;
// uint8_t bNoSave;
// uint32_t dwSeed;
// uint8_t bCheats;
// uint8_t bTeen;
// uint8_t bNoSound;
// uint8_t bQuests;
// uint8_t _01F9;
// uint8_t bBuild;
// struct BnClientInterface *pComInterface;
// uint32_t nTokenId;
// char szCharacterRealm[24];
// char szGamePassword[24];
// char szGameStatstring[256];
// uint8_t bSkipToBNet;
// D2C_LaunchType nLaunchType;
// uint8_t bShownLogo;
// uint8_t bUnk035A;
// char szCurrentChannelName[32];
// char szDefaultChannelName[32];
// uint8_t nComponents[16];
// uint8_t nComponentsColors[16];
// uint8_t nCharacterLevel;
// uint8_t bLadder;
// uint32_t nAccountPasswordHash;
// uint32_t nAccountPasswordLength;
// uint16_t nSaveFlags;
// };
// static_assert(sizeof(D2ConfigStrc) == 996, "D2ConfigStrc: layout differs from the database");

// struct D2CoordStrc
// {
// int32_t nX;
// int32_t nY;
// };
// static_assert(sizeof(D2CoordStrc) == 8, "D2CoordStrc: layout differs from the database");

// struct D2GSPacketClt01
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nPosX;
// uint16_t nPosY;
// };
// static_assert(sizeof(D2GSPacketClt01) == 6, "D2GSPacketClt01: layout differs from the database");

// struct D2GSPacketClt02
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwUnitType;
// uint32_t dwUnitGUID;
// };
// static_assert(sizeof(D2GSPacketClt02) == 12, "D2GSPacketClt02: layout differs from the database");

// struct D2GSPacketClt03
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nPosX;
// uint16_t nPosY;
// };
// static_assert(sizeof(D2GSPacketClt03) == 6, "D2GSPacketClt03: layout differs from the database");

// struct D2GSPacketClt04
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwUnitType;
// uint32_t dwUnitGUID;
// };
// static_assert(sizeof(D2GSPacketClt04) == 12, "D2GSPacketClt04: layout differs from the database");

// struct D2GSPacketClt05
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nPosX;
// uint16_t nPosY;
// };
// static_assert(sizeof(D2GSPacketClt05) == 6, "D2GSPacketClt05: layout differs from the database");

// struct D2GSPacketClt06
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwUnitType;
// uint32_t dwUnitGUID;
// };
// static_assert(sizeof(D2GSPacketClt06) == 12, "D2GSPacketClt06: layout differs from the database");

// struct D2GSPacketClt07
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwUnitType;
// uint32_t dwUnitGUID;
// };
// static_assert(sizeof(D2GSPacketClt07) == 12, "D2GSPacketClt07: layout differs from the database");

// struct D2GSPacketClt08
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nPosX;
// uint16_t nPosY;
// };
// static_assert(sizeof(D2GSPacketClt08) == 6, "D2GSPacketClt08: layout differs from the database");

// struct D2GSPacketClt09
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwUnitType;
// uint32_t dwUnitGUID;
// };
// static_assert(sizeof(D2GSPacketClt09) == 12, "D2GSPacketClt09: layout differs from the database");

// struct D2GSPacketClt0A
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwUnitType;
// uint32_t dwUnitGUID;
// };
// static_assert(sizeof(D2GSPacketClt0A) == 12, "D2GSPacketClt0A: layout differs from the database");

// struct D2GSPacketClt0B
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketClt0B) == 1, "D2GSPacketClt0B: layout differs from the database");

// struct D2GSPacketClt0C
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nPosX;
// uint16_t nPosY;
// };
// static_assert(sizeof(D2GSPacketClt0C) == 6, "D2GSPacketClt0C: layout differs from the database");

// struct D2GSPacketClt0D
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwUnitType;
// uint32_t dwUnitGUID;
// };
// static_assert(sizeof(D2GSPacketClt0D) == 12, "D2GSPacketClt0D: layout differs from the database");

// struct D2GSPacketClt0E
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwUnitType;
// uint32_t dwUnitGUID;
// };
// static_assert(sizeof(D2GSPacketClt0E) == 12, "D2GSPacketClt0E: layout differs from the database");

// struct D2GSPacketClt0F
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nPosX;
// uint16_t nPosY;
// };
// static_assert(sizeof(D2GSPacketClt0F) == 6, "D2GSPacketClt0F: layout differs from the database");

// struct D2GSPacketClt10
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwUnitType;
// uint32_t dwUnitGUID;
// };
// static_assert(sizeof(D2GSPacketClt10) == 12, "D2GSPacketClt10: layout differs from the database");

// struct D2GSPacketClt11
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwUnitType;
// uint32_t dwUnitGUID;
// };
// static_assert(sizeof(D2GSPacketClt11) == 12, "D2GSPacketClt11: layout differs from the database");

// struct D2GSPacketClt12
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketClt12) == 1, "D2GSPacketClt12: layout differs from the database");

// struct D2GSPacketClt13
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwUnitType;
// uint32_t dwUnitGUID;
// };
// static_assert(sizeof(D2GSPacketClt13) == 12, "D2GSPacketClt13: layout differs from the database");

// struct D2GSPacketClt14
// {
// uint8_t nHeader;
// uint8_t nMessageType;
// uint8_t nLang;
// char szMessage[256];
// };
// static_assert(sizeof(D2GSPacketClt14) == 259, "D2GSPacketClt14: layout differs from the database");

// struct D2GSPacketClt15
// {
// uint8_t nHeader;
// uint8_t nMessageType;
// uint8_t nLang;
// char szMessage[256];
// char szName[16];
// };
// static_assert(sizeof(D2GSPacketClt15) == 275, "D2GSPacketClt15: layout differs from the database");

// struct D2GSPacketClt16
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwUnitType;
// uint32_t dwUnitGUID;
// uint32_t bCursor;
// };
// static_assert(sizeof(D2GSPacketClt16) == 16, "D2GSPacketClt16: layout differs from the database");

// struct D2GSPacketClt17
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwItemGUID;
// };
// static_assert(sizeof(D2GSPacketClt17) == 8, "D2GSPacketClt17: layout differs from the database");

// struct D2GSPacketClt18
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwItemGUID;
// uint32_t dwPosX;
// uint32_t dwPosY;
// uint32_t dwInvPage;
// };
// static_assert(sizeof(D2GSPacketClt18) == 20, "D2GSPacketClt18: layout differs from the database");

// struct D2GSPacketClt19
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwItemGUID;
// };
// static_assert(sizeof(D2GSPacketClt19) == 8, "D2GSPacketClt19: layout differs from the database");

// struct D2GSPacketClt20
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t nItemGUID;
// int32_t nPosX;
// int32_t nPosY;
// };
// static_assert(sizeof(D2GSPacketClt20) == 16, "D2GSPacketClt20: layout differs from the database");

// struct D2GSPacketClt23
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t unk0x01;
// int32_t nBeltSlotId;
// };
// static_assert(sizeof(D2GSPacketClt23) == 12, "D2GSPacketClt23: layout differs from the database");

// struct D2GSPacketClt26
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t nItemGUID;
// uint32_t bUseOnMerc;
// int32_t unk0x09;
// };
// static_assert(sizeof(D2GSPacketClt26) == 16, "D2GSPacketClt26: layout differs from the database");

// struct D2GSPacketClt27
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t nTargetItemGUID;
// int32_t nUseItemGUID;
// };
// static_assert(sizeof(D2GSPacketClt27) == 12, "D2GSPacketClt27: layout differs from the database");

// struct D2GSPacketClt29
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t nScrollGUID;
// int32_t nBookGUID;
// };
// static_assert(sizeof(D2GSPacketClt29) == 12, "D2GSPacketClt29: layout differs from the database");

// struct D2GSPacketClt2A
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwItemGUID;
// uint32_t dwCubeGUID;
// };
// static_assert(sizeof(D2GSPacketClt2A) == 12, "D2GSPacketClt2A: layout differs from the database");

// struct D2GSPacketClt2B
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t nUnitGUID;
// uint8_t nOptionValues[39];
// uint8_t _gap1[1];
// };
// static_assert(sizeof(D2GSPacketClt2B) == 48, "D2GSPacketClt2B: layout differs from the database");

// struct D2GSPacketClt31
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t unk0x01[2];
// };
// static_assert(sizeof(D2GSPacketClt31) == 12, "D2GSPacketClt31: layout differs from the database");

// struct D2GSPacketClt32
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t dwNpcGUID;
// int32_t dwItemGUID;
// uint16_t nTransactionType;
// struct
// {
// unsigned __int16 nItemMode : 15;
// unsigned __int16 bMultibuy : 1;
// };
// int32_t dwCost;
// };
// static_assert(sizeof(D2GSPacketClt32) == 20, "D2GSPacketClt32: layout differs from the database");

// struct D2GSPacketClt33
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwMonGUID;
// uint32_t dwItemGUID;
// uint16_t nTab;
// uint16_t unk0x0B;
// uint32_t dwGold;
// };
// static_assert(sizeof(D2GSPacketClt33) == 20, "D2GSPacketClt33: layout differs from the database");

// struct D2GSPacketClt38
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t nAction;
// int32_t nNpcGUID;
// int32_t nItemGUID;
// };
// static_assert(sizeof(D2GSPacketClt38) == 16, "D2GSPacketClt38: layout differs from the database");

// struct D2GSPacketClt3A
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nStat;
// int32_t nPoints;
// };
// static_assert(sizeof(D2GSPacketClt3A) == 8, "D2GSPacketClt3A: layout differs from the database");

// struct D2GSPacketClt3B
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nSkill;
// };
// static_assert(sizeof(D2GSPacketClt3B) == 4, "D2GSPacketClt3B: layout differs from the database");

// struct D2GSPacketClt3C
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nSkill;
// uint16_t nMode;
// uint8_t _gap1[2];
// uint32_t dwFlags;
// };
// static_assert(sizeof(D2GSPacketClt3C) == 12, "D2GSPacketClt3C: layout differs from the database");

// struct D2GSPacketClt3D
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t nDoorGUID;
// };
// static_assert(sizeof(D2GSPacketClt3D) == 8, "D2GSPacketClt3D: layout differs from the database");

// struct D2GSPacketClt41
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketClt41) == 1, "D2GSPacketClt41: layout differs from the database");

// struct D2GSPacketClt42
// {
// uint8_t nHeader;
// char szVersion[10];
// };
// static_assert(sizeof(D2GSPacketClt42) == 11, "D2GSPacketClt42: layout differs from the database");

// struct D2GSPacketClt44
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t nPlayerGUID;
// int32_t nAnvilInteractGUID;
// int32_t nItemGUID;
// int16_t nState;
// int16_t unk0x0F;
// };
// static_assert(sizeof(D2GSPacketClt44) == 20, "D2GSPacketClt44: layout differs from the database");

// struct D2GSPacketClt45
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t nPortalGUID;
// int32_t nLevelId;
// };
// static_assert(sizeof(D2GSPacketClt45) == 12, "D2GSPacketClt45: layout differs from the database");

// struct D2GSPacketClt49
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t nWaypointGUID;
// int32_t nLevelId;
// };
// static_assert(sizeof(D2GSPacketClt49) == 12, "D2GSPacketClt49: layout differs from the database");

// struct D2GSPacketClt4F
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nButtonId;
// union
// {
// struct
// {
// uint16_t nParams[2];
// };
// int32_t nItemGUID;
// };
// };
// static_assert(sizeof(D2GSPacketClt4F) == 8, "D2GSPacketClt4F: layout differs from the database");

// struct D2GSPacketClt51
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t nSkillId;
// int32_t nOwnerGUID;
// };
// static_assert(sizeof(D2GSPacketClt51) == 12, "D2GSPacketClt51: layout differs from the database");

// struct D2GSPacketClt58
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nQuest;
// };
// static_assert(sizeof(D2GSPacketClt58) == 4, "D2GSPacketClt58: layout differs from the database");

// struct D2GSPacketClt59
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t nUnitType;
// uint32_t dwUnitGUID;
// uint32_t dwTargetX;
// uint32_t dwTargetY;
// };
// static_assert(sizeof(D2GSPacketClt59) == 20, "D2GSPacketClt59: layout differs from the database");

// struct D2GSPacketClt5D
// {
// uint8_t nHeader;
// uint8_t nType;
// uint8_t nToggle;
// uint8_t _gap0[1];
// uint32_t dwPlayerGUID;
// };
// static_assert(sizeof(D2GSPacketClt5D) == 8, "D2GSPacketClt5D: layout differs from the database");

// struct D2GSPacketClt5E
// {
// uint8_t nHeader;
// uint8_t nType;
// uint8_t _gap0[2];
// int32_t dwPlayerGUID;
// };
// static_assert(sizeof(D2GSPacketClt5E) == 8, "D2GSPacketClt5E: layout differs from the database");

// struct D2GSPacketClt5F
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nX;
// uint16_t nY;
// };
// static_assert(sizeof(D2GSPacketClt5F) == 6, "D2GSPacketClt5F: layout differs from the database");

// struct D2GSPacketClt63
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwItemGUID;
// };
// static_assert(sizeof(D2GSPacketClt63) == 8, "D2GSPacketClt63: layout differs from the database");

// struct D2GSPacketClt64
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t unk0x01[2];
// };
// static_assert(sizeof(D2GSPacketClt64) == 12, "D2GSPacketClt64: layout differs from the database");

// struct D2GSPacketClt65
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t unk0x01[4];
// };
// static_assert(sizeof(D2GSPacketClt65) == 20, "D2GSPacketClt65: layout differs from the database");

// struct D2GSPacketClt66
// {
// uint8_t nHeader;
// char szGameName[16];
// uint8_t nGameType;
// uint8_t nPlayerClass;
// int8_t nTemplate;
// int8_t nDifficulty;
// char szClientName[16];
// uint8_t _gap0[1];
// int16_t wArena;
// uint32_t nGameFlags;
// int8_t unk0x2B;
// int8_t unk0x2C;
// int8_t nLocale;
// uint8_t _gap1[1];
// };
// static_assert(sizeof(D2GSPacketClt66) == 48, "D2GSPacketClt66: layout differs from the database");

// struct D2GSPacketClt67
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t nTokenId;
// uint16_t nGameId;
// uint8_t nPlayerClass;
// uint8_t _gap1[1];
// int32_t unk0x08;
// uint8_t nLocale;
// char szClientName[16];
// uint8_t _gap2[3];
// };
// static_assert(sizeof(D2GSPacketClt67) == 36, "D2GSPacketClt67: layout differs from the database");

// struct D2GSPacketClt68
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketClt68) == 1, "D2GSPacketClt68: layout differs from the database");

// struct D2GSPacketClt69
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketClt69) == 1, "D2GSPacketClt69: layout differs from the database");

// struct D2GSPacketClt6A
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketClt6A) == 1, "D2GSPacketClt6A: layout differs from the database");

// struct D2GSPacketClt6B
// {
// uint8_t nHeader;
// uint8_t nSize;
// uint8_t _gap0[2];
// uint32_t nTotalSize;
// uint8_t saveData[8192];
// };
// static_assert(sizeof(D2GSPacketClt6B) == 8200, "D2GSPacketClt6B: layout differs from the database");

// struct D2GSPacketClt6C
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t unk0x01;
// int32_t unk0x05;
// };
// static_assert(sizeof(D2GSPacketClt6C) == 12, "D2GSPacketClt6C: layout differs from the database");

// struct D2GSPacketClt6D
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketClt6D) == 1, "D2GSPacketClt6D: layout differs from the database");

// struct D2GSPacketClt6F
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketClt6F) == 1, "D2GSPacketClt6F: layout differs from the database");

// struct D2GSPacketSrv00
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrv00) == 1, "D2GSPacketSrv00: layout differs from the database");

// struct D2GSPacketSrv01
// {
// uint8_t nHeader;
// uint8_t nDifficulty;
// uint8_t _gap0[2];
// uint32_t dwArenaFlags;
// uint8_t nExpansion;
// uint8_t nLadder;
// uint8_t _gap1[2];
// };
// static_assert(sizeof(D2GSPacketSrv01) == 12, "D2GSPacketSrv01: layout differs from the database");

// struct D2GSPacketSrv02
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrv02) == 1, "D2GSPacketSrv02: layout differs from the database");

// struct D2GSPacketSrv03
// {
// uint8_t nHeader;
// uint8_t nAct;
// uint8_t _gap0[2];
// uint32_t dwGameSeed;
// uint16_t nTownLevelId;
// uint8_t _gap1[2];
// uint32_t nObjectSeed;
// };
// static_assert(sizeof(D2GSPacketSrv03) == 16, "D2GSPacketSrv03: layout differs from the database");

// struct D2GSPacketSrv04
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrv04) == 1, "D2GSPacketSrv04: layout differs from the database");

// struct D2GSPacketSrv05
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrv05) == 1, "D2GSPacketSrv05: layout differs from the database");

// struct D2GSPacketSrv06
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrv06) == 1, "D2GSPacketSrv06: layout differs from the database");

// struct D2GSPacketSrv07
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nX;
// uint16_t nY;
// uint8_t nLevelId;
// uint8_t _gap1[1];
// };
// static_assert(sizeof(D2GSPacketSrv07) == 8, "D2GSPacketSrv07: layout differs from the database");

// struct D2GSPacketSrv08
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nX;
// uint16_t nY;
// uint8_t nLevelId;
// uint8_t _gap1[1];
// };
// static_assert(sizeof(D2GSPacketSrv08) == 8, "D2GSPacketSrv08: layout differs from the database");

// struct D2GSPacketSrv09
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// uint8_t unk0x06;
// uint8_t _gap1[1];
// uint16_t nPosX;
// uint16_t nPosY;
// uint8_t _gap2[2];
// };
// static_assert(sizeof(D2GSPacketSrv09) == 16, "D2GSPacketSrv09: layout differs from the database");

// struct D2GSPacketSrv0A
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// };
// static_assert(sizeof(D2GSPacketSrv0A) == 8, "D2GSPacketSrv0A: layout differs from the database");

// struct D2GSPacketSrv0B
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// };
// static_assert(sizeof(D2GSPacketSrv0B) == 8, "D2GSPacketSrv0B: layout differs from the database");

// struct D2GSPacketSrv0C
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// uint8_t unk0x06;
// uint8_t nHitClass;
// uint8_t nLife;
// uint8_t _gap1[1];
// };
// static_assert(sizeof(D2GSPacketSrv0C) == 12, "D2GSPacketSrv0C: layout differs from the database");

// struct D2GSPacketSrv0D
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// uint8_t nHitClass;
// uint8_t _gap1[1];
// uint16_t nX;
// uint16_t nY;
// uint8_t nHitType;
// uint8_t nUnitLifePct;
// };
// static_assert(sizeof(D2GSPacketSrv0D) == 16, "D2GSPacketSrv0D: layout differs from the database");

// struct D2GSPacketSrv0E
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// uint8_t unk0x06;
// uint8_t unk0x07;
// uint8_t _gap1[2];
// uint32_t unk0x08;
// };
// static_assert(sizeof(D2GSPacketSrv0E) == 16, "D2GSPacketSrv0E: layout differs from the database");

// struct D2GSPacketSrv0F
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// uint8_t nFlags;
// uint8_t _gap1[1];
// uint16_t nDestPosX;
// uint16_t nDestPosY;
// uint8_t unk0x00B;
// uint8_t _gap2[1];
// uint16_t nCurrentPosX;
// uint16_t nCurrentPosY;
// };
// static_assert(sizeof(D2GSPacketSrv0F) == 20, "D2GSPacketSrv0F: layout differs from the database");

// struct D2GSPacketSrv10
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// uint8_t nFlags;
// uint8_t nTargetType;
// uint8_t _gap1[2];
// uint32_t dwTargetGUID;
// uint16_t nCurrentPosX;
// uint16_t nCurrentPosY;
// };
// static_assert(sizeof(D2GSPacketSrv10) == 20, "D2GSPacketSrv10: layout differs from the database");

// struct D2GSPacketSrv11
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// uint16_t unk0x006;
// uint8_t _gap1[2];
// };
// static_assert(sizeof(D2GSPacketSrv11) == 12, "D2GSPacketSrv11: layout differs from the database");

// struct D2GSPacketSrv12
// {
// uint8_t nHeader;
// uint8_t unk0x001[25];
// };
// static_assert(sizeof(D2GSPacketSrv12) == 26, "D2GSPacketSrv12: layout differs from the database");

// struct D2GSPacketSrv13
// {
// uint8_t nHeader;
// uint8_t unk0x001[13];
// };
// static_assert(sizeof(D2GSPacketSrv13) == 14, "D2GSPacketSrv13: layout differs from the database");

// struct D2GSPacketSrv14
// {
// uint8_t nHeader;
// uint8_t unk0x001[17];
// };
// static_assert(sizeof(D2GSPacketSrv14) == 18, "D2GSPacketSrv14: layout differs from the database");

// struct D2GSPacketSrv15
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// uint16_t nPosX;
// uint16_t nPosY;
// uint8_t unk0x00A;
// uint8_t _gap1[3];
// };
// static_assert(sizeof(D2GSPacketSrv15) == 16, "D2GSPacketSrv15: layout differs from the database");

// struct D2GSPacketSrv16
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nSize;
// uint8_t nNumUpdates;
// uint8_t _gap1[3];
// D2ClientUnitUpdateStrc unitUpdate[55];
// };
// static_assert(sizeof(D2GSPacketSrv16) == 668, "D2GSPacketSrv16: layout differs from the database");

// struct D2GSPacketSrv17
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrv17) == 1, "D2GSPacketSrv17: layout differs from the database");

// struct D2GSPacketSrv18
// {
// uint8_t nHeader;
// uint8_t unk0x001[14];
// };
// static_assert(sizeof(D2GSPacketSrv18) == 15, "D2GSPacketSrv18: layout differs from the database");

// struct D2GSPacketSrv19
// {
// uint8_t nHeader;
// uint8_t nGold;
// };
// static_assert(sizeof(D2GSPacketSrv19) == 2, "D2GSPacketSrv19: layout differs from the database");

// struct D2GSPacketSrv1A
// {
// uint8_t nHeader;
// uint8_t nExperience;
// };
// static_assert(sizeof(D2GSPacketSrv1A) == 2, "D2GSPacketSrv1A: layout differs from the database");

// struct D2GSPacketSrv1B
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nExperience;
// };
// static_assert(sizeof(D2GSPacketSrv1B) == 4, "D2GSPacketSrv1B: layout differs from the database");

// struct D2GSPacketSrv1C
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t nExperience;
// };
// static_assert(sizeof(D2GSPacketSrv1C) == 8, "D2GSPacketSrv1C: layout differs from the database");

// struct D2GSPacketSrv21
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t unk0x02;
// uint8_t _gap0[1];
// uint32_t nUnitGUID;
// uint16_t nSkillId;
// uint8_t nSkillLevel;
// uint8_t nBonusSkillLevel;
// uint8_t unk0x0B;
// uint8_t _gap1[3];
// };
// static_assert(sizeof(D2GSPacketSrv21) == 16, "D2GSPacketSrv21: layout differs from the database");

// struct D2GSPacketSrv22
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t unk0x02;
// uint8_t _gap0[1];
// int32_t nUnitGUID;
// uint16_t nSkillId;
// uint8_t nQuantity;
// uint8_t unk0x0A;
// uint8_t bIsCorpse;
// uint8_t _gap1[3];
// };
// static_assert(sizeof(D2GSPacketSrv22) == 16, "D2GSPacketSrv22: layout differs from the database");

// struct D2GSPacketSrv23
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// uint8_t nPosition;
// uint8_t _gap1[1];
// uint16_t nSkill;
// uint32_t unk0x009;
// };
// static_assert(sizeof(D2GSPacketSrv23) == 16, "D2GSPacketSrv23: layout differs from the database");

// struct D2GSPacketSrv24
// {
// uint8_t nHeader;
// uint8_t unk0x001[89];
// };
// static_assert(sizeof(D2GSPacketSrv24) == 90, "D2GSPacketSrv24: layout differs from the database");

// struct D2GSPacketSrv25
// {
// uint8_t nHeader;
// uint8_t unk0x001[89];
// };
// static_assert(sizeof(D2GSPacketSrv25) == 90, "D2GSPacketSrv25: layout differs from the database");

// struct D2GSPacketSrv26
// {
// uint8_t nHeader;
// uint8_t nMessageType;
// uint8_t nLang;
// uint8_t nUnitType;
// uint32_t dwUnitGUID;
// uint8_t nMessageColor;
// uint8_t nNameColor;
// char szName[16];
// char szMessage[256];
// uint8_t _gap0[2];
// };
// static_assert(sizeof(D2GSPacketSrv26) == 284, "D2GSPacketSrv26: layout differs from the database");

// struct D2GSPacketSrv26Args
// {
// D2GSPacketSrv26 *pPacket26;
// D2GSPacketClt15 *pPacket15;
// int32_t nUnitId;
// };
// static_assert(sizeof(D2GSPacketSrv26Args) == 12, "D2GSPacketSrv26Args: layout differs from the database");

// struct D2GSPacketSrv27
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// D2MessageListStrc pMessageList;
// uint8_t _gap1[2];
// };
// static_assert(sizeof(D2GSPacketSrv27) == 44, "D2GSPacketSrv27: layout differs from the database");

// struct D2GSPacketSrv28
// {
// uint8_t nHeader;
// uint8_t unk0x01;
// uint8_t _gap0[2];
// uint32_t unk0x02;
// uint8_t unk0x06;
// uint8_t pData[96];
// uint8_t _gap1[3];
// };
// static_assert(sizeof(D2GSPacketSrv28) == 108, "D2GSPacketSrv28: layout differs from the database");

// struct D2GSPacketSrv29
// {
// uint8_t nHeader;
// uint8_t pData[96];
// };
// static_assert(sizeof(D2GSPacketSrv29) == 97, "D2GSPacketSrv29: layout differs from the database");

// struct D2GSPacketSrv2A
// {
// uint8_t nHeader;
// uint8_t unk0x01;
// uint8_t unk0x02;
// uint8_t _gap0[1];
// uint32_t unk0x03;
// uint32_t unk0x07;
// uint32_t unk0x0B;
// };
// static_assert(sizeof(D2GSPacketSrv2A) == 16, "D2GSPacketSrv2A: layout differs from the database");

// struct D2GSPacketSrv2B
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwSkill;
// uint32_t dwFrame;
// };
// static_assert(sizeof(D2GSPacketSrv2B) == 12, "D2GSPacketSrv2B: layout differs from the database");

// struct D2GSPacketSrv2C
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// uint16_t nSoundEvent;
// uint8_t _gap1[2];
// };
// static_assert(sizeof(D2GSPacketSrv2C) == 12, "D2GSPacketSrv2C: layout differs from the database");

// struct D2GSPacketSrv3A
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrv3A) == 1, "D2GSPacketSrv3A: layout differs from the database");

// struct D2GSPacketSrv3B
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrv3B) == 1, "D2GSPacketSrv3B: layout differs from the database");

// struct D2GSPacketSrv3E
// {
// uint8_t nHeader;
// uint8_t nSize;
// uint8_t pBuffer[32];
// };
// static_assert(sizeof(D2GSPacketSrv3E) == 34, "D2GSPacketSrv3E: layout differs from the database");

// struct D2GSPacketSrv3F
// {
// uint8_t nHeader;
// uint8_t nCursor;
// uint8_t _gap0[2];
// uint32_t dwItemId;
// uint16_t nSkill;
// uint8_t _gap1[2];
// };
// static_assert(sizeof(D2GSPacketSrv3F) == 12, "D2GSPacketSrv3F: layout differs from the database");

// struct D2GSPacketSrv40
// {
// uint8_t nHeader;
// uint8_t unk0x001[12];
// };
// static_assert(sizeof(D2GSPacketSrv40) == 13, "D2GSPacketSrv40: layout differs from the database");

// struct D2GSPacketSrv42
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// };
// static_assert(sizeof(D2GSPacketSrv42) == 8, "D2GSPacketSrv42: layout differs from the database");

// struct D2GSPacketSrv45
// {
// uint8_t nHeader;
// uint8_t unk0x001[12];
// };
// static_assert(sizeof(D2GSPacketSrv45) == 13, "D2GSPacketSrv45: layout differs from the database");

// struct D2GSPacketSrv47
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t unk0x02;
// uint8_t _gap0[1];
// uint32_t nUnitGUID;
// uint32_t unk0x07;
// };
// static_assert(sizeof(D2GSPacketSrv47) == 12, "D2GSPacketSrv47: layout differs from the database");

// struct D2GSPacketSrv48
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t unk0x02;
// uint8_t _gap0[1];
// uint32_t nUnitGUID;
// uint32_t unk0x07;
// };
// static_assert(sizeof(D2GSPacketSrv48) == 12, "D2GSPacketSrv48: layout differs from the database");

// struct D2GSPacketSrv4C
// {
// uint8_t nHeader;
// uint8_t unk0x001[15];
// };
// static_assert(sizeof(D2GSPacketSrv4C) == 16, "D2GSPacketSrv4C: layout differs from the database");

// struct D2GSPacketSrv4D
// {
// uint8_t nHeader;
// uint8_t unk0x01;
// uint8_t _gap0[2];
// uint32_t unk0x02;
// uint32_t unk0x06;
// uint8_t unk0x0A;
// uint8_t _gap1[1];
// uint16_t unk0x0B;
// uint16_t unk0x0D;
// uint16_t unk0x0F;
// };
// static_assert(sizeof(D2GSPacketSrv4D) == 20, "D2GSPacketSrv4D: layout differs from the database");

// struct D2GSPacketSrv4E
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nMercName;
// uint32_t nSeed;
// };
// static_assert(sizeof(D2GSPacketSrv4E) == 8, "D2GSPacketSrv4E: layout differs from the database");

// struct D2GSPacketSrv4F
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrv4F) == 1, "D2GSPacketSrv4F: layout differs from the database");

// struct D2GSPacketSrv50
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nQuestId;
// union
// {
// struct anon_63503F4130976E24F3A9E580D3B242ED QuestsStatusPayload;
// struct anon_E8C05EF12DE2F4465A905B87E8CB9726 HirelingPayload;
// struct anon_55738B68E7BB1318E56D3D4C5D360CAA TombOffsetPayload;
// struct anon_3F2D4F5E8D1F490FBA7D93D8C6A24CCE A1Q4_CainPayload;
// struct anon_114F3EFF484AED11DD38F3E16D75C987 A5Q6_BaalPayload;
// };
// };
// static_assert(sizeof(D2GSPacketSrv50) == 18, "D2GSPacketSrv50: layout differs from the database");

// struct D2GSPacketSrv51
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// uint16_t nObject;
// uint16_t nPosX;
// uint16_t nPosY;
// uint8_t unk0x0C[2];
// };
// static_assert(sizeof(D2GSPacketSrv51) == 16, "D2GSPacketSrv51: layout differs from the database");

// struct D2GSPacketSrv52
// {
// uint8_t nHeader;
// uint8_t pQuestList[41];
// };
// static_assert(sizeof(D2GSPacketSrv52) == 42, "D2GSPacketSrv52: layout differs from the database");

// struct D2GSPacketSrv53
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t unk0x01;
// int32_t unk0x05;
// uint8_t unk0x09;
// uint8_t _gap1[3];
// };
// static_assert(sizeof(D2GSPacketSrv53) == 16, "D2GSPacketSrv53: layout differs from the database");

// struct D2GSPacketSrv54
// {
// uint8_t nHeader;
// uint8_t unk0x001[9];
// };
// static_assert(sizeof(D2GSPacketSrv54) == 10, "D2GSPacketSrv54: layout differs from the database");

// struct D2GSPacketSrv55
// {
// uint8_t nHeader;
// uint8_t unk0x001[2];
// };
// static_assert(sizeof(D2GSPacketSrv55) == 3, "D2GSPacketSrv55: layout differs from the database");

// struct D2GSPacketSrv57
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t unk0x01;
// uint8_t unk0x05;
// uint8_t _gap1[1];
// uint16_t unk0x06;
// uint16_t unk0x08;
// uint16_t unk0x0A;
// uint16_t unk0x0C;
// uint8_t _gap2[2];
// };
// static_assert(sizeof(D2GSPacketSrv57) == 20, "D2GSPacketSrv57: layout differs from the database");

// struct D2GSPacketSrv58
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t nUnitId;
// uint8_t unk0x05;
// uint8_t unk0x06;
// uint8_t _gap1[2];
// };
// static_assert(sizeof(D2GSPacketSrv58) == 12, "D2GSPacketSrv58: layout differs from the database");

// struct D2GSPacketSrv59
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwGUID;
// uint8_t nClass;
// char szName[16];
// uint8_t _gap1[1];
// uint16_t nPosX;
// uint16_t nPosY;
// uint8_t _gap2[2];
// };
// static_assert(sizeof(D2GSPacketSrv59) == 32, "D2GSPacketSrv59: layout differs from the database");

// struct D2GSPacketSrv5A
// {
// uint8_t nHeader;
// uint8_t nType;
// union
// {
// struct
// {
// uint8_t nColor;
// uint32_t dwParam;
// uint8_t nParam;
// union
// {
// char szText[32];
// char szTextData[2][16];
// };
// };
// struct
// {
// uint32_t dwSkill;
// uint32_t dwEndFrame;
// uint8_t nExtraData2[30];
// };
// struct
// {
// uint8_t nGroup;
// uint32_t dwGroupEndFrame;
// uint8_t nExtraData3[33];
// };
// struct
// {
// int32_t nItemId;
// int32_t nPlayerId;
// uint8_t nExtraData4[30];
// };
// struct
// {
// uint8_t nDamageColor;
// uint32_t dwDamage;
// int32_t nDefenderGUID;
// int32_t nDefenderType;
// uint8_t nExtraData5[25];
// };
// struct
// {
// uint8_t nUnitType;
// int32_t nItemGUID;
// uint8_t nAction;
// int32_t nPosX;
// int32_t nPosY;
// uint8_t nExtraData6[24];
// };
// struct
// {
// uint8_t nDefenderUnitType;
// int32_t nDefenderUnitGUID;
// uint8_t nAttackerUnitType;
// int32_t nAttackerUnitGUID;
// uint8_t nAttackMessageColor;
// uint8_t nExtraData7[27];
// };
// struct
// {
// int32_t nFrames;
// uint8_t nExtraData8[34];
// };
// struct
// {
// int32_t nCount;
// uint8_t nExtraData9[34];
// };
// };
// };
// static_assert(sizeof(D2GSPacketSrv5A) == 48, "D2GSPacketSrv5A: layout differs from the database");

// struct D2GSPacketSrv5B
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t nPacketLen;
// uint32_t nUnitGUID;
// uint8_t nClass;
// char szName[16];
// uint8_t _gap1[1];
// uint16_t nLevel;
// uint16_t nPartyId;
// uint16_t unk0x1C;
// uint16_t nPartyFlags;
// uint16_t unk0x20;
// char unk0x22[256];
// };
// static_assert(sizeof(D2GSPacketSrv5B) == 292, "D2GSPacketSrv5B: layout differs from the database");

// struct D2GSPacketSrv5C
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwPlayerGUID;
// };
// static_assert(sizeof(D2GSPacketSrv5C) == 8, "D2GSPacketSrv5C: layout differs from the database");

// struct D2GSPacketSrv5D
// {
// uint8_t nHeader;
// uint8_t nQuestId;
// uint8_t nFlags;
// uint8_t nState;
// uint16_t field_4;
// };
// static_assert(sizeof(D2GSPacketSrv5D) == 6, "D2GSPacketSrv5D: layout differs from the database");

// struct D2GSPacketSrv5E
// {
// uint8_t nHeader;
// uint8_t unk0x001[37];
// };
// static_assert(sizeof(D2GSPacketSrv5E) == 38, "D2GSPacketSrv5E: layout differs from the database");

// struct D2GSPacketSrv5F
// {
// uint8_t nHeader;
// uint8_t unk0x001[4];
// };
// static_assert(sizeof(D2GSPacketSrv5F) == 5, "D2GSPacketSrv5F: layout differs from the database");

// struct D2GSPacketSrv60
// {
// uint8_t nHeader;
// uint8_t nFlags;
// uint8_t nLevel;
// uint8_t _gap0[1];
// uint32_t dwUnitGUID;
// };
// static_assert(sizeof(D2GSPacketSrv60) == 8, "D2GSPacketSrv60: layout differs from the database");

// struct D2GSPacketSrv61
// {
// uint8_t nHeader;
// uint8_t unk0x001;
// };
// static_assert(sizeof(D2GSPacketSrv61) == 2, "D2GSPacketSrv61: layout differs from the database");

// struct D2GSPacketSrv62
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// int32_t nUnitId;
// uint8_t unk0x06;
// uint8_t _gap1[3];
// };
// static_assert(sizeof(D2GSPacketSrv62) == 12, "D2GSPacketSrv62: layout differs from the database");

// struct D2GSPacketSrv63
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwUnitId;
// uint16_t pData[8];
// };
// static_assert(sizeof(D2GSPacketSrv63) == 24, "D2GSPacketSrv63: layout differs from the database");

// struct D2GSPacketSrv65
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwUnitId;
// uint16_t unk0x05;
// uint8_t _gap1[2];
// };
// static_assert(sizeof(D2GSPacketSrv65) == 12, "D2GSPacketSrv65: layout differs from the database");

// struct D2GSPacketSrv66
// {
// uint8_t nHeader;
// uint8_t unk0x001[6];
// };
// static_assert(sizeof(D2GSPacketSrv66) == 7, "D2GSPacketSrv66: layout differs from the database");

// struct D2GSPacketSrv67
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t nUnitGUID;
// uint8_t unk0x05;
// uint8_t _gap1[1];
// int16_t nX;
// int16_t nY;
// int8_t unk0x0A;
// int8_t unk0x0B;
// int8_t nPathType;
// uint8_t _gap2[1];
// int16_t nVelocity;
// int8_t unk0x0F;
// uint8_t _gap3[3];
// };
// static_assert(sizeof(D2GSPacketSrv67) == 24, "D2GSPacketSrv67: layout differs from the database");

// struct D2GSPacketSrv68
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t nUnitGUID;
// uint8_t unk0x05;
// uint8_t _gap1[1];
// int16_t nX;
// int16_t nY;
// int8_t unk0x0A;
// uint8_t _gap2[1];
// int32_t unk0x0B;
// int8_t unk0x0F;
// int8_t unk0x10;
// int8_t nPathType;
// uint8_t _gap3[1];
// int16_t nVelocity;
// int8_t unk0x14;
// uint8_t _gap4[1];
// };
// static_assert(sizeof(D2GSPacketSrv68) == 28, "D2GSPacketSrv68: layout differs from the database");

// struct D2GSPacketSrv69
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t unk0x01;
// uint8_t unk0x05;
// uint8_t _gap1[1];
// uint16_t unk0x06;
// uint16_t unk0x08;
// uint8_t unk0x0A;
// uint8_t unk0x0B;
// };
// static_assert(sizeof(D2GSPacketSrv69) == 16, "D2GSPacketSrv69: layout differs from the database");

// struct D2GSPacketSrv6A
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t nUnitGUID;
// uint8_t unk0x05;
// uint8_t nTargetUnitType;
// uint8_t _gap1[2];
// uint32_t nTargetUnitGuid;
// uint8_t nDirection;
// uint8_t _gap2[3];
// };
// static_assert(sizeof(D2GSPacketSrv6A) == 20, "D2GSPacketSrv6A: layout differs from the database");

// struct D2GSPacketSrv6B
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t unk0x01;
// uint8_t unk0x05;
// uint8_t _gap1[1];
// uint16_t unk0x06;
// uint16_t unk0x08;
// uint8_t unk0x0A;
// uint8_t unk0x0B;
// uint16_t unk0x0C;
// uint16_t unk0x0E;
// };
// static_assert(sizeof(D2GSPacketSrv6B) == 20, "D2GSPacketSrv6B: layout differs from the database");

// struct D2GSPacketSrv6C
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t unk0x01;
// uint8_t unk0x05;
// uint8_t unk0x06;
// uint8_t _gap1[2];
// uint32_t unk0x07;
// uint8_t unk0x0B;
// uint8_t _gap2[1];
// uint16_t unk0x0C;
// uint16_t unk0x0E;
// uint8_t _gap3[2];
// };
// static_assert(sizeof(D2GSPacketSrv6C) == 24, "D2GSPacketSrv6C: layout differs from the database");

// struct D2GSPacketSrv6D
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t unk0x01;
// uint16_t unk0x05;
// uint16_t unk0x07;
// uint8_t unk0x09;
// uint8_t _gap1[3];
// };
// static_assert(sizeof(D2GSPacketSrv6D) == 16, "D2GSPacketSrv6D: layout differs from the database");

// struct D2GSPacketSrv6E
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrv6E) == 1, "D2GSPacketSrv6E: layout differs from the database");

// struct D2GSPacketSrv6F
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrv6F) == 1, "D2GSPacketSrv6F: layout differs from the database");

// struct D2GSPacketSrv70
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrv70) == 1, "D2GSPacketSrv70: layout differs from the database");

// struct D2GSPacketSrv71
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrv71) == 1, "D2GSPacketSrv71: layout differs from the database");

// struct D2GSPacketSrv72
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrv72) == 1, "D2GSPacketSrv72: layout differs from the database");

// struct D2GSPacketSrv73
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t unk0x01;
// int16_t nMissileId;
// uint8_t _gap1[2];
// int32_t nX;
// int32_t nY;
// int32_t nFirstX;
// int32_t nFirstY;
// int16_t nCurrentFrame;
// uint8_t nUnitType;
// uint8_t _gap2[1];
// int32_t nUnitGUID;
// uint8_t nLevel;
// uint8_t nPierce;
// uint8_t _gap3[2];
// };
// static_assert(sizeof(D2GSPacketSrv73) == 40, "D2GSPacketSrv73: layout differs from the database");

// struct D2GSPacketSrv74
// {
// uint8_t nHeader;
// uint8_t unk0x01;
// uint8_t _gap0[2];
// int32_t unk0x02;
// int32_t unk0x06;
// };
// static_assert(sizeof(D2GSPacketSrv74) == 12, "D2GSPacketSrv74: layout differs from the database");

// struct D2GSPacketSrv75
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t nUnitGUID;
// uint16_t nPartyId;
// uint16_t nLevel;
// uint16_t nFlags;
// uint16_t unk0x0B;
// };
// static_assert(sizeof(D2GSPacketSrv75) == 16, "D2GSPacketSrv75: layout differs from the database");

// struct D2GSPacketSrv76
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// };
// static_assert(sizeof(D2GSPacketSrv76) == 8, "D2GSPacketSrv76: layout differs from the database");

// struct D2GSPacketSrv77
// {
// uint8_t nHeader;
// uint8_t nAction;
// };
// static_assert(sizeof(D2GSPacketSrv77) == 2, "D2GSPacketSrv77: layout differs from the database");

// struct D2GSPacketSrv78
// {
// uint8_t nHeader;
// char szName[16];
// uint8_t _gap0[3];
// uint32_t dwPlayerGUID;
// };
// static_assert(sizeof(D2GSPacketSrv78) == 24, "D2GSPacketSrv78: layout differs from the database");

// struct D2GSPacketSrv79
// {
// uint8_t nHeader;
// uint8_t nOwnerGUID;
// uint8_t _gap0[2];
// uint32_t dwGold;
// };
// static_assert(sizeof(D2GSPacketSrv79) == 8, "D2GSPacketSrv79: layout differs from the database");

// struct D2GSPacketSrv7A
// {
// uint8_t nHeader;
// uint8_t unk0x01;
// uint8_t unk0x02;
// uint8_t _gap0[1];
// uint16_t unk0x03;
// uint8_t _gap1[2];
// uint32_t unk0x05;
// uint32_t unk0x09;
// };
// static_assert(sizeof(D2GSPacketSrv7A) == 16, "D2GSPacketSrv7A: layout differs from the database");

// struct D2GSPacketSrv7B
// {
// uint8_t nHeader;
// uint8_t nSlot;
// struct
// {
// unsigned __int16 nSkill : 12;
// unsigned __int16 nHand : 4;
// };
// uint32_t nItemGUID;
// };
// static_assert(sizeof(D2GSPacketSrv7B) == 8, "D2GSPacketSrv7B: layout differs from the database");

// struct D2GSPacketSrv7C
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// int32_t nItemGUID;
// };
// static_assert(sizeof(D2GSPacketSrv7C) == 8, "D2GSPacketSrv7C: layout differs from the database");

// struct D2GSPacketSrv7D
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t nUnitGUID1;
// uint32_t nUnitGUID2;
// uint32_t unk0x0A;
// uint32_t unk0x0E;
// };
// static_assert(sizeof(D2GSPacketSrv7D) == 20, "D2GSPacketSrv7D: layout differs from the database");

// struct D2GSPacketSrv7E
// {
// uint8_t nHeader;
// uint8_t unk0x001[4];
// };
// static_assert(sizeof(D2GSPacketSrv7E) == 5, "D2GSPacketSrv7E: layout differs from the database");

// struct D2GSPacketSrv7F
// {
// uint8_t nHeader;
// uint8_t bIsPlayer;
// uint16_t nLifePercentage;
// int32_t nUnitGUID;
// uint16_t nLevelId;
// uint8_t _gap0[2];
// };
// static_assert(sizeof(D2GSPacketSrv7F) == 12, "D2GSPacketSrv7F: layout differs from the database");

// struct D2GSPacketSrv81
// {
// uint8_t nHeader;
// uint8_t unk0x01;
// uint16_t unk0x02;
// uint32_t unk0x04;
// uint32_t unk0x08;
// uint32_t unk0x0C;
// uint32_t unk0x10;
// };
// static_assert(sizeof(D2GSPacketSrv81) == 20, "D2GSPacketSrv81: layout differs from the database");

// struct D2GSPacketSrv82
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwOwnerGUID;
// char szOwnerName[16];
// uint32_t dwPortalGUID[2];
// };
// static_assert(sizeof(D2GSPacketSrv82) == 32, "D2GSPacketSrv82: layout differs from the database");

// struct D2GSPacketSrv89
// {
// uint8_t nHeader;
// uint8_t nEvent;
// };
// static_assert(sizeof(D2GSPacketSrv89) == 2, "D2GSPacketSrv89: layout differs from the database");

// struct D2GSPacketSrv8A
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// };
// static_assert(sizeof(D2GSPacketSrv8A) == 8, "D2GSPacketSrv8A: layout differs from the database");

// struct D2GSPacketSrv8B
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t nUnitGUID;
// uint8_t bParty;
// uint8_t _gap1[3];
// };
// static_assert(sizeof(D2GSPacketSrv8B) == 12, "D2GSPacketSrv8B: layout differs from the database");

// struct D2GSPacketSrv8C
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwPlayerGUID[2];
// uint16_t nFlags;
// uint8_t _gap1[2];
// };
// static_assert(sizeof(D2GSPacketSrv8C) == 16, "D2GSPacketSrv8C: layout differs from the database");

// struct D2GSPacketSrv8D
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwPlayerGUID;
// uint16_t nPartyGUID;
// uint8_t _gap1[2];
// };
// static_assert(sizeof(D2GSPacketSrv8D) == 12, "D2GSPacketSrv8D: layout differs from the database");

// struct D2GSPacketSrv8E
// {
// uint8_t nHeader;
// uint8_t nType;
// uint8_t _gap0[2];
// uint32_t dwPlayerGUID;
// uint32_t dwCorpseGUID;
// };
// static_assert(sizeof(D2GSPacketSrv8E) == 12, "D2GSPacketSrv8E: layout differs from the database");

// struct D2GSPacketSrv8F
// {
// uint8_t nHeader;
// uint8_t unk0x001[32];
// };
// static_assert(sizeof(D2GSPacketSrv8F) == 33, "D2GSPacketSrv8F: layout differs from the database");

// struct D2GSPacketSrv90
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwGUID;
// uint32_t dwPosX;
// uint32_t dwPosY;
// };
// static_assert(sizeof(D2GSPacketSrv90) == 16, "D2GSPacketSrv90: layout differs from the database");

// struct D2GSPacketSrv91
// {
// uint8_t nHeader;
// uint8_t nAct;
// uint16_t pNPCFlags[12];
// };
// static_assert(sizeof(D2GSPacketSrv91) == 26, "D2GSPacketSrv91: layout differs from the database");

// struct D2GSPacketSrv92
// {
// uint8_t nHeader;
// uint8_t unk0x001[5];
// };
// static_assert(sizeof(D2GSPacketSrv92) == 6, "D2GSPacketSrv92: layout differs from the database");

// struct D2GSPacketSrv93
// {
// uint8_t nHeader;
// uint8_t unk0x001[7];
// };
// static_assert(sizeof(D2GSPacketSrv93) == 8, "D2GSPacketSrv93: layout differs from the database");

// struct D2GSPacketSrv94
// {
// uint8_t nHeader;
// uint8_t nSkills;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// struct D2GSPacketSrv94__SkillInfo Skills[255];
// };
// static_assert(sizeof(D2GSPacketSrv94) == 1028, "D2GSPacketSrv94: layout differs from the database");

// struct D2GSPacketSrv94__SkillInfo
// {
// uint16_t nSkill;
// uint8_t nSkillLevel;
// uint8_t _gap0[1];
// };
// static_assert(sizeof(D2GSPacketSrv94__SkillInfo) == 4, "D2GSPacketSrv94__SkillInfo: layout differs from the database");

// struct D2GSPacketSrv95
// {
// uint8_t nHeader;
// uint8_t unk0x01[12];
// };
// static_assert(sizeof(D2GSPacketSrv95) == 13, "D2GSPacketSrv95: layout differs from the database");

// struct D2GSPacketSrv96
// {
// uint8_t nHeader;
// uint8_t unk0x01[8];
// };
// static_assert(sizeof(D2GSPacketSrv96) == 9, "D2GSPacketSrv96: layout differs from the database");

// struct D2GSPacketSrv97
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrv97) == 1, "D2GSPacketSrv97: layout differs from the database");

// struct D2GSPacketSrv98
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t unk0x01;
// uint16_t unk0x05;
// uint8_t _gap1[2];
// };
// static_assert(sizeof(D2GSPacketSrv98) == 12, "D2GSPacketSrv98: layout differs from the database");

// struct D2GSPacketSrv99
// {
// uint8_t nHeader;
// uint8_t unk0x01;
// uint8_t _gap0[2];
// uint32_t unk0x02;
// uint16_t unk0x06;
// uint8_t unk0x08;
// uint8_t nUnitType;
// int32_t nUnitGUID;
// uint16_t unk0x0E;
// uint8_t _gap1[2];
// };
// static_assert(sizeof(D2GSPacketSrv99) == 20, "D2GSPacketSrv99: layout differs from the database");

// struct D2GSPacketSrv9A
// {
// uint8_t nHeader;
// uint8_t unk0x01;
// uint8_t _gap0[2];
// uint32_t unk0x02;
// uint32_t unk0x06;
// uint8_t unk0x0A;
// uint8_t _gap1[1];
// uint16_t nX;
// uint16_t nY;
// uint16_t unk0x0F;
// };
// static_assert(sizeof(D2GSPacketSrv9A) == 20, "D2GSPacketSrv9A: layout differs from the database");

// struct D2GSPacketSrv9B
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t unk0x01;
// uint32_t unk0x03;
// };
// static_assert(sizeof(D2GSPacketSrv9B) == 8, "D2GSPacketSrv9B: layout differs from the database");

// struct D2GSPacketSrv9C
// {
// uint8_t nHeader;
// uint8_t nAction;
// uint8_t nPacketSize;
// uint8_t nComponent;
// int32_t nItemId;
// uint8_t pBitstream[244];
// };
// static_assert(sizeof(D2GSPacketSrv9C) == 252, "D2GSPacketSrv9C: layout differs from the database");

// struct D2GSPacketSrv9D
// {
// uint8_t nHeader;
// uint8_t nAction;
// uint8_t nPacketSize;
// uint8_t nComponent;
// int32_t nItemId;
// uint8_t nUnitType;
// uint8_t _gap0[3];
// int32_t nUnitId;
// uint8_t pBitstream[244];
// };
// static_assert(sizeof(D2GSPacketSrv9D) == 260, "D2GSPacketSrv9D: layout differs from the database");

// struct D2GSPacketSrvA3
// {
// uint8_t nHeader;
// uint8_t unk0x01;
// uint16_t unk0x02;
// uint16_t unk0x04;
// uint8_t unk0x06;
// uint8_t _gap0[1];
// uint32_t unk0x07;
// uint8_t unk0x0B;
// uint8_t _gap1[3];
// uint32_t unk0x0C;
// uint32_t unk0x10;
// uint32_t unk0x14;
// };
// static_assert(sizeof(D2GSPacketSrvA3) == 28, "D2GSPacketSrvA3: layout differs from the database");

// struct D2GSPacketSrvA4
// {
// uint8_t nHeader;
// uint8_t _gap0[1];
// uint16_t unk0x01;
// };
// static_assert(sizeof(D2GSPacketSrvA4) == 4, "D2GSPacketSrvA4: layout differs from the database");

// struct D2GSPacketSrvA5
// {
// uint8_t nHeader;
// uint8_t unk0x01;
// uint8_t _gap0[2];
// int32_t unk0x02;
// uint16_t unk0x06;
// uint8_t _gap1[2];
// };
// static_assert(sizeof(D2GSPacketSrvA5) == 12, "D2GSPacketSrvA5: layout differs from the database");

// struct D2GSPacketSrvA6
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrvA6) == 1, "D2GSPacketSrvA6: layout differs from the database");

// struct D2GSPacketSrvA7
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// int32_t nUnitId;
// uint16_t nState;
// uint8_t _gap1[2];
// };
// static_assert(sizeof(D2GSPacketSrvA7) == 12, "D2GSPacketSrvA7: layout differs from the database");

// struct D2GSPacketSrvA8
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// int32_t nUnitId;
// uint8_t nSize;
// uint8_t _gap1[1];
// uint16_t nState;
// uint8_t pStream[512];
// };
// static_assert(sizeof(D2GSPacketSrvA8) == 524, "D2GSPacketSrvA8: layout differs from the database");

// struct D2GSPacketSrvA9
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// int32_t nUnitId;
// uint16_t nState;
// uint8_t _gap1[2];
// };
// static_assert(sizeof(D2GSPacketSrvA9) == 12, "D2GSPacketSrvA9: layout differs from the database");

// struct D2GSPacketSrvAA
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// int32_t nUnitId;
// uint8_t nSize;
// uint8_t pStream[512];
// uint8_t _gap1[3];
// };
// static_assert(sizeof(D2GSPacketSrvAA) == 524, "D2GSPacketSrvAA: layout differs from the database");

// struct D2GSPacketSrvAB
// {
// uint8_t nHeader;
// uint8_t nUnitType;
// uint8_t _gap0[2];
// uint32_t dwUnitGUID;
// uint8_t nLife;
// uint8_t _gap1[3];
// };
// static_assert(sizeof(D2GSPacketSrvAB) == 12, "D2GSPacketSrvAB: layout differs from the database");

// struct D2GSPacketSrvAC
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwUnitGUID;
// uint16_t nUnitIndex;
// uint16_t nPosX;
// uint16_t nPosY;
// uint8_t nLifePct;
// uint8_t nPacketLength;
// uint8_t bitstream[244];
// };
// static_assert(sizeof(D2GSPacketSrvAC) == 260, "D2GSPacketSrvAC: layout differs from the database");

// struct D2GSPacketSrvAD
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// int32_t unk0x01;
// int32_t unk0x05;
// };
// static_assert(sizeof(D2GSPacketSrvAD) == 12, "D2GSPacketSrvAD: layout differs from the database");

// struct D2GSPacketSrvAE
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrvAE) == 1, "D2GSPacketSrvAE: layout differs from the database");

// struct D2GSPacketSrvAF
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrvAF) == 1, "D2GSPacketSrvAF: layout differs from the database");

// struct D2GSPacketSrvB0
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrvB0) == 1, "D2GSPacketSrvB0: layout differs from the database");

// struct D2GSPacketSrvB1
// {
// uint8_t nHeader;
// char szGameName[48];
// uint8_t _gap0[1];
// uint16_t nClientCount;
// uint16_t nGameId;
// };
// static_assert(sizeof(D2GSPacketSrvB1) == 54, "D2GSPacketSrvB1: layout differs from the database");

// struct D2GSPacketSrvB2
// {
// uint8_t nHeader;
// };
// static_assert(sizeof(D2GSPacketSrvB2) == 1, "D2GSPacketSrvB2: layout differs from the database");

// struct D2GSPacketSrvB3
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t nErrorCode;
// };
// static_assert(sizeof(D2GSPacketSrvB3) == 8, "D2GSPacketSrvB3: layout differs from the database");

// struct D2GSPacketSrvB4
// {
// uint8_t nHeader;
// uint8_t _gap0[3];
// uint32_t dwType;
// };
// static_assert(sizeof(D2GSPacketSrvB4) == 8, "D2GSPacketSrvB4: layout differs from the database");

// struct D2GSPacketSrvFF01
// {
// uint8_t nPacketSubType;
// uint8_t _gap0[3];
// D2GameServerInfoStrc tServerInfo;
// int32_t field_0xbd;
// int32_t aSomeInts[8];
// int32_t field_0xe1;
// int32_t field_0xe5;
// int32_t field_0xe9;
// int32_t field_0xed;
// int32_t field_0xf1;
// int16_t field_0xf5;
// char field_0xf6[20];
// uint8_t _gap1[2];
// int32_t field_0x10b;
// int32_t field_0x10f;
// int32_t field_0x113;
// int32_t field_0x117;
// char nZeroed[74];
// uint8_t _gap2[2];
// };
// static_assert(sizeof(D2GSPacketSrvFF01) == 364, "D2GSPacketSrvFF01: layout differs from the database");

// struct D2GSPacketSrvFFFD
// {
// uint8_t nPacketSubType;
// uint8_t _gap0[3];
// D2GameServerInfoStrc tServerInfo;
// };
// static_assert(sizeof(D2GSPacketSrvFFFD) == 192, "D2GSPacketSrvFFFD: layout differs from the database");

// struct D2GameServerInfoExStrc : D2GameServerInfoStrc
// {
// uint32_t dwords0xBC[6];
// uint16_t word0xD4;
// char szUnk0xD6[20];
// uint16_t word0xEA;
// uint32_t dwords0xD8;
// uint32_t dwords0xF0[8];
// uint32_t dwords0x110;
// };
// static_assert(sizeof(D2GameServerInfoExStrc) == 276, "D2GameServerInfoExStrc: layout differs from the database");

// struct D2GameServerInfoStrc
// {
// int32_t field_0x0;
// int32_t field_0x4;
// int32_t field_0x8;
// int32_t field_0xc;
// int32_t field_0x10;
// int32_t field_0x14;
// int32_t field_0x18;
// int32_t field_0x1c;
// int32_t field_0x20;
// int32_t field_0x24;
// char szCompilationData[128];
// int32_t field_0xa8;
// int32_t field_0xac;
// int32_t field_0xb0;
// int32_t field_0xb4;
// int32_t field_0xb8;
// };
// static_assert(sizeof(D2GameServerInfoStrc) == 188, "D2GameServerInfoStrc: layout differs from the database");

// struct D2KeyConfigOptionStrc
// {
// uint16_t nStringIndex;
// uint8_t _gap0[2];
// void *pCallback;
// uint16_t word0x06;
// uint8_t _gap1[2];
// int32_t nTextBeginX;
// int32_t nTextEndX;
// int32_t nTextBeginY;
// int32_t nTextEndY;
// uint16_t word0x18;
// uint8_t _gap2[2];
// };
// static_assert(sizeof(D2KeyConfigOptionStrc) == 32, "D2KeyConfigOptionStrc: layout differs from the database");

// struct D2KeyConfigStrc
// {
// int32_t nConfig;
// uint16_t nStringIndex;
// uint8_t _gap0[2];
// int32_t unk0x06;
// };
// static_assert(sizeof(D2KeyConfigStrc) == 12, "D2KeyConfigStrc: layout differs from the database");

// struct D2MenuEntryStrc
// {
// int32_t nType;
// int32_t nLeft;
// int32_t nTop;
// int32_t nWidth;
// int32_t nHeight;
// int32_t unk0x014;
// int32_t nStrIndex;
// D2GfxDataStrc *pGfxData;
// int32_t (__stdcall *pBtnFunction)(void *);
// uint32_t unk0x024[2];
// int32_t nFont;
// };
// static_assert(sizeof(D2MenuEntryStrc) == 48, "D2MenuEntryStrc: layout differs from the database");

// struct D2MenuInfoStrc
// {
// int32_t nItemCount;
// int32_t unk0x04;
// int32_t unk0x08;
// int32_t unk0x0C;
// int32_t unk0x10;
// int32_t unk0x14;
// };
// static_assert(sizeof(D2MenuInfoStrc) == 24, "D2MenuInfoStrc: layout differs from the database");

// struct D2MenuItemStrc
// {
// uint32_t dwType;
// BOOL bExpansion;
// int32_t nHeight;
// char szImage[260];
// MENUENABLED pfEnabled;
// MENUSELECT pfSelect;
// MENUOPTION pfOption;
// MENUUPDATE pfUpdate;
// uint32_t dwCount;
// uint32_t dwMoveCount;
// uint32_t dwSliderType;
// char szChoices[4][260];
// D2CellFileStrc *pImage;
// D2CellFileStrc *pChoices[4];
// };
// static_assert(sizeof(D2MenuItemStrc) == 1360, "D2MenuItemStrc: layout differs from the database");

// struct D2MenuUIStateSaveStrc
// {
// BOOL bCloseWhenOpen;
// BOOL bSaveUIState;
// int32_t nUIState;
// };
// static_assert(sizeof(D2MenuUIStateSaveStrc) == 12, "D2MenuUIStateSaveStrc: layout differs from the database");

// struct D2MessageListStrc
// {
// uint8_t nCount;
// uint8_t pad0x01;
// D2MessageStrc pMessages[8];
// };
// static_assert(sizeof(D2MessageListStrc) == 34, "D2MessageListStrc: layout differs from the database");

// struct D2MessageStrc
// {
// uint8_t nMenu;
// uint8_t pad0x01;
// uint16_t nStringId;
// };
// static_assert(sizeof(D2MessageStrc) == 4, "D2MessageStrc: layout differs from the database");

// struct D2QuestDescStrc
// {
// uint16_t wTblTitle;
// uint16_t wTblInitDesc;
// };
// static_assert(sizeof(D2QuestDescStrc) == 4, "D2QuestDescStrc: layout differs from the database");

// struct D2QuestDescriptorStrc
// {
// uint8_t bActive;
// uint8_t nQuestNo;
// uint8_t nPosition;
// uint8_t nTab;
// uint16_t *pStringIds;
// int32_t nQuestFlag;
// int32_t nArrayId;
// };
// static_assert(sizeof(D2QuestDescriptorStrc) == 16, "D2QuestDescriptorStrc: layout differs from the database");

// struct D2QuestUiButtonCoordStrc
// {
// int32_t nCellfileX;
// int32_t nCellfileY;
// int32_t nClickX;
// int32_t nClickY;
// };
// static_assert(sizeof(D2QuestUiButtonCoordStrc) == 16, "D2QuestUiButtonCoordStrc: layout differs from the database");

// struct D2QuestUiStrc
// {
// uint8_t field_0;
// uint8_t _gap0[3];
// int32_t nQuest;
// int16_t nQuestTitleStringId;
// wchar_t wszQuestDesc[300];
// int16_t field_25F;
// uint8_t nPosition;
// uint8_t nQuestNo;
// uint8_t field_263;
// uint8_t field_264;
// uint8_t field_265;
// uint8_t _gap1[3];
// int32_t nQuestState;
// };
// static_assert(sizeof(D2QuestUiStrc) == 624, "D2QuestUiStrc: layout differs from the database");

// struct D2QuestUiTabStrc
// {
// int32_t nStartQuest;
// int32_t nEndQuest;
// };
// static_assert(sizeof(D2QuestUiTabStrc) == 8, "D2QuestUiTabStrc: layout differs from the database");

// struct D2WindowPlacementStrc
// {
// HWND hWnd;
// WINDOWPLACEMENT windowPlacement;
// };
// static_assert(sizeof(D2WindowPlacementStrc) == 48, "D2WindowPlacementStrc: layout differs from the database");

// typedef struct HD2ARCHIVE__ *HD2ARCHIVE;

// struct HSFILE__
// {
// int32_t unused;
// };
// static_assert(sizeof(HSFILE__) == 4, "HSFILE__: layout differs from the database");

// typedef int32_t (__fastcall *MENUENABLED)();

// typedef void (__fastcall *MENUOPTION)(D2MenuItemStrc *, int32_t);

// typedef void (__fastcall *MENUSELECT)(D2MenuItemStrc *, SMSGHANDLER_PARAMS *);

// typedef void (__fastcall *MENUUPDATE)(D2MenuItemStrc *, int32_t);

// struct SMSGHANDLER_PARAMS
// {
// HWND hWnd;
// uint32_t uMessage;
// union
// {
// WPARAM wParam;
// int32_t nKey;
// };
// union
// {
// struct
// {
// uint16_t nXpos;
// uint16_t nYpos;
// };
// LPARAM lParam;
// };
// uint32_t dwCommandSource;
// uint32_t dwArg;
// BOOL bReturn;
// LRESULT lResult;
// };
// static_assert(sizeof(SMSGHANDLER_PARAMS) == 32, "SMSGHANDLER_PARAMS: layout differs from the database");

// struct _OVERLAPPED
// {
// ULONG_PTR Internal;
// ULONG_PTR InternalHigh;
// union
// {
// struct
// {
// DWORD Offset;
// DWORD OffsetHigh;
// };
// PVOID Pointer;
// };
// HANDLE hEvent;
// };
// static_assert(sizeof(_OVERLAPPED) == 20, "_OVERLAPPED: layout differs from the database");

// struct anon_114F3EFF484AED11DD38F3E16D75C987
// {
// uint16_t nZooMonsterId;
// uint16_t padding0x05[5];
// };
// static_assert(sizeof(anon_114F3EFF484AED11DD38F3E16D75C987) == 12, "anon_114F3EFF484AED11DD38F3E16D75C987: layout differs from the database");

// struct anon_14587F979C641006F380FCBF23CEC161
// {
// int32_t nCount;
// uint8_t nExtraData9[34];
// uint8_t _gap0[2];
// };
// static_assert(sizeof(anon_14587F979C641006F380FCBF23CEC161) == 40, "anon_14587F979C641006F380FCBF23CEC161: layout differs from the database");

// struct anon_17EA52B533BA0577390A0A91F2834258
// {
// uint16_t nParams[2];
// };
// static_assert(sizeof(anon_17EA52B533BA0577390A0A91F2834258) == 4, "anon_17EA52B533BA0577390A0A91F2834258: layout differs from the database");

// struct anon_34227162FB87FBED6A1B09BFAFEE6B72
// {
// unsigned __int16 nSkill : 12;
// unsigned __int16 nHand : 4;
// };
// static_assert(sizeof(anon_34227162FB87FBED6A1B09BFAFEE6B72) == 2, "anon_34227162FB87FBED6A1B09BFAFEE6B72: layout differs from the database");

// union anon_380F2128073F347D8978CA0677085670
// {
// struct anon_63503F4130976E24F3A9E580D3B242ED QuestsStatusPayload;
// struct anon_E8C05EF12DE2F4465A905B87E8CB9726 HirelingPayload;
// struct anon_55738B68E7BB1318E56D3D4C5D360CAA TombOffsetPayload;
// struct anon_3F2D4F5E8D1F490FBA7D93D8C6A24CCE A1Q4_CainPayload;
// struct anon_114F3EFF484AED11DD38F3E16D75C987 A5Q6_BaalPayload;
// };
// static_assert(sizeof(anon_380F2128073F347D8978CA0677085670) == 14, "anon_380F2128073F347D8978CA0677085670: layout differs from the database");

// struct anon_3F2D4F5E8D1F490FBA7D93D8C6A24CCE
// {
// uint16_t nStoneOrder[5];
// uint16_t padding0x0D;
// };
// static_assert(sizeof(anon_3F2D4F5E8D1F490FBA7D93D8C6A24CCE) == 12, "anon_3F2D4F5E8D1F490FBA7D93D8C6A24CCE: layout differs from the database");

// union anon_41E2BDCDD1C2585266E3BA819DC7D701
// {
// struct
// {
// char szToken[4];
// char szComponent[4];
// char szArmorType[4];
// char szMode[4];
// char szHitClass[4];
// };
// uint32_t dwName[5];
// };
// static_assert(sizeof(anon_41E2BDCDD1C2585266E3BA819DC7D701) == 20, "anon_41E2BDCDD1C2585266E3BA819DC7D701: layout differs from the database");

// struct anon_427EBAF5AD08AB583E1D4DF460AB8325
// {
// uint8_t nDefenderUnitType;
// uint8_t _gap0[3];
// int32_t nDefenderUnitGUID;
// uint8_t nAttackerUnitType;
// uint8_t _gap1[3];
// int32_t nAttackerUnitGUID;
// uint8_t nAttackMessageColor;
// uint8_t nExtraData7[27];
// };
// static_assert(sizeof(anon_427EBAF5AD08AB583E1D4DF460AB8325) == 44, "anon_427EBAF5AD08AB583E1D4DF460AB8325: layout differs from the database");

// struct anon_50DA7804EBF03AEE90BE8F81913C2806
// {
// uint8_t nColor;
// uint8_t _gap0[3];
// uint32_t dwParam;
// uint8_t nParam;
// union
// {
// char szText[32];
// char szTextData[2][16];
// };
// uint8_t _gap1[3];
// };
// static_assert(sizeof(anon_50DA7804EBF03AEE90BE8F81913C2806) == 44, "anon_50DA7804EBF03AEE90BE8F81913C2806: layout differs from the database");

// struct anon_527403D9EC2CF491362D3AE59B5ADD67
// {
// uint8_t nDamageColor;
// uint8_t _gap0[3];
// uint32_t dwDamage;
// int32_t nDefenderGUID;
// int32_t nDefenderType;
// uint8_t nExtraData5[25];
// uint8_t _gap1[3];
// };
// static_assert(sizeof(anon_527403D9EC2CF491362D3AE59B5ADD67) == 44, "anon_527403D9EC2CF491362D3AE59B5ADD67: layout differs from the database");

// struct anon_55738B68E7BB1318E56D3D4C5D360CAA
// {
// uint16_t nStaffTombLevelOffset;
// uint16_t padding0x05[5];
// };
// static_assert(sizeof(anon_55738B68E7BB1318E56D3D4C5D360CAA) == 12, "anon_55738B68E7BB1318E56D3D4C5D360CAA: layout differs from the database");

// struct anon_63503F4130976E24F3A9E580D3B242ED
// {
// uint16_t nMonstersToBeKilled;
// uint16_t nStaffTombLevelOffset;
// uint16_t nBarbsToBeRescued;
// uint8_t field_9;
// uint8_t field_A;
// uint8_t field_B;
// uint8_t _gap0[1];
// uint16_t field_C;
// uint8_t field_E;
// uint8_t _gap1[1];
// };
// static_assert(sizeof(anon_63503F4130976E24F3A9E580D3B242ED) == 14, "anon_63503F4130976E24F3A9E580D3B242ED: layout differs from the database");

// union anon_83B6E30208D726A49ED3B6646DA7606D
// {
// uint32_t dwCTemp;
// struct anon_8E1104106B2EB30A90505EAA0C598AB1 unpackedCTemp;
// };
// static_assert(sizeof(anon_83B6E30208D726A49ED3B6646DA7606D) == 4, "anon_83B6E30208D726A49ED3B6646DA7606D: layout differs from the database");

// struct anon_8E1104106B2EB30A90505EAA0C598AB1
// {
// uint8_t nUnk;
// uint8_t nCharacterClassId;
// uint16_t nCharacterSaveFlags;
// };
// static_assert(sizeof(anon_8E1104106B2EB30A90505EAA0C598AB1) == 4, "anon_8E1104106B2EB30A90505EAA0C598AB1: layout differs from the database");

// union anon_9061D5506B188DF6886B65C2F499A751
// {
// struct
// {
// uint8_t nColor;
// uint32_t dwParam;
// uint8_t nParam;
// union
// {
// char szText[32];
// char szTextData[2][16];
// };
// };
// struct
// {
// uint32_t dwSkill;
// uint32_t dwEndFrame;
// uint8_t nExtraData2[30];
// };
// struct
// {
// uint8_t nGroup;
// uint32_t dwGroupEndFrame;
// uint8_t nExtraData3[33];
// };
// struct
// {
// int32_t nItemId;
// int32_t nPlayerId;
// uint8_t nExtraData4[30];
// };
// struct
// {
// uint8_t nDamageColor;
// uint32_t dwDamage;
// int32_t nDefenderGUID;
// int32_t nDefenderType;
// uint8_t nExtraData5[25];
// };
// struct
// {
// uint8_t nUnitType;
// int32_t nItemGUID;
// uint8_t nAction;
// int32_t nPosX;
// int32_t nPosY;
// uint8_t nExtraData6[24];
// };
// struct
// {
// uint8_t nDefenderUnitType;
// int32_t nDefenderUnitGUID;
// uint8_t nAttackerUnitType;
// int32_t nAttackerUnitGUID;
// uint8_t nAttackMessageColor;
// uint8_t nExtraData7[27];
// };
// struct
// {
// int32_t nFrames;
// uint8_t nExtraData8[34];
// };
// struct
// {
// int32_t nCount;
// uint8_t nExtraData9[34];
// };
// };
// static_assert(sizeof(anon_9061D5506B188DF6886B65C2F499A751) == 44, "anon_9061D5506B188DF6886B65C2F499A751: layout differs from the database");

// union anon_9DEA95786DEDD61FF48E8BC3CF3A06D9
// {
// uint8_t nComponent;
// uint8_t fItemFlags;
// };
// static_assert(sizeof(anon_9DEA95786DEDD61FF48E8BC3CF3A06D9) == 1, "anon_9DEA95786DEDD61FF48E8BC3CF3A06D9: layout differs from the database");

// struct anon_9E01DBFBEC86134033FD3477EE0CD60D
// {
// unsigned __int16 nItemMode : 15;
// unsigned __int16 bMultibuy : 1;
// };
// static_assert(sizeof(anon_9E01DBFBEC86134033FD3477EE0CD60D) == 2, "anon_9E01DBFBEC86134033FD3477EE0CD60D: layout differs from the database");

// union anon_9E656BAF27D4BA1E170AEB1A92E0D77D
// {
// struct
// {
// uint16_t nParams[2];
// };
// int32_t nItemGUID;
// };
// static_assert(sizeof(anon_9E656BAF27D4BA1E170AEB1A92E0D77D) == 4, "anon_9E656BAF27D4BA1E170AEB1A92E0D77D: layout differs from the database");

// struct anon_A0B592BF32EB0681EBBB5D252D2EFAF3
// {
// uint8_t nUnitType;
// uint8_t _gap0[3];
// int32_t nItemGUID;
// uint8_t nAction;
// uint8_t _gap1[3];
// int32_t nPosX;
// int32_t nPosY;
// uint8_t nExtraData6[24];
// };
// static_assert(sizeof(anon_A0B592BF32EB0681EBBB5D252D2EFAF3) == 44, "anon_A0B592BF32EB0681EBBB5D252D2EFAF3: layout differs from the database");

// struct anon_A3719FF1A699486E8629E16291555876
// {
// int32_t nFrames;
// uint8_t nExtraData8[34];
// uint8_t _gap0[2];
// };
// static_assert(sizeof(anon_A3719FF1A699486E8629E16291555876) == 40, "anon_A3719FF1A699486E8629E16291555876: layout differs from the database");

// struct anon_A85CD68F813312C5E6BB1494B5EEB9CC
// {
// char szToken[4];
// char szComponent[4];
// char szArmorType[4];
// char szMode[4];
// char szHitClass[4];
// };
// static_assert(sizeof(anon_A85CD68F813312C5E6BB1494B5EEB9CC) == 20, "anon_A85CD68F813312C5E6BB1494B5EEB9CC: layout differs from the database");

// struct anon_ABAA6F78D0909A5EC38B8DAC19886F2D
// {
// int32_t nItemId;
// int32_t nPlayerId;
// uint8_t nExtraData4[30];
// uint8_t _gap0[2];
// };
// static_assert(sizeof(anon_ABAA6F78D0909A5EC38B8DAC19886F2D) == 40, "anon_ABAA6F78D0909A5EC38B8DAC19886F2D: layout differs from the database");

// struct anon_B40DC28E98FE5F06F50437E23D68FF31
// {
// uint32_t dwSkill;
// uint32_t dwEndFrame;
// uint8_t nExtraData2[30];
// uint8_t _gap0[2];
// };
// static_assert(sizeof(anon_B40DC28E98FE5F06F50437E23D68FF31) == 40, "anon_B40DC28E98FE5F06F50437E23D68FF31: layout differs from the database");

// union anon_BBC6B91D16152AC7247EC2523B3ED144
// {
// char szText[32];
// char szTextData[2][16];
// };
// static_assert(sizeof(anon_BBC6B91D16152AC7247EC2523B3ED144) == 32, "anon_BBC6B91D16152AC7247EC2523B3ED144: layout differs from the database");

// struct anon_CAAEA91160A63143F2AE058B4EF58ACE
// {
// uint8_t nGroup;
// uint8_t _gap0[3];
// uint32_t dwGroupEndFrame;
// uint8_t nExtraData3[33];
// uint8_t _gap1[3];
// };
// static_assert(sizeof(anon_CAAEA91160A63143F2AE058B4EF58ACE) == 44, "anon_CAAEA91160A63143F2AE058B4EF58ACE: layout differs from the database");

// struct anon_E8C05EF12DE2F4465A905B87E8CB9726
// {
// uint16_t nHireling;
// uint16_t padding0x05[5];
// };
// static_assert(sizeof(anon_E8C05EF12DE2F4465A905B87E8CB9726) == 12, "anon_E8C05EF12DE2F4465A905B87E8CB9726: layout differs from the database");


/* ---- layout self-check: every type's size as measured in D2CMP.dll ---- */
#ifndef D2STRUCTS_NO_ASSERTS
/* MSVC reports __cplusplus as 199711L unless /Zc:__cplusplus is passed; _MSVC_LANG holds the real value. */
#if defined(_MSVC_LANG)
#  define D2STRUCTS_LANG _MSVC_LANG
#elif defined(__cplusplus)
#  define D2STRUCTS_LANG __cplusplus
#else
#  define D2STRUCTS_LANG 0L
#endif
#if D2STRUCTS_LANG >= 201103L
static_assert(sizeof(AsyncDataLoadFileCallback) == 28, "AsyncDataLoadFileCallback: layout differs from the database");
static_assert(sizeof(AsyncData) == 84, "AsyncData: layout differs from the database");
static_assert(sizeof(D2CmpBuiltCellFile) == 24, "D2CmpBuiltCellFile: layout differs from the database");
static_assert(sizeof(D2CmpCelHardwareNode) == 44, "D2CmpCelHardwareNode: layout differs from the database");
static_assert(sizeof(D2CmpCelHeaderHash) == 4096, "D2CmpCelHeaderHash: layout differs from the database");
static_assert(sizeof(D2CmpCelHeaderHashEntry) == 48, "D2CmpCelHeaderHashEntry: layout differs from the database");
static_assert(sizeof(D2CmpStormLink) == 8, "D2CmpStormLink: layout differs from the database");
static_assert(sizeof(D2CmpCellBuildEntry) == 36, "D2CmpCellBuildEntry: layout differs from the database");
static_assert(sizeof(D2CmpStormList) == 12, "D2CmpStormList: layout differs from the database");
static_assert(sizeof(D2CmpCellBuilder) == 56, "D2CmpCellBuilder: layout differs from the database");
static_assert(sizeof(D2CmpCellBuilderArray) == 16, "D2CmpCellBuilderArray: layout differs from the database");
static_assert(sizeof(D2CmpCellBuilderObjectChunk) == 24, "D2CmpCellBuilderObjectChunk: layout differs from the database");
static_assert(sizeof(D2CmpCellBuilderTableHeader) == 40, "D2CmpCellBuilderTableHeader: layout differs from the database");
static_assert(sizeof(D2CmpCellBuilderTablePrefix) == 104, "D2CmpCellBuilderTablePrefix: layout differs from the database");
static_assert(sizeof(D2CmpCellBuilderTableReusePrefix) == 68, "D2CmpCellBuilderTableReusePrefix: layout differs from the database");
static_assert(sizeof(D2CmpCellBuilderTableVtable) == 16, "D2CmpCellBuilderTableVtable: layout differs from the database");
static_assert(sizeof(D2CmpCodecBitReader) == 16, "D2CmpCodecBitReader: layout differs from the database");
static_assert(sizeof(D2CmpCodecByteSpan) == 8, "D2CmpCodecByteSpan: layout differs from the database");
static_assert(sizeof(D2CmpDccCellCursor) == 60, "D2CmpDccCellCursor: layout differs from the database");
static_assert(sizeof(D2CmpDccCellState) == 20, "D2CmpDccCellState: layout differs from the database");
static_assert(sizeof(D2CmpDccDirectionContext) == 44, "D2CmpDccDirectionContext: layout differs from the database");
static_assert(sizeof(D2CmpDccFileHeader) == 15, "D2CmpDccFileHeader: layout differs from the database");
static_assert(sizeof(D2CmpDccFrameState) == 44, "D2CmpDccFrameState: layout differs from the database");
static_assert(sizeof(D2CmpDccPaletteMap) == 260, "D2CmpDccPaletteMap: layout differs from the database");
static_assert(sizeof(D2CmpDccSourceInfo) == 16, "D2CmpDccSourceInfo: layout differs from the database");
static_assert(sizeof(D2CmpFogSystemInfoPrefix) == 16, "D2CmpFogSystemInfoPrefix: layout differs from the database");
static_assert(sizeof(D2CmpHSL) == 24, "D2CmpHSL: layout differs from the database");
static_assert(sizeof(D2CmpLruCache) == 52, "D2CmpLruCache: layout differs from the database");
static_assert(sizeof(D2CmpLruEntry) == 20, "D2CmpLruEntry: layout differs from the database");
static_assert(sizeof(D2CmpNormalizedTileProject) == 276, "D2CmpNormalizedTileProject: layout differs from the database");
static_assert(sizeof(D2CmpPaletteEntry) == 4, "D2CmpPaletteEntry: layout differs from the database");
static_assert(sizeof(D2CmpRGB24) == 3, "D2CmpRGB24: layout differs from the database");
static_assert(sizeof(D2CmpPaletteTables) == 443175, "D2CmpPaletteTables: layout differs from the database");
static_assert(sizeof(D2CmpRgb24FrameHeader) == 32, "D2CmpRgb24FrameHeader: layout differs from the database");
static_assert(sizeof(D2CmpRgb24RuntimeFilePrefix) == 24, "D2CmpRgb24RuntimeFilePrefix: layout differs from the database");
static_assert(sizeof(D2CmpRleCellHeader) == 32, "D2CmpRleCellHeader: layout differs from the database");
static_assert(sizeof(D2CmpSerializedCellFileV5Header) == 52, "D2CmpSerializedCellFileV5Header: layout differs from the database");
static_assert(sizeof(D2CmpSerializedRleCellHeaderV5) == 40, "D2CmpSerializedRleCellHeaderV5: layout differs from the database");
static_assert(sizeof(D2CmpSpriteDataCache) == 60, "D2CmpSpriteDataCache: layout differs from the database");
static_assert(sizeof(D2CmpSpriteDataNode) == 28, "D2CmpSpriteDataNode: layout differs from the database");
static_assert(sizeof(D2CmpSpriteDecodeJobPrefix) == 28, "D2CmpSpriteDecodeJobPrefix: layout differs from the database");
static_assert(sizeof(D2CmpSpriteFrameHeaderPrefix) == 32, "D2CmpSpriteFrameHeaderPrefix: layout differs from the database");
static_assert(sizeof(D2CmpSpriteGfxBlockPrefix) == 152, "D2CmpSpriteGfxBlockPrefix: layout differs from the database");
static_assert(sizeof(D2CmpSpriteGfxHashEntryPrefix) == 48, "D2CmpSpriteGfxHashEntryPrefix: layout differs from the database");
static_assert(sizeof(D2CmpSpriteGfxHashStorage) == 65536, "D2CmpSpriteGfxHashStorage: layout differs from the database");
static_assert(sizeof(D2CmpSpriteGfxNodePrefix) == 184, "D2CmpSpriteGfxNodePrefix: layout differs from the database");
static_assert(sizeof(D2CmpSpriteHeaderData) == 192, "D2CmpSpriteHeaderData: layout differs from the database");
static_assert(sizeof(D2CmpStormListArray) == 16, "D2CmpStormListArray: layout differs from the database");
static_assert(sizeof(D2CmpTileBuildComponent) == 20, "D2CmpTileBuildComponent: layout differs from the database");
static_assert(sizeof(D2CmpTileBuilder) == 96, "D2CmpTileBuilder: layout differs from the database");
static_assert(sizeof(D2CmpTileList) == 16, "D2CmpTileList: layout differs from the database");
static_assert(sizeof(D2CmpTileRuntimeComponent) == 20, "D2CmpTileRuntimeComponent: layout differs from the database");
static_assert(sizeof(D2GfxDataStrc) == 72, "D2GfxDataStrc: layout differs from the database");
static_assert(sizeof(D2TileLibraryEntryStrc) == 96, "D2TileLibraryEntryStrc: layout differs from the database");
static_assert(sizeof(D2TileLibraryHashNodeStrc) == 20, "D2TileLibraryHashNodeStrc: layout differs from the database");
static_assert(sizeof(D2TileLibraryHashRefStrc) == 8, "D2TileLibraryHashRefStrc: layout differs from the database");
static_assert(sizeof(D2TileLibraryHashStrc) == 512, "D2TileLibraryHashStrc: layout differs from the database");
static_assert(sizeof(D2TileLibraryHeaderStrc) == 276, "D2TileLibraryHeaderStrc: layout differs from the database");
static_assert(sizeof(D2TileRecordStrc) == 272, "D2TileRecordStrc: layout differs from the database");
#endif
#endif
