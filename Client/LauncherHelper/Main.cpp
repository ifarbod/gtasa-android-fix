// Client Core module entry point
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE.md or copy at
// https://opensource.org/licenses/MIT)

#include <Windows.h>
#include "Res.hpp"

void meow()
{
    auto thisModule = GetModuleHandle(nullptr);
    HRSRC resource = FindResource(thisModule, MAKEINTRESOURCE(IDR_GAMEBIN), RT_RCDATA);
    void* pMoonMask = static_cast<void*>(LoadResource(thisModule, resource));
    SizeofResource(thisModule, resource);
    FreeResource(pMoonMask);
}

BOOL WINAPI DllMain(HINSTANCE hDllInstance, DWORD dwReason, LPVOID lpvReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
    }

    else if (dwReason == DLL_PROCESS_DETACH)
    {
    }

    return TRUE;
}
