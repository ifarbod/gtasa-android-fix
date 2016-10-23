// Client Core module entry point
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE.md or copy at
// https://opensource.org/licenses/MIT)

#include <Win32\Util.Win32Headers.hpp>

BOOL WINAPI DllMain(HINSTANCE hDllInstance, DWORD dwReason, LPVOID lpvReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hDllInstance);

        //SetCurrentDirectory(gtaDir)
        //SetDllDirectory(SAOPath("SAO"));
        // initialize
        //SetCurrentDirectory(gtaDir)
    }

    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TerminateProcess(GetCurrentProcess(), 0);

        // shut down
    }
    return TRUE;
}
