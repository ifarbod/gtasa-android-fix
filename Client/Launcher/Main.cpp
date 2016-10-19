// Client Launcher entry point
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 San Andreas Online
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "Main.hpp"

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
        MessageBoxA(NULL, "You cannot play San Andreas Online in Safe Mode.", NULL, MB_ICONSTOP);
        return 1;
    }

    // Start San Andreas
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(si);


    if (!CreateProcess(L"gta_sa.exe", L"", nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, L"", &si, &pi))
    {
        MessageBoxA(nullptr, "Failed to start San Andreas. Cannot launch SA:Online.", nullptr, MB_ICONSTOP);
        return 1;
    }

    const wchar_t* pszLibToInject = L"Core.dll";
    unsigned long ulSize = 18ul /* (Length of pszLibToInject + 1) * 2 */, ulWrittenBytes;
    void* pAllocedMem;
    HANDLE hExternThread;
    if(!(pAllocedMem = VirtualAllocEx(pi.hProcess, NULL, ulSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE))) return 1; // TODO Error
    if(!WriteProcessMemory(pi.hProcess, pAllocedMem, (void*)pszLibToInject, ulSize, &ulWrittenBytes)) return 1;
    if(ulSize != ulWrittenBytes) return 1;
    if(!(hExternThread = CreateRemoteThread(pi.hProcess, NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryW), pAllocedMem, 0, NULL))) return 1;
    SetThreadPriority(hExternThread, THREAD_PRIORITY_HIGHEST);
    WaitForSingleObject(hExternThread, 3000);
    CloseHandle(hExternThread);
    VirtualFreeEx(pi.hProcess, pAllocedMem, ulSize, MEM_RELEASE);
    ResumeThread(pi.hThread); // TODO Prevent game's thread to be resumed before we say that!

    return 0;
}
