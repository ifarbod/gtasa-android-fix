// Launcher entry point
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  NTAuthority
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include "Main.hpp"

#include <Container/Str.hpp>

char g_szGamePath[MAX_PATH];
char g_szGameExecutable[MAX_PATH]; // "D:\\GTA San Andreas Online\\gta_sa.exe"
char g_szExecutablePath[MAX_PATH];

void wmain()
{
    if (!CheckRegistryIntegrity())
    {
        ExitProcess(1);
        return;
    }

    if (GetFileAttributesA(g_szGameExecutable) == INVALID_FILE_ATTRIBUTES)
    {
        FatalError("Could not find the game executable at the configured path.");
        return;
    }

    // FIXME: Set DLL Directory to CTN's path
    //SetDllDirectoryA(g_szGamePath);
    SetCurrentDirectoryA(g_szGamePath);

    GameLauncher::Launch(g_szGameExecutable);
}
