# D2MOO_BUGS

**Every inaccuracy of D2MOO's reconstructed source against the D2CMP.dll 1.10 binary found while rewriting it --
written only when it is certain.**

* **An entry is certain or it is not written.** Each one names the D2MOO file and line, the binary's addresses, and
  what each side does, measured in the bytes -- and says how D2MOO's own code uses and reads the thing in question.
  Where the bytes allow two readings, the difference is not written here.
* **It is not BUGS.md.** That file holds the ORIGINAL's defects; this one holds the places where D2MOO does not say
  what the original does. D2MOO's names are kept where this source took them.
* The source marks each place with a `D2MOO BUG:` line; "Ours" names the file.

## The entries

### D2CMP #10077, GetTileType, is declared at #10082's address

D2MOO: `D2CMP/include/D2CMP.h:176`,
`D2FUNC_DLL(D2CMP, 10077_GetTileType, int, __stdcall, (D2TileLibraryEntryStrc* pTileLibraryEntry), 0xFFF0)`.
The binary: D2CMP.dll's export directory gives #10077 the RVA 0xFF00
(TILELIB_TileGetType, 0x6FDFFF00) and #10082 the RVA 0xFFF0 (TILELIB_TileGetSequence,
0x6FDFFFF0; pefile) -- D2CMP.h:181 declares #10082 at 0xFFF0 too.

**The answers do not differ.** `D2FUNC_DLL` drops its OFFSET argument
(`D2CommonDefinitions/include/D2Dll.h:5`: it declares `DLL##_##NAME ARGS`), so
D2MOO binds #10077 by the import library's ordinal and the wrong offset is text
only.

**How D2MOO uses it**: `grep -rn "D2CMP_10077\|10077_GetTileType"` over D2MOO's
source, D2CMP.h aside, finds nothing -- no D2Common, D2Game or D2Client call
(control: the same grep for D2CMP_10081_GetTileRarity finds D2Common's
DrlgDrlgAnim.cpp:146). In the stock binaries only D2Client.dll imports #10077,
once (0x6FACD3F2).

Ours: TileLib.cpp, TILELIB_TileGetType.

### D2CMP #10098, MixPalette, takes its first argument as a byte where the binary reads a signed dword

D2MOO: `D2CMP/include/D2CMP.h:187`,
`D2FUNC_DLL(D2CMP, MixPalette, uint8_t*, __stdcall, (uint8_t nTrans, int nColor), 0xB760)`.
The binary: PALSHIFT_PalShiftGetTransform (0x6FDFB760) loads the slot whole and
compares it signed -- `mov ecx, [esp+4]` / `cmp ecx, 9` / `jge` (0x6FDFB760,
0x6FDFB764, 0x6FDFB767), then `cmp ecx, 2` / `jle` (0x6FDFB776, 0x6FDFB779) --
and computes the row from all 32 bits (`lea eax, [ecx*8]` ... `shl eax, 8`,
0x6FDFB780..0x6FDFB791). A `uint8_t` parameter is a byte read; this is an `int`.

