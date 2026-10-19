#include "D2CMP.h"

//  Shared.cpp -- SHARED_CopyImageRows from D2CMP.dll 1.10 (image base 0x6FDF0000): a row copier at 0x6FE00388, packed among the import thunks at the
//  end of the image. No assert or string names the original's source file for it, so this file's name is ours.


//1.10f: D2CMP.0x6FE00388
//  Copies nRows rows of nRowBytes bytes from pSrc to pDst, each pointer stepping by its own stride. Per row it copies the nRowBytes & 3 leading bytes
//  one at a time first, then nRowBytes >> 2 dwords forward (the original's rep movsd), then advances each pointer by its stride less the row. The
//  dwords are copied one at a time, in order, as rep movsd does: one block copy would differ if the two rows overlapped. void: the eax the original
//  leaves is a residue that no caller reads.
//  BUG: the row count is tested after the first row (0x6FE00401), so nRows 0 copies one row and then wraps to 0xFFFFFFFF more. UNREACHABLE: both
//  callers, CelCompressAddImage and TileAddAddSubTile, are functions the stock game never reaches.
void __fastcall SHARED_CopyImageRows(uint8_t *pDst, const uint8_t *pSrc, int32_t nDstStride, int32_t nSrcStride, uint32_t nRows, uint32_t nRowBytes)
{
    const int32_t nSrcSkip = nSrcStride - (int32_t)nRowBytes;
    const int32_t nDstSkip = nDstStride - (int32_t)nRowBytes;

    do
    {
        switch (nRowBytes & 3)
        {
        case 1:
            pDst[0] = pSrc[0];
            pDst += 1;
            pSrc += 1;
            break;
        case 2:
            pDst[0] = pSrc[0];
            pDst[1] = pSrc[1];
            pDst += 2;
            pSrc += 2;
            break;
        case 3:
            pDst[0] = pSrc[0];
            pDst[1] = pSrc[1];
            pDst[2] = pSrc[2];
            pDst += 3;
            pSrc += 3;
            break;
        }
        for (uint32_t nDwords = nRowBytes >> 2; nDwords; --nDwords)
        {
            memcpy(pDst, pSrc, 4);
            pDst += 4;
            pSrc += 4;
        }
        pSrc += nSrcSkip;
        pDst += nDstSkip;
    } while (--nRows);
}
