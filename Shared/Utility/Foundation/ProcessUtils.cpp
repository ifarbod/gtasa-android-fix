// Process utils
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include <Foundation/ProcessUtils.hpp>
#include <Path.hpp>

#include <cstdio>
#include <io.h>

namespace Util
{

static bool consoleOpened = false;
static String currentLine;
static Vector<String> arguments;

void OpenConsoleWindow()
{
    if (consoleOpened)
        return;

    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    //SetConsoleTitleW(WString(MOD_NAME " Debug console").CString());

    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    // If you have multiple monitors, this will automatically move the console to the 2nd
    if (GetSystemMetrics(SM_CMONITORS) > 1)
    {
        // Console Window
        HWND hConsole = GetConsoleWindow();
        RECT rect;
        GetWindowRect(hConsole, &rect);

        // Desktop
        HWND hDesktop = GetDesktopWindow();
        RECT desktopRect;
        GetWindowRect(hDesktop, &desktopRect);

        // Update Position
        SetWindowPos(hConsole, nullptr, desktopRect.right + 100, 50, rect.right - rect.left, rect.bottom - rect.top, 0);
    }

    consoleOpened = true;
}

void PrintUnicode(const String& str, bool error)
{
    // If the output stream has been redirected, use fprintf instead of WriteConsoleW,
    // though it means that proper Unicode output will not work
    FILE* out = error ? stderr : stdout;
    if (!_isatty(_fileno(out)))
        fprintf(out, "%s", str.CString());
    else
    {
        HANDLE stream = GetStdHandle(error ? STD_ERROR_HANDLE : STD_OUTPUT_HANDLE);
        if (stream == INVALID_HANDLE_VALUE)
            return;
        WString strW(str);
        DWORD charsWritten;
        WriteConsoleW(stream, strW.CString(), strW.Length(), &charsWritten, 0);
    }
}

void PrintUnicodeLine(const String& str, bool error)
{
    PrintUnicode(str + "\n", error);
}

void PrintLine(const String& str, bool error)
{
    fprintf(error ? stderr : stdout, "%s\n", str.CString());
}

const Vector<String>& ParseArguments(const String& cmdLine, bool skipFirstArgument)
{
    arguments.Clear();

    unsigned cmdStart = 0, cmdEnd = 0;
    bool inCmd = false;
    bool inQuote = false;

    for (unsigned i = 0; i < cmdLine.Length(); ++i)
    {
        if (cmdLine[i] == '\"')
            inQuote = !inQuote;
        if (cmdLine[i] == ' ' && !inQuote)
        {
            if (inCmd)
            {
                inCmd = false;
                cmdEnd = i;
                // Do not store the first argument (executable name)
                if (!skipFirstArgument)
                    arguments.Push(cmdLine.Substring(cmdStart, cmdEnd - cmdStart));
                skipFirstArgument = false;
            }
        }
        else
        {
            if (!inCmd)
            {
                inCmd = true;
                cmdStart = i;
            }
        }
    }
    if (inCmd)
    {
        cmdEnd = cmdLine.Length();
        if (!skipFirstArgument)
            arguments.Push(cmdLine.Substring(cmdStart, cmdEnd - cmdStart));
    }

    // Strip double quotes from the arguments
    for (unsigned i = 0; i < arguments.Size(); ++i)
        arguments[i].Replace("\"", "");

    return arguments;
}

const Vector<String>& ParseArguments(const char* cmdLine)
{
    return ParseArguments(String(cmdLine));
}

const Vector<String>& ParseArguments(const WString& cmdLine)
{
    return ParseArguments(String(cmdLine));
}

const Vector<String>& ParseArguments(const wchar_t* cmdLine)
{
    return ParseArguments(String(cmdLine));
}

const Vector<String>& ParseArguments(int argc, char** argv)
{
    String cmdLine;

    for (int i = 0; i < argc; ++i)
        cmdLine.AppendWithFormat("\"%s\" ", (const char*)argv[i]);

    return ParseArguments(cmdLine);
}

const Vector<String>& GetArguments()
{
    return arguments;
}

void QuoteArguments(Vector<String>& args)
{
    for (unsigned i = 0; i < args.Size(); i++)
    {
        if (args[i].Contains(' ') && !args[i].Contains('"'))
        {
            args[i] = "\"" + args[i] + "\"";
        }
    }
}

String GetConsoleInput()
{
    String ret;
    HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (input == INVALID_HANDLE_VALUE || output == INVALID_HANDLE_VALUE)
        return ret;

    // Use char-based input
    SetConsoleMode(input, ENABLE_PROCESSED_INPUT);

    INPUT_RECORD record;
    DWORD events = 0;
    DWORD readEvents = 0;

    if (!GetNumberOfConsoleInputEvents(input, &events))
        return ret;

    while (events--)
    {
        ReadConsoleInputW(input, &record, 1, &readEvents);
        if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown)
        {
            unsigned c = record.Event.KeyEvent.uChar.UnicodeChar;
            if (c)
            {
                if (c == '\b')
                {
                    PrintUnicode("\b \b");
                    int length = currentLine.LengthUTF8();
                    if (length)
                        currentLine = currentLine.SubstringUTF8(0, length - 1);
                }
                else if (c == '\r')
                {
                    PrintUnicode("\n");
                    ret = currentLine;
                    currentLine.Clear();
                    return ret;
                }
                else
                {
                    // We have disabled echo, so echo manually
                    wchar_t out = c;
                    DWORD charsWritten;
                    WriteConsoleW(output, &out, 1, &charsWritten, 0);
                    currentLine.AppendUTF8(c);
                }
            }
        }
    }

