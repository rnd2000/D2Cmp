#include "D2CMP.h"

#include <math.h>

//  Palette.cpp -- the palettes of D2CMP.dll 1.10 (image base 0x6FDF0000), address range 0x6FDF8F60..0x6FDFB1A0. CreateD2Palette (#10000) turns a
//  256-entry palette into one 0x6C327-byte block of lookup tables (32 light levels, 16 brighten levels, three transparency blends, additive,
//  multiply, darken, hue rotations, the standard-colour tints), each entry the palette index nearest the colour it stands for. PaletteInit (#10006)
//  with #10007..#10012 is a small 24-bit-to-8-bit converter over a palette of its own, read from a .pal or a .pcx file into the module's globals. The
//  original's source file is D2CMP\SRC\Palette.cpp (the asserts in this unit name it). PALETTE_VirtualAllocRWX and PALETTE_VirtualRelease, at the end
//  of the file, lie outside that range, among the import thunks at the end of the image.

//  DIFFERENCE: CreateD2Palette, BuildColorShiftTables, HSLToRGB256 and BuildLuminanceTable compute on the x87 stack in the original and compile here
//  as SSE2. Under the 53-bit precision control the runtime sets, each x87 intermediate rounds as a double does, and the constants are the original's
//  bit for bit; under 24-bit precision control (what Direct3D leaves without D3DCREATE_FPU_PRESERVE) the original rounds to single precision and ours
//  does not, a difference this code does not reproduce.
//  Each floating-point comparison is written to take the arm the original's flag test (test ah, 41h / jp and the like) takes, a NaN included, though
//  no NaN reaches any of them: every input is a byte or a constant. The original's _ftol conversions truncate as (int) does for every value the
//  tables can produce (0..441).

//  The path literal of the asserts (the original's __FILE__).
static const char szPaletteCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2CMP\\SRC\\Palette.cpp";

static_assert(sizeof(D2CmpPaletteTables) == 0x6C327, "the block: mov ecx, 6C327h (0x6FDF911A), and #10001's size");
static_assert(sizeof(D2CmpHSL) == 0x18, "the HSL stride: add ecx, 18h (0x6FDF9322)");
static_assert(offsetof(D2CmpPaletteTables, light) == 0x400, "add edx, 400h (0x6FDF94EB)");
static_assert(offsetof(D2CmpPaletteTables, brighten) == 0x2400, "add ecx, 2400h (0x6FDF9BDD)");
static_assert(offsetof(D2CmpPaletteTables, luminance) == 0x3400, "[esi+ebp+3400h] (0x6FDFAAD1)");
static_assert(offsetof(D2CmpPaletteTables, transparency) == 0x3500, "[eax+edx+3500h] (0x6FDF93CC)");
static_assert(offsetof(D2CmpPaletteTables, additive) == 0x33500, "add ecx, 33500h (0x6FDF8F7A)");
static_assert(offsetof(D2CmpPaletteTables, multiply) == 0x43500, "add ecx, 43500h (0x6FDF95CA)");
static_assert(offsetof(D2CmpPaletteTables, hueRotate) == 0x53500, "add ecx, 53500h (0x6FDF9D4B)");
static_assert(offsetof(D2CmpPaletteTables, hueRotateDark) == 0x54D00, "add ecx, 54D00h (0x6FDF9E8E)");
static_assert(offsetof(D2CmpPaletteTables, hueRotateLight) == 0x56500, "add ecx, 56500h (0x6FDF9FF5)");
static_assert(offsetof(D2CmpPaletteTables, grayDark) == 0x57D00, "[edx+edi+57D00h] (0x6FDFA231)");
static_assert(offsetof(D2CmpPaletteTables, grayLight) == 0x57E00, "[eax+edi+57E00h] (0x6FDFA320)");
static_assert(offsetof(D2CmpPaletteTables, hueRotateSelective) == 0x57F00, "add eax, 57F00h (0x6FDFA336)");
static_assert(offsetof(D2CmpPaletteTables, fixedHue) == 0x59800, "add ecx, 59800h (0x6FDFA499)");
static_assert(offsetof(D2CmpPaletteTables, greenMagnitude) == 0x5A500, "[eax+5A500h] (0x6FDFA5BF)");
static_assert(offsetof(D2CmpPaletteTables, redMagnitude) == 0x5A500 - 0x100, "[edx-100h] (0x6FDFA676)");
static_assert(offsetof(D2CmpPaletteTables, blueMagnitude) == 0x5A500 + 0x100, "[eax+100h] (0x6FDFA740)");
static_assert(offsetof(D2CmpPaletteTables, darkBlend) == 0x5B500, "add edx, 5B500h (0x6FDF98B1)");
static_assert(offsetof(D2CmpPaletteTables, darken) == 0x6B500, "[edx+ecx+6B500h] (0x6FDF988A)");
static_assert(offsetof(D2CmpPaletteTables, standardColors) == 0x6B600, "lea edi, [ebx+6B600h] (0x6FDF9599)");
static_assert(offsetof(D2CmpPaletteTables, standardShifts) == 0x6B727, "lea eax, [ecx+6B727h] (0x6FDF9A65)");


//1.10f: D2CMP.0x6FDF8F60
//  #10003: rebuilds the additive table over whatever colours the block holds now. Two stack arguments (retn 8): the second, nUnused, is never read,
//  but the callee still pops it. void: the eax the original leaves is BuildAdditiveTable's leftover, and nothing imports it.
extern "C" void __stdcall PALETTE_UpdateD2TransparencyTable(D2CmpPaletteTables *pTables, int32_t nUnused)
{
    PALETTE_BuildAdditiveTable(pTables);
}


//1.10f: D2CMP.0x6FDF8F70
//  Fills additive[second * 256 + first] with the entry nearest the two colours added, each channel saturating at 255. The original inlines
//  PALETTE_GetClosestColor here. void: the eax the original leaves is the outer counter's 0, and neither caller reads it.
void __fastcall PALETTE_BuildAdditiveTable(D2CmpPaletteTables *pTables)
{
    for (int32_t nFirst = 0; nFirst < 256; ++nFirst)
    {
        const D2CmpPaletteEntry *pFirst = &pTables->colors[nFirst];
        for (int32_t nSecond = 0; nSecond < 256; ++nSecond)
        {
            const D2CmpPaletteEntry *pSecond = &pTables->colors[nSecond];
            int32_t nRed = pFirst->red + pSecond->red;
            int32_t nGreen = pFirst->green + pSecond->green;
            int32_t nBlue = pFirst->blue + pSecond->blue;
            pTables->additive[(nSecond << 8) + nFirst] = PALETTE_GetClosestColor(pTables->colors, 256, nRed < 0xFF ? (uint8_t)nRed : 0xFF, nGreen < 0xFF ? (uint8_t)nGreen : 0xFF, nBlue < 0xFF ? (uint8_t)nBlue : 0xFF);
        }
    }
}


