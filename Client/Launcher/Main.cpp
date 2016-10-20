// Client Launcher entry point
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE.md or copy at
// https://opensource.org/licenses/MIT)

#include "Main.hpp"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR strCmdLine, int nCmdShow)
{
    // Check Windows version
    if (!IsWindows7SP1OrGreater())
    {
        // ERR_WINDOWS_NOT_SUPPORTED
        MessageBoxA(NULL, "You need Windows 7 SP1 or greater to play San Andreas Online.", NULL, MB_ICONSTOP);
        return 1;
    }

    // Check for safe mode
    if (GetSystemMetrics(SM_CLEANBOOT) != 0)
    {
        // ERR_CLEANBOOT
        MessageBoxA(NULL, "You cannot play San Andreas Online in Safe Mode.", NULL, MB_ICONSTOP);
        return 1;
    }

    // Launch GTA:SA
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(si);

    if (!CreateProcess(L"D:\\SA\\gta_sa.exe", L"", nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, L"", &si, &pi))
    {
        MessageBoxA(nullptr, "Failed to start San Andreas. Cannot launch SA:Online.", nullptr, MB_ICONSTOP);
        return 1;
    }

#ifdef SAO_DEBUG
    const wchar_t* pszLibToInject = L"Core-d.dll";
#else
    const wchar_t* pszLibToInject = L"Core.dll";
#endif

    u32 ulSize = 18ul;
    SIZE_T ulWrittenBytes;

    void* pAllocedMem = nullptr;
    HANDLE hExternThread = nullptr;

    // Allocate memory in the remote process
    pAllocedMem = VirtualAllocEx(pi.hProcess, NULL, ulSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if(!(pAllocedMem))
        return 1;

    if(!WriteProcessMemory(pi.hProcess, pAllocedMem, (void*)pszLibToInject, ulSize, &ulWrittenBytes))
        return 1;

    if(ulSize != ulWrittenBytes)
        return 1;

    hExternThread = CreateRemoteThread(pi.hProcess, NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryW), pAllocedMem, 0, NULL);

    if(!hExternThread)
        return 1;

    SetThreadPriority(hExternThread, THREAD_PRIORITY_HIGHEST);

    WaitForSingleObject(hExternThread, 3000);

    CloseHandle(hExternThread);

    VirtualFreeEx(pi.hProcess, pAllocedMem, ulSize, MEM_RELEASE);

    ResumeThread(pi.hThread);

    return 0;
}
