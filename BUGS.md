# BUGS

Every defect found in the original `D2CMP.dll` 1.10, with the disassembly it rests on and a verdict on whether the
game can hit it. Each one is reproduced in the source -- never fixed -- and marked there with `BUG:`; an entry's
**In ours** names the place. The inaccuracies of D2MOO's reconstructed source are in `D2MOO_BUGS.md`.

## The verdicts

| verdict | what it means |
|---|---|
| **REACHABLE** | ordinary play, or ordinary input, can reach it. The argument is given, not assumed |
| **INERT** | real and reached, and no consumer can observe it |
| **LATENT** | real, and the data or the callers prevent it. What prevents it is named |
| **UNREACHABLE** | the code cannot execute at all |
| **NOT ESTABLISHED** | real, its reachability looked at and not settled; the entry says what would settle it |
| **REFUTED** | it looked like a bug and is not. The proof is given |

The disassembly is of the stock DLL (decoded with iced-x86); the names are this source's.

---

# REACHABLE

## `SPRITECACHE_SpriteCacheDestroy(0)` forgets the sprite cache's critical section, at the end of every game

**In ours**: SpriteCache.cpp:780.

`0x6FDFBBF0`, `SpriteCache.cpp`.

With bFreeCaches 0 the frees are jumped over and the whole data cache is cleared, its critical-section pointer with it:

    6FDFBBF5  test esi,esi             bFreeCaches
    6FDFBBF8  je short 6FDFBC70h       0: past FreePool, DeleteCriticalSection, FOG_Free
    ...
    6FDFBC70  mov ecx,0Fh
    6FDFBC77  mov edi,6FEE1D18h        &sgtGfxCache -- +30h is pcs
    6FDFBC7E  rep stosd

The section came from FOG_Alloc in the data cache's set-up, and only its pointer at +30h holds it:

    6FDFD921  call 6FE005B6h           FOG_Alloc (:2308)
    6FDFD926  mov [esi+30h],eax
    6FDFD92A  call dword ptr ds:[6FE0907Ch]    InitializeCriticalSection

and SpriteCacheInit releases a section only through that pointer (`mov eax, ds:[6FEE1D48h]` / `test` / `je`,
0x6FDFBB13), so after a clear it finds none, and the set-up makes a new one. **REACHABLE**: D2Client ends every game
in APPMODE_Game with CLIENT_DestroyCaches(0) --

    6FAAB91C  xor ecx,ecx
    6FAAB91E  call 6FAA14E0h           CLIENT_DestroyCaches
    6FAA14E9  push esi                 its argument, 0
    6FAA14EA  call 6FB6A84Eh           D2CMP #10053, SpriteCacheDestroy