//1.10f: D2CMP.0x6FDF90E0
//  #10000: builds the lookup tables for a 256-entry palette and returns the block (the flags byte of each entry is copied and never read). In order:
//  the block (VirtualAlloc, committed and so zero), the colours, each colour's HSL into a local array (asserting that L, S and H are each in range),
//  the three transparency blends (191/64, 127/128 and 63/192 of the first colour and the second, divided by 0FFh unsigned) and the 32 light levels
//  ((c * level) >> 5), each entry matched by a call to GetClosestColor; then the builders, the thirteen standard colours and the standard shifts.
//  The HSL is the usual one: L = (max + min) / 2; S = chroma / (max + min) up to L = 0.5 and chroma / (2 - max - min) above; H in sextants from the
//  channel that is the maximum (red tested first, then green), times 60, with 360 added below 0. BuildLuminanceTable later raises the L of the HSL
//  array in place, after BuildColorShiftTables has read it.
//  BUG: PALETTE_VirtualAllocRWX's result is not tested (0x6FDF9124), so on a failed allocation the next statement copies the palette to address 0.
//  UNREACHABLE: no module of the stock install imports #10000, nor #10001 (CreateD2PaletteByteStream), which calls it.
extern "C" D2CmpPaletteTables *__stdcall PALETTE_CreateD2Palette(const D2CmpPaletteEntry *pPalette)
{
    D2CmpHSL aSourceHSL[256];

    if (!pPalette)
    {
        FOG_DisplayAssert("pPal", szPaletteCpp, 683);
        exit(-1);
    }

    D2CmpPaletteTables *pTables = (D2CmpPaletteTables *)PALETTE_VirtualAllocRWX(sizeof(D2CmpPaletteTables));
    memcpy(pTables->colors, pPalette, sizeof(pTables->colors));

    for (uint32_t nColor = 0; nColor < 256; ++nColor)
    {
        const D2CmpPaletteEntry *pRGB = &pTables->colors[nColor];
        D2CmpHSL *pHSL = &aSourceHSL[nColor];
        double dRed = (double)pRGB->red * gdPaletteInv255;
        double dGreen = (double)pRGB->green * gdPaletteInv255;
        double dBlue = (double)pRGB->blue * gdPaletteInv255;

        double dMax = dGreen > dBlue ? dGreen : dBlue;
        if (dRed > dMax)
        {
            dMax = dRed;
        }
        double dMin = dGreen < dBlue ? dGreen : dBlue;
        if (dRed < dMin)
        {
            dMin = dRed;
        }

        double dMaxPlusMin = dMax + dMin;
        double dLum = dMaxPlusMin * gdPaletteHalf;
        if (!(dLum >= 0.0f && dLum <= 1.0f))
        {
            FOG_DisplayAssert("dLum >= 0.0f && dLum <= 1.0f", szPaletteCpp, 63);
            exit(-1);
        }

        if (dMax == dMin)
        {
            pHSL->hue = 0.0;
            pHSL->saturation = 0.0;
        }
        else
        {
            double dChroma = dMax - dMin;
            double dSat = dLum <= gdPaletteHalf ? dChroma / dMaxPlusMin : dChroma / (gdPaletteTwo - dMaxPlusMin);
            if (!(dSat >= 0.0f && dSat <= 1.0f))
            {
                FOG_DisplayAssert("dSat >= 0.0f && dSat <= 1.0f", szPaletteCpp, 77);
                exit(-1);
            }

            double dHue;
            if (dRed == dMax)
            {
                dHue = (dGreen - dBlue) / dChroma;
            }
            else if (dGreen == dMax)
            {
                dHue = (dBlue - dRed) / dChroma + gdPaletteTwo;
            }
            else
            {
                dHue = (dRed - dGreen) / dChroma + gdPaletteBlueHueOffset;
            }
            dHue = dHue * gdPaletteHueSextant;
            if (dHue < gdPaletteZero)
            {
                dHue = dHue + gdPaletteHueCircle;
            }
            if (!(dHue >= 0.0f && dHue < 360.0f))
            {
                FOG_DisplayAssert("dHue >= 0.0f && dHue < 360.0f", szPaletteCpp, 92);
                exit(-1);
            }
            pHSL->hue = dHue;
            pHSL->saturation = dSat;
        }
        pHSL->luminance = dLum;
    }

    for (uint32_t nTable = 0; nTable < 3; ++nTable)
    {
        int32_t nFirstWeight;
        switch (nTable)
        {
        case 0:
            nFirstWeight = 0xBF;
            break;
        case 1:
            nFirstWeight = 0x7F;
            break;
        default:
            nFirstWeight = 0x3F;
            break;
        }
        int32_t nSecondWeight = 0xFF - nFirstWeight;
        uint8_t *pTable = &pTables->transparency[nTable << 16];
        for (int32_t nFirst = 0; nFirst < 256; ++nFirst)
        {
            const D2CmpPaletteEntry *pFirst = &pTables->colors[nFirst];
            for (int32_t nSecond = 0; nSecond < 256; ++nSecond)
            {
                const D2CmpPaletteEntry *pSecond = &pTables->colors[nSecond];
                uint8_t nRed = (uint8_t)((uint32_t)(pSecond->red * nSecondWeight + pFirst->red * nFirstWeight) / 0xFF);
                uint8_t nGreen = (uint8_t)((uint32_t)(pSecond->green * nSecondWeight + pFirst->green * nFirstWeight) / 0xFF);
                uint8_t nBlue = (uint8_t)((uint32_t)(pFirst->blue * nFirstWeight + pSecond->blue * nSecondWeight) / 0xFF);
                pTable[(nSecond << 8) + nFirst] = PALETTE_GetClosestColor(pTables->colors, 256, nRed, nGreen, nBlue);
            }
        }
    }

    for (uint32_t nLevel = 1; nLevel <= 32; ++nLevel)
    {
        uint8_t *pRow = &pTables->light[(nLevel - 1) << 8];
        for (uint32_t nColor = 0; nColor < 256; ++nColor)
        {
            const D2CmpPaletteEntry *pRGB = &pTables->colors[nColor];
            pRow[nColor] = PALETTE_GetClosestColor(pTables->colors, 256, (nLevel * pRGB->red) >> 5, (nLevel * pRGB->green) >> 5, (nLevel * pRGB->blue) >> 5);
        }
    }

    PALETTE_BuildBrightenTables(pTables);
    PALETTE_BuildAdditiveTable(pTables);
    PALETTE_BuildMultiplyTable(pTables);
    PALETTE_BuildColorShiftTables(pTables, aSourceHSL);
    PALETTE_BuildLuminanceTable(pTables, aSourceHSL);
    PALETTE_BuildDarkenAndDarkBlend(pTables);
    memcpy(pTables->standardColors, gaPaletteStandardColors, sizeof(pTables->standardColors));
    PALETTE_BuildStandardShifts(pTables);
    return pTables;
}


