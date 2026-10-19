#pragma once

/****************************************************************************
*                                                                           *
*   D2Constants.h                                                           *
*   Copyright (C) Olivier Verville                                          *
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
*   This file is meant to declare various constant data. As you add more    *
*   custom code to your library, you will be using many constant values     *
*   used by the game's internal code. Unit types are a good example.        *
*   Declaring constants allows you to assign a name to these constants      *
*   which are more convenient to use in your source code.                   *
*                                                                           *
*   Another advantage is smaller impact in cases where these values would   *
*   need to change. Rather than revising every single piece of code using   *
*   this value, you only need to change your constant's value               *
*                                                                           *
*****************************************************************************/

/* ------------------------------------------------------------------
 * GENERATED from the reverse-engineering database of D2CMP.dll 1.10 (image base 0x6FDF0000).
 *
 * Underlying type from the size the database records; values in hex where
 * the enum is a bitmask; members in the database's declaration order.
 * ------------------------------------------------------------------ */

#include <cstdint>

enum D2C_TileComponentFlags : uint16_t
{
	TILECOMP_GAMESQUARE = 0x2000,
};

enum D2C_TileProjectFlags : uint32_t
{
	TILEPROJECT_FLAG_24BIT = 0x00000001,
	TILEPROJECT_FEWBLOCKS = 0x00000008,
};

/* ------------------------------------------------------------------
 * NOT USED BY THIS SOURCE -- the database's other enums, commented out.
 * ------------------------------------------------------------------ */

// enum D2C_Acts : int32_t
// {
// 	ACT_I = 0,
// 	ACT_II = 1,
// 	ACT_III = 2,
// 	ACT_IV = 3,
// 	ACT_V = 4,
// 	NUM_ACTS = 5,
// };

// enum D2C_ArenaMatchTypes : int32_t
// {
// 	D2ARENAMATCH_DEATHMATCH = 0,
// 	D2ARENAMATCH_TEAMDEATHMATCH = 1,
// 	D2ARENAMATCH_CAPTURETHEFLAG = 2,
// 	D2ARENAMATCH_KINGOFTHEHILL = 3,
// };

// enum D2C_AutomapCells : int32_t
// {
// 	MAPIMGCELL_REDCROSS = 221,
// 	MAPIMGCELL_HAMMER = 302,
// 	MAPIMGCELL_CAINCAGE = 303,
// 	MAPIMGCELL_MEPHORB = 305,
// 	MAPIMGCELL_DIABSEAL = 306,
// 	MAPIMGCELL_WAYPOINT = 307,
// 	MAPIMGCELL_WELL = 309,
// 	MAPIMGCELL_SHRINE = 310,
// 	MAPIMGCELL_FALLCAMPFLAG = 312,
// 	MAPIMGCELL_INITREE = 313,
// 	MAPIMGCELL_CAINROCK = 314,
// 	MAPIMGCELL_GIDBINN = 315,
// 	MAPIMGCELL_QHAMMER = 316,
// 	MAPIMGCELL_BLUECROSS = 317,
// 	MAPIMGCELL_QCHEST = 318,
// 	MAPIMGCELL_STASH = 319,
// 	MAPIMGCELL_ARCANEPORTAL = 339,
// 	MAPIMGCELL_ROGUEFIRE = 405,
// 	MAPIMGCELL_BOOK = 427,
// 	MAPIMGCELL_PLACEHOLDER = 1176,
// 	MAPIMGCELL_BARRITOWER = 1258,
// };

// enum D2C_ButtonActions : int32_t
// {
// 	TRADEBTN_CANCEL = 2,
// 	TRADEBTN_PERFORM = 3,
// 	TRADEBTN_ACCEPT = 4,
// 	TRADEBTN_GREENCHECK = 7,
// 	TRADEBTN_SENDGOLD = 8,
// 	TRADEBTN_CLOSESTASH = 18,
// 	TRADEBTN_STASHWITHDRAW = 19,
// 	TRADEBTN_STASHDEPOSIT = 20,
// 	TRADEBTN_CLOSECUBE = 23,
// 	TRADEBTN_TRANSMUTE = 24,
// };

