# D2CMP 1.10

A rewrite of `D2CMP.dll` from Diablo II: Lord of Destruction 1.10 in C++, function by function, written to behave
exactly as the original does -- its bugs included.

`D2CMP.dll` is the game's graphics-compression module: the DC6 and DCC cel files (decoding, encoding, drawing), the
sprite and tile caches, the tile libraries and projects, palettes and palette shifts. Every game function of the
original DLL is rewritten here, and all 107 of its exports (ordinals 10000..10106) are implemented. The Visual C++ 6
runtime the original was linked with is not: the rewrite uses the runtime of the compiler that builds it. The built DLL
loads no part of the original `D2CMP.dll`; it calls Fog and Storm through their exports, as the original does.

## Reading the source

* `//1.10f: D2CMP.0x6FXXXXXX` above a function or a global is its address in the original DLL (image base
  0x6FDF0000).
* `BUG:` marks a defect of the original that the rewrite reproduces on purpose, with a verdict: `LATENT` (the code is
  wrong but nothing in the game triggers it), `REACHABLE`, `UNREACHABLE` or `NOT ESTABLISHED`. `BUGS.md`
  holds every defect found, each with the disassembly it rests on and its verdict.
* `DIFFERENCE:` marks a place where the rewrite does not do exactly what the original does -- where a faithful copy
  would be undefined behaviour in C++, the rewrite takes the defined program and says what differs.
* `D2MOO BUG:` marks a place where D2MOO's reconstructed source declares something the binary does not do;
  `D2MOO_BUGS.md` holds each with its evidence.
* One `.cpp` and one `.h` per source file of the original, named after the file its asserts name; `Shared.cpp` holds
  the function two units share.

## Generated headers

`D2Constants.h`, `D2Structs.h`, `D2Ptrs.h`, `D2Vars.h` and `D2Imports.h` are generated from the reverse-engineering
database the rewrite was made from, and are not edited by hand. `D2Ptrs.h` and `D2Vars.h` list every game function
and global of the original with its offset; every line is commented out, since this source implements them all.
`implib/named/` holds the import libraries (and their `.def` files) for Fog and Storm, so every call links to the
export the original imports. `D2CMP.def` gives the DLL the original's 107 exports at their ordinals.

## Building

Visual Studio 2022 or later, platform toolset v143, C++20, configuration `Release|Win32`:

    msbuild D2CMP.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=Win32

Floating-point code compiles as SSE2. The original computes on the x87 stack; under 53- and 64-bit precision control
the results are the same, and a difference only 24-bit precision would show is noted beside the code.

## Using it

The DLL is built to stand in for `D2CMP.dll` in a Diablo II 1.10 installation. Keep a copy of the original. A copy of
the game is needed; nothing of Blizzard's is in this repository.

## Credits

* [D2MOO](https://github.com/ThePhrozenKeep/D2MOO), the Phrozen Keep community's reconstruction of the 1.10 sources,
  for many enum, structure and function names.
* [Mir Drualga](https://github.com/IAmTrial), for Blizzard's own names of D2CMP's functions.
* [D2Template](https://github.com/olivier-verville/D2Template) by Olivier Verville, which `DLLmain.cpp` and `DLLmain.h`
  come from (Apache License 2.0).

## License

Apache License 2.0 -- see `LICENSE`. Diablo II is a trademark of Blizzard Entertainment; this project is not affiliated
with Blizzard.
