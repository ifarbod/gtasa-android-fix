// PE32 Loader
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  NTAuthority
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include "Main.hpp"

#include <sdk/MemoryUtils.stream.h>

#include <sstream>

ExecutableLoader::ExecutableLoader(const uint8_t* origBinary, size_t binarySize)
{
    struct memstr : public PEStream
    {
    private:
        struct nullBufferManager
        {
            inline void EstablishBufferView(const void*& memPtr, pe_file_ptr_t& bufSize, long long reqSize)
            {
                // Nothing to do.
                return;
            }
        };

        nullBufferManager allocMan;

        memoryBufferStream <pe_file_ptr_t, nullBufferManager, true, false> memStream;

    public:
        inline memstr(const char *origBinary, size_t binarySize)
            : memStream(origBinary, binarySize, allocMan)
        {
            return;
        }

        size_t Read(void *buf, size_t readCount) override
        {
            return this->memStream.Read(buf, readCount);
        }

        bool Write(const void *buf, size_t writeCount) override
        {
            return false;
        }

        bool Seek(pe_file_ptr_t ptr) override
        {
            this->memStream.Seek( ptr );

            return true;
        }

        pe_file_ptr_t Tell() const override
        {
            return this->memStream.Tell();
        }
    };

    memstr memstr((const char*)origBinary, binarySize);

    // TODO: handle exceptions.

    try
    {
        this->binary.LoadFromDisk(&memstr);
    }
    catch( ... )
    {
        // PEFramework could throw exceptions, if EXE invalid.

        throw;
    }

    loadLimit_ = UINT_MAX;

    SetLibraryLoader([] (const char* name)
    {
        return LoadLibraryA(name);
    });

    SetFunctionResolver([](HMODULE module, const char* name)
    {
        return (LPVOID)GetProcAddress(module, name);
    });
}

void ExecutableLoader::LoadImports()
{
    for ( const PEFile::PEImportDesc& impDesc : this->binary.imports )
    {
        const char* name = impDesc.DLLName.c_str();

        HMODULE module = ResolveLibrary(name);

        if (!module)
        {
            FatalError(va("Could not load dependent module %s. Error code was %i.", name, GetLastError()));
        }

        if (reinterpret_cast<uint32_t>(module) == 0xFFFFFFFF)
        {
            continue;
        }

        auto addressTableEntry = GetTargetRVA<uint32_t>(impDesc.firstThunkRef.GetRVA());

        for ( const PEFile::PEImportDesc::importFunc& impFunc : impDesc.funcs )
        {
            FARPROC function;
            const char* functionName;

            if ( impFunc.isOrdinalImport )
            {
                function = reinterpret_cast<FARPROC>(
                    ResolveLibraryFunction(module,
                        MAKEINTRESOURCEA(impFunc.ordinal_hint)
                    )
                );
                functionName = va("#%d", impFunc.ordinal_hint);
            }
            else
            {
                function = reinterpret_cast<FARPROC>(ResolveLibraryFunction(module, impFunc.name.c_str()));
                functionName = (const char*)impFunc.name.c_str();
            }

            if (!function)
            {
                char pathName[MAX_PATH];
                GetModuleFileNameA(module, pathName, sizeof(pathName));

                FatalError(va("Could not load function %s in dependent module %s (%s).", functionName, name, pathName));
            }

            *addressTableEntry = (uint32_t)function;

            addressTableEntry++;
        }
    }
}

void ExecutableLoader::LoadSection(PEFile::PESection& sect)
{
    void* targetAddress = GetTargetRVA<uint8_t>(sect.GetVirtualAddress());

    if ((uintptr_t)targetAddress >= loadLimit_)
    {
        FatalError("Exceeded load limit.");
        return;
    }

    if ( std::int32_t rawSize = sect.stream.Size() )
    {
        // TODO: maybe set correct protection status, not required of course.

        DWORD oldProtect;
        VirtualProtect(targetAddress, sect.GetVirtualSize(), PAGE_EXECUTE_READWRITE, &oldProtect);

        sect.stream.Seek( 0 );
        sect.stream.Read( targetAddress, rawSize );
    }
}

void ExecutableLoader::LoadSections()
{
    this->binary.ForAllSections(
        [&]( PEFile::PESection *sect )
        {
            LoadSection( *sect );
        }
    );
}

void ExecutableLoader::LoadIntoModule(HMODULE module)
{
    executableHandle_ = module;

    LoadSections();
    LoadImports();

    entryPoint_ = GetTargetRVA<void>(this->binary.peOptHeader.addressOfEntryPoint);

    // Implement this following code if you want to support packers crackers hackers meow.
#if 0
    IMAGE_DOS_HEADER* sourceHeader = (IMAGE_DOS_HEADER*)module;
    IMAGE_NT_HEADERS* sourceNtHeader = GetTargetRVA<IMAGE_NT_HEADERS>(sourceHeader->e_lfanew);

    DWORD oldProtect;
    VirtualProtect(sourceNtHeader, 0x1000, PAGE_EXECUTE_READWRITE, &oldProtect);

    sourceNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT] = ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
#endif
}

HMODULE ExecutableLoader::ResolveLibrary(const char* name)
{
    return libraryLoader_(name);
}

LPVOID ExecutableLoader::ResolveLibraryFunction(HMODULE module, const char* name)
{
    return functionResolver_(module, name);
}