// enum D2C_ChatMessageTypes : int32_t
// {
// 	CHAT_NONE = 0,
// 	CHAT_PLAYERMESSAGE = 1,
// 	CHAT_RECEIVEDWHISPER = 2,
// 	CHAT_SERVERMESSAGE = 3,
// 	CHAT_NONAME = 4,
// 	CHAT_EMOTE = 5,
// 	CHAT_SENTWHISPER = 6,
// 	CHAT_CLUESCROLL = 7,
// };

// enum D2C_D2WinMenuEntryTypes : int32_t
// {
// 	MENUENTRY_EDITBOX = 1,
// 	MENUENTRY_IMAGE = 2,
// 	MENUENTRY_ANIMATION = 3,
// 	MENUENTRY_TEXT = 4,
// 	MENUENTRY_SCROLLBAR = 5,
// 	MENUENTRY_BUTTON = 6,
// 	MENUENTRY_LIST = 7,
// 	MENUENTRY_TIMER = 8,
// 	MENUENTRY_SMACKER = 9,
// 	MENUENTRY_PROGRESSBAR = 10,
// 	MENUENTRY_POPUP = 11,
// 	MENUENTRY_ACCOUNTLIST = 12,
// };

// enum D2C_Difficulties : int32_t
// {
// 	DIFFMODE_NORMAL = 0,
// 	DIFFMODE_NIGHTMARE = 1,
// 	DIFFMODE_HELL = 2,
// 	NUM_DIFFICULTIES = 3,
// };

// enum D2C_GlobalUiStates : int32_t
// {
// 	UISTATE_NONE = 0,
// 	UISTATE_RIGHT = 1,
// 	UISTATE_LEFT = 2,
// 	UISTATE_BOTH = 3,
// };

// enum D2C_ItemModes : int32_t
// {
// 	IMODE_STORED = 0,
// 	IMODE_EQUIP = 1,
// 	IMODE_INBELT = 2,
// 	IMODE_ONGROUND = 3,
// 	IMODE_ONCURSOR = 4,
// 	IMODE_DROPPING = 5,
// 	IMODE_SOCKETED = 6,
// };

// enum D2C_ItemsTxtArmorComps : int32_t
// {
// 	TXTARMORCOMP_RARM = 0,
// 	TXTARMORCOMP_LARM = 1,
// 	TXTARMORCOMP_TORSO = 2,
// 	TXTARMORCOMP_LEGS = 3,
// 	TXTARMORCOMP_RSPAD = 4,
// 	TXTARMORCOMP_LSPAD = 5,
// };

// enum D2C_Language : int32_t
// {
// 	LANGUAGE_ENGLISH = 0,
// 	LANGUAGE_SPANISH = 1,
// 	LANGUAGE_GERMAN = 2,
// 	LANGUAGE_FRENCH = 3,
// 	LANGUAGE_PORTUGUESE = 4,
// 	LANGUAGE_ITALIAN = 5,
// 	LANGUAGE_JAPANESE = 6,
// 	LANGUAGE_KOREAN = 7,
// 	LANGUAGE_CHINESESIN = 8,
// 	LANGUAGE_CHINESETWN = 9,
// 	LANGUAGE_POLISH = 10,
// 	LANGUAGE_RUSSIAN = 11,
// 	LANGUAGE_ENGLISHKOR = 12,
// 	NUM_LANGUAGES = 13,
// };

// enum D2C_LaunchType : uint8_t
// {
// 	LAUNCHTYPE_NONE = 0,
// 	LAUNCHTYPE_LOCAL = 1,
// 	LAUNCHTYPE_REALM = 2,
// 	LAUNCHTYPE_TCPIP = 3,
// 	LAUNCHTYPE_OPENBNET = 4,
// };

// enum D2C_MenuImageLoadType : int32_t
// {
// 	MENUIMGTYPE_DC6 = 0,
// 	MENUIMGTYPE_DCC = 1,
// };

