// CEF Launcher dll entry point
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "SAOCefApp.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

int __declspec(dllexport) InitCEF ()
{
    // Get absolute CEFLauncher.exe path
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    std::wstring currentFileName{ buffer };

    // Extract SAO path and set DLL directory
    size_t pos = currentFileName.find_last_of(L'\\');
    std::wstring saoPath = currentFileName.substr(0, pos - 3); // Strip "CEF"
    SetDllDirectoryW(saoPath.c_str());

    // Load CEF
    CefMainArgs mainArgs(GetModuleHandleW(nullptr));
    CefRefPtr<SAOCefApp> app{ new SAOCefApp };

    return CefExecuteProcess(mainArgs, app, nullptr);
}