    return ret;
}

FARPROC GetProcedureAddress(HMODULE hModule, const String& procName)
{
    PVOID functionAddress = nullptr;

    __try
    {
        PIMAGE_DOS_HEADER dos = reinterpret_cast<PIMAGE_DOS_HEADER>(hModule);
        PIMAGE_NT_HEADERS nt = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<ULONG>(hModule) + dos->e_lfanew);

        PIMAGE_DATA_DIRECTORY expdir =
            reinterpret_cast<PIMAGE_DATA_DIRECTORY>(nt->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT);
        ULONG addr = expdir->VirtualAddress;
        PIMAGE_EXPORT_DIRECTORY exports =
            reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(reinterpret_cast<ULONG>(hModule) + addr);
        PULONG functions = reinterpret_cast<PULONG>(reinterpret_cast<ULONG>(hModule) + exports->AddressOfFunctions);
        PSHORT ordinals = reinterpret_cast<PSHORT>(reinterpret_cast<ULONG>(hModule) + exports->AddressOfNameOrdinals);
        PULONG names = reinterpret_cast<PULONG>(reinterpret_cast<ULONG>(hModule) + exports->AddressOfNames);
        ULONG maxName = exports->NumberOfNames;
        ULONG maxFunc = exports->NumberOfFunctions;

        for (ULONG i = 0; i < maxName; i++)
        {
            ULONG ord = ordinals[i];
            if (i >= maxName || ord >= maxFunc)
                return nullptr;
            if (functions[ord] < addr || functions[ord] >= addr)
            {
                if (strcmp(reinterpret_cast<PCHAR>(hModule) + names[i], procName.CString()) == 0)
                {
                    functionAddress = reinterpret_cast<PVOID>((PCHAR)hModule + functions[ord]);
                    break;
                }
            }
        }
    } __except (EXCEPTION_EXECUTE_HANDLER)
    {
        functionAddress = nullptr;
    }

    return reinterpret_cast<FARPROC>(functionAddress);
}

bool CallRemoteFunction(HANDLE hProcess, const String& functionName, const String& fileName)
{
    // Store the path as wchar_t
    WString libPath = WString{fileName}.CString();
    unsigned libPathLength = libPath.Length();

    // Store kernel32 handle
    HMODULE hKernel32 = GetModuleHandleW(L"KERNEL32");

    // Store remote thread handle
    HANDLE hThread = nullptr;

    // Allocate memory in the remote process
    void* libPathRemote = nullptr;
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
    LPTHREAD_START_ROUTINE functionPtr =
        reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcedureAddress(hKernel32, functionName));

    // Does the function exist?
    if (functionPtr == nullptr)
    {
        // Clean up the resources used for injection of the DLL
        VirtualFreeEx(hProcess, libPathRemote, libPathSize, MEM_RELEASE);

        return false;
    }

    // Start a remote thread calling the desired function with the parameter
    hThread = CreateRemoteThread(hProcess, nullptr, 0, functionPtr, libPathRemote, 0, nullptr);

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
    // WaitForObject(hProcess, hThread, INFINITE, NULL);

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