// enum D2C_NpcMenuIdx : int32_t
// {
// 	NPCMENUIDX_AKARA = 0,
// 	NPCMENUIDX_ATMA = 1,
// 	NPCMENUIDX_CAIN1 = 2,
// 	NPCMENUIDX_CHARSI = 3,
// 	NPCMENUIDX_DROGNAN = 4,
// 	NPCMENUIDX_ELZIX = 5,
// 	NPCMENUIDX_GEGLASH = 6,
// 	NPCMENUIDX_GHEED = 7,
// 	NPCMENUIDX_GREIZ = 8,
// 	NPCMENUIDX_JERHYN = 9,
// 	NPCMENUIDX_LYSANDER = 10,
// 	NPCMENUIDX_KASHYA = 11,
// 	NPCMENUIDX_FARA = 12,
// 	NPCMENUIDX_WARRIV1 = 13,
// 	NPCMENUIDX_WARRIV2 = 14,
// 	NPCMENUIDX_MESHIF1 = 15,
// 	NPCMENUIDX_CAIN2 = 16,
// 	NPCMENUIDX_CAIN5 = 17,
// 	NPCMENUIDX_CAIN3 = 18,
// 	NPCMENUIDX_CAIN4 = 19,
// 	NPCMENUIDX_TYRAEL1 = 20,
// 	NPCMENUIDX_TYRAEL2 = 21,
// 	NPCMENUIDX_HALBU = 22,
// 	NPCMENUIDX_JAMELLA = 23,
// 	NPCMENUIDX_ASHEARA = 24,
// 	NPCMENUIDX_HRATLI = 25,
// 	NPCMENUIDX_ALKOR = 26,
// 	NPCMENUIDX_ORMUS = 27,
// 	NPCMENUIDX_IZUALGHOST = 28,
// 	NPCMENUIDX_HALBU2 = 29,
// 	NPCMENUIDX_MESHIF2 = 30,
// 	NPCMENUIDX_NATALYA = 31,
// 	NPCMENUIDX_FLAVIE = 32,
// 	NPCMENUIDX_KAELAN = 33,
// 	NPCMENUIDX_HADRIEL = 34,
// 	NPCMENUIDX_GUARD1 = 35,
// 	NPCMENUIDX_GUARD2 = 36,
// 	NPCMENUIDX_TYRAEL3 = 37,
// 	NPCMENUIDX_CAIN6 = 38,
// 	NPCMENUIDX_LARZUK = 39,
// 	NPCMENUIDX_MALAH = 40,
// 	NPCMENUIDX_ANYA = 41,
// 	NPCMENUIDX_NIHLATHAK = 42,
// 	NPCMENUIDX_QUALKEHK = 43,
// 	NPCMENUIDX_FROZENANYA = 44,
// 	NPCMENUIDX_ANCIENTSTATUE1 = 45,
// 	NPCMENUIDX_ANCIENTSTATUE2 = 46,
// 	NPCMENUIDX_ANCIENTSTATUE3 = 47,
// };

// enum D2C_OriginalQuestFlags : int32_t
// {
// 	QFLAG_REWARDGRANTED = 0,
// 	QFLAG_REWARDPENDING = 1,
// 	QFLAG_STARTED = 2,
// 	QFLAG_LEAVETOWN = 3,
// 	QFLAG_ENTERAREA = 4,
// 	QFLAG_CUSTOM1 = 5,
// 	QFLAG_CUSTOM2 = 6,
// 	QFLAG_CUSTOM3 = 7,
// 	QFLAG_CUSTOM4 = 8,
// 	QFLAG_CUSTOM5 = 9,
// 	QFLAG_CUSTOM6 = 10,
// 	QFLAG_CUSTOM7 = 11,
// 	QFLAG_UPDATEQUESTLOG = 12,
// 	QFLAG_PRIMARYGOALDONE = 13,
// 	QFLAG_COMPLETEDNOW = 14,
// 	QFLAG_COMPLETEDBEFORE = 15,
// };

