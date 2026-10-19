#include "D2CMP.h"

//  Archive.cpp -- the file helpers of D2CMP.dll 1.10 (image base 0x6FDF0000): open, close, seek and read-exactly through Fog.
//  Not a D2CMP source file: its asserts and error reports name D2Hell's C:\projects\D2\head\Diablo2\Source\D2Hell\SRC\Archive.cpp, compiled into
//  D2CMP. Address range 0x6FE00418..0x6FE00596. Every function takes an unused ECX argument (nUnused) and the file handle in EDX.

//  The path literal of the unit's reports (__FILE__ in the original).
static const char szArchiveCpp[] = "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Hell\\SRC\\Archive.cpp";


//1.10f: D2CMP.0x6FE00418
//  Opens a file through Fog (FOG_FOpenFile: name in ECX, handle slot in EDX) and answers 1, not Fog's value. On a failure it reads GetLastError
//  first and traces "Error opening file: %s" unless bSuppressMissing is set and the error is ERROR_FILE_NOT_FOUND; answers 0. nUnused is never read.
int32_t __fastcall ARCHIVE_OpenFile(int32_t nUnused, const char *szFileName, void **ppFileHandle, int32_t bSuppressMissing)
{
    if (FOG_FOpenFile(szFileName, ppFileHandle))
        return 1;

    uint32_t dwError = GetLastError();
    if (!bSuppressMissing || dwError != ERROR_FILE_NOT_FOUND)
        FOG_Trace("Error opening file: %s", szFileName);
    return 0;
}


//1.10f: D2CMP.0x6FE00454
//  Closes a file: asserts the handle, then tail-jumps to FOG_FCloseFile. The jump leaves Fog's answer in eax; no caller reads it, so it is void.
void __fastcall ARCHIVE_CloseFile(int32_t nUnused, void *hFile)
{
    if (!hFile)
    {
        FOG_DisplayAssert("hFile", szArchiveCpp, 73);
        exit(-1);
    }
    FOG_FCloseFile(hFile);
}


//1.10f: D2CMP.0x6FE0047A
//  Moves a file's pointer (FOG_FSetFilePointer, the arguments passed on) and halts (FOG_DisplayError, category 3, exit) when Fog answers 0xFFFFFFFF,
//  without looking at GetLastError, so a position that is itself 0xFFFFFFFF halts too. Asserts the handle. Void: its one caller overwrites eax.
//  szFileName is left uninitialised on purpose: STORM_SFileGetFileName stores a NUL first, so a failed lookup reports an empty name.
void __fastcall ARCHIVE_SeekFile(int32_t nUnused, void *hFile, int32_t lDistanceToMove, int32_t *lpDistanceToMoveHigh, uint32_t dwMoveMethod)
{
    if (!hFile)
    {
        FOG_DisplayAssert("hFile", szArchiveCpp, 112);
        exit(-1);
    }
    if (FOG_FSetFilePointer(hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod) == 0xFFFFFFFF)
    {
        char szFileName[260];
        STORM_SFileGetFileName(hFile, szFileName, 260);
        FOG_DisplayError(3, szFileName, szArchiveCpp, 121);
        exit(-1);
    }
}


//1.10f: D2CMP.0x6FE004F0
//  Reads exactly dwBytesToRead bytes (FOG_FReadFile: handle in ECX, buffer in EDX, then size, count slot and three zeros). Asserts the handle; a
//  failed read halts as ARCHIVE_SeekFile does, a short one asserts "dwBytesToRead == dwBytes" and exits. Void: the answer of FReadFile is not read.
//  The original lets Fog write the count into the argument's own stack slot; the C has a local.
void __fastcall ARCHIVE_ReadFileExact(int32_t nUnused, void *hFile, void *pBuffer, uint32_t dwBytesToRead)
{
    if (!hFile)
    {
        FOG_DisplayAssert("hFile", szArchiveCpp, 135);
        exit(-1);
    }

    uint32_t dwBytes;
    if (!FOG_FReadFile(hFile, pBuffer, dwBytesToRead, &dwBytes, 0, 0, 0))
    {
        char szFileName[260];
        STORM_SFileGetFileName(hFile, szFileName, 260);
        FOG_DisplayError(3, szFileName, szArchiveCpp, 143);
        exit(-1);
    }
    if (dwBytesToRead != dwBytes)
    {
        FOG_DisplayAssert("dwBytesToRead == dwBytes", szArchiveCpp, 146);
        exit(-1);
    }
}
