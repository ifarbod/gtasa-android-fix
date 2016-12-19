// Process utils
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Container/Str.hpp"

#include <cstdlib>

namespace Util
{

// Open a console window.
void OpenConsoleWindow();
// Print Unicode text to the console. Will not be printed to the MSVC output window.
void PrintUnicode(const String& str, bool error = false);
// Print Unicode text to the console with a newline appended. Will not be printed to the MSVC output window.
void PrintUnicodeLine(const String& str, bool error = false);
// Print ASCII text to the console with a newline appended. Uses printf() to allow printing into the MSVC output window.
void PrintLine(const String& str, bool error = false);
// Parse arguments from the command line. First argument is by default assumed to be the executable name and is skipped.
const Vector<String>& ParseArguments(const String& cmdLine, bool skipFirstArgument = true);
// Parse arguments from the command line.
const Vector<String>& ParseArguments(const char* cmdLine);
// Parse arguments from a wide char command line.
const Vector<String>& ParseArguments(const WString& cmdLine);
// Parse arguments from a wide char command line.
const Vector<String>& ParseArguments(const wchar_t* cmdLine);
// Parse arguments from argc & argv.
const Vector<String>& ParseArguments(int argc, char** argv);
// Return previously parsed arguments.
const Vector<String>& GetArguments();
// Encloses arguments that contain space in quotes
void QuoteArguments(Vector<String>& args);
// Read input from the console window. Return empty if no input.
String GetConsoleInput();
// AV friendly version of GetProcAddress
FARPROC GetProcedureAddress(HMODULE hModule, const String& procName);
// Force a remote process to call a kernel32 function
bool CallRemoteFunction(HANDLE hProcess, const String& functionName, const String& fileName);
// Force a remote process to load a DLL (injection)
bool RemoteLoadLibrary(HANDLE hProcess, const String& fileName);
// RemoteLoadLibrary with process ID instead of handle
bool RemoteLoadLibrary(unsigned processId, const String& fileName);

}