// enum D2C_PlayerModes : int32_t
// {
// 	PLRMODE_DEATH = 0,
// 	PLRMODE_NEUTRAL = 1,
// 	PLRMODE_WALK = 2,
// 	PLRMODE_RUN = 3,
// 	PLRMODE_GETHIT = 4,
// 	PLRMODE_TOWNNEUTRAL = 5,
// 	PLRMODE_TOWNWALK = 6,
// 	PLRMODE_ATTACK1 = 7,
// 	PLRMODE_ATTACK2 = 8,
// 	PLRMODE_BLOCK = 9,
// 	PLRMODE_CAST = 10,
// 	PLRMODE_THROW = 11,
// 	PLRMODE_KICK = 12,
// 	PLRMODE_SPECIAL1 = 13,
// 	PLRMODE_SPECIAL2 = 14,
// 	PLRMODE_SPECIAL3 = 15,
// 	PLRMODE_SPECIAL4 = 16,
// 	PLRMODE_DEAD = 17,
// 	PLRMODE_SEQUENCE = 18,
// 	PLRMODE_KNOCKBACK = 19,
// 	NUMBER_OF_PLRMODES = 20,
// };

// enum D2C_PvpFlags : int32_t
// {
// 	PVPFLAG_INVITEYOU = 1,
// 	PVPFLAG_YOUINVITE = 2,
// 	PVPFLAG_HOSTILEYOU = 4,
// 	PVPFLAG_YOUHOSTILE = 8,
// 	PVPFLAG_ISALLIED = 16,
// 	PVPFLAG_ALLIEDWITHYOU = 32,
// };

// enum D2C_SaveMagicHeaders : uint32_t
// {
// 	D2SMAGIC_HEADER = 0xAA55AA55,
// 	D2SMAGIC_QUESTINFO = 0x576F6F21,
// 	D2SMAGIC_WPINFO = 0x00005753,
// 	D2SMAGIC_NPCINFO = 0x00007701,
// 	D2SMAGIC_MAINSTATINFO = 0x00006766,
// 	D2SMAGIC_SKILLINFO = 0x00006866,
// 	D2SMAGIC_ITEMLIST = 0x00004A4D,
// 	D2SMAGIC_MERCINFO = 0x00006A66,
// 	D2SMAGIC_IRONGOLEM = 0x00006B66,
// };

// enum D2C_SRV2CLT5A_TYPES : int32_t
// {
// 	EVENTTYPE_DISCONNECT = 0,
// 	EVENTTYPE_DROPERROR = 1,
// 	EVENTTYPE_PLAYERJOIN = 2,
// 	EVENTTYPE_PLAYERLEFT = 3,
// 	EVENTTYPE_NOTINGAME = 4,
// 	EVENTTYPE_NOTLOGGEDIN = 5,
// 	EVENTTYPE_SLAIN_BY = 6,
// 	EVENTTYPE_PVPSTATUS = 7,
// 	EVENTTYPE_PLAYERISBUSY = 8,
// 	EVENTTYPE_PLEASEWAIT = 9,
// 	EVENTTYPE_HASITEMSINBOX = 10,
// 	EVENTTYPE_UNUSED1 = 11,
// 	EVENTTYPE_UNUSED2 = 12,
// 	EVENTTYPE_NOTLISTENINGTOYOU = 13,
// 	EVENTTYPE_CANTDOTHAT = 14,
// 	EVENTTYPE_REALMGOINGDOWN = 15,
// 	EVENTTYPE_PLEASEWAITHOSTILE = 16,
// 	EVENTTYPE_JORDANSTONES = 17,
// 	EVENTTYPE_DIABLOCLONE = 18,
// };

// enum D2C_SRV2CLT77_PARAMS : int32_t
// {
// 	UPDATEUI_ASKTOTRADE = 0,
// 	UPDATEUI_ASKEDTOTRADE = 1,
// 	UPDATEUI_ACCEPTTRADE = 5,
// 	UPDATEUI_OPENTRADE = 6,
// 	UPDATEUI_SELLERNOROOM = 9,
// 	UPDATEUI_BUYERNOROOM = 10,
// 	UPDATEUI_CLOSETRADE = 12,
// 	UPDATEUI_TRADEDONE = 13,
// 	UPDATEUI_CHECKBOX = 14,
// 	UPDATEUI_NORMCHECKBOX = 15,
// 	UPDATEUI_OPENSTASH = 16,
// 	UPDATEUI_CLOSESTASH = 17,
// 	UPDATEUI_OPENCUBE = 21,
// };

