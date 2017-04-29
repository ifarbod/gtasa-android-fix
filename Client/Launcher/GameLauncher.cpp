// Game EXE loader
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  NTAuthority
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include "Main.hpp"
#include <winternl.h>
#include <Hooking/Hook.hpp>
#include <Container/Str.hpp>
#include <Foundation/ProcessUtils.hpp>
#include <Container/ArrayPtr.hpp>
#include <PEImage.hpp>

using namespace ctn;

static LONG NTAPI HandleVariant(PEXCEPTION_POINTERS exceptionInfo)
{
    SetForegroundWindow(GetDesktopWindow());
    return (exceptionInfo->ExceptionRecord->ExceptionCode == STATUS_INVALID_HANDLE) ? EXCEPTION_CONTINUE_EXECUTION : EXCEPTION_CONTINUE_SEARCH;
}

void GameLauncher::InvokeEntryPoint(void(*entryPoint)())
{
    // SEH call to prevent STATUS_INVALID_HANDLE
    __try
    {
        // and call the entry point
        entryPoint();
    }
    __except (HandleVariant(GetExceptionInformation()))
    {
    }
}

void GameLauncher::Launch(const char* gamePath)
{
    // Load the game executable data in temporary memory
    FILE* gameFile = _wfopen(WString{ gamePath }.CString(), L"rb");

    if (!gameFile)
    {
        return;
    }

    // Find the file length and allocate a related buffer
    u32 length;

    fseek(gameFile, 0, SEEK_END);
    length = ftell(gameFile);

    SharedArrayPtr<u8> data(new u8[length]);

    // Seek back to the start and read the file
    rewind(gameFile);
    fread(data, 1, length, gameFile);

    // Close the file, and continue on
    fclose(gameFile);

    // Load the executable into our module context
    HMODULE exeModule = GetModuleHandleW(nullptr);

    ExecutableLoader exeLoader(data.Get());

    SetCurrentDirectoryA("D:\\CTN-SA\\");

    exeLoader.SetLibraryLoader([](const char* libName)
    {
        if (String(libName).ToLower() == "vorbisfile.dll")
        {
            return LoadLibraryW(L"sdvf.dll");
        }

        if (String(libName).ToLower() == "eax.dll")
        {
            return LoadLibraryW(L"sde.dll");
        }

        return LoadLibraryA(libName);
    });

    exeLoader.LoadIntoModule(exeModule);

    LoadLibraryA("Core.dll");

    // Get the entry point
    auto entryPoint = reinterpret_cast<void(*)()>(exeLoader.GetEntryPoint());

    // Call the entry point
    AddVectoredExceptionHandler(0, HandleVariant);
    return InvokeEntryPoint(entryPoint);
}