//1.10f: D2CMP.0x6FDF95C0
//  Fills multiply[second * 256 + first] with the entry nearest the two colours multiplied, each channel c1 * c2 / 0FFh, unsigned. The original
//  inlines PALETTE_GetClosestColor here. void: the eax the original leaves is the outer counter's 0, and no caller reads it.
void __fastcall PALETTE_BuildMultiplyTable(D2CmpPaletteTables *pTables)
{
    for (int32_t nFirst = 0; nFirst < 256; ++nFirst)
    {
        const D2CmpPaletteEntry *pFirst = &pTables->colors[nFirst];
        for (int32_t nSecond = 0; nSecond < 256; ++nSecond)
        {
            const D2CmpPaletteEntry *pSecond = &pTables->colors[nSecond];
            uint8_t nRed = (uint8_t)((uint32_t)(pSecond->red * pFirst->red) / 0xFF);
            uint8_t nGreen = (uint8_t)((uint32_t)(pSecond->green * pFirst->green) / 0xFF);
            uint8_t nBlue = (uint8_t)((uint32_t)(pSecond->blue * pFirst->blue) / 0xFF);
            pTables->multiply[(nSecond << 8) + nFirst] = PALETTE_GetClosestColor(pTables->colors, 256, nRed, nGreen, nBlue);
        }
    }
}


//1.10f: D2CMP.0x6FDF9740
//  Fills two tables. darken[c]: each non-zero channel of colour c becomes c + c / -3 (a signed division), that is c - c / 3 as a byte, and a zero
//  channel is left as it is; then the nearest entry. darkBlend[second * 256 + first]: the first colour weighted by its brightest channel (red against
//  green with a tie to green, that against blue with a tie to blue), the second by 255 less that, divided by 0FFh unsigned, then the nearest entry.
//  The original inlines PALETTE_GetClosestColor here. void: the eax the original leaves is the outer counter's 0, and no caller reads it.
void __fastcall PALETTE_BuildDarkenAndDarkBlend(D2CmpPaletteTables *pTables)
{
    for (uint32_t nColor = 0; nColor < 256; ++nColor)
    {
        const D2CmpPaletteEntry *pRGB = &pTables->colors[nColor];
        uint8_t nRed = pRGB->red;
        uint8_t nGreen = pRGB->green;
        uint8_t nBlue = pRGB->blue;
        if (nRed)
        {
            nRed += nRed / -3;
        }
        if (nBlue)
        {
            nBlue += nBlue / -3;
        }
        if (nGreen)
        {
            nGreen += nGreen / -3;
        }
        pTables->darken[nColor] = PALETTE_GetClosestColor(pTables->colors, 256, nRed, nGreen, nBlue);
    }

    for (int32_t nFirst = 0; nFirst < 256; ++nFirst)
    {
        const D2CmpPaletteEntry *pFirst = &pTables->colors[nFirst];
        int32_t nMaxRedGreen = pFirst->red;
        if (nMaxRedGreen <= pFirst->green)
        {
            nMaxRedGreen = pFirst->green;
        }
        int32_t nFirstWeight = nMaxRedGreen > pFirst->blue ? nMaxRedGreen : pFirst->blue;
        int32_t nSecondWeight = 0xFF - nFirstWeight;
        for (int32_t nSecond = 0; nSecond < 256; ++nSecond)
        {
            const D2CmpPaletteEntry *pSecond = &pTables->colors[nSecond];
            uint8_t nRed = (uint8_t)((uint32_t)(pSecond->red * nSecondWeight + pFirst->red * nFirstWeight) / 0xFF);
            uint8_t nGreen = (uint8_t)((uint32_t)(pSecond->green * nSecondWeight + pFirst->green * nFirstWeight) / 0xFF);
            uint8_t nBlue = (uint8_t)((uint32_t)(pSecond->blue * nSecondWeight + pFirst->blue * nFirstWeight) / 0xFF);
            pTables->darkBlend[(nSecond << 8) + nFirst] = PALETTE_GetClosestColor(pTables->colors, 256, nRed, nGreen, nBlue);
        }
    }
}


//1.10f: D2CMP.0x6FDF9A60
//  Fills standardShifts[k * 256 + c], k = 0..11: the red byte of colour c alone as an intensity, tinted by standard colour k + 1 (intensity * tint /
//  0FFh per channel, signed), then the nearest entry. Standard colour 0 has no row. The original inlines PALETTE_GetClosestColor here. void: the eax
//  the original leaves is the colour counter, and no caller reads it.
void __fastcall PALETTE_BuildStandardShifts(D2CmpPaletteTables *pTables)
{
    for (int32_t nShift = 0; nShift < 12; ++nShift)
    {
        const D2CmpRGB24 *pTint = &pTables->standardColors[nShift + 1];
        uint8_t *pRow = &pTables->standardShifts[nShift << 8];
        for (int32_t nColor = 0; nColor < 256; ++nColor)
        {
            int32_t nIntensity = pTables->colors[nColor].red;
            uint8_t nRed = (uint8_t)(pTint->red * nIntensity / 0xFF);
            uint8_t nGreen = (uint8_t)(pTint->green * nIntensity / 0xFF);
            uint8_t nBlue = (uint8_t)(pTint->blue * nIntensity / 0xFF);
            pRow[nColor] = PALETTE_GetClosestColor(pTables->colors, 256, nRed, nGreen, nBlue);
        }
    }
}