// enum D2C_SRV2CLT9C_9D_PARAMS : int32_t
// {
// 	ITEMACTION_ADDTOGROUND = 0,
// 	ITEMACTION_GROUNDTOCURSOR = 1,
// 	ITEMACTION_DROPTOGROUND = 2,
// 	ITEMACTION_ONGROUND = 3,
// 	ITEMACTION_PUTINCONTAINER = 4,
// 	ITEMACTION_REMOVEFROMCONTAINER = 5,
// 	ITEMACTION_EQUIP = 6,
// 	ITEMACTION_INDIRECTLYSWAPBODYITEM = 7,
// 	ITEMACTION_UNEQUIP = 8,
// 	ITEMACTION_SWAPBODYITEM = 9,
// 	ITEMACTION_ADDQUANTITY = 10,
// 	ITEMACTION_ADDTOSHOP = 11,
// 	ITEMACTION_REMOVEFROMSHOP = 12,
// 	ITEMACTION_SWAPINCONTAINER = 13,
// 	ITEMACTION_PUTINBELT = 14,
// 	ITEMACTION_REMOVEFROMBELT = 15,
// 	ITEMACTION_SWAPINBELT = 16,
// 	ITEMACTION_AUTOUNEQUIP = 17,
// 	ITEMACTION_TOCURSOR = 18,
// 	ITEMACTION_ITEMINSOCKET = 19,
// 	ITEMACTION_UNKNOWN0x14 = 20,
// 	ITEMACTION_UPDATESTATS = 21,
// 	ITEMACTION_UNKNOWN0x16 = 22,
// 	ITEMACTION_WEAPONSWITCH = 23,
// };

// enum D2C_StringColorCodes : int32_t
// {
// 	STRCOLOR_WHITE = 0,
// 	STRCOLOR_RED = 1,
// 	STRCOLOR_LIGHTGREEN = 2,
// 	STRCOLOR_BLUE = 3,
// 	STRCOLOR_DARK_GOLD = 4,
// 	STRCOLOR_GREY = 5,
// 	STRCOLOR_BLACK = 6,
// 	STRCOLOR_TAN = 7,
// 	STRCOLOR_ORANGE = 8,
// 	STRCOLOR_YELLOW = 9,
// 	STRCOLOR_DARKGREEN = 10,
// 	STRCOLOR_PURPLE = 11,
// 	STRCOLOR_DARKGREEN2 = 12,
// };

// enum D2C_StringTablesHcidx : int32_t
// {
// 	STRTABLE_STRING = 0,
// 	STRTABLE_PATCHSTRING = 1,
// 	STRTABLE_EXPSTRING = 2,
// };

// enum D2C_TownLevels : int32_t
// {
// 	D2TOWN_ROGUECAMP = 1,
// 	D2TOWN_LUTGHOL = 40,
// 	D2TOWN_KURAST = 75,
// 	D2TOWN_PANDFORT = 103,
// 	D2TOWN_HARRO = 109,
// 	D2TOWN_NULL = 0,
// };

// enum D2C_TransactionTypes : int32_t
// {
// 	TRANSACTIONTYPE_BUY = 0,
// 	TRANSACTIONTYPE_SELL = 1,
// 	TRANSACTIONTYPE_GAMBLE = 2,
// 	TRANSACTIONTYPE_REPAIR = 3,
// };

