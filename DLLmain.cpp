#include "DLLmain.h"

/****************************************************************************
*                                                                           *
*   DLLmain.cpp                                                             *
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
*   D2Template core file, do not modify unless you know what you're doing   *
*                                                                           *
*****************************************************************************/

//  DLLmain.cpp -- the D2Template helpers (GetOrLoadModule, GetDllOffset, GetModuleExt) and the module entry of D2CMP.dll 1.10, DllMain
//  (0x6FDF5C00).

HMODULE GetOrLoadModule(const char* module_path)
{
    HMODULE hModule = GetModuleHandle(module_path);
    if (!hModule)
    {
        hModule = LoadLibrary(module_path);
    }

    return hModule;
}

DWORD GetDllOffset(const char* ModuleName, DWORD BaseAddress, int32_t Offset)
{
    if (!BaseAddress)
        BaseAddress = (DWORD)LoadLibraryA(GetModuleExt(ModuleName).c_str());

    if (Offset < 0)
        return (DWORD)GetProcAddress((HMODULE)BaseAddress, (LPCSTR)(-Offset));

    return BaseAddress + Offset;
}

//  Returns std::string by value: the D2Template original returned a pointer to a local array.
std::string GetModuleExt(const char* ModuleName)
{
    const char DLLExt[] = ".dll";
    char DLLName[32] = { 0 };

    strncpy_s(DLLName, sizeof(DLLName), ModuleName, _TRUNCATE);
    size_t nUsed = strnlen(DLLName, sizeof(DLLName));
    size_t nExtLen = sizeof(DLLExt) - 1;
    if (nUsed + nExtLen < sizeof(DLLName)) {
        strcat_s(DLLName, sizeof(DLLName), DLLExt);
    }

    return std::string(DLLName);
}

//1.10f: D2CMP.0x6FDF5C00
//  Returns TRUE for every reason (`mov eax, 1` / `retn 0Ch`): the three stack arguments are unread and no module handle is kept.
//  extern "C" __stdcall gives the symbol the original's name, _DllMain@12.
//  DIFFERENCE: the original's caller is the VC6 runtime's DLL startup (the PE entry), which is not rewritten; the project keeps the default
//  /ENTRY and the runtime's own _DllMainCRTStartup calls this function in its place.
extern "C" BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    return TRUE;
}
