#pragma once

/****************************************************************************
*                                                                           *
*   DLLmain.h                                                               *
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

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _CRT_SECURE_NO_DEPRECATE
#define _WIN32_WINNT 0x600

#pragma warning(disable : 4200)

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <shellapi.h>
#include <algorithm>
#include <utility>
#include <vector>
#include <functional>
#include <memory>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <format>

HMODULE GetOrLoadModule(const char* module_path);
//  DWORD, not uint32_t: D2Ptrs.h declares `extern DWORD GetDllOffset(const char*, DWORD, int)`, and DWORD is `unsigned long` where uint32_t
//  is `unsigned int`, so uint32_t would be a second overload.
DWORD GetDllOffset(const char* ModuleName, DWORD BaseAddress, int32_t Offset);
std::string GetModuleExt(const char* ModuleName);


#include "D2Constants.h"
#include "D2Structs.h"
#include "D2Ptrs.h"
#include "D2Vars.h"
