// PE32 Executable loader
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  NTAuthority
//
// Copyright (c) 2015-2017 CTNorth Team
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
    ExecutableLoader(const u8* origBinary);
    // Set memory load limit.
    void SetLoadLimit(uintptr_t loadLimit) { m_loadLimit = loadLimit; }
    // Get EXE's entry point offset.
    void* GetEntryPoint() { return m_entryPoint; }
    // Load in to the given module handle.
    void LoadIntoModule(HMODULE module);
    // Set a custom function for handling dll loading.
    void SetLibraryLoader(HMODULE(*loader)(const char*)) { m_libraryLoader = loader; }
    // Set a custom function for handling function resolving.
    void SetFunctionResolver(LPVOID (*functionResolver)(HMODULE, const char*)) { m_functionResolver = functionResolver; }

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
    template <class T> inline const T* GetRVA(unsigned rva) { return reinterpret_cast<T*>(m_origBinary + rva);}
    // Get target RVA.
    template <class T>
    inline T* GetTargetRVA(unsigned rva)
    {
        return reinterpret_cast<T*>(reinterpret_cast<char*>(m_executableHandle) + rva);
    }

    // Binary stream.
    const u8* m_origBinary;
    // Executable handle.
    HMODULE m_executableHandle;
    // Memory load limit.
    uintptr_t m_loadLimit;
    // Entry point offset.
    void* m_entryPoint;
    // LoadLibrary replacement function.
    HMODULE(*m_libraryLoader)(const char*);
    // GetProcAddress replacement function.
    LPVOID(*m_functionResolver)(HMODULE, const char*);
};