**The answers do not differ** for any caller D2MOO has. Its one user is
D2Common's `Items/Items.cpp`, thirteen calls (lines 3730..4011; `grep -c
D2CMP_MixPalette` 13), each passing an items.txt `nTransform` or `nInvTrans`
field -- `int8_t` (`DataTbls/ItemsTbls.h:386`, `:387`) -- converted to the
declared `uint8_t`, so 0..255, which the binary's `int` holds alike; the stock
D2Common's thirteen call sites (ITEMS_GetColor, 0x6FD9E710) push it
zero-extended, `xor edx, edx` / `mov dl, [eax+141h]` or `[eax+142h]`. D2Game
has no call: `grep -rn D2CMP_ D2Game` finds one commented line, #10079's, and
`grep -rln D2CMP_MixPalette` over the whole source finds Items.cpp alone.

Ours: PalShift.cpp, PALSHIFT_PalShiftGetTransform.

### D2CMP #10004, GetNearestPaletteIndex, returns a byte, and counts unsigned

D2MOO: `D2CMP/include/D2CMP.h:161`,
`D2FUNC_DLL(D2CMP, GetNearestPaletteIndex, int, __stdcall, (PALETTEENTRY* pPalette, int nPaletteSize, int nRed, int nGreen, int nBlue), 0xAB20)`.
The binary: PALETTE_GetClosestColor (0x6FDFAB20; GetClosestColor in 1.07's
debug .def) answers in AL alone -- `mov al, byte ptr [esp+14h+var_4]`
(0x6FDFAB88) over an eax that holds the count, loaded in the loop
(`mov eax, [esp+14h+nColors]`, 0x6FDFAB7C), so for a 256-entry palette eax
is 100h plus the index. And the count is compared unsigned: `test ecx, ecx` /
`jbe` (0x6FDFAB2E, 0x6FDFAB34), `cmp ebp, eax` / `jb` (0x6FDFAB84,
0x6FDFAB86); an `int` would be `jle` / `jl`.

**The answers do not differ** for any caller D2MOO has. D2Win's three
(`D2Win/src/D2WinPalette.cpp:119`, `:120`, `:235`) store the answer in a byte
array or return it as `uint8_t`, and pass `0x100u` or `ARRAY_SIZE` of a
256-entry palette; the stock install's seven call sites (D2DDraw 2, D2Gdi 2,
D2Win 3) push 100h, and six store AL, the seventh returning it as D2Win
#10034's answer. A caller that read the declared `int` whole would get 256
more than the index. `grep -rn D2CMP_GetNearestPaletteIndex` over D2MOO's
source: those three, all D2Win's (the declaration spells the name through
`D2FUNC_DLL`); D2Common and D2Game have none (control: the same grep for
D2CMP_MixPalette finds D2Common's Items.cpp).

Ours: Palette.cpp, PALETTE_GetClosestColor.

### A cel frame's stream and a cel file's frames are declared as pointers where the binary keeps them inline

D2MOO: `D2Gfx/include/D2Gfx.h:43`, D2GfxCellStrc's `uint8_t* pPixels; //0x20`, and `D2Gfx.h:59`, D2CellFileStrc's
`D2GfxCellStrc* pGfxCells; //0x18`. The binary: a frame's encoded stream FOLLOWS its 32-byte header -- #10041,
CelGetControlData, answers the frame's address plus 0x20 (`add eax, 20h`, 0x6FDF26C6) and reads nothing there, and
#10030 builds each frame by copying the stream to that place (`lea edi, [edx+20h]`, 0x6FDF1BF3); and a cel file's +0x18
is the first slot of a TABLE of frame pointers, one per frame -- #10036, CelGetHandle, indexes it (`mov eax,
[esi+ecx*4+18h]`, 0x6FDF25C9), #10024 turns each slot from an offset into a pointer.

**How D2MOO uses them**: `grep -rn "pPixels\|pGfxCells"` over D2MOO's whole source finds the two declarations and
nothing else -- no D2Common, D2Game, D2Client, D2Gfx or D2Win read (control: the same grep for D2CellFileStrc finds
D2Common's D2Roster.h:36 and D2Skills.h:134). The other fields of both structs match the binary's offsets.

Ours: CelCmp.cpp; the types are D2Structs.h's D2CmpRleCellHeader and D2CmpBuiltCellFile.

### D2TileLibraryEntryStrc is laid out wrong from +0x28, and is 80 bytes where the binary's entry is 96

D2MOO: `D2CMP/include/D2CMP.h:67..78`, D2TileLibraryEntryStrc's `uint8_t dwTileFlags[4]; //0x28`,
`dwBlockOffset_pBlock //0x2C`, `nBlockSize //0x30`, `nBlocks //0x34`, `D2TileRecordStrc* pParent; //0x38`, `unk0x3C`,
`uint16_t nCacheIndex; //0x3E`, `uint32_t unk0x40[4]; //0x40`, and `field_50..field_5C` commented out -- 0x50 bytes.
The binary: TILECMP_TileListCreateProject copies 25 bytes to +0x28 (`lea edi, [ebp+28h]`, 0x6FDFF0A5; `mov ecx, 6` /
`rep movsd` / `movsb`, 0x6FDFF0D7..0x6FDFF0E3); D2CMP's tile cache reads +0x48..+0x5C as four dwords and two pointers
(`mov ecx, [esi+48h]` 0x6FDFE740, `[esi+4Ch]` 0x6FDFE6FC, `[esi+50h]` 0x6FDFE6EF, `mov [esi+54h], edi` 0x6FDFE717,
`[esi+58h]` 0x6FDFE728, `[esi+5Ch]` 0x6FDFE6D5); and the renderers' cache index is a signed WORD at +0x42, not +0x3E --
`mov ax, [esi+42h]` / `test ax, ax` / `jle` in D2Direct3D's and D2Glide's hardware-free callbacks (0x6F8419A4,
0x6F811824), under the original's own asserts "pTile->sCacheIndex >= INVALID_HARDWARE_ADDRESS" and "pTile->sCacheIndex-1
>= 0" (D2Direct3D 0x6F841485, 0x6F8414C1). The entry is 96 bytes (D2Structs.h's size assertion).

**The answers do not differ** for any code D2MOO has: it takes the entries by pointer only (`D2TileLibraryEntryStrc**`
arrays, DrlgDrlgAnim.cpp:142; no `sizeof(D2TileLibraryEntryStrc)` and no index of an entry array anywhere), and reads
none of the fields from +0x28 on. `grep -rnw` over D2Common, D2Game, D2Gfx, D2Client, D2Win and D2CMP for dwTileFlags,
dwBlockOffset_pBlock, nBlockSize, nBlocks, unk0x3C, nCacheIndex and unk0x40, D2CMP.h aside: 12 lines, every one another
struct's field or a local (Missile.h, Monster.h, Player.h, Clients.h, AStar.cpp, PathWF.cpp, SCmd.cpp, SUnit.cpp).
Control: the same tree's `pTileLibraryEntry->field_50` matches D2Gfx's CmnSubtile.cpp:1259 -- commented-out code that
reads +0x50 and +0x54 as the component count and array, the binary's own layout.

Ours: Tilecmp.cpp, TILECMP_TileListCreateProject; D2Structs.h's D2TileLibraryEntryStrc (nSubtileFlags[25], sCacheIndex,
dwComponentFileOffset .. ptLRUCache).