// enum D2C_TxtLinkerIndexTypes : int32_t
// {
// 	TXTLINKER_NONE = 0,
// 	TXTLINKER_PLAYERCLASS = 1,
// 	TXTLINKER_BODYLOCS = 2,
// 	TXTLINKER_STOREPAGE = 3,
// 	TXTLINKER_ELEMTYPES = 4,
// 	TXTLINKER_HITCLASS = 5,
// 	TXTLINKER_MONMODE = 6,
// 	TXTLINKER_PLRMODE = 7,
// 	TXTLINKER_SKILLCALC = 8,
// 	TXTLINKER_MISSCALC = 9,
// 	TXTLINKER_SKILLSCODE = 10,
// 	TXTLINKER_EVENTS = 11,
// 	TXTLINKER_COMPCODE = 12,
// 	TXTLINKER_MONAI = 13,
// 	TXTLINKER_ITEMS = 14,
// 	TXTLINKER_PROPERTIES = 15,
// 	TXTLINKER_RUNES = 16,
// 	TXTLINKER_HIREDESC = 17,
// 	TXTLINKER_STATES = 18,
// 	TXTLINKER_SOUNDS = 19,
// 	TXTLINKER_COLORS = 20,
// 	TXTLINKER_TREASURECLASS = 21,
// 	TXTLINKER_MONSTATS = 22,
// 	TXTLINKER_MONSOUNDS = 23,
// 	TXTLINKER_MONSTATS2 = 24,
// 	TXTLINKER_MONPLACE = 25,
// 	TXTLINKER_SUPERUNIQUES = 26,
// 	TXTLINKER_MISSILES = 27,
// 	TXTLINKER_MONSEQ = 28,
// 	TXTLINKER_SKILLDESC = 29,
// 	TXTLINKER_SKILLS = 30,
// 	TXTLINKER_OVERLAY = 31,
// 	TXTLINKER_ITEMSTATCOST = 32,
// 	TXTLINKER_PETTYPE = 33,
// 	TXTLINKER_ITEMTYPES = 34,
// 	TXTLINKER_SETS = 35,
// 	TXTLINKER_SETITEMS = 36,
// 	TXTLINKER_UNIQUEITEMS = 37,
// 	TXTLINKER_MONPROP = 38,
// 	TXTLINKER_MONTYPE = 39,
// 	TXTLINKER_MONUMOD = 40,
// 	TXTLINKER_LEVELS = 41,
// 	TXTLINKER_INVALID = 42,
// };

// enum D2C_UiButtonModes : int32_t
// {
// 	BTNMODE_ENABLED = 0,
// 	BTNMODE_PRESSED = 1,
// 	BTNMODE_DISABLED = 2,
// };

// enum D2C_UiStates : int32_t
// {
// 	UI_TURNON = 0,
// 	UI_TURNOFF = 1,
// 	UI_TOGGLE = 2,
// };

// enum D2C_UIvars : int32_t
// {
// 	UI_GAME = 0,
// 	UI_INVENTORY = 1,
// 	UI_STATSCREEN = 2,
// 	UI_MINISKILL = 3,
// 	UI_SKILLTREE = 4,
// 	UI_CHATBOX = 5,
// 	UI_NEWSTATS = 6,
// 	UI_NEWSKILLS = 7,
// 	UI_NPCMENU = 8,
// 	UI_ESCMENU = 9,
// 	UI_AUTOMAP = 10,
// 	UI_CONFIG = 11,
// 	UI_NPCSHOP = 12,
// 	UI_HOLDALT = 13,
// 	UI_ANVIL = 14,
// 	UI_QUESTSCREEN = 15,
// 	UI_INISCROLL = 16,
// 	UI_QUESTLOG = 17,
// 	UI_UNKNOWN18 = 18,
// 	UI_HIRICONS = 19,
// 	UI_WAYPOINT = 20,
// 	UI_MINIPANEL = 21,
// 	UI_PARTYSCREEN = 22,
// 	UI_MPTRADE = 23,
// 	UI_MSGLOG = 24,
// 	UI_STASH = 25,
// 	UI_CUBE = 26,
// 	UI_STEEGSTONE = 27,
// 	UI_GUILDVAULT = 28,
// 	UI_UNKNOWN29 = 29,
// 	UI_UNKNOWN30 = 30,
// 	UI_BELTROWS = 31,
// 	UI_UNKNOWN32 = 32,
// 	UI_HELPSCREEN = 33,
// 	UI_HELPBUTTON = 34,
// 	UI_HIREICONS = 35,
// 	UI_MERCINV = 36,
// 	UI_RECIPESCROLL = 37,
// };

