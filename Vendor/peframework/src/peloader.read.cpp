// I am (more or less) thankful of certain other people that have done research on PE loading aswell,
// such as...
//  http://python-pefile.sourcearchive.com/
//  https://code.google.com/archive/p/corkami/wikis/PE.wiki

// Deserialization of PE files.
#include "peloader.h"

#include "peloader.internal.hxx"

void PEFile::PEFileSpaceData::ReadFromFile( PEStream *peStream, const PESectionMan& sections, std::uint32_t rva, std::uint32_t filePtr, std::uint32_t dataSize )
{
    // Determine the storage type of this debug information.
    eStorageType storageType;

    if ( rva != 0 )
    {
        // Since we are allocated on address space, we assume its entirely allocated.
        // There is no special storage required.
        storageType = eStorageType::SECTION;
    }
    else if ( filePtr != 0 )
    {
        // Being placed out-of-band is very interesting because you essentially are an
        // attachment appended to the PE file, basically being not a part of it at all.
        // This storage type is assumingly legacy.
        storageType = eStorageType::FILE;
    }
    else
    {
        // Otherwise we can have no storage at all, which is fine.
        storageType = eStorageType::NONE;
    }

    this->storageType = storageType;

    // Register data presence depending on the storage type.
    if ( storageType == eStorageType::SECTION )
    {
        PESection *fileDataSect;

        bool gotLocation = sections.GetPEDataLocation( rva, NULL, &fileDataSect );

        if ( !gotLocation )
        {
            throw std::exception( "failed to get section location of PE file-space data" );
        }

        fileDataSect->SetPlacedMemory( this->sectRef, rva, dataSize );
    }
    else if ( storageType == eStorageType::FILE )
    {
        // In this case we have to read the data out of the file manually.
        // After all, debug information is a 'special citizen' of the PE standard.
        peStream->Seek( filePtr );

        this->fileRef.resize( dataSize );

        size_t readCount = peStream->Read( this->fileRef.data(), dataSize );

        if ( readCount != dataSize )
        {
            throw std::exception( "truncated PE file-space data error" );
        }
    }
    // Having no storage is perfectly fine.
}

static AINLINE std::uint32_t VA2RVA( std::uint64_t va, std::uint64_t imageBase )
{
    if ( va == 0 )
        return 0;

    return (std::uint32_t)( va - imageBase );
}

PEFile::PEImportDesc::functions_t PEFile::PEImportDesc::ReadPEImportFunctions( PESectionMan& sections, std::uint32_t rva, PESectionAllocation& allocEntry, bool is64Bit )
{
    PESection *importNameArraySect;
    PEDataStream importNameArrayStream;
    {
        bool hasStream = sections.GetPEDataStream( rva, importNameArrayStream, &importNameArraySect );

        if ( !hasStream )
            throw std::exception( "failed to read PE import function name array" );
    }
                    
    importNameArraySect->SetPlacedMemory( allocEntry, rva );

    // The array goes on until a terminating NULL.
    functions_t funcs;

    while ( true )
    {
        // Read the entry properly.
        std::uint64_t importNameRVA;

        if ( is64Bit )
        {
            std::uint64_t importNameRVA_read;
            importNameArrayStream.Read( &importNameRVA_read, sizeof( importNameRVA_read ) );

            importNameRVA = importNameRVA_read;
        }
        else
        {
            std::uint32_t importNameRVA_read;
            importNameArrayStream.Read( &importNameRVA_read, sizeof( importNameRVA_read ) );

            importNameRVA = importNameRVA_read;
        }

        if ( !importNameRVA )
            break;

        PEImportDesc::importFunc funcInfo;

        // Check if this is an ordinal import or a named import.
        bool isOrdinalImport;

        if ( is64Bit )
        {
            isOrdinalImport = ( importNameRVA & PEL_IMAGE_ORDINAL_FLAG64 ) != 0;
        }
        else
        {
            isOrdinalImport = ( importNameRVA & PEL_IMAGE_ORDINAL_FLAG32 ) != 0;
        }

        if ( isOrdinalImport )
        {
            // According to the documentation ordinals are 16bit numbers.

            // The documentation says that even for PE32+ the number stays 31bit.
            // It is really weird that this was made a 64bit number tho.
            funcInfo.ordinal_hint = (std::uint16_t)importNameRVA;
        }
        else
        {
            PESection *importNameSect;
            PEDataStream importNameStream;
            {
                bool gotStream = sections.GetPEDataStream( (std::uint32_t)importNameRVA, importNameStream, &importNameSect );

                if ( !gotStream )
                    throw std::exception( "failed to read PE import function name entry" );
            }

            importNameSect->SetPlacedMemory( funcInfo.nameAllocEntry, (std::uint32_t)importNameRVA );

            // Read stuff.
            std::uint16_t ordinal_hint;
            importNameStream.Read( &ordinal_hint, sizeof(ordinal_hint) );

            funcInfo.ordinal_hint = ordinal_hint;

            ReadPEString( importNameStream, funcInfo.name );
        }
        funcInfo.isOrdinalImport = isOrdinalImport;
                        
        funcs.push_back( std::move( funcInfo ) );
    }

    return funcs;
}

