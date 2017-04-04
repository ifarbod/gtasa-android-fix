// Launcher entry point
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  NTAuthority
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include "Main.hpp"

#include <Container/Str.hpp>

char g_gamePath[MAX_PATH];
char g_gameExecutable[MAX_PATH]; // "D:\\GTA San Andreas Online\\gta_sa.exe"
char g_executablePath[MAX_PATH];

void wmain()
{
    if (!CheckRegistryIntegrity())
    {
        ExitProcess(1);
        return;
    }

    if (!GetProcAddress(GetModuleHandle(L"kernelbase.dll"), "SetThreadDescription"))
    {
        MessageBox(nullptr, L"Your PC is running an outdated version of Windows. This may lead to issues using the "
                            L"CTNorth client. It's recommended that you update to Windows 10 version 1607 or higher in "
                            L"case you are experiencing any issues. The game will continue to start now.",
            L"CTNorth", MB_OK | MB_ICONWARNING);
    }

    if (GetFileAttributesA(g_gameExecutable) == INVALID_FILE_ATTRIBUTES)
    {
        FatalError("Could not find the game executable at the configured path.");
        return;
    }

    // FIXME: Set DLL Directory to CTN's path
    //SetDllDirectoryA(g_gamePath);
    SetCurrentDirectoryA(g_gamePath);

    GameLauncher::Launch(g_gameExecutable);
}
