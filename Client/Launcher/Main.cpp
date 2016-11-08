// Client Launcher entry point
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE.md or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include "Utils.hpp"
#include <Str.hpp>
#include <Path.hpp>
#include <ProcessUtils.hpp>
#include <Registry.hpp>

using namespace Util;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR strCmdLine, int nCmdShow)
{
    // Check Windows version
    if (!IsWindows7SP1OrGreater())
    {
        // ERR_WINDOWS_NOT_SUPPORTED
        MessageBoxW(nullptr, L"You need Windows 7 SP1 or greater to play San Andreas Online.", NULL, MB_ICONSTOP);
        return 1;
    }

    // Check for safe mode
    if (GetSystemMetrics(SM_CLEANBOOT) != 0)
    {
        // ERR_CLEANBOOT
        MessageBoxW(nullptr, L"You cannot play San Andreas Online in Safe Mode.", NULL, MB_ICONSTOP);
        return 1;
    }

    // Store paths to GTA and SAO
    const String gtaPath = GrabSAPath();
    const String saoPath = GetSAOPath();
    String saoDir = PathJoin(saoPath, "SAO");

    // Set the current directory
    SetDllDirectoryW(WString{ saoDir }.CString());
    SetCurrentDirectoryW(WString{ gtaPath }.CString());

    // Generate command line
    String commandLine = String(GetGTAPath("GTASA.exe")) + " " + strCmdLine;

    // Try launching SA
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(si);

    if (!CreateProcessW(
        WString{ GetGTAPath("GTASA.exe") }.CString(),
        const_cast<wchar_t *>(WString{ commandLine }.CString()),
        nullptr,
        nullptr,
        FALSE,
        CREATE_SUSPENDED,
        nullptr,
        WString{ saoDir }.CString(),
        &si,
        &pi))
    {
        MessageBoxW(nullptr, L"Failed to launch San Andreas", nullptr, MB_ICONSTOP);
        return 1;
    }

    if (!RemoteLoadLibrary(pi.hProcess, PathJoin(GetSAOPath("SAO"), "Core_d.dll")))
    {
        TerminateProcess(pi.hProcess, 0);
        return 1;
    }

    // Resume the thread
    ResumeThread(pi.hThread);

    // Success
    return 0;
}