-- and the next game's CLIENT_InitMemoryPartitions calls #10052 (0x6FAA14CA). So every game played in a session
leaves one CRITICAL_SECTION allocated and never deleted. Reproduced in `SpriteCache.cpp` (SpriteCacheDestroy's note):
ours clears the pointer the same way.

# INERT

None entered yet.

# LATENT

## `LRUCACHE_AllocEntry` links the free list's head into the active list before it asserts there was one

**In ours**: LRUCache.cpp:142.

`0x6FDF8C30`, `LRUCache.cpp`.

The take from the free list answers 0 for an empty list, and the append to the active tail stores through that 0
before the "ptBlock" assert (LRUCache.cpp:159) tests it -- twice, once per copy of the take-and-append:

    6FDF8C7B  mov eax,[esi+20h]        pFreeHead
    6FDF8C80  cmp eax,ebp
    6FDF8C82  jne short 6FDF8C88h
    6FDF8C84  xor eax,eax              an empty free list: the entry is 0
    ...
    6FDF8CA0  mov ecx,[esi+1Ch]        pActiveTail
    6FDF8CA3  cmp ecx,ebp
    6FDF8CA5  jne short 6FDF8CACh
    6FDF8CAC  mov [ecx+0Ch],eax
    6FDF8CAF  mov ecx,[esi+1Ch]
    6FDF8CB2  mov [eax+10h],ecx        pEntry->pPrev: a store at 0x10 when eax is 0
    6FDF8CB5  cmp eax,ebp
    6FDF8CB7  mov [esi+1Ch],eax
    6FDF8CBA  jne short 6FDF8D3Ah      the assert at 0x6FDF8CBC runs only after the store

The second copy, after the eviction loop, is 0x6FDF8CF9..0x6FDF8D38, its store `mov [eax+10h],ecx` at 0x6FDF8D30.

**LATENT**: it needs an empty free list with fewer active entries than entries -- the loop before the second copy
leaves only with nActive != nEntries (`cmp edx,eax` / `je 6FDF8CDAh`, 0x6FDF8CE9) -- and in a consistent cache the
free list then holds nEntries - nActive entries. Every writer of the lists keeps that: Init threads every entry onto
the free list; AllocEntry moves one free entry to the active list; EvictEntry moves one active entry to the free list
(and is handed only active ones -- the next entry); TouchEntry moves an active entry to the active tail; Destroy
evicts until nActive is 0 (call sites: Init 2, AllocEntry 2, EvictEntry 3, TouchEntry 2, Destroy 4).
Destroy leaves the free list's pointers into the array it freed, and every caller clears the 52-byte cache before
the next use -- `mov ecx,0Dh` / `rep stosd` at 0x6FDFBA72 (SpriteCacheInit), 0x6FDFBC8C (SpriteCacheDestroy) and
0x6FDFE664 (TileCacheDestroy); TileFreeAsyncLoads (#10102), whose `jmp` is the fourth, is called by D2Client's
APPMODE_Game at 0x6FAAB917 and followed by CLIENT_DestroyCaches(0) at 0x6FAAB91E, whose TileCacheDestroy clears it. A
cleared cache has dwMaxAllocated 0, and AllocEntry halts at its first test before it touches a list:

    6FDF8C38  mov eax,[esi+4]          dwMaxAllocated
    6FDF8C3B  cmp ebx,eax              dwSize
    6FDF8C3D  jb short 6FDF8C5Ah       below the budget: go on; else the :118 assert and exit

Reproduced in `LRUCache.cpp` (AllocEntry's BUG note), the store before the assert.

## `LRUCACHE_EvictEntry` and `LRUCACHE_TouchEntry` unlink an entry without asking whether it is active

**In ours**: LRUCache.cpp:226.

`0x6FDF8DC0` and `0x6FDF8EB0`, `LRUCache.cpp`.

Handed an entry that is not the active head, both compare it with the tail and rewrite its neighbours' links through
pointers they never test, and neither asks whether the entry is on the active list at all:

    6FDF8E20  mov eax,[edi+1Ch]        EvictEntry: pActiveTail
    6FDF8E23  cmp esi,eax
    6FDF8E25  jne short 6FDF8E32h
    6FDF8E27  mov eax,[eax+10h]        the tail's pPrev, untested
    6FDF8E2A  mov [edi+1Ch],eax
    6FDF8E2D  mov [eax+0Ch],ebx
    6FDF8E30  jmp short 6FDF8E44h
    6FDF8E32  mov eax,[esi+10h]        a middle entry: pPrev and pNext, untested
    6FDF8E35  mov ecx,[esi+0Ch]
    6FDF8E38  mov [eax+0Ch],ecx
    6FDF8E3B  mov edx,[esi+0Ch]
    6FDF8E3E  mov eax,[esi+10h]
    6FDF8E41  mov [edx+10h],eax

TouchEntry's unlink is the same, 0x6FDF8F13..0x6FDF8F34 (registers renamed;
32 instructions of 32).

**LATENT**: every non-NULL entry either is handed is a holder's back-pointer, and a back-pointer names an active entry
whenever nActive is not 0. The holders, and every write to them over the whole image (iced-x86 over every function
from its own start):

* the tile's ptLRUCache, +5Ch -- TileValidate `mov [esi+5Ch],eax` (0x6FDFE70E, AllocEntry's answer), OnEvict
  `mov dword ptr [esi+5Ch],0` (0x6FDFE6C5), DetachProjectTiles `mov [esi+5Ch],eax` (0x6FDFE8A2, eax 0 at 0x6FDFE89D);
  the fourth write at +5Ch, TileListAddTile's 0x6FDFEA3F, is a tile builder's pNext. A project's tiles are read from
  their file, and NormalizeProject halts on a nonzero +5Ch (`mov ebx,[eax+5Ch]` / `test` / `jne 6FDFF83Dh`,
  0x6FDFF7F4, the :931 assert); its other loader, #10072, is imported by no stock module (pefile);
* the cel-header hash entry's lpBlock, +24h -- CELDATAHASH_NewEntry 0x6FDF3DEB (its argument; its one caller,
  ResolveHeader, `push 0` at 0x6FDFC5A4), ResolveHeader `mov [esi+24h],eax` (0x6FDFC5E6, AllocEntry's answer),
  OnHeaderEvict `mov dword ptr [eax+24h],0` (0x6FDFBBD9) before it deletes the hash entry; the other +24h stores
  through a heap pointer, LoadDirectionBlockSync's and Async's (0x6FDFCF6E, 0x6FDFD5AD), write GfxHash.cpp's entries
  (eax from GFXHASH_NewEntry, 0x6FDFCD81).

Every entry carries its holder as user data (TileValidate `push esi`, 0x6FDFE700; ResolveHeader `push esi`,
0x6FDFC5D4), and EvictEntry calls the cache's callback for an entry with user data (`call dword ptr [edi+28h]`,
0x6FDF8E62) -- OnEvict and OnHeaderEvict, which clear the holder. So an entry leaves the active list only through a
callback that clears what points at it. A back-pointer outlives its entry only across a clear of the cache that
evicts nothing -- TileCacheInit's and TileCacheDestroy(0)'s `rep stosd`, SpriteCacheDestroy(0)'s -- and nActive is 0
after it, where both functions return at their first test:

    6FDF8DC9  cmp [edi+0Ch],ebx        EvictEntry: nActive
    6FDF8DCC  je near ptr 6FDF8EACh
    6FDF8EB0  mov eax,[ecx+0Ch]        TouchEntry: nActive
    6FDF8EB7  cmp eax,edi
    6FDF8EB9  je near ptr 6FDF8F59h