//1.10f: D2CMP.0x6FDF9BD0
//  Fills brighten[(level - 1) * 256 + c], level = 1..16: each channel raised by ((255 - c) * level) >> 4 (a logical shift; the target is an int and
//  is never narrowed, as it cannot pass 255), then the nearest entry. The original inlines PALETTE_GetClosestColor here. void: the eax the original
//  leaves is the level, 16, and no caller reads it.
void __fastcall PALETTE_BuildBrightenTables(D2CmpPaletteTables *pTables)
{
    for (uint32_t nLevel = 1; nLevel <= 16; ++nLevel)
    {
        uint8_t *pRow = &pTables->brighten[(nLevel - 1) << 8];
        for (uint32_t nColor = 0; nColor < 256; ++nColor)
        {
            const D2CmpPaletteEntry *pRGB = &pTables->colors[nColor];
            pRow[nColor] = PALETTE_GetClosestColor(pTables->colors, 256, (int32_t)(((0xFF - pRGB->red) * nLevel) >> 4) + pRGB->red, (int32_t)(((0xFF - pRGB->green) * nLevel) >> 4) + pRGB->green, (int32_t)(((0xFF - pRGB->blue) * nLevel) >> 4) + pRGB->blue);
        }
    }
}


//1.10f: D2CMP.0x6FDF9D20
//  Builds the colour-shift rows from the source HSL array (pSourceHSL), through one working HSL array and one RGB array that run on from stage to
//  stage:
//
//      hueRotate[k], k = 0..23     H + 15k, less 360 above 360
//      hueRotateDark[k]            the same, S = 0.5, L - 0.1f, 0 below 0
//      hueRotateLight[k]           the same, S = 0.5, L + 0.2f, 1 above 1
//      grayDark                    S = 0, L * 0.5
//      grayLight                   S = 0, L * gdPaletteGrayLightScale + gdPaletteGrayLightBias
//      hueRotateSelective[k]       H + 15k for a source H in (45, 315); any other entry keeps what the stage before left in the working array
//      fixedHue[k], k = 0..11      H = 30k, S = 1
//      red/green/blueMagnitude     |colour| on one channel
//
//  each converted by HSLToRGB256 and each RGB matched by the nearest entry. The original inlines PALETTE_GetClosestColor here. The hue steps (15.0f,
//  30.0f) are floats and the pass counter is an unsigned int; a hue wraps when it is above 360; the magnitude is (BYTE)_ftol(fsqrt(r*r + b*b + g*g)),
//  0 to 441, narrowed to its low byte. void: the eax the original leaves, the end of greenMagnitude, is read by no caller.
//  BUG: a source entry whose hue is outside (45, 315), the reds and every grey (hue 0), is not copied into the working array in the
//  hueRotateSelective stage (the skips at 0x6FDFA369 and 0x6FDFA378), so it is converted from the values the grayLight stage left there, not from its
//  own colour. UNREACHABLE: only CreateD2Palette calls this function, and no module of the stock install imports #10000.
//  BUG: a magnitude above 255 wraps in the byte (white's 441 becomes 185). UNREACHABLE: only CreateD2Palette calls this function, and no module of
//  the stock install imports #10000.
void __fastcall PALETTE_BuildColorShiftTables(D2CmpPaletteTables *pTables, const D2CmpHSL *pSourceHSL)
{
    D2CmpPaletteEntry aWorkingRGB[256];
    D2CmpHSL aWorkingHSL[256];

    for (uint32_t nPass = 0; nPass < 24; ++nPass)
    {
        for (int32_t nColor = 0; nColor < 256; ++nColor)
        {
            D2CmpHSL *pHSL = &aWorkingHSL[nColor];
            *pHSL = pSourceHSL[nColor];
            pHSL->hue = (double)nPass * gfPaletteHueStep + pHSL->hue;
            if (pHSL->hue > gdPaletteHueCircle)
            {
                pHSL->hue = pHSL->hue - gdPaletteHueCircle;
            }
        }
        PALETTE_HSLToRGB256(aWorkingHSL, aWorkingRGB);
        for (uint32_t nColor = 0; nColor < 256; ++nColor)
        {
            pTables->hueRotate[(nPass << 8) + nColor] = PALETTE_GetClosestColor(pTables->colors, 256, aWorkingRGB[nColor].red, aWorkingRGB[nColor].green, aWorkingRGB[nColor].blue);
        }
    }

    for (uint32_t nPass = 0; nPass < 24; ++nPass)
    {
        for (int32_t nColor = 0; nColor < 256; ++nColor)
        {
            D2CmpHSL *pHSL = &aWorkingHSL[nColor];
            *pHSL = pSourceHSL[nColor];
            pHSL->hue = (double)nPass * gfPaletteHueStep + pHSL->hue;
            if (pHSL->hue > gdPaletteHueCircle)
            {
                pHSL->hue = pHSL->hue - gdPaletteHueCircle;
            }
            pHSL->saturation = 0.5;
            pHSL->luminance = pHSL->luminance - gdPaletteLuminanceReduction;
            if (pHSL->luminance < gdPaletteZero)
            {
                pHSL->luminance = 0.0;
            }
        }
        PALETTE_HSLToRGB256(aWorkingHSL, aWorkingRGB);
        for (uint32_t nColor = 0; nColor < 256; ++nColor)
        {
            pTables->hueRotateDark[(nPass << 8) + nColor] = PALETTE_GetClosestColor(pTables->colors, 256, aWorkingRGB[nColor].red, aWorkingRGB[nColor].green, aWorkingRGB[nColor].blue);
        }
    }

    for (uint32_t nPass = 0; nPass < 24; ++nPass)
    {
        for (int32_t nColor = 0; nColor < 256; ++nColor)
        {
            D2CmpHSL *pHSL = &aWorkingHSL[nColor];
            *pHSL = pSourceHSL[nColor];
            pHSL->hue = (double)nPass * gfPaletteHueStep + pHSL->hue;
            if (pHSL->hue > gdPaletteHueCircle)
            {
                pHSL->hue = pHSL->hue - gdPaletteHueCircle;
            }
            pHSL->saturation = 0.5;
            pHSL->luminance = pHSL->luminance + gdPaletteLuminanceBoost;
            if (pHSL->luminance > gdPaletteOne)
            {
                pHSL->luminance = 1.0;
            }
        }
        PALETTE_HSLToRGB256(aWorkingHSL, aWorkingRGB);
        for (uint32_t nColor = 0; nColor < 256; ++nColor)
        {
            pTables->hueRotateLight[(nPass << 8) + nColor] = PALETTE_GetClosestColor(pTables->colors, 256, aWorkingRGB[nColor].red, aWorkingRGB[nColor].green, aWorkingRGB[nColor].blue);
        }
    }

    for (int32_t nColor = 0; nColor < 256; ++nColor)
    {
        D2CmpHSL *pHSL = &aWorkingHSL[nColor];
        *pHSL = pSourceHSL[nColor];
        pHSL->luminance = pHSL->luminance * gdPaletteHalf;
        pHSL->saturation = 0.0;
    }
    PALETTE_HSLToRGB256(aWorkingHSL, aWorkingRGB);
    for (uint32_t nColor = 0; nColor < 256; ++nColor)
    {
        pTables->grayDark[nColor] = PALETTE_GetClosestColor(pTables->colors, 256, aWorkingRGB[nColor].red, aWorkingRGB[nColor].green, aWorkingRGB[nColor].blue);
    }

    for (int32_t nColor = 0; nColor < 256; ++nColor)
    {
        D2CmpHSL *pHSL = &aWorkingHSL[nColor];
        *pHSL = pSourceHSL[nColor];
        pHSL->luminance = pHSL->luminance * gdPaletteGrayLightScale + gdPaletteGrayLightBias;
        pHSL->saturation = 0.0;
    }
    PALETTE_HSLToRGB256(aWorkingHSL, aWorkingRGB);
    for (uint32_t nColor = 0; nColor < 256; ++nColor)
    {
        pTables->grayLight[nColor] = PALETTE_GetClosestColor(pTables->colors, 256, aWorkingRGB[nColor].red, aWorkingRGB[nColor].green, aWorkingRGB[nColor].blue);
    }

    for (uint32_t nPass = 0; nPass < 24; ++nPass)
    {
        for (int32_t nColor = 0; nColor < 256; ++nColor)
        {
            if (pSourceHSL[nColor].hue > gdPaletteSelectiveHueMin && pSourceHSL[nColor].hue < gdPaletteSelectiveHueMax)
            {
                D2CmpHSL *pHSL = &aWorkingHSL[nColor];
                *pHSL = pSourceHSL[nColor];
                pHSL->hue = (double)nPass * gfPaletteHueStep + pHSL->hue;
                if (pHSL->hue > gdPaletteHueCircle)
                {
                    pHSL->hue = pHSL->hue - gdPaletteHueCircle;
                }
            }
        }
        PALETTE_HSLToRGB256(aWorkingHSL, aWorkingRGB);
        for (uint32_t nColor = 0; nColor < 256; ++nColor)
        {
            pTables->hueRotateSelective[(nPass << 8) + nColor] = PALETTE_GetClosestColor(pTables->colors, 256, aWorkingRGB[nColor].red, aWorkingRGB[nColor].green, aWorkingRGB[nColor].blue);
        }
    }

    for (uint32_t nPass = 0; nPass < 12; ++nPass)
    {
        for (int32_t nColor = 0; nColor < 256; ++nColor)
        {
            D2CmpHSL *pHSL = &aWorkingHSL[nColor];
            *pHSL = pSourceHSL[nColor];
            pHSL->hue = (double)nPass * gfPaletteFixedHueStep;
            pHSL->saturation = 1.0;
        }
        PALETTE_HSLToRGB256(aWorkingHSL, aWorkingRGB);
        for (uint32_t nColor = 0; nColor < 256; ++nColor)
        {
            pTables->fixedHue[(nPass << 8) + nColor] = PALETTE_GetClosestColor(pTables->colors, 256, aWorkingRGB[nColor].red, aWorkingRGB[nColor].green, aWorkingRGB[nColor].blue);
        }
    }

    for (uint32_t nColor = 0; nColor < 256; ++nColor)
    {
        const D2CmpPaletteEntry *pRGB = &pTables->colors[nColor];
        uint8_t nMagnitude = (uint8_t)(int32_t)sqrt((double)(pRGB->red * pRGB->red + pRGB->blue * pRGB->blue + pRGB->green * pRGB->green));
        pTables->redMagnitude[nColor] = PALETTE_GetClosestColor(pTables->colors, 256, nMagnitude, 0, 0);
        pTables->greenMagnitude[nColor] = PALETTE_GetClosestColor(pTables->colors, 256, 0, nMagnitude, 0);
        pTables->blueMagnitude[nColor] = PALETTE_GetClosestColor(pTables->colors, 256, 0, 0, nMagnitude);
    }
}


