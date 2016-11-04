// Process utils
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 San Andreas Online
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Str.hpp"

namespace Util
{

// AV friendly version of GetProcAddress
FARPROC GetProcedureAddress(HMODULE hModule, const String& procName);
// Force a remote process to call a kernel32 function
bool CallRemoteFunction(HANDLE hProcess, const String& functionName, const String& fileName);
// Force a remote process to load a DLL (injection)
bool RemoteLoadLibrary(HANDLE hProcess, const String& fileName);
// RemoteLoadLibrary with process ID instead of handle
bool RemoteLoadLibrary(unsigned processId, const String& fileName);

}