void PEFile::LoadFromDisk( PEStream *peStream )
{
    // We read the DOS stub.
    DOSStub dos;

    // Cache some properties.
    std::int32_t peFileStartOffset;
    {
        // It's data is expected to have no complicated things
        PEStructures::IMAGE_DOS_HEADER dosHeader;

        bool couldReadDOS = peStream->ReadStruct( dosHeader );

        if ( !couldReadDOS )
            throw std::exception( "cannot read MSDOS header" );

        // Verify DOS header (light).
        bool isValidDOSHeader =
            ( dosHeader.e_magic == 'ZM' );

        if ( !isValidDOSHeader )
            throw std::exception( "invalid MSDOS checksum" );

        // Save all information about the DOS stub.
        dos.cblp = dosHeader.e_cblp;
        dos.cp = dosHeader.e_cp;
        dos.crlc = dosHeader.e_crlc;
        dos.cparhdr = dosHeader.e_cparhdr;
        dos.minalloc = dosHeader.e_minalloc;
        dos.maxalloc = dosHeader.e_maxalloc;
        dos.ss = dosHeader.e_ss;
        dos.sp = dosHeader.e_sp;
        dos.csum = dosHeader.e_csum;
        dos.ip = dosHeader.e_ip;
        dos.cs = dosHeader.e_cs;
        dos.lfarlc = dosHeader.e_lfarlc;
        dos.ovno = dosHeader.e_ovno;
        memcpy( dos.reserved1, dosHeader.e_res, sizeof( dos.reserved1 ) );
        dos.oemid = dosHeader.e_oemid;
        dos.oeminfo = dosHeader.e_oeminfo;
        memcpy( dos.reserved2, dosHeader.e_res2, sizeof( dos.reserved2 ) );

        // We need the program data aswell.
        // Assumption is that the data directly follows the header and ends in the new data ptr.
        {
            std::int32_t newDataOffset = dosHeader.e_lfanew;

            std::int32_t sizeOfStubData = ( newDataOffset - sizeof( dosHeader ) );

            assert( sizeOfStubData >= 0 );

            std::vector <unsigned char> progData( sizeOfStubData );
            {
                size_t progReadCount = peStream->Read( progData.data(), sizeOfStubData );

                if ( progReadCount != sizeOfStubData )
                {
                    throw std::exception( "invalid MSDOS stub" );
                }
            }

            dos.progData = std::move( progData );
        }

        peFileStartOffset = dosHeader.e_lfanew;
    }

    // Go on to the PE header.
    PEFileInfo peInfo;

    // The loader runtime needs to know if we are PE32 or PE32+.
    bool is64Bit;

    // Cache some properties.
    std::uint16_t numSections;
    {
        bool seekSuccess = peStream->Seek( peFileStartOffset );

        assert( seekSuccess == true );

        // Read PE information.
        PEStructures::IMAGE_PE_HEADER peHeader;

        bool couldReadPE = peStream->ReadStruct( peHeader );

        if ( couldReadPE == false )
            throw std::exception( "failed to read PE NT headers" );

        // Validate some things.
        if ( peHeader.Signature != 'EP' )
            throw std::exception( "invalid PE header signature" );

        // We only support machine types we know.
        std::int16_t machineType = peHeader.FileHeader.Machine;
        {
            if ( machineType == PEL_IMAGE_FILE_MACHINE_I386 )
            {
                is64Bit = false;
            }
            else if ( machineType == PEL_IMAGE_FILE_MACHINE_AMD64 )
            {
                is64Bit = true;
            }
            else
            {
                throw std::exception( "unsupported PE file machine type" );
            }
        }

        // Store stuff.
        peInfo.machine_id = machineType;
        peInfo.timeDateStamp = peHeader.FileHeader.TimeDateStamp;
    
        // Flags that matter.
        std::uint16_t chars = peHeader.FileHeader.Characteristics;

        peInfo.isExecutableImage        = ( chars & PEL_IMAGE_FILE_EXECUTABLE_IMAGE ) != 0;
        peInfo.hasLocalSymbols          = ( chars & PEL_IMAGE_FILE_LOCAL_SYMS_STRIPPED ) == 0;
        peInfo.hasAggressiveTrim        = ( chars & PEL_IMAGE_FILE_AGGRESIVE_WS_TRIM ) != 0;
        peInfo.largeAddressAware        = ( chars & PEL_IMAGE_FILE_LARGE_ADDRESS_AWARE ) != 0;
        peInfo.bytesReversedLO          = ( chars & PEL_IMAGE_FILE_BYTES_REVERSED_LO ) != 0;
        peInfo.removableRunFromSwap     = ( chars & PEL_IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP ) != 0;
        peInfo.netRunFromSwap           = ( chars & PEL_IMAGE_FILE_NET_RUN_FROM_SWAP ) != 0;
        peInfo.isSystemFile             = ( chars & PEL_IMAGE_FILE_SYSTEM ) != 0;
        peInfo.isDLL                    = ( chars & PEL_IMAGE_FILE_DLL ) != 0;
        peInfo.upSystemOnly             = ( chars & PEL_IMAGE_FILE_UP_SYSTEM_ONLY ) != 0;
        peInfo.bytesReversedHI          = ( chars & PEL_IMAGE_FILE_BYTES_REVERSED_HI ) != 0;

        // Other properties should be respected during parsing.
        bool hasRelocsStripped          = ( chars & PEL_IMAGE_FILE_RELOCS_STRIPPED ) != 0;
        bool hasLineNumsStripped        = ( chars & PEL_IMAGE_FILE_LINE_NUMS_STRIPPED ) != 0;
        bool hasLocalSymsStripped       = ( chars & PEL_IMAGE_FILE_LOCAL_SYMS_STRIPPED ) != 0;
        bool hasDebugStripped           = ( chars & PEL_IMAGE_FILE_DEBUG_STRIPPED ) != 0;
    
        // Check if the 32bit flag matches what we know.
        {
            bool flag_is32bit = ( chars & PEL_IMAGE_FILE_32BIT_MACHINE ) != 0;

            if ( flag_is32bit != !is64Bit )
            {
                throw std::exception( "charactersitics define 32bit PE file while machine type says otherwise" );
            }
        }

        // Remember that we were here.
        pe_file_ptr_t optionalHeaderOffset = peStream->Tell();

        // We should definately try reading symbol information.
        std::uint32_t symbolOffset = peHeader.FileHeader.PointerToSymbolTable;
        std::uint32_t numOfSymbols = peHeader.FileHeader.NumberOfSymbols;

        if ( symbolOffset != 0 && numOfSymbols != 0 )
        {
            // Try locating the symbols and read them!
            peStream->Seek( symbolOffset );

            // Do it meow.
            throw std::exception( "unsupported COFF debug information format" );

            // Move back to the optional header we should read next.
            peStream->Seek( optionalHeaderOffset );
        }

        numSections = peHeader.FileHeader.NumberOfSections;

        // Verify that we have a proper optional header size.
        std::uint16_t optHeaderSize = peHeader.FileHeader.SizeOfOptionalHeader;

        bool hasValidOptionalHeaderSize;

        if ( is64Bit )
        {
            hasValidOptionalHeaderSize = ( optHeaderSize == sizeof(PEStructures::IMAGE_OPTIONAL_HEADER64) );
        }
        else
        {
            hasValidOptionalHeaderSize = ( optHeaderSize == sizeof(PEStructures::IMAGE_OPTIONAL_HEADER32) );
        }

        if ( !hasValidOptionalHeaderSize )
        {
            throw std::exception( "invalid optional header size" );
        }
    }

    // Let's read our optional header!
    PEOptHeader peOpt;

    // We have to extract this.
    std::uint32_t sectionAlignment;
    PEStructures::IMAGE_DATA_DIRECTORY dataDirs[PEL_IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
    {
        std::uint16_t dllChars;

        if ( is64Bit )
        {
            PEStructures::IMAGE_OPTIONAL_HEADER64 optHeader;

            bool readOptHeader = peStream->ReadStruct( optHeader );

            if ( !readOptHeader )
                throw std::exception( "reading optional header failed" );

            // Verify magic number.
            if ( optHeader.Magic != 0x010B )
                throw std::exception( "invalid optional header magic number" );

            // Fetch the information.
            // We will store the pointers in 64bit format regardless of the machine type.
            // This is to keep a simple data layout.
            peOpt.majorLinkerVersion = optHeader.MajorLinkerVersion;
            peOpt.minorLinkerVersion = optHeader.MinorLinkerVersion;
            peOpt.sizeOfCode = optHeader.SizeOfCode;
            peOpt.sizeOfInitializedData = optHeader.SizeOfInitializedData;
            peOpt.sizeOfUninitializedData = optHeader.SizeOfUninitializedData;
            peOpt.addressOfEntryPoint = optHeader.AddressOfEntryPoint;
            peOpt.baseOfCode = optHeader.BaseOfCode;
            peOpt.baseOfData = 0;   // not available.
            peOpt.imageBase = optHeader.ImageBase;
            peOpt.fileAlignment = optHeader.FileAlignment;
            peOpt.majorOSVersion = optHeader.MajorOperatingSystemVersion;
            peOpt.minorOSVersion = optHeader.MinorOperatingSystemVersion;
            peOpt.majorImageVersion = optHeader.MajorImageVersion;
            peOpt.minorImageVersion = optHeader.MinorImageVersion;
            peOpt.majorSubsysVersion = optHeader.MajorSubsystemVersion;
            peOpt.minorSubsysVersion = optHeader.MinorSubsystemVersion;
            peOpt.win32VersionValue = optHeader.Win32VersionValue;
            peOpt.sizeOfImage = optHeader.SizeOfImage;
            peOpt.sizeOfHeaders = optHeader.SizeOfHeaders;
            peOpt.checkSum = optHeader.CheckSum;
            peOpt.subsys = optHeader.Subsystem;
            dllChars = optHeader.DllCharacteristics;
            peOpt.sizeOfStackReserve = optHeader.SizeOfStackReserve;
            peOpt.sizeOfStackCommit = optHeader.SizeOfStackCommit;
            peOpt.sizeOfHeapReserve = optHeader.SizeOfHeapReserve;
            peOpt.sizeOfHeapCommit = optHeader.SizeOfHeapCommit;
            peOpt.loaderFlags = optHeader.LoaderFlags;

            // Extract the section alignment.
            sectionAlignment = optHeader.SectionAlignment;

            // Extract the data directory information.
            std::uint32_t numDataDirs = optHeader.NumberOfRvaAndSizes;

            if ( numDataDirs != PEL_IMAGE_NUMBEROF_DIRECTORY_ENTRIES )
                throw std::exception( "invalid number of PE directory entries" );

            memcpy( dataDirs, optHeader.DataDirectory, sizeof( dataDirs ) );
        }
        else
        {
            PEStructures::IMAGE_OPTIONAL_HEADER32 optHeader;

            bool readOptHeader = peStream->ReadStruct( optHeader );

            if ( !readOptHeader )
                throw std::exception( "reading optional header failed" );

            // Verify magic number.
            if ( optHeader.Magic != 0x010B )
                throw std::exception( "invalid optional header magic number" );

            // Fetch the information.
            // We will store the pointers in 64bit format regardless of the machine type.
            // This is to keep a simple data layout.
            peOpt.majorLinkerVersion = optHeader.MajorLinkerVersion;
            peOpt.minorLinkerVersion = optHeader.MinorLinkerVersion;
            peOpt.sizeOfCode = optHeader.SizeOfCode;
            peOpt.sizeOfInitializedData = optHeader.SizeOfInitializedData;
            peOpt.sizeOfUninitializedData = optHeader.SizeOfUninitializedData;
            peOpt.addressOfEntryPoint = optHeader.AddressOfEntryPoint;
            peOpt.baseOfCode = optHeader.BaseOfCode;
            peOpt.baseOfData = optHeader.BaseOfData;
            peOpt.imageBase = optHeader.ImageBase;
            peOpt.fileAlignment = optHeader.FileAlignment;
            peOpt.majorOSVersion = optHeader.MajorOperatingSystemVersion;
            peOpt.minorOSVersion = optHeader.MinorOperatingSystemVersion;
            peOpt.majorImageVersion = optHeader.MajorImageVersion;
            peOpt.minorImageVersion = optHeader.MinorImageVersion;
            peOpt.majorSubsysVersion = optHeader.MajorSubsystemVersion;
            peOpt.minorSubsysVersion = optHeader.MinorSubsystemVersion;
            peOpt.win32VersionValue = optHeader.Win32VersionValue;
            peOpt.sizeOfImage = optHeader.SizeOfImage;
            peOpt.sizeOfHeaders = optHeader.SizeOfHeaders;
            peOpt.checkSum = optHeader.CheckSum;
            peOpt.subsys = optHeader.Subsystem;
            dllChars = optHeader.DllCharacteristics;
            peOpt.sizeOfStackReserve = optHeader.SizeOfStackReserve;
            peOpt.sizeOfStackCommit = optHeader.SizeOfStackCommit;
            peOpt.sizeOfHeapReserve = optHeader.SizeOfHeapReserve;
            peOpt.sizeOfHeapCommit = optHeader.SizeOfHeapCommit;
            peOpt.loaderFlags = optHeader.LoaderFlags;

            // Extract the section alignment.
            sectionAlignment = optHeader.SectionAlignment;

            // Extract the data directory information.
            std::uint32_t numDataDirs = optHeader.NumberOfRvaAndSizes;

            if ( numDataDirs != PEL_IMAGE_NUMBEROF_DIRECTORY_ENTRIES )
                throw std::exception( "invalid number of PE directory entries" );

            // Extract the data directory information.
            memcpy( dataDirs, optHeader.DataDirectory, sizeof( dataDirs ) );
        }

        // Process the DLL flags and store them sensibly.
        peOpt.dll_supportsHighEntropy =     ( dllChars & PEL_IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA ) != 0;
        peOpt.dll_hasDynamicBase =          ( dllChars & PEL_IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE ) != 0;
        peOpt.dll_forceIntegrity =          ( dllChars & PEL_IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY ) != 0;
        peOpt.dll_nxCompat =                ( dllChars & PEL_IMAGE_DLLCHARACTERISTICS_NX_COMPAT ) != 0;
        peOpt.dll_noIsolation =             ( dllChars & PEL_IMAGE_DLLCHARACTERISTICS_NO_ISOLATION ) != 0;
        peOpt.dll_noSEH =                   ( dllChars & PEL_IMAGE_DLLCHARACTERISTICS_NO_ISOLATION ) != 0;
        peOpt.dll_noBind =                  ( dllChars & PEL_IMAGE_DLLCHARACTERISTICS_NO_BIND ) != 0;
        peOpt.dll_appContainer =            ( dllChars & PEL_IMAGE_DLLCHARACTERISTICS_APPCONTAINER ) != 0;
        peOpt.dll_wdmDriver =               ( dllChars & PEL_IMAGE_DLLCHARACTERISTICS_WDM_DRIVER ) != 0;
        peOpt.dll_guardCF =                 ( dllChars & PEL_IMAGE_DLLCHARACTERISTICS_GUARD_CF ) != 0;
        peOpt.dll_termServAware =           ( dllChars & PEL_IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE ) != 0;
    }

    // Should handle data sections first because data directories depend on them.
    PESectionMan sections( sectionAlignment, peOpt.baseOfCode );

    for ( size_t n = 0; n < numSections; n++ )
    {
        PEStructures::IMAGE_SECTION_HEADER sectHeader;

        bool readSection = peStream->ReadStruct( sectHeader );

        if ( !readSection )
            throw std::exception( "failed to read PE section header" );

        pe_file_ptr_t sectHeaderOff = peStream->Tell();

        PESection section;
        section.shortName = std::string( (const char*)sectHeader.Name, strnlen( (const char*)sectHeader.Name, _countof(sectHeader.Name) ) );
        section.SetPlacementInfo( sectHeader.VirtualAddress, sectHeader.Misc.VirtualSize );
        
        // Save characteristics flags.
        std::uint32_t schars = sectHeader.Characteristics;

        section.chars.sect_hasNoPadding             = ( schars & PEL_IMAGE_SCN_TYPE_NO_PAD ) != 0;
        section.chars.sect_containsCode             = ( schars & PEL_IMAGE_SCN_CNT_CODE ) != 0;
        section.chars.sect_containsInitData         = ( schars & PEL_IMAGE_SCN_CNT_INITIALIZED_DATA ) != 0;
        section.chars.sect_containsUninitData       = ( schars & PEL_IMAGE_SCN_CNT_UNINITIALIZED_DATA ) != 0;
        section.chars.sect_link_other               = ( schars & PEL_IMAGE_SCN_LNK_OTHER ) != 0;
        section.chars.sect_link_info                = ( schars & PEL_IMAGE_SCN_LNK_INFO ) != 0;
        section.chars.sect_link_remove              = ( schars & PEL_IMAGE_SCN_LNK_REMOVE ) != 0;
        section.chars.sect_link_comdat              = ( schars & PEL_IMAGE_SCN_LNK_COMDAT ) != 0;
        section.chars.sect_noDeferSpecExcepts       = ( schars & PEL_IMAGE_SCN_NO_DEFER_SPEC_EXC ) != 0;
        section.chars.sect_gprel                    = ( schars & PEL_IMAGE_SCN_GPREL ) != 0;
        section.chars.sect_mem_farData              = ( schars & PEL_IMAGE_SCN_MEM_FARDATA ) != 0;
        section.chars.sect_mem_purgeable            = ( schars & PEL_IMAGE_SCN_MEM_PURGEABLE ) != 0;
        section.chars.sect_mem_16bit                = ( schars & PEL_IMAGE_SCN_MEM_16BIT ) != 0;
        section.chars.sect_mem_locked               = ( schars & PEL_IMAGE_SCN_MEM_LOCKED ) != 0;
        section.chars.sect_mem_preload              = ( schars & PEL_IMAGE_SCN_MEM_PRELOAD ) != 0;
        
        // Parse the alignment information out of the chars.
        PESection::eAlignment alignNum = (PESection::eAlignment)( ( schars & 0x00F00000 ) >> 20 );
        section.chars.sect_alignment = alignNum;

        section.chars.sect_link_nreloc_ovfl         = ( schars & PEL_IMAGE_SCN_LNK_NRELOC_OVFL ) != 0;
        section.chars.sect_mem_discardable          = ( schars & PEL_IMAGE_SCN_MEM_DISCARDABLE ) != 0;
        section.chars.sect_mem_not_cached           = ( schars & PEL_IMAGE_SCN_MEM_NOT_CACHED ) != 0;
        section.chars.sect_mem_not_paged            = ( schars & PEL_IMAGE_SCN_MEM_NOT_PAGED ) != 0;
        section.chars.sect_mem_shared               = ( schars & PEL_IMAGE_SCN_MEM_SHARED ) != 0;
        section.chars.sect_mem_execute              = ( schars & PEL_IMAGE_SCN_MEM_EXECUTE ) != 0;
        section.chars.sect_mem_read                 = ( schars & PEL_IMAGE_SCN_MEM_READ ) != 0;
        section.chars.sect_mem_write                = ( schars & PEL_IMAGE_SCN_MEM_WRITE ) != 0;

        // Read raw data.
        {
            peStream->Seek( sectHeader.PointerToRawData );

            section.stream.Truncate( (std::uint32_t)sectHeader.SizeOfRawData );

            size_t actualReadCount = peStream->Read( section.stream.Data(), sectHeader.SizeOfRawData );

            if ( actualReadCount != sectHeader.SizeOfRawData )
                throw std::exception( "failed to read PE section raw data" );
        }

        // Read relocation information.
        {
            peStream->Seek( sectHeader.PointerToRelocations );

            std::vector <PERelocation> relocs;
            relocs.reserve( sectHeader.NumberOfRelocations );

            for ( std::uint32_t n = 0; n < sectHeader.NumberOfRelocations; n++ )
            {
                PEStructures::IMAGE_RELOCATION relocEntry;

                bool readReloc = peStream->ReadStruct( relocEntry );

                if ( !readReloc )
                    throw std::exception( "failed to read PE section relocation information" );

                // Store it.
                PERelocation data;
                data.virtAddr = relocEntry.VirtualAddress;
                data.symbolTableIndex = relocEntry.SymbolTableIndex;
                data.type = relocEntry.Type;

                relocs.push_back( std::move( data ) );
            }

            section.relocations = std::move( relocs );
        }

        // Read linenumber information.
        {
            peStream->Seek( sectHeader.PointerToLinenumbers );

            std::vector <PELinenumber> linenums;
            linenums.reserve( sectHeader.NumberOfLinenumbers );

            for ( size_t n = 0; n < sectHeader.NumberOfLinenumbers; n++ )
            {
                PEStructures::IMAGE_LINENUMBER lineInfo;

                bool gotLinenum = peStream->ReadStruct( lineInfo );

                if ( !gotLinenum )
                    throw std::exception( "failed to read PE linenumber info" );

                PELinenumber line;
                line.symTableIndex = lineInfo.Type.SymbolTableIndex;
                line.number = lineInfo.Linenumber;

                linenums.push_back( std::move( line ) );
            }

            section.linenumbers = std::move( linenums );
        }

        // We need to set our stream back on track.
        peStream->Seek( sectHeaderOff );

        // Remember this section.
        bool regSuccess = ( sections.PlaceSection( std::move( section ) ) != NULL );

        if ( !regSuccess )
        {
            throw std::exception( "invalid PE section configuration" );
        }
    }

    // This is the (official) end of the executable data reading.
    // Now we dispatch onto the data directories, which base on things found inside the sections.

    // NOTE: the DEBUG DIRECTORY probably needs to use peStream again.

    // If we ever encounter an absolute VA during deserialization, we need to subtract the image base.
    const std::uint64_t imageBase = peOpt.imageBase;

    // Load the directory information now.
    // We decide to create meta-data structs out of them.
    // If possible, delete the section that contains the meta-data.
    // * EXPORT INFORMATION.
    PEExportDir expInfo;
    {
        const PEStructures::IMAGE_DATA_DIRECTORY& expDirEntry = dataDirs[ PEL_IMAGE_DIRECTORY_ENTRY_EXPORT ];

        if ( expDirEntry.VirtualAddress != 0 )
        {
            PESection *expDirSect;

            PEStructures::IMAGE_EXPORT_DIRECTORY expEntry;
            {
                bool gotData = sections.ReadPEData( expDirEntry.VirtualAddress, sizeof(expEntry), &expEntry, &expDirSect );

                if ( !gotData )
                    throw std::exception( "invalid PE export directory" );
            }

            expDirSect->SetPlacedMemory( expInfo.allocEntry, expDirEntry.VirtualAddress, expDirEntry.Size );

            // Store the usual tidbits.
            expInfo.chars = expEntry.Characteristics;
            expInfo.timeDateStamp = expEntry.TimeDateStamp;
            expInfo.majorVersion = expEntry.MajorVersion;
            expInfo.minorVersion = expEntry.MinorVersion;
            expInfo.ordinalBase = expEntry.Base;

            size_t ordinalBase = ( expInfo.ordinalBase - 1 );

            // Read the name.
            PESection *sectOfName;
            {
                bool gotName = sections.ReadPEString( expEntry.Name, expInfo.name, &sectOfName );

                if ( !gotName )
                    throw std::exception( "failed to read PE export directory name" );
            }

            sectOfName->SetPlacedMemory( expInfo.nameAllocEntry, expEntry.Name );

            // Allocate functions.
            if ( expEntry.AddressOfFunctions != 0 )
            {
                std::vector <PEExportDir::func> funcs;
                funcs.reserve( expEntry.NumberOfFunctions );

                std::uint64_t tabSize;

                if ( is64Bit )
                {
                    tabSize = ( sizeof(std::uint64_t) * expEntry.NumberOfFunctions );
                }
                else
                {
                    tabSize = ( sizeof(std::uint32_t) * expEntry.NumberOfFunctions );
                }
                
                PESection *addrPtrSect;
                PEDataStream addrPtrStream;
                {
                    bool gotStream = sections.GetPEDataStream(
                        expEntry.AddressOfFunctions, addrPtrStream,
                        &addrPtrSect
                    );

                    if ( !gotStream )
                    {
                        throw std::exception( "failed to get PE export info function entries" );
                    }
                }

                addrPtrSect->SetPlacedMemory( expInfo.funcAddressAllocEntry, expEntry.AddressOfFunctions );

                for ( std::uint32_t n = 0; n < expEntry.NumberOfFunctions; n++ )
                {
                    PEExportDir::func fentry;
                    fentry.isNamed = false; // by default no export is named.

                    bool isForwarder;
                    {
                        std::uint32_t ptr;
                        addrPtrStream.Read( &ptr, sizeof(ptr) );

                        // Determine if we are a forwarder or an export.
                        {
                            typedef sliceOfData <std::uint32_t> rvaSlice_t;

                            rvaSlice_t requestSlice( ptr, 1 );

                            rvaSlice_t expDirSlice( expDirEntry.VirtualAddress, expDirEntry.Size );

                            rvaSlice_t::eIntersectionResult intResult = requestSlice.intersectWith( expDirSlice );

                            isForwarder = ( rvaSlice_t::isFloatingIntersect( intResult ) == false );
                        }

                        // Store properties according to the type.
                        PESection *exportOffPtrSect;
                        PEDataStream expOffStream;
                        {
                            bool gotStream = sections.GetPEDataStream( ptr, expOffStream, &exportOffPtrSect );

                            if ( !gotStream )
                                throw std::exception( "failed to get PE export offset pointer" );
                        }

                        // We store the location of the data entry, but NOTE that
                        // this behavior NEVER is an allocation!
                        {
                            std::uint32_t offStore;

                            if ( isForwarder )
                            {
                                offStore = ( ptr - expDirEntry.VirtualAddress );
                            }
                            else
                            {
                                offStore = ( ptr - exportOffPtrSect->GetVirtualAddress() );
                            }

                            fentry.forwExpFuncOffset = offStore;
                            fentry.forwExpFuncSection = exportOffPtrSect;
                        }

                        if ( isForwarder )
                        {
                            ReadPEString( expOffStream, fentry.forwarder );
                        }
                    }
                    fentry.isForwarder = isForwarder;

                    funcs.push_back( std::move( fentry ) );
                }

                // Read names and ordinals, if available.
                if ( expEntry.AddressOfNames != 0 && expEntry.AddressOfNameOrdinals != 0 )
                {
                    // Establish name ptr array.
                    PESection *addrNamesSect;
                    PEDataStream addrNamesStream;
                    {
                        bool gotStream = sections.GetPEDataStream( expEntry.AddressOfNames, addrNamesStream, &addrNamesSect );

                        if ( !gotStream )
                            throw std::exception( "failed to get PE export directory function name list" );
                    }

                    addrNamesSect->SetPlacedMemory( expInfo.funcNamesAllocEntry, expEntry.AddressOfNames );

                    // Establish ordinal mapping array.
                    PESection *addrNameOrdSect;
                    PEDataStream addrNameOrdStream;
                    {
                        bool gotStream = sections.GetPEDataStream( expEntry.AddressOfNameOrdinals, addrNameOrdStream, &addrNameOrdSect );
                        
                        if ( !gotStream )
                            throw std::exception( "failed to get PE export directory function ordinals" );
                    }

                    addrNameOrdSect->SetPlacedMemory( expInfo.funcOrdinalsAllocEntry, expEntry.AddressOfNameOrdinals );

                    // Map names to functions.
                    for ( std::uint32_t n = 0; n < expEntry.NumberOfNames; n++ )
                    {
                        std::uint16_t ordinal;
                        addrNameOrdStream.Read( &ordinal, sizeof(ordinal) );

                        // Get the index to map the function name to (== ordinal).
                        size_t mapIndex = ( ordinal - ordinalBase );

                        if ( mapIndex >= funcs.size() )
                        {
                            // Invalid mapping.
                            throw std::exception( "PE binary has broken export mapping (ordinal out of bounds)" );
                        }

                        // Get the name we should map to.
                        PESection *realNamePtrSect;

                        std::uint32_t namePtrRVA;
                        addrNamesStream.Read( &namePtrRVA, sizeof(namePtrRVA) );

                        // Read the actual name.
                        std::string realName;
                        {
                            bool gotString = sections.ReadPEString( namePtrRVA, realName, &realNamePtrSect );

                            if ( !gotString )
                                throw std::exception( "failed to get PE export directory function name ptr" );
                        }

                        if ( realName.empty() )
                        {
                            // Kind of invalid.
                            throw std::exception( "invalid PE export name: empty string" );
                        }

                        PEExportDir::func& fentry = funcs[ mapIndex ];

                        // Check for ambiguous name mappings.
                        // TODO: this is actually allowed and is called "alias"; pretty evil.
                        if ( fentry.isNamed )
                        {
                            throw std::exception( "ambiguous PE export name mapping" );
                        }

                        fentry.name = std::move( realName );
                        fentry.isNamed = true;  // yes, we have a valid name!

                        realNamePtrSect->SetPlacedMemory( fentry.nameAllocEntry, namePtrRVA );
                    }
                }

                expInfo.functions = std::move( funcs );
            }

            // We got the export directory! :)
        }
    }

    // * IMPORT directory.
    std::vector <PEImportDesc> impDescs;
    {
        const PEStructures::IMAGE_DATA_DIRECTORY& impDir = dataDirs[ PEL_IMAGE_DIRECTORY_ENTRY_IMPORT ];

        if ( impDir.VirtualAddress != 0 )
        {
            PESection *impDirSect;
            PEDataStream importDescsStream;
            {
                bool gotStream = sections.GetPEDataStream( impDir.VirtualAddress, importDescsStream, &impDirSect );

                if ( !gotStream )
                    throw std::exception( "failed to read PE import descriptors" );
            }

            impDirSect->SetPlacedMemory( this->importsAllocEntry, impDir.VirtualAddress, impDir.Size );

            // Read all the descriptors.
            const std::uint32_t potentialNumDescriptors = ( impDir.Size / sizeof( PEStructures::IMAGE_IMPORT_DESCRIPTOR ) );

            impDescs.reserve( potentialNumDescriptors );

            std::uint32_t n = 0;

            while ( n++ < potentialNumDescriptors )
            {
                PEStructures::IMAGE_IMPORT_DESCRIPTOR importInfo;
                importDescsStream.Read( &importInfo, sizeof(importInfo) );

                // TODO: allow secure bounded parsing of PE files, so we check for
                // violations of PE rules and reject those files.

                // By definition, an IMAGE_IMPORT_DESCRIPTOR with all entries zero
                // is the end of the table.
                if ( importInfo.Characteristics == 0 &&
                     importInfo.TimeDateStamp == 0 &&
                     importInfo.ForwarderChain == 0 &&
                     importInfo.Name == 0 && 
                     importInfo.FirstThunk == 0 )
                {
                    break;
                }

                PEImportDesc impDesc;

                // Get the function names (with their ordinals).
                if ( importInfo.Characteristics != 0 )
                {
                    impDesc.funcs =
                        PEImportDesc::ReadPEImportFunctions(
                            sections, importInfo.Characteristics, impDesc.impNameArrayAllocEntry, is64Bit
                        );
                }

                // Store the DLL name we import from.
                {
                    PESection *dllNameSect;
                    {
                        bool gotName = sections.ReadPEString( importInfo.Name, impDesc.DLLName, &dllNameSect );

                        if ( !gotName )
                            throw std::exception( "failed to read PE import desc DLL name" );
                    }

                    dllNameSect->SetPlacedMemory( impDesc.DLLName_allocEntry, importInfo.Name );
                }

                impDesc.firstThunkRef = sections.ResolveRVAToRef( importInfo.FirstThunk );

                // Store this import desc.
                impDescs.push_back( std::move( impDesc ) );

                // Done with this import desc!
            }

            // Done with all imports.
        }
    }

    // * Resources.
    PEResourceDir resourceRoot = std::u16string();  // very weird assignment here, but required; call the constructor.
    {
        const PEStructures::IMAGE_DATA_DIRECTORY& resDir = dataDirs[ PEL_IMAGE_DIRECTORY_ENTRY_RESOURCE ];

        if ( resDir.VirtualAddress != 0 )
        {
            PESection *resDataSect;
            PEDataStream resDataStream;
            {
                bool gotStream = sections.GetPEDataStream( resDir.VirtualAddress, resDataStream, &resDataSect );

                if ( !gotStream )
                    throw std::exception( "invalid PE resource root" );
            }

            resDataSect->SetPlacedMemory( this->resAllocEntry, resDir.VirtualAddress, resDir.Size );

            PEStructures::IMAGE_RESOURCE_DIRECTORY resDir;
            resDataStream.Read( &resDir, sizeof(resDir) );

            resourceRoot = LoadResourceDirectory( sections, resDataStream, std::u16string(), resDir );
        }
    }

    // * Exception Information.
    std::vector <PERuntimeFunction> exceptRFs;
    {
        const PEStructures::IMAGE_DATA_DIRECTORY& rtDir = dataDirs[ PEL_IMAGE_DIRECTORY_ENTRY_EXCEPTION ];

        if ( rtDir.VirtualAddress != 0 )
        {
            // TODO: apparently exception data is machine dependent, so we should
            // deserialize this in a special way depending on machine_id.
            // (currently we specialize on x86/AMD64)

            PESection *rtFuncsSect;
            PEDataStream rtFuncsStream;
            {
                bool gotStream = sections.GetPEDataStream( rtDir.VirtualAddress, rtFuncsStream, &rtFuncsSect );

                if ( !gotStream )
                    throw std::exception( "invalid PE exception directory" );
            }

            rtFuncsSect->SetPlacedMemory( this->exceptAllocEntry, rtDir.VirtualAddress, rtDir.Size );

            const std::uint32_t numFuncs = ( rtDir.Size / sizeof( PEStructures::IMAGE_RUNTIME_FUNCTION_ENTRY_X64 ) );

            exceptRFs.reserve( numFuncs );

            for ( size_t n = 0; n < numFuncs; n++ )
            {
                PEStructures::IMAGE_RUNTIME_FUNCTION_ENTRY_X64 func;
                rtFuncsStream.Read( &func, sizeof(func) );

                // Since the runtime function entry stores RVAs, we want to remember them
                // relocation independent.
                PESection *beginAddrSect;
                std::uint32_t beginAddrSectOff;
                {
                    bool gotLocation = sections.GetPEDataLocation( func.BeginAddress, &beginAddrSectOff, &beginAddrSect );

                    if ( !gotLocation )
                    {
                        throw std::exception( "invalid PE runtime function begin address" );
                    }
                }
                PESection *endAddrSect;
                std::uint32_t endAddrSectOff;
                {
                    bool gotLocation = sections.GetPEDataLocation( func.EndAddress, &endAddrSectOff, &endAddrSect );

                    if ( !gotLocation )
                    {
                        throw std::exception( "invalid PE runtime function end address" );
                    }
                }
                PESection *unwindInfoSect;
                std::uint32_t unwindInfoSectOff;
                {
                    bool gotLocation = sections.GetPEDataLocation( func.UnwindInfoAddress, &unwindInfoSectOff, &unwindInfoSect );

                    if ( !gotLocation )
                    {
                        throw std::exception( "invalid PE runtime function unwind info address" );
                    }
                }

                PERuntimeFunction funcInfo;
                funcInfo.beginAddrRef = PESectionDataReference( beginAddrSect, beginAddrSectOff );
                funcInfo.endAddrRef = PESectionDataReference( endAddrSect, endAddrSectOff );
                funcInfo.unwindInfoRef = PESectionDataReference( unwindInfoSect, unwindInfoSectOff );

                exceptRFs.push_back( std::move( funcInfo ) );
            }
        }
    }

    // * ATTRIBUTE CERTIFICATES.
    PESecurity securityCookie;
    {
        const PEStructures::IMAGE_DATA_DIRECTORY& certDir = dataDirs[ PEL_IMAGE_DIRECTORY_ENTRY_SECURITY ];

        // VirtualAddress in this data directory is a file pointer.
        std::uint32_t certFilePtr = certDir.VirtualAddress;
        std::uint32_t certBufSize = certDir.Size;

        securityCookie.certStore.ReadFromFile( peStream, sections, 0, certFilePtr, certBufSize );
    }

    // * BASE RELOC.
    std::map <std::uint32_t, PEBaseReloc> baseRelocs;
    {
        const PEStructures::IMAGE_DATA_DIRECTORY& baserelocDir = dataDirs[ PEL_IMAGE_DIRECTORY_ENTRY_BASERELOC ];

        if ( baserelocDir.VirtualAddress != 0 )
        {
            const std::uint32_t sizeRelocations = baserelocDir.Size;

            PESection *baseRelocDescsSect;
            PEDataStream baseRelocDescsStream;
            {
                bool gotStream = sections.GetPEDataStream( baserelocDir.VirtualAddress, baseRelocDescsStream, &baseRelocDescsSect );

                if ( !gotStream )
                    throw std::exception( "invalid PE base relocation directory" );
            }

            baseRelocDescsSect->SetPlacedMemory( this->baseRelocAllocEntry, baserelocDir.VirtualAddress, baserelocDir.Size );

            // We read relocation data until we are at the end of the directory.
            while ( true )
            {
                // Remember our current offset.
                std::uint32_t curOffset = baseRelocDescsStream.Tell();

                if ( curOffset >= sizeRelocations )
                    break;

                // Get current relocation.
                PEStructures::IMAGE_BASE_RELOCATION baseReloc;
                baseRelocDescsStream.Read( &baseReloc, sizeof(baseReloc) );

                // Store it.
                const std::uint32_t blockSize = baseReloc.SizeOfBlock;

                // Validate the blockSize.
                if ( blockSize < sizeof(PEStructures::IMAGE_BASE_RELOCATION) )
                    throw std::exception( "malformed PE base relocation sub block" );

                // Subtract the meta-data size.
                const std::uint32_t entryBlockSize = ( blockSize - sizeof(PEStructures::IMAGE_BASE_RELOCATION) );
                {
                    std::uint32_t relVirtAddr = baseReloc.VirtualAddress;

                    // Verify that it is a valid block address.
                    if ( ( relVirtAddr % baserelocChunkSize ) != 0 )
                    {
                        throw std::exception( "invalid PE base relocation block virtual address (must be aligned on 4K boundaries)" );
                    }

                    PEBaseReloc info;
                    info.offsetOfReloc = relVirtAddr;

                    // Read all relocations.
                    const std::uint32_t numRelocItems = ( entryBlockSize / sizeof( PEStructures::IMAGE_BASE_RELOC_TYPE_ITEM ) );

                    info.items.reserve( numRelocItems );

                    // Base relocation are stored in a stream-like array. Some entries form tuples,
                    // so that two entries have to be next to each other.
                    size_t reloc_index = 0;

                    while ( reloc_index < numRelocItems )
                    {
                        PEStructures::IMAGE_BASE_RELOC_TYPE_ITEM reloc;
                        baseRelocDescsStream.Read( &reloc, sizeof(reloc) );

                        PEBaseReloc::item itemInfo;
                        itemInfo.offset = reloc.offset;
                        itemInfo.type = (PEBaseReloc::eRelocType)reloc.type;

                        info.items.push_back( std::move( itemInfo ) );

                        reloc_index++;
                    }

                    // Remember us.
                    // We take advantage of the alignedness and divide by that number.
                    std::uint32_t baseRelocIndex = ( relVirtAddr / baserelocChunkSize );

                    baseRelocs.insert( std::make_pair( baseRelocIndex, std::move( info ) ) );
                }

                // Done reading this descriptor.
            }

            // Done reading all base relocations.
        }
    }

    // * DEBUG.
    decltype(this->debugDescs) debugDescs;
    {
        const PEStructures::IMAGE_DATA_DIRECTORY& debugDir = dataDirs[ PEL_IMAGE_DIRECTORY_ENTRY_DEBUG ];

        if ( debugDir.VirtualAddress != 0 )
        {
            PESection *debugEntrySection;
            PEDataStream debugEntryStream;
            {
                bool gotStream = sections.GetPEDataStream( debugDir.VirtualAddress, debugEntryStream, &debugEntrySection );

                if ( !gotStream )
                    throw std::exception( "invalid PE debug directory" );
            }

            std::uint32_t debugDirSize = debugDir.Size;

            debugEntrySection->SetPlacedMemory( this->debugDescsAlloc, debugDir.VirtualAddress, debugDirSize );

            // Get the amount of debug descriptors available.
            const std::uint32_t numDescriptors = ( debugDirSize / sizeof(PEStructures::IMAGE_DEBUG_DIRECTORY) );

            for ( size_t n = 0; n < numDescriptors; n++ )
            {
                PEStructures::IMAGE_DEBUG_DIRECTORY debugEntry;
                debugEntryStream.Read( &debugEntry, sizeof(debugEntry) );

                // We store this debug information entry.
                // Debug information can be of many types and we cannot ever handle all of them!
                PEDebugDesc debugInfo;
                debugInfo.characteristics = debugEntry.Characteristics;
                debugInfo.timeDateStamp = debugEntry.TimeDateStamp;
                debugInfo.majorVer = debugEntry.MajorVersion;
                debugInfo.minorVer = debugEntry.MinorVersion;
                debugInfo.type = debugEntry.Type;

                // Fetch the debug data from the file, so that we do not depend on "file pointers"
                // for PE structures.
                debugInfo.dataStore.ReadFromFile(
                    peStream, sections,
                    debugEntry.AddressOfRawData, debugEntry.PointerToRawData, debugEntry.SizeOfData
                );

                // Store our information.
                debugDescs.push_back( std::move( debugInfo ) );
            }
        }
    }

    // * ARCHITECTURE.
    {
        // Reserved. Must be zero.
    }

    // * GLOBAL PTR.
    PEGlobalPtr globalPtr;
    {
        const PEStructures::IMAGE_DATA_DIRECTORY& globptrData = dataDirs[ PEL_IMAGE_DIRECTORY_ENTRY_GLOBALPTR ];

        globalPtr.ptrOffset = globptrData.VirtualAddress;
    }

    // * THREAD LOCAL STORAGE.
    PEThreadLocalStorage tlsInfo;
    {
        const PEStructures::IMAGE_DATA_DIRECTORY& tlsDataDir = dataDirs[ PEL_IMAGE_DIRECTORY_ENTRY_TLS ];

        if ( tlsDataDir.VirtualAddress != 0 )
        {
            PESection *tlsDirSect;
            PEDataStream tlsDirStream;
            {
                bool gotStream = sections.GetPEDataStream( tlsDataDir.VirtualAddress, tlsDirStream, &tlsDirSect );

                if ( !gotStream )
                {
                    throw std::exception( "invalid PE thread-local-storage directory" );
                }
            }

            // It depends on the architecture what directory type we encounter here.
            uint32_t startOfRawDataRVA = 0;
            uint32_t endOfRawDataRVA = 0;
            uint32_t addressOfIndicesRVA = 0;
            uint32_t addressOfCallbacksRVA = 0;

            if ( is64Bit )
            {
                PEStructures::IMAGE_TLS_DIRECTORY64 tlsDir;
                tlsDirStream.Read( &tlsDir, sizeof(tlsDir) );

                // Convert the VAs into RVAs.
                startOfRawDataRVA = VA2RVA( tlsDir.StartAddressOfRawData, imageBase );
                endOfRawDataRVA = VA2RVA( tlsDir.EndAddressOfRawData, imageBase );
                addressOfIndicesRVA = VA2RVA( tlsDir.AddressOfIndex, imageBase );
                addressOfCallbacksRVA = VA2RVA( tlsDir.AddressOfCallBacks, imageBase );

                // Write some things.
                tlsInfo.sizeOfZeroFill = tlsDir.SizeOfZeroFill;
                tlsInfo.characteristics = tlsDir.Characteristics;
            }
            else
            {
                PEStructures::IMAGE_TLS_DIRECTORY32 tlsDir;
                tlsDirStream.Read( &tlsDir, sizeof(tlsDir) );

                // Convert the VAs into RVAs.
                startOfRawDataRVA = VA2RVA( tlsDir.StartAddressOfRawData, imageBase );
                endOfRawDataRVA = VA2RVA( tlsDir.EndAddressOfRawData, imageBase );
                addressOfIndicesRVA = VA2RVA( tlsDir.AddressOfIndex, imageBase );
                addressOfCallbacksRVA = VA2RVA( tlsDir.AddressOfCallBacks, imageBase );

                // Write some things.
                tlsInfo.sizeOfZeroFill = tlsDir.SizeOfZeroFill;
                tlsInfo.characteristics = tlsDir.Characteristics;
            }

            // Get the references.
            tlsInfo.startOfRawDataRef = sections.ResolveRVAToRef( startOfRawDataRVA );
            tlsInfo.endOfRawDataRef = sections.ResolveRVAToRef( endOfRawDataRVA );
            tlsInfo.addressOfIndicesRef = sections.ResolveRVAToRef( addressOfIndicesRVA );
            tlsInfo.addressOfCallbacksRef = sections.ResolveRVAToRef( addressOfCallbacksRVA );

            tlsDirSect->SetPlacedMemory( tlsInfo.allocEntry, tlsDataDir.VirtualAddress, tlsDataDir.Size );
        }
    }

    // * LOAD CONFIG.
    PELoadConfig loadConfig;
    {
        const PEStructures::IMAGE_DATA_DIRECTORY& lcfgDataDir = dataDirs[ PEL_IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG ];

        if ( lcfgDataDir.VirtualAddress != 0 )
        {
            PESection *lcfgDirSect;
            PEDataStream lcfgDirStream;
            {
                bool gotStream = sections.GetPEDataStream( lcfgDataDir.VirtualAddress, lcfgDirStream, &lcfgDirSect );

                if ( !gotStream )
                {
                    throw std::exception( "invalid PE load config directory" );
                }
            }

            // RVAs need special handling.
            std::uint32_t lockPrefixTableRVA;
            std::uint32_t editListRVA;
            std::uint32_t securityCookieRVA;
            std::uint32_t SEHandlerTableRVA;
            std::uint32_t guardCFCheckFunctionPointerRVA;
            std::uint32_t guardCFFunctionTableRVA;

            if ( is64Bit )
            {
                PEStructures::IMAGE_LOAD_CONFIG_DIRECTORY64 lcfgDir;
                lcfgDirStream.Read( &lcfgDir, sizeof(lcfgDir) );

                if ( lcfgDir.Size < sizeof(lcfgDir) )
                    throw std::exception( "invalid PE load configuration directory size" );

                loadConfig.timeDateStamp = lcfgDir.TimeDateStamp;
                loadConfig.majorVersion = lcfgDir.MajorVersion;
                loadConfig.minorVersion = lcfgDir.MinorVersion;
                loadConfig.globFlagsClear = lcfgDir.GlobalFlagsClear;
                loadConfig.globFlagsSet = lcfgDir.GlobalFlagsSet;
                loadConfig.critSecDefTimeOut = lcfgDir.CriticalSectionDefaultTimeout;
                loadConfig.deCommitFreeBlockThreshold = lcfgDir.DeCommitFreeBlockThreshold;
                loadConfig.deCommitTotalFreeThreshold = lcfgDir.DeCommitTotalFreeThreshold;
                lockPrefixTableRVA = VA2RVA( lcfgDir.LockPrefixTable, imageBase );
                loadConfig.maxAllocSize = lcfgDir.MaximumAllocationSize;
                loadConfig.virtualMemoryThreshold = lcfgDir.VirtualMemoryThreshold;
                loadConfig.processAffinityMask = lcfgDir.ProcessAffinityMask;
                loadConfig.processHeapFlags = lcfgDir.ProcessHeapFlags;
                loadConfig.CSDVersion = lcfgDir.CSDVersion;
                loadConfig.reserved1 = lcfgDir.Reserved1;
                editListRVA = VA2RVA( lcfgDir.EditList, imageBase );
                securityCookieRVA = VA2RVA( lcfgDir.SecurityCookie, imageBase );
                SEHandlerTableRVA = VA2RVA( lcfgDir.SEHandlerTable, imageBase );
                loadConfig.SEHandlerCount = lcfgDir.SEHandlerCount;
                guardCFCheckFunctionPointerRVA = VA2RVA( lcfgDir.GuardCFCheckFunctionPointer, imageBase );
                loadConfig.reserved2 = lcfgDir.Reserved2;
                guardCFFunctionTableRVA = VA2RVA( lcfgDir.GuardCFFunctionTable, imageBase );
                loadConfig.guardCFFunctionCount = lcfgDir.GuardCFFunctionCount;
                loadConfig.guardFlags = lcfgDir.GuardFlags;
            }
            else
            {
                PEStructures::IMAGE_LOAD_CONFIG_DIRECTORY32 lcfgDir;
                lcfgDirStream.Read( &lcfgDir, sizeof(lcfgDir) );

                if ( lcfgDir.Size < sizeof(lcfgDir) )
                    throw std::exception( "invalid PE load configuration directory size" );

                loadConfig.timeDateStamp = lcfgDir.TimeDateStamp;
                loadConfig.majorVersion = lcfgDir.MajorVersion;
                loadConfig.minorVersion = lcfgDir.MinorVersion;
                loadConfig.globFlagsClear = lcfgDir.GlobalFlagsClear;
                loadConfig.globFlagsSet = lcfgDir.GlobalFlagsSet;
                loadConfig.critSecDefTimeOut = lcfgDir.CriticalSectionDefaultTimeout;
                loadConfig.deCommitFreeBlockThreshold = lcfgDir.DeCommitFreeBlockThreshold;
                loadConfig.deCommitTotalFreeThreshold = lcfgDir.DeCommitTotalFreeThreshold;
                lockPrefixTableRVA = VA2RVA( lcfgDir.LockPrefixTable, imageBase );
                loadConfig.maxAllocSize = lcfgDir.MaximumAllocationSize;
                loadConfig.virtualMemoryThreshold = lcfgDir.VirtualMemoryThreshold;
                loadConfig.processAffinityMask = lcfgDir.ProcessAffinityMask;
                loadConfig.processHeapFlags = lcfgDir.ProcessHeapFlags;
                loadConfig.CSDVersion = lcfgDir.CSDVersion;
                loadConfig.reserved1 = lcfgDir.Reserved1;
                editListRVA = VA2RVA( lcfgDir.EditList, imageBase );
                securityCookieRVA = VA2RVA( lcfgDir.SecurityCookie, imageBase );
                SEHandlerTableRVA = VA2RVA( lcfgDir.SEHandlerTable, imageBase );
                loadConfig.SEHandlerCount = lcfgDir.SEHandlerCount;
                guardCFCheckFunctionPointerRVA = VA2RVA( lcfgDir.GuardCFCheckFunctionPointer, imageBase );
                loadConfig.reserved2 = lcfgDir.Reserved2;
                guardCFFunctionTableRVA = VA2RVA( lcfgDir.GuardCFFunctionTable, imageBase );
                loadConfig.guardCFFunctionCount = lcfgDir.GuardCFFunctionCount;
                loadConfig.guardFlags = lcfgDir.GuardFlags;
            }

            // Process the VA registrations.
            loadConfig.lockPrefixTableRef = sections.ResolveRVAToRef( lockPrefixTableRVA );
            loadConfig.editListRef = sections.ResolveRVAToRef( editListRVA );
            loadConfig.securityCookieRef = sections.ResolveRVAToRef( securityCookieRVA );
            loadConfig.SEHandlerTableRef = sections.ResolveRVAToRef( SEHandlerTableRVA );
            loadConfig.guardCFCheckFunctionPtrRef = sections.ResolveRVAToRef( guardCFCheckFunctionPointerRVA );
            loadConfig.guardCFFunctionTableRef = sections.ResolveRVAToRef( guardCFFunctionTableRVA );

            // We kinda need the load config.
            loadConfig.isNeeded = true;

            lcfgDirSect->SetPlacedMemory( loadConfig.allocEntry, lcfgDataDir.VirtualAddress, lcfgDataDir.Size );
        }
    }

    // * BOUND IMPORT DIR.
    std::vector <PEBoundImports> boundImports;
    {
        const PEStructures::IMAGE_DATA_DIRECTORY& boundDataDir = dataDirs[ PEL_IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT ];

        if ( boundDataDir.VirtualAddress != 0 )
        {
#if 0
            const std::uint32_t numDescs = ( boundDataDir.Size / sizeof(std::uint32_t) );

            const std::uint32_t *boundImportDescsOffsets = (const std::uint32_t*)GetPEDataPointer( boundDataDir.VirtualAddress, boundDataDir.Size );

            if ( !boundImportDescsOffsets )
                throw std::exception( "invalid PE bound imports directory" );

            // Read all bound import descriptors.
            for ( size_t n = 0; n < numDescs; n++ )
            {
                std::uint32_t boundImportDescOffset = boundImportDescsOffsets[ n ];

                if ( boundImportDescOffset == NULL )
                    continue;

                const IMAGE_BOUND_IMPORT_DESCRIPTOR *desc = (const IMAGE_BOUND_IMPORT_DESCRIPTOR*)GetPEDataPointer( boundImportDescOffset, sizeof( IMAGE_BOUND_IMPORT_DESCRIPTOR ) );

                if ( !desc )
                    throw std::exception( "failed to read PE bound imports directory entries" );

                PEBoundImports boundImport;
                boundImport.timeDateStamp = desc->TimeDateStamp;
                
                // Read the name.
                {
                    const char *namePtr = (const char*)( (const char*)desc + desc->OffsetModuleName );

                    boundImport.DLLName = namePtr;
                }

                // Get all modules that are bindings.
                {
                    const IMAGE_BOUND_FORWARDER_REF *boundRefs = (const IMAGE_BOUND_FORWARDER_REF*)( desc + 1 );

                    const size_t numForwarders = desc->NumberOfModuleForwarderRefs;

                    boundImport.bindings.reserve( numForwarders );

                    for ( size_t n = 0; n < numForwarders; n++ )
                    {
                        const IMAGE_BOUND_FORWARDER_REF& ref = boundRefs[ n ];

                        PEBoundImports::binding bindInfo;
                        bindInfo.timeDateStamp = ref.TimeDateStamp;
                        
                        // Read the name.
                        {
                            const char *modName = (const char*)( (const char*)&ref + ref.OffsetModuleName );

                            bindInfo.DLLName = modName;
                        }

                        bindInfo.reserved = ref.Reserved;

                        boundImport.bindings.push_back( std::move( bindInfo ) );
                    }
                }

                boundImports.push_back( std::move( boundImport ) );
            }
#endif
            throw std::exception( "bound import loading not implemented, because not documented and no example" );

            // OK.
        }
    }

    // * IMPORT ADDRESS TABLE.
    // This is a pointer to the entire THUNK IAT array that is used in the IMPORTS DIRECTORY.
    // All thunks have to be contiguously allocated inside of this directory.
    PEThunkIATStore thunkIAT;
    {
        const PEStructures::IMAGE_DATA_DIRECTORY& iatDataDir = dataDirs[ PEL_IMAGE_DIRECTORY_ENTRY_IAT ];

        thunkIAT.thunkDataStart = iatDataDir.VirtualAddress;
        thunkIAT.thunkDataSize = iatDataDir.Size;
    }

    // * DELAY LOAD IMPORTS.
    std::vector <PEDelayLoadDesc> delayLoads;
    {
        const PEStructures::IMAGE_DATA_DIRECTORY& delayDataDir = dataDirs[ PEL_IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT ];

        if ( delayDataDir.VirtualAddress != 0 )
        {
            PESection *delayLoadDescsSect;
            PEDataStream delayLoadDescsStream;
            {
                bool gotStream = sections.GetPEDataStream( delayDataDir.VirtualAddress, delayLoadDescsStream, &delayLoadDescsSect );

                if ( !gotStream )
                    throw std::exception( "invalid PE delay loads directory" );
            }

            delayLoadDescsSect->SetPlacedMemory( this->delayLoadsAllocEntry, delayDataDir.VirtualAddress, delayDataDir.Size );

            const std::uint32_t numDelayLoads = ( delayDataDir.Size / sizeof(PEStructures::IMAGE_DELAYLOAD_DESCRIPTOR) );

            delayLoads.reserve( numDelayLoads );

            // Store all of the details.
            std::uint32_t n = 0;

            while ( n < numDelayLoads )
            {
                // Seek to this descriptor.
                delayLoadDescsStream.Seek( n * sizeof(PEStructures::IMAGE_DELAYLOAD_DESCRIPTOR) );

                PEStructures::IMAGE_DELAYLOAD_DESCRIPTOR delayLoad;
                delayLoadDescsStream.Read( &delayLoad, sizeof(delayLoad) );

                // If we found a NULL descriptor, terminate.
                if ( delayLoad.Attributes.AllAttributes == 0 &&
                     delayLoad.DllNameRVA == 0 &&
                     delayLoad.ModuleHandleRVA == 0 &&
                     delayLoad.ImportAddressTableRVA == 0 &&
                     delayLoad.BoundImportAddressTableRVA == 0 &&
                     delayLoad.UnloadInformationTableRVA == 0 &&
                     delayLoad.TimeDateStamp == 0 )
                {
                    // Encountered terminating NULL descriptor.
                    break;
                }

                PEDelayLoadDesc desc;
                desc.attrib = delayLoad.Attributes.AllAttributes;
                
                // Read DLL name.
                if ( std::uint32_t DllNameRVA = delayLoad.DllNameRVA )
                {
                    PESection *dllNamePtrSect;
                    {
                        bool gotName = sections.ReadPEString( DllNameRVA, desc.DLLName, &dllNamePtrSect );

                        if ( !gotName )
                            throw std::exception( "failed to read PE delay load desc DLL name" );
                    }

                    dllNamePtrSect->SetPlacedMemory( desc.DLLName_allocEntry, DllNameRVA );
                }

                desc.DLLHandleRef = sections.ResolveRVAToRef( delayLoad.ModuleHandleRVA );
                desc.IATRef = sections.ResolveRVAToRef( delayLoad.ImportAddressTableRVA );
                
                if ( std::uint32_t importNamesRVA = delayLoad.ImportNameTableRVA )
                {
                    desc.importNames =
                        PEImportDesc::ReadPEImportFunctions(
                            sections, importNamesRVA, desc.importNamesAllocEntry, is64Bit
                        );
                }

                desc.boundImportAddrTableRef = sections.ResolveRVAToRef( delayLoad.BoundImportAddressTableRVA );
                desc.unloadInfoTableRef = sections.ResolveRVAToRef( delayLoad.UnloadInformationTableRVA );
                desc.timeDateStamp = delayLoad.TimeDateStamp;

                // Store it.
                delayLoads.push_back( std::move( desc ) );

                // Advance to the next.
                n++;
            }
        }
    }

    // * COMMON LANGUAGE RUNTIME INFO.
    PECommonLanguageRuntimeInfo clrInfo;
    {
        const PEStructures::IMAGE_DATA_DIRECTORY& clrDataDir = dataDirs[ PEL_IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR ];

        clrInfo.dataOffset = clrDataDir.VirtualAddress;
        clrInfo.dataSize = clrDataDir.Size;
    }

    // TODO: maybe validate all structures more explicitly in context now.
    
    // Successfully loaded!
    // Store everything inside ourselves.
    this->dos_data = std::move( dos );
    this->pe_finfo = std::move( peInfo );
    this->peOptHeader = std::move( peOpt );
    this->sections = std::move( sections );
    
    // Data directories.
    this->exportDir = std::move( expInfo );
    this->imports = std::move( impDescs );
    this->resourceRoot = std::move( resourceRoot );
    this->exceptRFs = std::move( exceptRFs );
    this->securityCookie = std::move( securityCookie );
    this->baseRelocs = std::move( baseRelocs );
    this->debugDescs = std::move( debugDescs );
    this->globalPtr = std::move( globalPtr );
    this->tlsInfo = std::move( tlsInfo );
    this->loadConfig = std::move( loadConfig );
    this->boundImports = std::move( boundImports );
    this->iatThunkAll = std::move( thunkIAT );
    this->delayLoads = std::move( delayLoads );
    this->clrInfo = std::move( clrInfo );

    // Store some meta-data.
    this->is64Bit = is64Bit;        // important for casting certain offsets.

    // Next thing we would need is writing support.
}