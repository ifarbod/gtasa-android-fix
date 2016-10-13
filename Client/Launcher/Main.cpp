// Client Launcher entry point
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 San Andreas Online
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <VersionHelpers.h>

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR strCmdLine, int nCmdShow)
{
    // Check Windows version
    if (!IsWindows7SP1OrGreater())
    {
        MessageBoxA(NULL, "You need Windows 7 SP1 or greater to play San Andreas Online.", NULL, MB_ICONSTOP);
        return 1;
    }

    // Check for safe mode
    if (GetSystemMetrics(SM_CLEANBOOT) != 0)
    {
        MessageBoxA(NULL, "You cannot launch SAO in Safe Mode.", NULL, MB_ICONSTOP);
        return 1;
    }

    return 0;
}