//1.10f: D2CMP.0x6FDFA770
//  Converts 256 HSL entries to RGB, leaving the flags byte of each untouched. S == 0 is grey: _ftol(L * 255) on all three channels (a NaN S takes the
//  colour arm). Otherwise high = (S + 1) * L up to L = 0.5 and L + S - L * S above, low = 2L - high, and each channel comes from a hue (H + 120 for
//  red, H for green, H - 120 for blue; 360 is taken off above 360 and added below 0): low + (high - low) * h / 60 below 60, high below 180, low +
//  (high - low) * (240 - h) / 60 below 240, low from there on; the "/ 60" is a multiply by gdPaletteInv60. void: the eax the original leaves is the
//  counter's 0, and no caller reads it.
//  The original inlines a hue-to-channel computation here, once per channel; it has no out-of-line copy, so the code writes it out three times.
void __fastcall PALETTE_HSLToRGB256(const D2CmpHSL *pHSL, D2CmpPaletteEntry *pRGB)
{
    for (int32_t nColorsLeft = 256; nColorsLeft != 0; --nColorsLeft, ++pHSL, ++pRGB)
    {
        if (pHSL->saturation == gdPaletteZero)
        {
            uint8_t nGray = (uint8_t)(int32_t)(pHSL->luminance * gdPaletteChannelScale255);
            pRGB->blue = nGray;
            pRGB->red = nGray;
            pRGB->green = nGray;
            continue;
        }

        double dHigh;
        if (pHSL->luminance <= gdPaletteHalf)
        {
            dHigh = (pHSL->saturation + gdPaletteOne) * pHSL->luminance;
        }
        else
        {
            dHigh = pHSL->luminance + pHSL->saturation - pHSL->luminance * pHSL->saturation;
        }
        double dLow = pHSL->luminance + pHSL->luminance - dHigh;
        double dHue;
        double dValue;

        dHue = pHSL->hue + gdPaletteChannelHueOffset;
        if (dHue > gdPaletteHueCircle)
        {
            dHue = dHue - gdPaletteHueCircle;
        }
        if (dHue < gdPaletteZero)
        {
            dHue = dHue + gdPaletteHueCircle;
        }
        if (dHue < gdPaletteHueSextant)
        {
            dValue = (dHigh - dLow) * dHue * gdPaletteInv60 + dLow;
        }
        else if (dHue < gdPaletteHueGreenBoundary)
        {
            dValue = dHigh;
        }
        else if (dHue < gdPaletteHueBlueBoundary)
        {
            dValue = (dHigh - dLow) * (gdPaletteHueBlueBoundary - dHue) * gdPaletteInv60 + dLow;
        }
        else
        {
            dValue = dLow;
        }
        uint8_t nRed = (uint8_t)(int32_t)(dValue * gdPaletteChannelScale255);

        dHue = pHSL->hue;
        if (dHue > gdPaletteHueCircle)
        {
            dHue = dHue - gdPaletteHueCircle;
        }
        if (dHue < gdPaletteZero)
        {
            dHue = dHue + gdPaletteHueCircle;
        }
        if (dHue < gdPaletteHueSextant)
        {
            dValue = (dHigh - dLow) * dHue * gdPaletteInv60 + dLow;
        }
        else if (dHue < gdPaletteHueGreenBoundary)
        {
            dValue = dHigh;
        }
        else if (dHue < gdPaletteHueBlueBoundary)
        {
            dValue = (dHigh - dLow) * (gdPaletteHueBlueBoundary - dHue) * gdPaletteInv60 + dLow;
        }
        else
        {
            dValue = dLow;
        }
        uint8_t nGreen = (uint8_t)(int32_t)(dValue * gdPaletteChannelScale255);

        dHue = pHSL->hue - gdPaletteChannelHueOffset;
        if (dHue > gdPaletteHueCircle)
        {
            dHue = dHue - gdPaletteHueCircle;
        }
        if (dHue < gdPaletteZero)
        {
            dHue = dHue + gdPaletteHueCircle;
        }
        if (dHue < gdPaletteHueSextant)
        {
            dValue = (dHigh - dLow) * dHue * gdPaletteInv60 + dLow;
        }
        else if (dHue < gdPaletteHueGreenBoundary)
        {
            dValue = dHigh;
        }
        else if (dHue < gdPaletteHueBlueBoundary)
        {
            dValue = (dHigh - dLow) * (gdPaletteHueBlueBoundary - dHue) * gdPaletteInv60 + dLow;
        }
        else
        {
            dValue = dLow;
        }
        uint8_t nBlue = (uint8_t)(int32_t)(dValue * gdPaletteChannelScale255);

        pRGB->blue = nBlue;
        pRGB->red = nRed;
        pRGB->green = nGreen;
    }
}


