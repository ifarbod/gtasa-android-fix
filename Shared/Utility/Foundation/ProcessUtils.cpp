// Process utils
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include <Foundation/ProcessUtils.hpp>
#include <Path.hpp>

#include <cstdio>
#include <fcntl.h>

#if !defined(__linux__)
#include <libcpuid/libcpuid.h>
#endif

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#if defined(_MSC_VER)
#include <float.h>
#endif
#else
#include <unistd.h>
#endif

#include <DebugNew.hpp>

namespace ctn
{

static bool consoleOpened = false;
static String currentLine;
static Vector<String> arguments;

#if defined(__linux__)
struct CpuCoreCount
{
    unsigned m_numPhysicalCores;
    unsigned m_numLogicalCores;
};

// This function is used by all the target triplets with Linux as the OS, such as Android, RPI, desktop Linux, etc
static void GetCPUData(struct CpuCoreCount* data)
{
    // Sanity check
    assert(data);
    // At least return 1 core
    data->m_numPhysicalCores = data->m_numLogicalCores = 1;

    FILE* fp;
    int res;
    unsigned i, j;

    fp = fopen("/sys/devices/system/cpu/present", "r");
    if (fp)
    {
        res = fscanf(fp, "%d-%d", &i, &j);
        fclose(fp);

        if (res == 2 && i == 0)
        {
            data->m_numPhysicalCores = data->m_numLogicalCores = j + 1;

            fp = fopen("/sys/devices/system/cpu/cpu0/topology/thread_siblings_list", "r");
            if (fp)
            {
                res = fscanf(fp, "%d,%d,%d,%d", &i, &j, &i, &j);
                fclose(fp);

                // Having sibling thread(s) indicates the CPU is using HT/SMT technology
                if (res > 1)
                    data->m_numPhysicalCores /= res;
            }
        }
    }
}

#else
static void GetCPUData(struct cpu_id_t* data)
{
    if (cpu_identify(nullptr, data) < 0)
    {
        data->num_logical_cpus = 1;
        data->num_cores = 1;
    }
}
#endif

void InitFPU()
{
    // Make sure FPU is in round-to-nearest, single precision mode
    // This ensures Direct3D and all threads behave similarly
#if defined(_MSC_VER) && defined(_M_IX86)
    _controlfp(_RC_NEAR | _PC_24, _MCW_RC | _MCW_PC);
#elif defined(__i386__)
    unsigned control = GetFPUState();
    control &= ~(FPU_CW_PREC_MASK | FPU_CW_ROUND_MASK);
    control |= (FPU_CW_PREC_SINGLE | FPU_CW_ROUND_NEAR);
    SetFPUState(control);
#endif
}

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

String GetPlatform()
{
#if defined(_WIN32)
    return "Windows";
#elif defined(__APPLE__)
    return "Mac OS X";
#elif defined(__linux__)
    return "Linux";
#else
    return String::EMPTY;
#endif
}

String GetCPUVendorString()
{
#if !defined(__linux__)
    struct cpu_id_t data;
    GetCPUData(&data);
    return data.vendor_str;
#endif
}

String GetCPUBrandString()
{
#if !defined(__linux__)
    struct cpu_id_t data;
    GetCPUData(&data);
    return data.brand_str;
#endif
}

unsigned GetCPUFamily()
{
#if !defined(__linux__)
    struct cpu_id_t data;
    GetCPUData(&data);
    return static_cast<unsigned>(data.family);
#endif
}

unsigned GetCPUModel()
{
#if !defined(__linux__)
    struct cpu_id_t data;
    GetCPUData(&data);
    return static_cast<unsigned>(data.model);
#endif
}

unsigned GetCPUStepping()
{
#if !defined(__linux__)
    struct cpu_id_t data;
    GetCPUData(&data);
    return static_cast<unsigned>(data.stepping);
#endif
}

unsigned GetCPUExtFamily()
{
#if !defined(__linux__)
    struct cpu_id_t data;
    GetCPUData(&data);
    return static_cast<unsigned>(data.ext_family);
#endif
}

unsigned GetCPUExtModel()
{
#if !defined(__linux__)
    struct cpu_id_t data;
    GetCPUData(&data);
    return static_cast<unsigned>(data.ext_model);
#endif
}

unsigned GetNumPhysicalCPUs()
{
#if defined(__linux__)
    struct CpuCoreCount data;
#else
    struct cpu_id_t data;
#endif

    GetCPUData(&data);

#if defined(__linux__)
    return data.m_numPhysicalCores;
#else
    return static_cast<unsigned>(data.num_cores);
#endif
}

unsigned GetNumLogicalCPUs()
{
#if defined(__linux__)
    struct CpuCoreCount data;
#else
    struct cpu_id_t data;
#endif

    GetCPUData(&data);

#if defined(__linux__)
    return data.m_numLogicalCores;
#else
    return static_cast<unsigned>(data.num_logical_cpus);
#endif
}

unsigned GetTotalLogicalCPUs()
{
#if !defined(__linux__)
    struct cpu_id_t data;
    GetCPUData(&data);
    return static_cast<unsigned>(data.total_logical_cpus);
#endif
}

bool IsUserAdmin()
{
#if defined(_WIN32)
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    PSID administratorsGroup;
    auto b = AllocateAndInitializeSid(
        &ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &administratorsGroup);

    if (b)
    {
        if (!CheckTokenMembership(NULL, administratorsGroup, &b))
        {
            b = FALSE;
        }
        FreeSid(administratorsGroup);
    }

    return !!b;
#else
    return false;
#endif
}

}
