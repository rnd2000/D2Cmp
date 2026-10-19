#include "D2CMP.h"

/*---------------------------------------------------------------------------
 *
 *  Count.cpp -- D2CMP.dll 1.10 (image base 0x6FDF0000), address range 0x6FDF5A60..0x6FDF5BF2.
 *  The original's source file is D2CMP\Src\Count.cpp (its asserts name it).
 *
 *  The run counters of the run-length encoders: from pSrc toward pEnd, how many bytes are equal to a value
 *  (CountConsecutive; CountConsecutiveFirst, the value being pSrc's own) or differ from it (CountConsecutiveDiff), at
 *  most nMaxCount; then two flags -- *pbCountEqualsDist, the run reached pEnd, and *pbIsRemainingSame, it did not but
 *  every byte left to pEnd is the value.
 *
 *  The second flag is computed from the bytes left after the counted run, and only when the run stopped short of pEnd:
 *  a run cut at nMaxCount with the rest all the value sets it, as does a run that stopped on a byte that is the value
 *  (possible only in CountConsecutiveDiff). When the run reaches pEnd the flags are 1 and 0.
 *
 *-------------------------------------------------------------------------*/

//  The path literal of the unit's asserts (the original's __FILE__).
static const char szCountCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2CMP\\Src\\Count.cpp";


//1.10f: D2CMP.0x6FDF5A60
//  The run of bytes equal to *pSrc, at most nMaxCount, and the two flags. pSrc must be below pEnd (assert :25, and exit).
//  The loop tests the byte, then the count (signed), then steps and tests the end (unsigned). Returns the count.
int32_t __fastcall COUNT_CountConsecutiveFirst(const uint8_t *pSrc, const uint8_t *pEnd, int32_t nMaxCount, int32_t *pbCountEqualsDist, int32_t *pbIsRemainingSame)
{
    if (pSrc >= pEnd)
    {
        FOG_DisplayAssert("pSrc < pEnd", szCountCpp, 25);
        exit(-1);
    }

    const uint8_t bValue = *pSrc;
    int32_t nCount = 0;
    do
    {
        if (*pSrc != bValue || nCount >= nMaxCount)
            break;
        ++nCount;
        ++pSrc;
    } while (pSrc < pEnd);

    if (pSrc == pEnd)
    {
        *pbCountEqualsDist = 1;
        *pbIsRemainingSame = 0;
        return nCount;
    }
    *pbCountEqualsDist = 0;
    while (pSrc < pEnd && *pSrc == bValue)
        ++pSrc;
    *pbIsRemainingSame = (pSrc == pEnd);
    return nCount;
}


//1.10f: D2CMP.0x6FDF5AF0
//  The run of bytes equal to bValue, at most nMaxCount, and the two flags; bValue arrives in CL, pSrc in EDX, the rest on the stack.
//  pSrc must be below pEnd (assert :69, and exit). As CountConsecutiveFirst otherwise. Returns the count.
int32_t __fastcall COUNT_CountConsecutive(uint8_t bValue, const uint8_t *pSrc, const uint8_t *pEnd, int32_t nMaxCount, int32_t *pbCountEqualsDist, int32_t *pbIsRemainingSame)
{
    if (pSrc >= pEnd)
    {
        FOG_DisplayAssert("pSrc < pEnd", szCountCpp, 69);
        exit(-1);
    }

    int32_t nCount = 0;
    do
    {
        if (*pSrc != bValue || nCount >= nMaxCount)
            break;
        ++nCount;
        ++pSrc;
    } while (pSrc < pEnd);

    if (pSrc == pEnd)
    {
        *pbCountEqualsDist = 1;
        *pbIsRemainingSame = 0;
        return nCount;
    }
    *pbCountEqualsDist = 0;
    while (pSrc < pEnd && *pSrc == bValue)
        ++pSrc;
    *pbIsRemainingSame = (pSrc == pEnd);
    return nCount;
}


//1.10f: D2CMP.0x6FDF5B70
//  The run of bytes different from bValue (the first stack argument, read as a byte), at most nMaxCount -- the loop stops on the value --
//  and the two flags; the second, as in the others, is whether every byte left is the value. pSrc must be below pEnd (assert :111, and exit).
//  Returns the count.
int32_t __fastcall COUNT_CountConsecutiveDiff(const uint8_t *pSrc, const uint8_t *pEnd, uint8_t bValue, int32_t nMaxCount, int32_t *pbCountEqualsDist, int32_t *pbIsRemainingSame)
{
    if (pSrc >= pEnd)
    {
        FOG_DisplayAssert("pSrc < pEnd", szCountCpp, 111);
        exit(-1);
    }

    int32_t nCount = 0;
    do
    {
        if (*pSrc == bValue || nCount >= nMaxCount)
            break;
        ++nCount;
        ++pSrc;
    } while (pSrc < pEnd);

    if (pSrc == pEnd)
    {
        *pbCountEqualsDist = 1;
        *pbIsRemainingSame = 0;
        return nCount;
    }
    *pbCountEqualsDist = 0;
    while (pSrc < pEnd && *pSrc == bValue)
        ++pSrc;
    *pbIsRemainingSame = (pSrc == pEnd);
    return nCount;
}
