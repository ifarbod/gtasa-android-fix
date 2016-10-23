// Forwarding kernel32 functions for the sake of 'some' antivirus software
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 San Andreas Online
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Util.Win32Headers.hpp>

using VirtualAllocEx_t = LPVOID(WINAPI *)(
    HANDLE hProcess,
    LPVOID lpAddress,
    SIZE_T dwSize,
    DWORD flAllocationType,
    DWORD flProtect
    );

class Kernel32
{
public:


private:

};