// enum D2C_VendorInventoryModes : int32_t
// {
// 	VENDORMODE_NONE = 0,
// 	VENDORMODE_TRADE = 1,
// 	VENDORMODE_BUY = 2,
// 	VENDORMODE_SELL = 3,
// 	VENDORMODE_REPAIR = 4,
// 	VENDORMODE_VERIFY = 5,
// 	VENDORMODE_HEAL = 6,
// 	VENDORMODE_DISPLAYING_RESULTS = 7,
// 	VENDORMODE_LASTVENDOR = 8,
// 	VENDORMODE_UNKNOWN = 9,
// 	VENDORMODE_PLRTRADE_OLD = 10,
// 	VENDORMODE_PLRTRADE = 11,
// 	VENDORMODE_STASH = 12,
// 	VENDORMODE_DEPOSIT = 13,
// 	VENDORMODE_CUBE = 14,
// 	VENDORMODE_IMBUE = 19,
// };

// enum D2C_WarpTileTypes : int32_t
// {
// 	WARPTILE_TOWNENTRY = 0,
// 	WARPTILE_TOWNENTRY2 = 5,
// 	WARPTILE_CORPSELOC = 10,
// 	WARPTILE_MAPENTRY = 11,
// 	WARPTILE_TPLOC = 12,
// 	WARPTILE_TOWNWP = 13,
// };

// enum D2GameResolutionMode : int32_t
// {
// 	D2GAMERES_640x480 = 0,
// 	D2GAMERES_800x600 = 1,
// 	NUM_GAME_RESOLUTIONS = 2,
// 	D2GAMERES_1344x700 = 3,
// };

// enum D2GameTypeMode : int32_t
// {
// 	MODE_LOCAL = 0,
// 	MODE_CLOSED = 1,
// 	MODE_TCPIP = 2,
// 	MODE_OPEN = 3,
// };

// enum D2HarcodedConstants : int32_t
// {
// 	HARDCODEDCST_MAX_LEVEL = 99,
// 	DEFAULT_FRAMES_PER_SECOND = 25,
// };

// enum D2InteractStates : int32_t
// {
// 	INTERACT_NONE = 0,
// 	INTERACT_TALKING = 1,
// 	INTERACT_TRADE = 2,
// };

// enum D2Killtypes : int32_t
// {
// 	KILLTYPE_SUICIDE = 0,
// 	KILLTYPE_PVP_KILL = 1,
// 	KILLTYPE_PVP2 = 2,
// 	KILLTYPE_PVM_KILL = 3,
// 	KILLTYPE_NVM = 4,
// 	KILLTYPE_MVP_KILL = 5,
// 	KILLTYPE = 6,
// };

// enum D2QuestAlert : int32_t
// {
// 	QALERT_DEFAULT = 0,
// 	QALERT_MESSAGE = 1,
// 	QALERT_SOUND = 2,
// 	QALERT_SOUNDEX = 16,
// 	QALERT_BLOCKED = 32,
// };

// enum D2QuestEvents : int32_t
// {
// 	QUESTEVENT_NPCACTIVATE = 0,
// 	QUESTEVENT_NPCDEACTIVATE = 2,
// 	QUESTEVENT_CHANGEDLEVEL = 3,
// 	QUESTEVENT_ITEMPICKEDUP = 4,
// 	QUESTEVENT_ITEMDROPPED = 5,
// 	QUESTEVENT_MONSTERKILLED = 8,
// 	QUESTEVENT_PLAYERDROPPEDWITHQUESTITEM = 9,
// 	QUESTEVENT_PLAYERLEAVESGAME = 10,
// 	QUESTEVENT_SCROLLMESSAGE = 11,
// 	QUESTEVENT_PLAYERSTARTEDGAME = 13,
// 	QUESTEVENT_PLAYERJOINEDGAME = 14,
// };

// enum D2SaveCharType : int32_t
// {
// 	CHARTYPE_HARDCORE = 36,
// 	CHARTYPE_SOFTCORE = 40,
// };

// enum D2ScrollMenus : int32_t
// {
// 	SCROLLMENU_MESSAGE = 0,
// 	SCROLLMENU_MENU = 1,
// 	SCROLLMENU_NONE = 2,
// };

