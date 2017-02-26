// PE32 Executable loader
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  NTAuthority
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <winnt.h>

// PE32 Executable loader.
class ExecutableLoader
{
public:
    // Construct an ExecutableLoader instance with given binary stream.
    ExecutableLoader(const uint8_t* origBinary);
    // Set memory load limit.
    void SetLoadLimit(uintptr_t loadLimit) { loadLimit_ = loadLimit; }
    // Get EXE's entry point offset.
    void* GetEntryPoint() { return entryPoint_; }
    // Load in to the given module handle.
    void LoadIntoModule(HMODULE module);
    // Set a custom function for handling dll loading.
    void SetLibraryLoader(HMODULE(*loader)(const char*)) { libraryLoader_ = loader; }
    // Set a custom function for handling function resolving.
    void SetFunctionResolver(LPVOID (*functionResolver)(HMODULE, const char*)) { functionResolver_ = functionResolver; }

private:
    // Load a section.
    void LoadSection(IMAGE_SECTION_HEADER* section);
    // Load all sections.
    void LoadSections(IMAGE_NT_HEADERS* ntHeader);
    // Load Imports
    bool LoadDependentLibraries(IMAGE_NT_HEADERS* ntHeader);
    // Resolve a library.
    HMODULE ResolveLibrary(const char* name);
    // Resolve a library function.
    LPVOID ResolveLibraryFunction(HMODULE module, const char* name);
    // Get RVA.
    template <class T> inline const T* GetRVA(unsigned rva) { return reinterpret_cast<T*>(origBinary_ + rva);}
    // Get target RVA.
    template <class T>
    inline T* GetTargetRVA(unsigned rva)
    {
        return reinterpret_cast<T*>(reinterpret_cast<char*>(executableHandle_) + rva);
    }

    // Binary stream.
    const uint8_t* origBinary_;
    // Executable handle.
    HMODULE executableHandle_;
    // Memory load limit.
    uintptr_t loadLimit_;
    // Entry point offset.
    void* entryPoint_;
    // LoadLibrary replacement function.
    HMODULE(*libraryLoader_)(const char*);
    // GetProcAddress replacement function.
    LPVOID(*functionResolver_)(HMODULE, const char*);
};
