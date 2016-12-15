// Launcher entry point
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  NTAuthority
//
// Copyright (c) 2015-2017 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include "Main.hpp"

#include <Container/Str.hpp>

char g_szGamePath[MAX_PATH];
char g_szGameExecutable[MAX_PATH]; // "D:\\GTA San Andreas Online\\SA4g.exe"
char g_szExecutablePath[MAX_PATH]; // "C:\\Users\\iFarbod\\Desktop\\uglaunch\\Debug\\"

void wmain()
{
    if(!CheckRegistryIntegrity())
    {
        ExitProcess(1);
        return;
    }

    if(GetFileAttributesA(g_szGameExecutable) == INVALID_FILE_ATTRIBUTES)
    {
        FatalError("Could not find the game executable at the configured path.");
        return;
    }

    SetDllDirectoryA(g_szGamePath);
    SetCurrentDirectoryA(g_szGamePath);

    GTASALauncher::Launch(g_szGameExecutable);
}