// enum D2Vendors : int32_t
// {
// 	VENDOR_AKARA = 0,
// 	VENDOR_GHEED = 1,
// 	VENDOR_CHARSI = 2,
// 	VENDOR_FARA = 3,
// 	VENDOR_LYSANDER = 4,
// 	VENDOR_DROGNAN = 5,
// 	VENDOR_HRALTI = 6,
// 	VENDOR_ALKOR = 7,
// 	VENDOR_ORMUS = 8,
// 	VENDOR_ELZIX = 9,
// 	VENDOR_ASHEARA = 10,
// 	VENDOR_CAIN = 11,
// 	VENDOR_HALBU = 12,
// 	VENDOR_JAMELLA = 13,
// 	VENDOR_MALAH = 14,
// 	VENDOR_LARZUK = 15,
// 	VENDOR_DREHYA = 16,
// };

/* ------------------------------------------------------------------
 * READ-ONLY SCALAR CONSTANTS, from the same database.
 *
 * 22 of them, and the section is defined by a RULE rather than
 * curated: every named item in .rdata that is one element wide,
 * whose value IDA holds, and which IDA does NOT mark as an offset.
 *
 * That last clause is the whole filter. 32 more items pass the
 * first two and are addresses -- the gVtbl* pointers, the
 * __IMPORT_DESCRIPTOR_* entries, the gpsz* string pointers. IDA
 * already knows they are addresses because it carries a
 * cross-reference from each, so the database answers the question
 * and nothing here has to guess from the value.
 *
 * .rdata and not .data on purpose: a number in .data is a variable
 * with an initial value, and calling that a constant would be a
 * claim the section headers contradict.
 * ------------------------------------------------------------------ */

constexpr double gdPaletteBlueHueOffset = 4.0;   // 0x4010000000000000
constexpr double gdPaletteChannelHueOffset = 120.0;   // 0x405E000000000000
constexpr double gdPaletteChannelScale255 = 255.0;   // 0x406FE00000000000
constexpr double gdPaletteGrayLightBias = 0.1666666625274554;   // 0x3FC555554C71C723
constexpr double gdPaletteGrayLightScale = 0.8333333002196431;   // 0x3FEAAAAA98E38E45
constexpr double gdPaletteHalf = 0.5;   // 0x3FE0000000000000
constexpr double gdPaletteHueBlueBoundary = 240.0;   // 0x406E000000000000
constexpr double gdPaletteHueCircle = 360.0;   // 0x4076800000000000
constexpr double gdPaletteHueGreenBoundary = 180.0;   // 0x4066800000000000
constexpr double gdPaletteHueSextant = 60.0;   // 0x404E000000000000
constexpr double gdPaletteInv255 = 0.00392156862745098;   // 0x3F70101010101010
constexpr double gdPaletteInv60 = 0.016666666666666666;   // 0x3F91111111111111
constexpr double gdPaletteLuminanceBoost = 0.20000000298023224;   // 0x3FC99999A0000000
constexpr double gdPaletteLuminanceReduction = 0.10000000149011612;   // 0x3FB99999A0000000
constexpr double gdPaletteOne = 1.0;   // 0x3FF0000000000000
constexpr double gdPaletteSelectiveHueMax = 315.0;   // 0x4073B00000000000
constexpr double gdPaletteSelectiveHueMin = 45.0;   // 0x4046800000000000
constexpr double gdPaletteTwo = 2.0;   // 0x4000000000000000
constexpr double gdPaletteZero = 0.0;   // 0x0000000000000000
constexpr float gfPaletteFixedHueStep = 30.0f;   // 0x41F00000
constexpr float gfPaletteHueStep = 15.0f;   // 0x41700000
constexpr wchar_t SrcStr = 0;

/* ==================================================================
 * NOT IN THE DATABASE -- carried over from the previous D2Constants.h
 *
 * Nothing below is measured against the binary. It is here because
 * the previous file held it and deleting it would lose it: the
 * project this header lives in is not under version control.
 *
 * 0 whole enum(s) the database does not have, and 0 member(s) left
 * over from 0 enum(s) it does. The leftovers are given their own
 * type so that the generated enum above stays exactly the database's.
 * ================================================================== */
