// Process utils
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 San Andreas Online
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "PCH.hpp"

#include "ProcessUtils.hpp"
#include "Path.hpp"

namespace Util
{

// TODO: Reformat this function
FARPROC GetProcedureAddress(HMODULE hModule, const String& procName)
{
    PVOID pFunctionAddress = nullptr;

    __try
    {
        PIMAGE_DOS_HEADER dos = reinterpret_cast<PIMAGE_DOS_HEADER>(hModule);
        PIMAGE_NT_HEADERS nt = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<ULONG>(hModule) + dos->e_lfanew);

        PIMAGE_DATA_DIRECTORY expdir = (PIMAGE_DATA_DIRECTORY)(nt->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT);
        ULONG addr = expdir->VirtualAddress;
        PIMAGE_EXPORT_DIRECTORY exports = (PIMAGE_EXPORT_DIRECTORY)((ULONG)hModule + addr);
        PULONG functions = (PULONG)((ULONG)hModule + exports->AddressOfFunctions);
        PSHORT ordinals = (PSHORT)((ULONG)hModule + exports->AddressOfNameOrdinals);
        PULONG names = (PULONG)((ULONG)hModule + exports->AddressOfNames);
        ULONG max_name = exports->NumberOfNames;
        ULONG max_func = exports->NumberOfFunctions;

        for (ULONG i = 0; i < max_name; i++)
        {
            ULONG ord = ordinals[i];
            if (i >= max_name || ord >= max_func) return nullptr;
            if (functions[ord] < addr || functions[ord] >= addr)
            {
                if (strcmp((PCHAR)hModule + names[i], procName.CString()) == 0)
                {
                    pFunctionAddress = (PVOID)((PCHAR)hModule + functions[ord]);
                    break;
                }
            }
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        pFunctionAddress = nullptr;
    }

    return (FARPROC)pFunctionAddress;
}

bool CallRemoteFunction(HANDLE hProcess, const String& functionName, const String& fileName)
{
    // Store the path as wchar_t
    WString libPath = WString{ fileName }.CString();
    unsigned libPathLength = libPath.Length();
    
    // Store kernel32 handle
    HMODULE hKernel32 = GetModuleHandleW(L"KERNEL32");

    // Store remote thread handle
    HANDLE hThread = nullptr;

    // Allocate memory in the remote process
    void * libPathRemote = nullptr;
    unsigned libPathSize = (libPathLength + 1) * sizeof(wchar_t);
    libPathRemote = VirtualAllocEx(hProcess, nullptr, libPathSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    // Allocation failed, return false
    if (libPathRemote == nullptr)
        return false;

    // Write the DLL path to the remote memory allocated
    SIZE_T bytesWritten = 0;
    WriteProcessMemory(hProcess, libPathRemote, (void*)libPath.CString(), libPathSize, &bytesWritten);

    if (bytesWritten != libPathSize)
    {
        // Clean up the resources used for injection of the DLL
        VirtualFreeEx(hProcess, libPathRemote, libPathSize, MEM_RELEASE);

        return false;
    }

    // Get the function's address
    LPTHREAD_START_ROUTINE functionPtr = reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcedureAddress(hKernel32, functionName));

    // Does the function exist?
    if (functionPtr == nullptr)
    {
        // Clean up the resources used for injection of the DLL
        VirtualFreeEx(hProcess, libPathRemote, libPathSize, MEM_RELEASE);

        return false;
    }

    // Start a remote thread calling the desired function with the parameter
    hThread = CreateRemoteThread(
        hProcess,
        nullptr,
        0,
        functionPtr,
        libPathRemote,
        0,
        nullptr);

    if (hThread == nullptr)
    {
        // Clean up the resources used for injection of the DLL
        VirtualFreeEx(hProcess, libPathRemote, libPathSize, MEM_RELEASE);

        return false;
    }

    // We wait for the created remote thread to finish executing. When it's done, the DLL
    // is loaded into the game's userspace, and we can destroy the thread-handle. We wait
    // 5 seconds which is way longer than this should take to prevent this application
    // from deadlocking if something goes really wrong allowing us to kill the injected
    // game executable and avoid user inconvenience.
    //WaitForObject(hProcess, hThread, INFINITE, NULL);


    // Get the handle of the remotely loaded DLL module
    DWORD hLibModule = 0;
    GetExitCodeThread(hThread, &hLibModule);

    // Clean up the resources used for injection of the DLL
    VirtualFreeEx(hProcess, libPathRemote, libPathSize, MEM_RELEASE);

    // Success
    return true;
}

bool RemoteLoadLibrary(HANDLE hProcess, const String& fileName)
{
    if (!CallRemoteFunction(hProcess, "SetDllDirectoryW", GetNativePath(GetPath(fileName))))
    {
        return false;
    }

    if (!CallRemoteFunction(hProcess, "LoadLibraryW", fileName))
    {
        return false;
    }

    // Success
    return true;
}

bool RemoteLoadLibrary(unsigned processId, const String& fileName)
{
    // Open our target process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

    if (!hProcess)
    {
        // Failed to open the process
        return false;
    }

    // Inject the library into the process
    bool result = RemoteLoadLibrary(hProcess, fileName);

    // Close the process handle
    CloseHandle(hProcess);
    return result;
}

}