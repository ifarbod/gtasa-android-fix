// A bit of PE stuff
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 San Andreas Online
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

namespace Utils
{

FARPROC GetProcAddr(HMODULE hModule, const char* pszProcName)
{
    PVOID pFunctionAddress = nullptr;

    __try
    {
        PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)hModule;
        PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)((ULONG)hModule + dos->e_lfanew);

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
            if (i >= max_name || ord >= max_func) return NULL;
            if (functions[ord] < addr || functions[ord] >= addr)
            {
                if (strcmp((PCHAR)hModule + names[i], pszProcName) == 0)
                {
                    pFunctionAddress = (PVOID)((PCHAR)hModule + functions[ord]);
                    break;
                }
            }
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        pFunctionAddress = NULL;
    }

    return (FARPROC)pFunctionAddress;
}

}
