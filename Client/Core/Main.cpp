// Client Core module entry point
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE.md or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include "Core.hpp"

#include <Container/Str.hpp>
#include <Path.hpp>

Core * g_pCore;

using namespace Util;

BOOL WINAPI DllMain(HINSTANCE hDllInstance, DWORD dwReason, LPVOID lpvReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hDllInstance);

        SetCurrentDirectoryW(WString{ GetGTAPath() }.CString());
        SetDllDirectoryW(WString{ PathJoin(GetSAOPath(), "SAO") }.CString());
        g_pCore = new Core();
        SetCurrentDirectoryW(WString{ GetGTAPath() }.CString());
    }

    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TerminateProcess(GetCurrentProcess(), 0);

        delete g_pCore;
        g_pCore = nullptr;
    }
    return TRUE;
}