//1.10f: D2CMP.0x6FDFA9E0
//  Fills luminance[c] with the entry nearest colour c with its L raised by 0.2f and capped at 1; a zero L is left at 0. The raise is made in the
//  caller's HSL array in place, and the array is then converted by HSLToRGB256. The original inlines PALETTE_GetClosestColor here. void: the eax the
//  original leaves is the candidate counter's 256, and no caller reads it.
void __fastcall PALETTE_BuildLuminanceTable(D2CmpPaletteTables *pTables, D2CmpHSL *pHSL)
{
    D2CmpPaletteEntry aBoostedRGB[256];

    for (int32_t nColor = 0; nColor < 256; ++nColor)
    {
        if (pHSL[nColor].luminance != gdPaletteZero)
        {
            pHSL[nColor].luminance = pHSL[nColor].luminance + gdPaletteLuminanceBoost;
            if (pHSL[nColor].luminance > gdPaletteOne)
            {
                pHSL[nColor].luminance = 1.0;
            }
        }
    }
    PALETTE_HSLToRGB256(pHSL, aBoostedRGB);
    for (uint32_t nColor = 0; nColor < 256; ++nColor)
    {
        pTables->luminance[nColor] = PALETTE_GetClosestColor(pTables->colors, 256, aBoostedRGB[nColor].red, aBoostedRGB[nColor].green, aBoostedRGB[nColor].blue);
    }
}


//1.10f: D2CMP.0x6FDFAAF0
//  #10001: CreateD2Palette, with the block's size, 0x6C327, stored through the second argument first. Returns the block.
extern "C" D2CmpPaletteTables *__stdcall PALETTE_CreateD2PaletteByteStream(const D2CmpPaletteEntry *pPalette, uint32_t *pdwSize)
{
    *pdwSize = sizeof(D2CmpPaletteTables);
    return PALETTE_CreateD2Palette(pPalette);
}


//1.10f: D2CMP.0x6FDFAB10
//  #10002: releases the block, if there is one. void: the eax the original leaves is PALETTE_VirtualRelease's answer (for NULL, whatever eax held),
//  and nothing imports it.
extern "C" void __stdcall PALETTE_DestroyD2Palette(D2CmpPaletteTables *pTables)
{
    if (pTables)
    {
        PALETTE_VirtualRelease(pTables);
    }
}


//1.10f: D2CMP.0x6FDFAB20
//  #10004: the index of the entry of pPalette nearest (nRed, nGreen, nBlue): the least sum of the three squared differences, the first entry on a
//  tie; 0 for no entries. The channel arguments are ints read whole, the differences are taken in 32 bits and the count is compared unsigned. The
//  answer is AL alone, the index's low byte; the rest of eax is the count's.
//  D2MOO BUG: D2MOO's GetNearestPaletteIndex returns int and takes an int count; the binary answers in AL alone and compares the count unsigned.
//  The squared differences are computed in unsigned arithmetic, which wraps in 32 bits as the original's imul does for any argument;
//  signed C would leave an overflow undefined.
extern "C" uint8_t __stdcall PALETTE_GetClosestColor(const D2CmpPaletteEntry *pPalette, uint32_t nColors, int32_t nRed, int32_t nGreen, int32_t nBlue)
{
    uint8_t nBestIndex = 0;
    uint32_t nBestDistance = 0xFFFFFFFF;
    for (uint32_t nColor = 0; nColor < nColors; ++nColor)
    {
        uint32_t nDeltaRed = (uint32_t)pPalette[nColor].red - (uint32_t)nRed;
        uint32_t nDeltaGreen = (uint32_t)pPalette[nColor].green - (uint32_t)nGreen;
        uint32_t nDeltaBlue = (uint32_t)pPalette[nColor].blue - (uint32_t)nBlue;
        uint32_t nDistance = nDeltaBlue * nDeltaBlue + nDeltaGreen * nDeltaGreen + nDeltaRed * nDeltaRed;
        if (nDistance < nBestDistance)
        {
            nBestIndex = (uint8_t)nColor;
            nBestDistance = nDistance;
        }
    }
    return nBestIndex;
}


