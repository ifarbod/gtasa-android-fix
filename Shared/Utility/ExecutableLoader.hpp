// <desc>
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Windows.h>
#include <Types.hpp>
#include <winnt.h>

class ExecutableLoader
{
private:
    const u8* origBinary_;
    HMODULE module_;
    uintptr_t loadLimit_;

    void* entryPoint_;

    HMODULE(*libraryLoader_)(const char*);
	LPVOID(*functionResolver_)(HMODULE, const char*);

private:
    void LoadSection(IMAGE_SECTION_HEADER* section);
    void LoadSections(IMAGE_NT_HEADERS* ntHeader);

    void LoadImports(IMAGE_NT_HEADERS* ntHeader);

    HMODULE ResolveLibrary(const char* name);

    template<typename T>
    inline const T* GetRVA(u32 rva)
    {
        return (T*)(origBinary_ + rva);
    }

    template<typename T>
    inline T* GetTargetRVA(u32 rva)
    {
        return (T*)((u8*)module_ + rva);
    }

public:
    ExecutableLoader(const u8* origBinary);

    inline void SetLoadLimit(uintptr_t loadLimit)
    {
        loadLimit_ = loadLimit;
    }

    inline void SetLibraryLoader(HMODULE(*loader)(const char*))
    {
        libraryLoader_ = loader;
    }

	inline void SetFunctionResolver(LPVOID(*functionResolver)(HMODULE, const char*))
	{
		functionResolver_ = functionResolver;
	}

    inline void* GetEntryPoint()
    {
        return entryPoint_;
    }

    void LoadIntoModule(HMODULE module);
};
