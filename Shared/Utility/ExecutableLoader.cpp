// <desc>
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include "ExecutableLoader.hpp"
#include <Container/Str.hpp>
#include <ProcessUtils.hpp>

ExecutableLoader::ExecutableLoader(const u8* origBinary)
{
    origBinary_ = origBinary;
    loadLimit_ = UINT_MAX;

    SetLibraryLoader([](const char* name)
    {
        return LoadLibraryA(name);
    });

	SetFunctionResolver([](HMODULE module, const char* name)
	{
		return (LPVOID)Util::GetProcedureAddress(module, name);
	});
}

void ExecutableLoader::LoadImports(IMAGE_NT_HEADERS* ntHeader)
{
    IMAGE_DATA_DIRECTORY* importDirectory = &ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

    auto descriptor = GetTargetRVA<IMAGE_IMPORT_DESCRIPTOR>(importDirectory->VirtualAddress);

    while (descriptor->Name)
    {
        const char* name = GetTargetRVA<char>(descriptor->Name);

        HMODULE module = ResolveLibrary(name);

        if (!module)
        {
            //FatalError("Could not load dependent module %s. Error code was %i.\n", name, GetLastError());
        }

        // "don't load"
        if (reinterpret_cast<u32>(module) == 0xFFFFFFFF)
        {
            descriptor++;
            continue;
        }

        auto nameTableEntry = GetTargetRVA<u32>(descriptor->OriginalFirstThunk);
        auto addressTableEntry = GetTargetRVA<u32>(descriptor->FirstThunk);

        // FirstThunk for original name addresses
        if (!descriptor->OriginalFirstThunk)
        {
            nameTableEntry = GetTargetRVA<u32>(descriptor->FirstThunk);
        }

        while (*nameTableEntry)
        {
            FARPROC function;
            const char* functionName;

            // Is this an ordinal-only import?
            if (IMAGE_SNAP_BY_ORDINAL(*nameTableEntry))
            {
                function = GetProcAddress(module, MAKEINTRESOURCEA(IMAGE_ORDINAL(*nameTableEntry)));
				Util::String temp;
                functionName = temp.AppendWithFormat("#%d", IMAGE_ORDINAL(*nameTableEntry)).CString();
            }
            else
            {
                auto import = GetTargetRVA<IMAGE_IMPORT_BY_NAME>(*nameTableEntry);

                function = reinterpret_cast<FARPROC>(functionResolver_(module, import->Name));
                functionName = import->Name;
            }

            if (!function)
            {
                char pathName[MAX_PATH];
                GetModuleFileNameA(module, pathName, sizeof(pathName));

                //FatalError("Could not load function %s in dependent module %s (%s).\n", functionName, name, pathName);
            }

            *addressTableEntry = (u32)function;

            nameTableEntry++;
            addressTableEntry++;
        }

        descriptor++;
    }
}

void ExecutableLoader::LoadSection(IMAGE_SECTION_HEADER* section)
{
    void* targetAddress = GetTargetRVA<u8>(section->VirtualAddress);
    const void* sourceAddress = origBinary_ + section->PointerToRawData;

    if ((uintptr_t)targetAddress >= loadLimit_)
    {
        return;
    }

    if (section->SizeOfRawData > 0)
    {
		// TODO: Replace with 'Util::Min'
        u32 sizeOfData = section->SizeOfRawData < section->Misc.VirtualSize ? section->SizeOfRawData : section->Misc.VirtualSize;

        memcpy(targetAddress, sourceAddress, sizeOfData);

        DWORD oldProtect;
        VirtualProtect(targetAddress, sizeOfData, PAGE_EXECUTE_READWRITE, &oldProtect);
    }
}

void ExecutableLoader::LoadSections(IMAGE_NT_HEADERS* ntHeader)
{
    IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION(ntHeader);

    for (int i = 0; i < ntHeader->FileHeader.NumberOfSections; i++)
    {
        LoadSection(section);

        section++;
    }
}

void ExecutableLoader::LoadIntoModule(HMODULE module)
{
    module_ = module;

    IMAGE_DOS_HEADER* header = (IMAGE_DOS_HEADER*)origBinary_;

    if (header->e_magic != IMAGE_DOS_SIGNATURE)
    {
        return;
    }

    IMAGE_DOS_HEADER* sourceHeader = (IMAGE_DOS_HEADER*)module;
    IMAGE_NT_HEADERS* sourceNtHeader = GetTargetRVA<IMAGE_NT_HEADERS>(sourceHeader->e_lfanew);

    IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS*)(origBinary_ + header->e_lfanew);

    LoadSections(ntHeader);
    LoadImports(ntHeader);

    // Copy over TLS index (source in this case indicates the TLS data to copy from, which is the launcher app itself)
    const IMAGE_TLS_DIRECTORY* targetTls = GetRVA<IMAGE_TLS_DIRECTORY>(ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
    const IMAGE_TLS_DIRECTORY* sourceTls = GetTargetRVA<IMAGE_TLS_DIRECTORY>(sourceNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);

    *(DWORD*)(targetTls->AddressOfIndex) = *(DWORD*)(sourceTls->AddressOfIndex);

    // Store the entry point
    entryPoint_ = GetTargetRVA<void>(ntHeader->OptionalHeader.AddressOfEntryPoint);

    // Copy over the offset to the new imports directory
    DWORD oldProtect;
    VirtualProtect(sourceNtHeader, 0x1000, PAGE_EXECUTE_READWRITE, &oldProtect);

    sourceNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT] = ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
}

HMODULE ExecutableLoader::ResolveLibrary(const char* name)
{
    return libraryLoader_(name);
}