//1.10f: D2CMP.0x6FDFABA0
//  #10005: GetClosestColor's body with the best distance starting at 0 and replaced only by a greater one: the first entry furthest from the colour;
//  0 for no entries, or when every distance is 0.
extern "C" uint8_t __stdcall PALETTE_GetFurthestColor(const D2CmpPaletteEntry *pPalette, uint32_t nColors, int32_t nRed, int32_t nGreen, int32_t nBlue)
{
    uint8_t nBestIndex = 0;
    uint32_t nBestDistance = 0;
    for (uint32_t nColor = 0; nColor < nColors; ++nColor)
    {
        uint32_t nDeltaRed = (uint32_t)pPalette[nColor].red - (uint32_t)nRed;
        uint32_t nDeltaGreen = (uint32_t)pPalette[nColor].green - (uint32_t)nGreen;
        uint32_t nDeltaBlue = (uint32_t)pPalette[nColor].blue - (uint32_t)nBlue;
        uint32_t nDistance = nDeltaBlue * nDeltaBlue + nDeltaGreen * nDeltaGreen + nDeltaRed * nDeltaRed;
        if (nDistance > nBestDistance)
        {
            nBestIndex = (uint8_t)nColor;
            nBestDistance = nDistance;
        }
    }
    return nBestIndex;
}


//1.10f: D2CMP.0x6FDFAC20
//  #10009: sets the transparent colour: the argument's low three bytes reversed (byte 0 into bits 16..23, byte 1 kept, byte 2 into bits 0..7), which
//  is Palette24BitTo8BitPixel's packing of (nBlue, nGreen, nRed). void: the eax the original leaves is the value stored, and nothing imports it.
extern "C" void __stdcall PALETTE_PaletteSetTransparency(uint32_t dwColor)
{
    gdwPaletteTransparentColor = ((dwColor & 0xFF) << 16) | (((dwColor >> 8) & 0xFF) << 8) | ((dwColor >> 16) & 0xFF);
}


//1.10f: D2CMP.0x6FDFAC50
//  #10006: sets up the converter's palette. It asserts that pszPalette is not NULL and that 0 <= nPaletteStartEntry <= nPaletteEndEntry < 256 (signed
//  compares), stores the end entry, sets the start entry to 1, fills the squares table and clears the transparent colour. An empty name gives a grey
//  ramp (entry c = (c, c, c)). Otherwise a file of exactly 768 bytes (GetFileSize) is a .pal, three bytes an entry, blue first (byte 2 goes to red);
//  any other file is reopened as a 256-colour .pcx: a 128-byte header whose bytes 0, 1 and 3 are 0Ah, 5 and 8, and its last 769 bytes a 0Ch followed
//  by red, green, blue for each entry. Returns gbPaletteLoaded's new value, 1 with a palette and 0 without, except that a first open that fails
//  returns 0 and leaves gbPaletteLoaded as it was.
//  BUG: nPaletteStartEntry is asserted and then not used: gnPaletteStartEntry is set to 1 whatever it is (0x6FDFAD06), so Palette24BitTo8BitPixel
//  never chooses entry 0. UNREACHABLE: no module of the stock install imports #10006.
//  BUG: the first failed open keeps the previous gbPaletteLoaded (0x6FDFAD6F stores nothing), where the second clears it. UNREACHABLE: no module of
//  the stock install imports #10006.
extern "C" int32_t __stdcall PALETTE_PaletteInit(const char *pszPalette, int32_t nPaletteStartEntry, int32_t nPaletteEndEntry)
{
    if (!pszPalette)
    {
        FOG_DisplayAssert("pszPalette != NULL", szPaletteCpp, 950);
        exit(-1);
    }
    if (nPaletteStartEntry > nPaletteEndEntry)
    {
        FOG_DisplayAssert("PaletteStartEntry <= PaletteEndEntry", szPaletteCpp, 951);
        exit(-1);
    }
    if (nPaletteStartEntry < 0)
    {
        FOG_DisplayAssert("PaletteStartEntry >= 0", szPaletteCpp, 952);
        exit(-1);
    }
    if (nPaletteEndEntry >= 256)
    {
        FOG_DisplayAssert("PaletteEndEntry < NUMBER_PALETTE_ENTRIES", szPaletteCpp, 953);
        exit(-1);
    }

    gnPaletteEndEntry = nPaletteEndEntry;
    gnPaletteStartEntry = 1;
    for (int32_t n = -256; n < 256; ++n)
    {
        gnPaletteSquares[n + 256] = n * n;
    }
    gdwPaletteTransparentColor = 0;

    if (!lstrlenA(pszPalette))
    {
        for (uint32_t nEntry = 0; nEntry < 256; ++nEntry)
        {
            gaPaletteEntries[nEntry].red = (uint8_t)nEntry;
            gaPaletteEntries[nEntry].green = (uint8_t)nEntry;
            gaPaletteEntries[nEntry].blue = (uint8_t)nEntry;
        }
        gbPaletteLoaded = 1;
        return 1;
    }

    HANDLE hFile = CreateFileA(pszPalette, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        OutputDebugStringA("Unable to open ");
        OutputDebugStringA(pszPalette);
        OutputDebugStringA("\n");
        return 0;
    }

    DWORD dwFileSizeHigh;
    if (GetFileSize(hFile, &dwFileSizeHigh) == 768)
    {
        DWORD dwBytesRead = 0;
        ReadFile(hFile, gabPaletteFileBytes, 768, &dwBytesRead, NULL);
        if (dwBytesRead == 768)
        {
            for (uint32_t nByte = 0; nByte < 768; nByte += 3)
            {
                D2CmpPaletteEntry *pEntry = &gaPaletteEntries[nByte / 3];
                pEntry->blue = gabPaletteFileBytes[nByte];
                pEntry->green = gabPaletteFileBytes[nByte + 1];
                pEntry->red = gabPaletteFileBytes[nByte + 2];
            }
            gbPaletteLoaded = 1;
            CloseHandle(hFile);
            return gbPaletteLoaded;
        }
        gbPaletteLoaded = 0;
        CloseHandle(hFile);
        return gbPaletteLoaded;
    }

    CloseHandle(hFile);
    hFile = CreateFileA(pszPalette, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        OutputDebugStringA("Unable to open ");
        OutputDebugStringA(pszPalette);
        OutputDebugStringA("\n");
        gbPaletteLoaded = 0;
        return 0;
    }

    uint8_t abHeader[128];
    memset(abHeader, 0, sizeof(abHeader));
    DWORD dwBytesRead = 0;
    ReadFile(hFile, abHeader, 128, &dwBytesRead, NULL);
    if (dwBytesRead == 128 && abHeader[0] == 10 && abHeader[1] == 5 && abHeader[3] == 8)
    {
        uint8_t abTail[769];
        memset(abTail, 0, sizeof(abTail));
        SetFilePointer(hFile, -769, NULL, FILE_END);
        DWORD dwTailBytesRead;
        ReadFile(hFile, abTail, 769, &dwTailBytesRead, NULL);
        if (dwTailBytesRead == 769 && abTail[0] == 12)
        {
            for (uint32_t nEntry = 0; nEntry < 256; ++nEntry)
            {
                gaPaletteEntries[nEntry].red = abTail[1 + 3 * nEntry];
                gaPaletteEntries[nEntry].green = abTail[2 + 3 * nEntry];
                gaPaletteEntries[nEntry].blue = abTail[3 + 3 * nEntry];
            }
            CloseHandle(hFile);
            gbPaletteLoaded = 1;
            return 1;
        }
        OutputDebugStringA("Unable to extract palette from\n");
    }
    else
    {
        OutputDebugStringA("File is not a valid 256-color PCX file.\n");
    }
    CloseHandle(hFile);
    gbPaletteLoaded = 0;
    return 0;
}