Before a cache counts again, no stale holder is left: SpriteCacheInit clears the hash (`call CELDATAHASH_Clear`,
0x6FDFBA5B) before it sets the header cache up (`call LRUCACHE_Init`, 0x6FDFBA94); the tile cache is set up only by
TileCacheInit, which D2Client calls once (0x6FAA1456, in CLIENT_InitMemoryPartitions) from D2Client_InitGame, called
once, by APPMODE_Game at 0x6FAAB471, whose one `ret` (0x6FAABB3F) comes after
TileFreeAsyncLoads at 0x6FAAB917 -- which evicts every entry through OnEvict -- and none of the 65 branches between
0x6FAAB471 and 0x6FAAB917 leaves past it. And for an
active entry the untested neighbours exist: one that is not the head has a predecessor, the tail arm reads only that,
and a middle entry has both.

Reproduced in `LRUCache.cpp` (EvictEntry's and TouchEntry's BUG notes).

## `LRUCACHE_Init` with no entries leaves the free tail before its array, and `LRUCACHE_AllocEntry` then never returns

**In ours**: LRUCache.cpp:52.

`0x6FDF8B30`, `LRUCache.cpp`.

Init never tests the count; with nEntries 0 the free list's tail is the entry before the (zero-byte) array:

    6FDF8BC2  mov ecx,[esi+14h]        nEntries
    6FDF8BC5  mov eax,[esi+10h]        pEntries
    6FDF8BCE  lea ecx,[ecx+ecx*4]
    6FDF8BDA  lea edx,[eax+ecx*4-14h]  pEntries - 1 when nEntries is 0
    6FDF8BE1  mov [esi+24h],edx        pFreeTail

and AllocEntry's eviction loop, whose test is nActive == nEntries, spins on 0 == 0, EvictEntry returning at once on
nActive 0:

    6FDF8CDA  xor edx,edx
    6FDF8CDC  mov ecx,esi
    6FDF8CDE  call 6FDF8DC0h           LRUCACHE_EvictEntry(pCache, NULL)
    6FDF8CE3  mov edx,[esi+0Ch]        nActive
    6FDF8CE6  mov eax,[esi+14h]        nEntries
    6FDF8CE9  cmp edx,eax
    6FDF8CEB  je short 6FDF8CDAh

**LATENT**: both callers pass 4096 -- TileCacheInit `push 1000h` (0x6FDFE611); SpriteCacheInit
gnCelHeaderCacheEntryCount (`mov ecx,ds:[6FE0F054h]`, 0x6FDFBA80), `00 10 00 00` in the image and that read its one
reference (1 instruction, 1 relocation, no writer).

