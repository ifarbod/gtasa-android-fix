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

// Initialize the FPU to round-to-nearest, single precision mode.
void InitFPU();
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
// Return the runtime platform identifier, one of "Windows", "Linux" or "Mac OS X".
String GetPlatform();
// Return the CPU vendor string.
String GetCPUVendorString();
// Return the CPU brand string.
String GetCPUBrandString();
// Return the CPU family.
unsigned GetCPUFamily();
// Return the CPU model.
unsigned GetCPUModel();
// Return the CPU stepping.
unsigned GetCPUStepping();
// Return the CPU extended family.
unsigned GetCPUExtFamily();
// Return the CPU extended model.
unsigned GetCPUExtModel();
// Return the number of physical CPU cores.
unsigned GetNumPhysicalCPUs();
// Return the number of logical CPUs (different from physical if hyperthreading is used.)
unsigned GetNumLogicalCPUs();
// Return the total number of logical CPUs (total CPU thread count).
unsigned GetTotalLogicalCPUs();

}