//1.10f: D2CMP.0x6FDFAFD0
//  #10010: sets gbPaletteTestBackground to the argument. void: the eax the original leaves is the argument, and nothing imports it.
extern "C" void __stdcall PALETTE_PaletteTestBackground(int32_t bTestBackground)
{
    gbPaletteTestBackground = bTestBackground;
}


//1.10f: D2CMP.0x6FDFAFE0
//  #10007: the entry of PaletteInit's palette nearest a 24-bit pixel, over the entries gnPaletteStartEntry..gnPaletteEndEntry (unsigned compares),
//  skipping an entry that PaletteSetMask's mask clears; the squared differences come from gnPaletteSquares, the best starts at 7FFFFFFFh and the
//  first entry wins a tie. With gbPaletteTestBackground set, a pixel equal to the transparent colour answers 0 at the first entry the mask lets
//  through. The arguments are bytes, each masked before use; nBlue is matched with an entry's blue and nRed with its red.
extern "C" uint8_t __stdcall PALETTE_Palette24BitTo8BitPixel(uint8_t nBlue, uint8_t nGreen, uint8_t nRed)
{
    uint8_t nBestIndex = 0;
    uint32_t nBestDistance = 0x7FFFFFFF;
    for (uint32_t nEntry = gnPaletteStartEntry; nEntry <= gnPaletteEndEntry; ++nEntry)
    {
        if (gbPaletteMaskEnabled && !gabPaletteMask[nEntry])
        {
            continue;
        }
        if (gbPaletteTestBackground && gdwPaletteTransparentColor == (((uint32_t)nBlue << 16) | ((uint32_t)nGreen << 8) | nRed))
        {
            return 0;
        }
        uint32_t nDistance = gnPaletteSquares[nBlue - gaPaletteEntries[nEntry].blue + 256] + gnPaletteSquares[nGreen - gaPaletteEntries[nEntry].green + 256] + gnPaletteSquares[nRed - gaPaletteEntries[nEntry].red + 256];
        if (nDistance < nBestDistance)
        {
            nBestDistance = nDistance;
            nBestIndex = (uint8_t)nEntry;
        }
    }
    return nBestIndex;
}


//1.10f: D2CMP.0x6FDFB0D0
//  #10008: converts nPixels 24-bit pixels of pSource, three bytes each, to as many palette indexes in pDest, through pfnConvert or, when it is NULL,
//  Palette24BitTo8BitPixel, once PaletteInit has a palette; with gbPaletteFixBlack set, a pixel whose three bytes are all 10 or less (unsigned)
//  becomes blue 4, green 0, red 0. Always returns 0.
//  DIFFERENCE: the original keeps each channel in the low byte of a stack argument slot it has finished with and pushes the slot whole, so the
//  callback's upper bytes are those of pSource, pDest and pfnConvert; ours passes the bytes. Palette24BitTo8BitPixel masks every argument, and so
//  does any callback of this type (its parameters are bytes), so nothing reads the difference.
extern "C" int32_t __stdcall PALETTE_Palette24BitTo8BitImage(const uint8_t *pSource, uint8_t *pDest, uint32_t nPixels, uint8_t (__stdcall *pfnConvert)(uint8_t, uint8_t, uint8_t))
{
    if (gbPaletteLoaded)
    {
        uint8_t (__stdcall *pfnPixel)(uint8_t, uint8_t, uint8_t) = PALETTE_Palette24BitTo8BitPixel;
        if (pfnConvert)
        {
            pfnPixel = pfnConvert;
        }
        for (uint32_t nLeft = nPixels; nLeft != 0; --nLeft)
        {
            uint8_t nBlue = pSource[0];
            uint8_t nGreen = pSource[1];
            uint8_t nRed = pSource[2];
            pSource += 3;
            if (gbPaletteFixBlack && nBlue <= 10 && nGreen <= 10 && nRed <= 10)
            {
                nBlue = 4;
                nRed = 0;
                nGreen = 0;
            }
            *pDest++ = pfnPixel(nBlue, nGreen, nRed);
        }
    }
    return 0;
}


//1.10f: D2CMP.0x6FDFB170
//  #10011: sets gbPaletteFixBlack to the argument. void: the eax the original leaves is the argument, and nothing imports it.
extern "C" void __stdcall PALETTE_PaletteFixBlack(int32_t bFixBlack)
{
    gbPaletteFixBlack = bFixBlack;
}


//1.10f: D2CMP.0x6FDFB180
//  #10012: turns the mask on (gbPaletteMaskEnabled) and copies 256 bytes of pMask into gabPaletteMask.
extern "C" void __stdcall PALETTE_PaletteSetMask(const uint8_t *pMask)
{
    gbPaletteMaskEnabled = 1;
    memcpy(gabPaletteMask, pMask, sizeof(gabPaletteMask));
}


//1.10f: D2CMP.0x6FE00368
//  Allocates dwSize bytes of committed read/write/execute memory: VirtualAlloc(NULL, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE).
void *__fastcall PALETTE_VirtualAllocRWX(uint32_t dwSize)
{
    return VirtualAlloc(NULL, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
}


//1.10f: D2CMP.0x6FE00379
//  Releases an allocation of PALETTE_VirtualAllocRWX: VirtualFree(pAllocation, 0, MEM_RELEASE). Returns VirtualFree's BOOL.
BOOL __fastcall PALETTE_VirtualRelease(void *pAllocation)
{
    return VirtualFree(pAllocation, 0, MEM_RELEASE);
}