Reproduced in `LRUCache.cpp` (Init's note).

## Reached code uses ten allocations' answers untested

**In ours**: CelCmp.cpp:680, CelCmp.cpp:2016, CelDataHash.cpp:65, Codec.cpp:177, GfxHash.cpp:126, LRUCache.cpp:52, SpriteCache.cpp:89.

`CelCmp.cpp`, `CelDataHash.cpp`, `Codec.cpp`, `GfxHash.cpp`, `LRUCache.cpp`, `SpriteCache.cpp`. Every call of the FOG_Alloc, FOG_AllocPool, STORM_SMemAlloc and STORM_SMemReAlloc thunks in a function
the game reaches, with the instructions after it: of 18,
eight test the answer (FindTiles.cpp's four, TileProjects.cpp's two, AllocEntry's, CellBuilderList_NewNode's, and
SetCount's SMemReAlloc) and these ten do not:

    6FDF1F49  call 6FE005B6h           CELCMP_CelFileNormalize: FOG_Alloc, then `rep stosd` through it
    6FDF3C10  call 6FE00362h           CELCMP_StormListArray_SetCount: SMemAlloc after SMemReAlloc answered NULL,
                                       the lists written into it -- at load, building the builders' table
    6FDF3DD7  call 6FE005C8h           CELDATAHASH_NewEntry: FOG_AllocPool, then `mov [esi+24h],eax`
    6FDF4253  call 6FE005C8h           CODEC_DecodeDccDirections: FOG_AllocPool
    6FDF8847  call 6FE005C8h           GFXHASH_NewEntry: FOG_AllocPool
    6FDF8B66  call 6FE005C8h           LRUCACHE_Init: FOG_AllocPool, then `rep stosd` (Init's NOT CHECKED note)
    6FDFCAFB  call 6FE005C8h           SPRITECACHE_InitializeHardwareSlots: FOG_AllocPool, then memset
    6FDFD89E  call 6FE005C8h           SPRITECACHE_SpriteDataCache_Init: FOG_AllocPool, then `rep stosd`
    6FDFD921  call 6FE005B6h           the same: FOG_Alloc, handed to InitializeCriticalSection
    6FDFDB6D  call 6FE005C8h           SPRITECACHE_SpriteDataCache_AllocateNode: FOG_AllocPool, stored as the data

**LATENT**: only a failed allocation reaches them. FOG_Alloc and FOG_AllocPool do not stop on one: each calls Fog's
error manager #10028 with code 2 when its allocator answers NULL (0x6FF58F73, 0x6FF5901F), which logs, calls the
callback the executable registered (`call eax`, 0x6FF5F258) and returns (`ret`, 0x6FF5F295), and the NULL is
returned -- the D2Client 1.10 rewrite's BUGS.md, "The palette-shift and demo units use FOG_Alloc's answer
untested", read the same path. Storm's own handling of a failed SMemAlloc was not read. This entry is the verdict of all ten. The
untested allocations of the functions the game does not reach are UNREACHABLE, below. Reproduced: every answer used
as the bytes use it.

## `PALSHIFT_LoadItemPaletteGroup` does not bound its group

**In ours**: PalShift.cpp:38.

`0x6FDFB5C0`, `PalShift.cpp`.

The group indexes the nine transform groups with no test. **LATENT**: its two callers are PalShiftInit's, with 7 and 8
(2 call sites, no immediate, no slot):

    6FDFB599  mov edx,7
    6FDFB59E  mov ecx,6FE0EF78h
    6FDFB5A3  call 6FDFB5C0h           PALSHIFT_LoadItemPaletteGroup
    6FDFB5A8  mov edx,8
    6FDFB5AD  mov ecx,6FE0EF68h
    6FDFB5B2  call 6FDFB5C0h

## `TILECACHE_GetTileCacheHits`' percentage wraps above 0x28F5C28 bytes in use

**In ours**: TileCache.cpp:12, TileCache.cpp:153.

`0x6FDFE7F0` (#10092), `TileCache.cpp`.

    6FDFE824  lea eax,[eax+eax*4]      dwAllocated * 5
    6FDFE827  lea eax,[eax+eax*4]      * 25
    6FDFE82A  shl eax,2                * 100, in 32 bits
    6FDFE82D  div ecx                  unsigned, by dwMaxAllocated

**LATENT**: the bytes in use never pass the budget -- AllocEntry evicts until dwAllocated + dwSize is within it (the
`ja` tests, 0x6FDF8C79, 0x6FDF8CF7) -- and the tile cache's one budget is D2Client's 0x600000 (`push 600000h`,
0x6FAA144A; #10089 imported by D2Client alone, pefile), so the product stays at or below 0x25800000.

## `TILEPROJECTS_LoadProjectRecord` copies the project's name into 260 bytes with no bound

**In ours**: TileProjects.cpp:10, TileProjects.cpp:51.

`0x6FE001A0` (#10095), `TileProjects.cpp`.

    6FE0030E  mov cl,[eax]             an inlined strcpy into the record's 260-byte szLibraryName
    6FE00310  mov [esi+eax],cl
    6FE00313  inc eax
    6FE00314  test cl,cl

**LATENT**: #10095 is imported by D2Client alone (pefile), which calls it at three sites in GAME_PreloadActData: with
`6FB75BB0h` and `6FB75B84h` (0x6FAA92ED, 0x6FAA92FC), 40 and 41 characters, and with a buffer
D2Common fills from LvlTypes.txt's `File N` columns (0x6FAA92D8) -- 60-byte fields (D2MOO's D2LvlTypesTxt, `char
szFile[32][60]`, copied by its DATATBLS_GetFileNameFromLevelTypeAndFileId), the longest stock value 34 characters
(`Expansion/Siege/rockclifffloor.dt1`, of the install's LvlTypes.txt, 37 rows by 32 columns).

# UNREACHABLE

## The stock game reaches 197 of D2CMP's 276 game functions; the defects in the other 79 cannot run

**In ours**: CelCmp.cpp:299, CelCmp.cpp:363, CelCmp.cpp:418, CelCmp.cpp:495, CelCmp.cpp:526, CelCmp.cpp:630, CelCmp.cpp:765, CelCmp.cpp:1668, FastCmp.cpp:48, FastCmp.cpp:104, FastCmp.cpp:170, FastCmp.cpp:205, Palette.cpp:86, Palette.cpp:354, Palette.cpp:742, Raw.cpp:40, Shared.cpp:14, SubTile.cpp:12, SubTile.cpp:23, TileLib.cpp:13, Tilecmp.cpp:18, Tilecmp.cpp:149, Tilecmp.cpp:599, Tilecmp.cpp:717.

 50 ordinals of the 107 are imported by some module of the stock install (pefile
over every .dll and .exe there; no file names D2CMP but in its import directory). Walked from those, `_DllMain@12` and
the runtime's functions over the call graph -- a table's slot followed only when a reached function reads the
table, the export table's 105 slots never -- 197 game functions are reached and 79 are not. The 79 are the tool chain: the cel compressor and converters, the palette
builders, the DC6/RGB24 encoders, the tile compiler, the subtile encoders and five unimported getters. And the image
agrees: of the 47 references to the 79 -- calls, address-takes, table slots -- every one is
from one of the 79 or from a table only they read (gpCellBuilderEncoders, gpfnCelConvert24To8, gpfnCelTransform). So these defects, real in the bytes and reproduced in ours, cannot run in the stock game:

* **CelCmp.cpp** -- a handle not in the builders' table leaves the builder NULL and is used: CelCompressAddImage hands
  0x2C on (`mov esi,[edx]`, 0x6FDF28D3, in CellEntryList_NewNode), CelCompressDestroy reads 0x34 (`mov eax,[edi+34h]`,
  0x6FDF181B), CelCompressCreateCelFile reads 0x24 (`mov ecx,[esi+24h]`, 0x6FDF1951); CreateCelFile's two error
  returns leave the lock entered (`ret 8` at 0x6FDF1960 and its twin); format 1's NULL slot is called
  (`call dword ptr [eax*4+6FE091B0h]`, 0x6FDF19D6; `[eax*4+6FE091C0h]`, 0x6FDF1AAD; `[eax*4+6FE091C8h]`, 0x6FDF2075);
  ConvertOldToNew copies the auxiliary bytes from the pointer field's own address (`lea esi,[ecx+20h]`, 0x6FDF1C52);
  Serialize frees the node block only through frame 0 (`test edx,edx`, 0x6FDF1E26) and leaves frames pointing into it;
  CellBuilderTable_New uses InternalNew's answer untested (0x6FDF35AE); FOG_Alloc's answer untested in AddImage
  (0x6FDF1786), CreateCelFile (0x6FDF1991), Convert24To8 (0x6FDF1A74), ConvertOldToNew (0x6FDF1B9C), Serialize
  (0x6FDF1D86).
* **FastCmp.cpp** -- FOG_Alloc's answer untested in EncodeIndexedCellFrames (0x6FDF5C9B), EncodeDC6RlePixels
  (0x6FDF5D7F) and ConvertRGB24CellFramesToIndexed (0x6FDF5F09, 0x6FDF5FC1), pfMapRgbToIndex called untested; fewer
  frames than directions divides by 0 (`div esi`, 0x6FDF5D11, 0x6FDF5D35); EncodeDC6RlePixels' size wraps;
  TransformDC6PaletteInPlace walks 2^32 row ends for a frame of height 0.
* **Palette.cpp** -- CreateD2Palette copies into PALETTE_VirtualAllocRWX's answer untested (`mov edx,eax`, 0x6FDF9124);
  BuildColorShiftTables' selective rows keep the grey-light values outside (45, 315) (`jne`, 0x6FDFA369; `jp`,
  0x6FDFA378) and a magnitude above 255 wraps; PaletteInit sets the start entry to 1 whatever it is asked (`mov
  ds:[6FE95400h],esi`, esi 1, 0x6FDFAD06) and its first failed open keeps gbPaletteLoaded.
* **Raw.cpp** -- FOG_Alloc's answer untested (0x6FDFB7FF); fewer frames than directions divides by 0 (`div esi`,
  0x6FDFB859, 0x6FDFB87D).
* **Shared.cpp** -- SHARED_CopyImageRows tests its row count after the first row (`dec ebx` / `jnz`, 0x6FE00401): 0
  rows copies 2^32.
* **SubTile.cpp** -- EncodeSquareRle writes FOG_Alloc's block untested (0x6FDFE06B).
* **Tilecmp.cpp** -- TileAddAddSubTile loads each three-byte pixel as a dword (`mov eax,[ecx]` / `add ecx,3`,
  0x6FDFEB1A), a byte past the region at its end; #10072 gates on `dwFlags & 3` (`test byte ptr [ebx+4],3`, 0x6FDFF699)
  where NormalizeProject gates on & 2; TileProjectIteratePixels rounds a length up to whole pixels (`dec ecx` / `mul`
  0AAAAAAABh / `shr edi,1` / `inc edi`, 0x6FDFF9E4..0x6FDFF9F0).
* **TileLib.cpp** -- the five getters no module imports (#10080 Width, #10083 TileLight, #10086 HeightToBottom, #10100
  RoofHeight, #10101 Filename) read a NULL tile after warning about it, as the seven reached ones do (NOT ESTABLISHED,
  below).

Reproduced in each file, the BUG notes there carrying this verdict.

# NOT ESTABLISHED

Each defect below is measured in the bytes; whether the game reaches it was looked at and not settled, and each says
what would settle it.

## `CELCMP_CelDrawEx` takes the decoder from the cel file without CelDraw's NULL test

**In ours**: CelCmp.cpp:887.

`0x6FDF22C0` (#10034), `CelCmp.cpp`. Imported by D2DDraw and D2Gdi (pefile).

    6FDF23A2  mov esi,[ebp+8]          the file's format, read at 0x08 of a file that may be NULL

**Would settle it**: the two renderers' calls of #10034, and whether D2Gfx hands them a context with no file.

## `CELCMP_CelGetHandle` lets a frame equal to the frames per direction through

**In ours**: CelCmp.cpp:1018.

`0x6FDF2540` (#10036), `CelCmp.cpp`. Imported by D2Client and D2Win.

    6FDF257D  mov eax,[edi+8]          the frame
    6FDF2580  mov ecx,[esi+14h]        nFramesPerDirection
    6FDF2583  cmp eax,ecx
    6FDF2585  jbe short 6FDF25A5h      equal passes: the next direction's first frame, or past the slots

**Would settle it**: every caller's frame against the file's count -- D2Client's nine sites (the D2Client 1.10 rewrite's
char.cpp, dialog.cpp, newstats.cpp, QuestLog.cpp, Restart.cpp, spellsel.cpp) and D2Win's.

## `CELCMP_CelFileFreeHardware` reads frame 0 before its loop tests every frame

**In ours**: CelCmp.cpp:1149.

`0x6FDF2750` (#10032), `CelCmp.cpp`.

    6FDF2784  mov eax,[esi+18h]        pFrames[0], untested
    6FDF2787  lea edi,[esi+18h]
    6FDF278A  mov ecx,[eax+18h]

Normalize makes every frame offset a pointer, so a normalized file's frame 0 is never NULL; a file of no frames reads
past its table. **Would settle it**: whether a cel file of 0 frames can reach #10032 (D2Client's
ARCHIVE_FreeCellFile, D2Win's).

## The cel header key is copied into 32 bytes with no bound, twice

**In ours**: CelDataHash.cpp:15, CelDataHash.cpp:65, SpriteCache.cpp:1015.

`CELDATAHASH_NewEntry` `0x6FDF3DC0` and `SPRITECACHE_ResolveHeader` `0x6FDFC530`.

    6FDF3E3A  mov dl,[eax]             NewEntry: the key into the hash entry's 32 bytes
    6FDF3E3C  mov [edi+eax],dl
    6FDFC620  mov cl,[eax]             ResolveHeader: the same key into the header's 32 bytes at +0xA0
    6FDFC622  mov [edx+eax],cl

The key is five 3-character name parts for players, monsters and objects (15 at most), and the context's name for
missiles, items and overlays. D2Client's names (GfxUtil.cpp in the D2Client 1.10 rewrite) come from 32-byte item fields and from
missiles.txt `CelFile` and overlay.txt `Filename`, 64-byte fields whose longest stock values are 30 and 31 characters
-- 32 bytes with the NUL, a margin of 0. **Would settle it**: D2Win's one call of #10055 (0x6F8A1782, its context filled
by D2Comp_A1000), not read.

## The DCC decoder bounds none of its four scratch buffers, reads before it asserts, and signs a 32-bit field from past its table

**In ours**: Codec.cpp:10, Codec.cpp:177, Codec.cpp:340, Codec.cpp:401, Codec.cpp:718.

`CODEC_DecodeDccDirections` `0x6FDF4160` and its helpers, `Codec.cpp`.

    6FDF43FA  mov edi,6FE12D20h        gaCodecFrames[256]: the frame loop, the count the file's
    6FDF476D  mov edi,6FE15918h        gaCodecCellHistory[5625]: cleared for the grid the file gives
    6FDF4775  rep stosd
    6FDF4C21  lea ecx,[esi+edx+6FE71090h]   gabCodecImage[120000]: a frame placed by the file's box
    6FDF445B  test [esi*4+6FE0B97Ch],eax    the sign bits; for 32 bits the dword after the table, a pointer

and DecodeDccCodebooks writes four bytes a cell into gabCodecCodebooks[0x40000]; the size prepass reads a dword at
every requested direction's offset before the direction count is asserted. Ours bounds nothing either and, for the
32-bit field, ORs nothing (a declared difference, Codec.cpp). **Would settle it**: a scan of every stock .dcc --
frames, grid, cells and box against the four extents, and whether any codes an offset in 32 bits.

## The DC6 decoders run 2^32 rows for nLinesToDraw 0, and the shadow ends a row on a 0x00 control

**In ours**: FastCmp.cpp:273, FastCmp.cpp:557, FastCmp.cpp:832.

`FASTCMP_DC6CopyClipped` `0x6FDF6080` and the drawers beside it, `FastCmp.cpp`.

    6FDF60AC  mov eax,[esp+1Ch]        the rows left
    6FDF60B0  dec eax                  decremented before it is tested
    6FDF60B1  mov [esp+1Ch],eax
    6FDF60B5  je short 6FDF611Ah

DrawDC6 asserts nLinesToDraw but not against 0. **Would settle it**: every caller's line count through
CelDraw/CelDrawEx (the renderers), and a scan of the stock .dc6 streams for a 0x00 control in a row the shadow passes
over.

## `PALSHIFT_PalShiftGetTransform` passes a negative colour and a negative transform

**In ours**: PalShift.cpp:81.

`0x6FDFB760` (#10098), `PalShift.cpp`. Imported by D2Common.

    6FDFB776  cmp ecx,2
    6FDFB779  jle short 6FDFB780h      signed: a negative transform goes on to the row arithmetic

and the colour is tested only `cmp edx,15h` / `jge`. D2Common sign-extends the colour at six of its thirteen call
sites (PalShift.cpp's head). **Would settle it**: whether any stock item colour reaching D2Common's six sites is 0x80
or more (the txt columns that feed them).

## The sprite data cache appends NULL before its assert, and evicts to no list without a lock

**In ours**: SpriteCache.cpp:89, SpriteCache.cpp:229.

`SPRITECACHE_SpriteDataCache_AllocateNode` `0x6FDFD940`, `_EvictNode` `0x6FDFDBA0`, `SpriteCache.cpp`.

    6FDFDA00  mov ecx,esi
    6FDFDA02  push edx
    6FDFDA03  mov edx,edi              the free head taken -- NULL when it is busy
    6FDFDA05  call 6FDFDD60h           AppendQueuedNode writes through it; the "ptBlock" assert comes after
    6FDFDC0E  call 6FDFDE60h           EvictNode: the node detached ...
    6FDFDC13  mov eax,[esi+30h]        ... and with no critical section
    6FDFDC18  jne short 6FDFDC1Fh
    6FDFDC1A  pop edi                  ... returned to no list

**Would settle it**: when the free head can be busy (the asynchronous loads' threads), and whether anything evicts
between a SpriteCacheDestroy(0) -- which leaves pcs NULL (REACHABLE, above) -- and the next SpriteCacheInit.

## `SPRITECACHE_ResolveHeader` bounds the offsets table with a product that wraps, and LoadDirectionBlockSync reads the offsets before it bounds them

**In ours**: SpriteCache.cpp:1015, SpriteCache.cpp:1330.

`0x6FDFC530`, `0x6FDFCC10`, `SpriteCache.cpp`.

    6FDFC6B6  imul eax,ecx             directions * frames, 32 bits
    6FDFC6C7  cmp eax,4000h            then `jb`: a wrapped product passes
    6FDFC74A  imul edx,eax             the index the loop reads gSpriteHeaderFileOffsets at
    6FDFCD26  mov eax,[ebx+edi*4+1Ch]  LoadDirectionBlockSync: the end direction's offset
    6FDFCD32  sub eax,[edx]            and the first's, before MAX_DIROFFSETS is asserted (0x6FDFCDFA)

Ours answers 0 past the table where the original reads beyond it (a declared difference, SpriteCache.cpp). **Would
settle it**: a scan of the stock .dc6 headers for a wrapping product, and every caller's directions against 33.

## `SPRITECACHE_SpriteValidate` hands ResolveHeader's NULL on

**In ours**: SpriteCache.cpp:1787.

`0x6FDFBD40` (#10055), `SpriteCache.cpp`. ResolveHeader answers NULL for a context with no key or no path (0x6FDFC54E,
0x6FDFC58F); SpriteValidate gives it to FindEntry, whose direction-block step reads the header for players, monsters,
missiles and kind 6. **Would settle it**: whether a context of those kinds with an empty name reaches #10055 -- a
missiles.txt row with no `CelFile`, drawn.

## `SPRITECACHE_SpriteRequest`'s fall-back queues block 0 a second time

**In ours**: SpriteCache.cpp:1923.

`0x6FDFD600` (#10056), `SpriteCache.cpp`.

    6FDFD72C  call 6FDF86A0h           GFXHASH_FindEntry: block 0 found present
    6FDFD7B7  call 6FDFD2D0h           LoadDirectionBlockAsync with dwDirBlock 0, not the later block asked

**Would settle it**: when the later block is absent and block 0 present at once -- the loads' timing.

## The tile files: a failed open leaves a tile valid, a count is the file's, and a project's open is not tested

**In ours**: TileCache.cpp:12, TileCache.cpp:92, TileCache.cpp:126, TileProjects.cpp:10, TileProjects.cpp:51.

`TILECACHE_TileValidate` `0x6FDFE6D0` (#10091), `_GetProjectAllocationSize` `0x6FDFE780`,
`TILEPROJECTS_LoadProjectRecord` `0x6FE001A0` (#10095).

    6FDFE732  call 6FE00418h           TileValidate: ARCHIVE_OpenFile after the entry is linked and zeroed;
    6FDFE737  test eax,eax             on 0 it answers 0 and the next call answers 1 over zeroed components
    6FDFE7C8  lea eax,[eax+eax*2]      GetProjectAllocationSize: the file's nTiles * 0x60 + 0x114, unchecked
    6FDFE7CB  shl eax,5
    6FE00274  call 6FE00418h           LoadProjectRecord: the open's answer dropped, the read follows

**Would settle it**: whether a .dt1 that LvlTypes.txt names can fail to open (the stock archives' listing), and the
stock .dt1 headers' nTiles.

## Seven tile getters read a NULL tile after warning about it

**In ours**: TileLib.cpp:13, TileLib.cpp:103.

`TILELIB_TileGetType` `0x6FDFFF00` (#10077) and six beside it, `TileLib.cpp`.

    6FDFFF05  test esi,esi
    6FDFFF07  jne short 6FDFFF1Dh
    6FDFFF15  call 6FE005F8h           FOG_DisplayWarning: logs and returns
    6FDFFF1D  mov eax,[esi+14h]        the load, NULL or not

**Would settle it**: whether D2Client's and D2Common's callers can hand them a NULL tile (their sources:
TileLibraryGetTiles' answers).

# REFUTED

## The archive's halt does not report the stack's bytes for a file name

**In ours**: Archive.cpp:8, Archive.cpp:43.

`ARCHIVE_SeekFile` `0x6FE0047A` and `_ReadFileExact` `0x6FE004F0`, `Archive.cpp`.

Their 260-byte name buffer (`sub esp,104h`, 0x6FE0047D) is not cleared before STORM_SFileGetFileName, whose answer is
dropped -- and it need not be: Storm writes its NUL first, before it looks the handle up.

    6FFC7295  test esi,esi             SFileGetFileName (Storm #276): the buffer
    6FFC7297  je short 6FFC72C9h
    6FFC729E  mov byte ptr [esi],0     an empty name, whatever follows

A failed lookup reports an empty name.

## `SPRITECACHE_ResolveHeader`'s file handle never reads as the key

**In ours**: SpriteCache.cpp:1015.

`0x6FDFC530`, `SpriteCache.cpp`.

The handle FOG_FOpenFile fills is the key's argument slot (`lea edx,[esp+34h]`, 0x6FDFC66B): does a failure leave
the key there? FOG_FOpenFile is SFileOpenFile (`call Storm_267`, Fog 0x6FF61602), which clears the
handle before anything else:

    6FFC7EFD  test ecx,ecx             SFileOpenFileEx: phFile
    6FFC7EFF  jne short 6FFC7F05h
    6FFC7F05  mov dword ptr [ecx],0

One failure path then stores -1 there (`mov [ecx],eax` after a CreateFileA that answered INVALID_HANDLE_VALUE,
0x6FFC844D), which the caller's `test ebx,ebx` would take for a handle; that path was not followed.

---

# LEFT IN THE SOURCE

Observations in the source that are not certainly defects, each BUG note there saying NOT ESTABLISHED -- 7:

* CelCmp.cpp: CelFileNormalize writes *ppOutFile only on its last path; both callers preset NULL (D2Client's
  CoreArchive.cpp, D2MOO's D2WinArchive.cpp), so whether that is a defect depends on a file on disk
  already flagged normalized.
* GfxHash.cpp: NewEntry leaves the special key and pCacheNode unwritten for the kinds FindEntry does not read them for;
  every other reader of +4 not looked for.
* GfxHash.cpp: DeleteEntry hands frame 0's hardware pointer to the renderer's callback and then frees it to the pool --
  a double release only if the callback frees it; the renderers' callbacks not read.
* SpriteCache.cpp: AllocateNode's floor tick, the older head tick compared signed from 0 -- the intent not known.
* SpriteCache.cpp: DetachQueuedNode leaves a busy head, and writes through a missing link only in a state a
  consistent queue does not have.
* SpriteCache.cpp: PrefetchSecondDirectionBlock and SpriteValidate leave the context's direction at the block's
  representative -- the intent not known.
* SpriteCache.cpp: LoadDirectionBlockAsync leaves the gfx node's nUnitType 0, which SpriteFindAsynchSiblings compares,
  and its size assert is reached by every kind but players and monsters -- the callers' kinds not read.
