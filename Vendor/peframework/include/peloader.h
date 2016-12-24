// Windows NT Portable Executable file loader written by The_GTA.
// Inspired by Bas Timmer's PE loading inside of FiveM.

// Target spec: Revision 10 – June 15, 2016
// https://www.microsoft.com/en-us/download/details.aspx?id=19509

#ifndef _PELOADER_CORE_
#define _PELOADER_CORE_

#include "pestream.h"

#include <sdk/rwlist.hpp>
#include <sdk/MemoryRaw.h>
#include <sdk/MemoryUtils.h>
#include <sdk/MemoryUtils.stream.h>

#include <unordered_map>
#include <map>
#include <set>
#include <functional>

#include "peloader.common.h"

namespace PEStructures
{

struct IMAGE_BASE_RELOC_TYPE_ITEM
{
    std::uint16_t offset : 12;
    std::uint16_t type : 4;
};

struct IMAGE_RESOURCE_DIRECTORY {
    std::uint32_t Characteristics;
    std::uint32_t TimeDateStamp;
    std::uint16_t MajorVersion;
    std::uint16_t MinorVersion;
    std::uint16_t NumberOfNamedEntries;
    std::uint16_t NumberOfIdEntries;
//  IMAGE_RESOURCE_DIRECTORY_ENTRY DirectoryEntries[];
};

struct IMAGE_RESOURCE_DIRECTORY_ENTRY
{
    union {
        struct {
            std::uint32_t NameOffset:31;
            std::uint32_t NameIsString:1;
        };
        std::uint32_t     Name;
        std::uint16_t     Id;
    };
    union {
        std::uint32_t     OffsetToData:31;
        struct {
            std::uint32_t OffsetToDirectory:31;
            std::uint32_t DataIsDirectory:1;
        };
    };
};

struct IMAGE_RESOURCE_DIRECTORY_STRING
{
    std::uint16_t       Length;
    char                NameString[ 1 ];
};

struct IMAGE_RESOURCE_DIR_STRING_U
{
    std::uint16_t       Length;
    wchar_t             NameString[ 1 ];
};

struct IMAGE_RESOURCE_DATA_ENTRY
{
    std::uint32_t OffsetToData;
    std::uint32_t Size;
    std::uint32_t CodePage;
    std::uint32_t Reserved;
};

struct IMAGE_ATTRIB_CERT_DESC
{
    std::uint32_t Size;
    std::uint16_t Revision;
    std::uint16_t CertificateType;
    // Followed by the actual certificate.
};

};

struct PEFile
{
    PEFile( void );
    ~PEFile( void );

    void LoadFromDisk( PEStream *peStream );
    void WriteToStream( PEStream *peStream );

    bool HasRelocationInfo( void ) const;
    bool HasLinenumberInfo( void ) const;
    bool HasDebugInfo( void ) const;

    bool IsDynamicLinkLibrary( void ) const;

private:
    // DOS information.
    struct DOSStub
    {
        std::uint16_t cblp;
        std::uint16_t cp;
        std::uint16_t crlc;
        std::uint16_t cparhdr;
        std::uint16_t minalloc, maxalloc;
        std::uint16_t ss;
        std::uint16_t sp;
        std::uint16_t csum;
        std::uint16_t ip;
        std::uint16_t cs;
        std::uint16_t lfarlc;
        std::uint16_t ovno;
        std::uint16_t reserved1[4];
        std::uint16_t oemid;
        std::uint16_t oeminfo;
        std::uint16_t reserved2[10];

        // Actual DOS program data.
        std::vector <unsigned char> progData;
    };
    DOSStub dos_data;

public:
    // Start of PE stuff.
    struct PEFileInfo
    {
        inline PEFileInfo( void )
        {
            this->machine_id = 0;
            this->timeDateStamp = 0;
            this->isExecutableImage = false;
            this->hasLocalSymbols = false;
            this->hasAggressiveTrim = false;
            this->largeAddressAware = false;
            this->bytesReversedLO = false;
            this->removableRunFromSwap = false;
            this->netRunFromSwap = false;
            this->isSystemFile = false;
            this->isDLL = false;
            this->upSystemOnly = false;
            this->bytesReversedHI = false;
        }

        std::uint16_t machine_id;
        std::uint32_t timeDateStamp;

        // More meta information.
        bool isExecutableImage;
        bool hasLocalSymbols;
        bool hasAggressiveTrim;
        bool largeAddressAware;
        bool bytesReversedLO;
        bool removableRunFromSwap;
        bool netRunFromSwap;
        bool isSystemFile;
        bool isDLL;
        bool upSystemOnly;
        bool bytesReversedHI;

        // Other stuff is used for parsing more advanced business.
    };
    PEFileInfo pe_finfo;

    struct PEOptHeader
    {
        std::uint8_t majorLinkerVersion;
        std::uint8_t minorLinkerVersion;
        std::uint32_t sizeOfCode;
        std::uint32_t sizeOfInitializedData;
        std::uint32_t sizeOfUninitializedData;
        std::uint32_t addressOfEntryPoint;
        std::uint32_t baseOfCode;
        std::uint32_t baseOfData;

        std::uint64_t imageBase;
        std::uint32_t fileAlignment;
        std::uint16_t majorOSVersion;
        std::uint16_t minorOSVersion;
        std::uint16_t majorImageVersion;
        std::uint16_t minorImageVersion;
        std::uint16_t majorSubsysVersion;
        std::uint16_t minorSubsysVersion;
        std::uint32_t win32VersionValue;
        std::uint32_t sizeOfImage;
        std::uint32_t sizeOfHeaders;
        std::uint32_t checkSum;
        std::uint16_t subsys;
        std::uint64_t sizeOfStackReserve;
        std::uint64_t sizeOfStackCommit;
        std::uint64_t sizeOfHeapReserve;
        std::uint64_t sizeOfHeapCommit;
        std::uint32_t loaderFlags;

        // DLL flags.
        bool dll_supportsHighEntropy;
        bool dll_hasDynamicBase;
        bool dll_forceIntegrity;
        bool dll_nxCompat;
        bool dll_noIsolation;
        bool dll_noSEH;
        bool dll_noBind;
        bool dll_appContainer;
        bool dll_wdmDriver;
        bool dll_guardCF;
        bool dll_termServAware;

        // More advanced stuff to follow.
    };
    PEOptHeader peOptHeader;

    // Executable sections.
    struct PERelocation
    {
        union
        {
            std::uint32_t virtAddr;
            std::uint32_t relocCount;
        };

        std::uint32_t symbolTableIndex;
        std::uint16_t type;
    };

    struct PELinenumber
    {
        union
        {
            std::uint32_t symTableIndex;
            std::uint32_t virtAddr;
        };
        std::uint16_t number;
    };

private:
    struct PESectionMan;
    struct PEDataStream;
    
public:
    struct PESection
    {
        friend struct PESectionMan;
        friend struct PEDataStream;

        PESection( void );
        PESection( const PESection& right ) = delete;
        PESection( PESection&& right )
            : shortName( std::move( right.shortName ) ), virtualSize( std::move( right.virtualSize ) ),
              virtualAddr( std::move( right.virtualAddr ) ), relocations( std::move( right.relocations ) ),
              linenumbers( std::move( right.linenumbers ) ), chars( std::move( right.chars ) ),
              isFinal( std::move( right.isFinal ) ), dataAlloc( std::move( right.dataAlloc ) ),
              dataRefList( std::move( right.dataRefList ) ), dataAllocList( std::move( right.dataAllocList ) ),
              streamAllocMan( std::move( right.streamAllocMan ) ), stream( std::move( right.stream ) ),
              placedOffsets( std::move( right.placedOffsets ) ), RVAreferalList( std::move( right.RVAreferalList ) )
        {
            // Since I have been writing this, how about a move constructor that allows
            // default-construction of all members but on top of that executes its own constructor body?

            // We keep a list of RVAs that point to us, which needs updating.
            patchSectionPointers();

            // If we belong to a PE image, we must move our node over.
            moveFromOwnerImage( right );
        }
        ~PESection( void );

    private:
        inline void moveFromOwnerImage( PESection& right )
        {
            PESectionMan *ownerImage = right.ownerImage;

            if ( ownerImage )
            {
                this->sectionNode.moveFrom( std::move( right.sectionNode ) );

                right.ownerImage = NULL;
            }

            this->ownerImage = ownerImage;
        }

        inline void unregisterOwnerImage( void )
        {
            if ( PESectionMan *ownerImage = this->ownerImage )
            {
                LIST_REMOVE( this->sectionNode );

                this->ownerImage = NULL;
            }
        }

        inline void patchSectionPointers( void )
        {
            // First we want to fix the allocations that have been made on this section.
            LIST_FOREACH_BEGIN( PESectionAllocation, this->dataAllocList.root, sectionNode )

                item->theSection = this;

            LIST_FOREACH_END

            // Section data references.
            LIST_FOREACH_BEGIN( PESectionDataReference, this->dataRefList.root, sectionNode )

                item->theSect = this;

            LIST_FOREACH_END

            // Then fix the RVAs that could target us.
            LIST_FOREACH_BEGIN( PEPlacedOffset, this->RVAreferalList.root, targetNode )

                item->targetSect = this;

            LIST_FOREACH_END
        }

    public:
        inline PESection& operator =( const PESection& right ) = delete;
        inline PESection& operator =( PESection&& right )
        {
            // The same default-assignment paradigm could be applied here as
            // for the move constructor.

            this->shortName = std::move( right.shortName );
            this->virtualSize = std::move( right.virtualSize );
            this->virtualAddr = std::move( right.virtualAddr );
            this->relocations = std::move( right.relocations );
            this->linenumbers = std::move( right.linenumbers );
            this->chars = std::move( right.chars );
            this->isFinal = std::move( right.isFinal );
            this->dataAlloc = std::move( right.dataAlloc );
            this->dataRefList = std::move( right.dataRefList );
            this->dataAllocList = std::move( right.dataAllocList );
            this->streamAllocMan = std::move( right.streamAllocMan );
            this->stream = std::move( right.stream );
            this->placedOffsets = std::move( right.placedOffsets );
            this->RVAreferalList = std::move( right.RVAreferalList );

            patchSectionPointers();

            // Update PE image.
            {
                // Make sure we long to no more PE image anymore
                unregisterOwnerImage(),

                // Set us into the new owner image.
                moveFromOwnerImage( right );
            }

            return *this;
        }

        std::string shortName;
    private:
        std::uint32_t virtualSize;
        std::uint32_t virtualAddr;

    public:
        std::vector <PERelocation> relocations;
        std::vector <PELinenumber> linenumbers;

        enum class eAlignment
        {
            BYTES_UNSPECIFIED,
            BYTES_1,
            BYTES_2,
            BYTES_4,
            BYTES_8,
            BYTES_16,
            BYTES_32,
            BYTES_64,
            BYTES_128,
            BYTES_256,
            BYTES_512,
            BYTES_1024,
            BYTES_2048,
            BYTES_4096,
            BYTES_8192
        };

        // Characteristics.
        struct
        {
            bool sect_hasNoPadding;
            bool sect_containsCode;
            bool sect_containsInitData;
            bool sect_containsUninitData;
            bool sect_link_other;
            bool sect_link_info;
            bool sect_link_remove;
            bool sect_link_comdat;
            bool sect_noDeferSpecExcepts;
            bool sect_gprel;
            bool sect_mem_farData;
            bool sect_mem_purgeable;
            bool sect_mem_16bit;
            bool sect_mem_locked;
            bool sect_mem_preload;
        
            eAlignment sect_alignment;

            bool sect_link_nreloc_ovfl;
            bool sect_mem_discardable;
            bool sect_mem_not_cached;
            bool sect_mem_not_paged;
            bool sect_mem_shared;
            bool sect_mem_execute;
            bool sect_mem_read;
            bool sect_mem_write;
        } chars;

    private:
        // Meta-data that we manage.
        // * Allocation status.
        bool isFinal;       // if true then virtualSize is valid.

        typedef InfiniteCollisionlessBlockAllocator <std::uint32_t> sectionSpaceAlloc_t;

    public:
        // Pointer into data inside of a PESection.
        struct PESectionDataReference
        {
            friend struct PESection;
            friend struct PEDataStream;

            inline PESectionDataReference( void )
            {
                this->theSect = NULL;
                this->sectOffset = 0;
                this->dataSize = 0;

                // No section means not adding to list.
            }

            inline PESectionDataReference( PESection *theSect, std::uint32_t sectOffset, std::uint32_t dataSize = 0 )
            {
                this->theSect = theSect;
                this->sectOffset = sectOffset;
                this->dataSize = dataSize;

                LIST_INSERT( theSect->dataRefList.root, this->sectionNode );
            }

            inline PESectionDataReference( const PESectionDataReference& right ) = delete;
            inline PESectionDataReference( PESectionDataReference&& right )
                : sectOffset( std::move( right.sectOffset ) ), dataSize( std::move( right.dataSize ) )
            {
                PESection *theSect = right.theSect;

                this->theSect = theSect;

                if ( theSect )
                {
                    // If we have a section, then the node is successfully linked into a section.
                    this->sectionNode.moveFrom( std::move( right.sectionNode ) );

                    // We turn the moved-from object invalid.
                    right.theSect = NULL;
                }
            }

            // We mirror a lot from PESectionAllocation, but remember
            // that the types are fundamentally meant for different purposes.

            inline ~PESectionDataReference( void )
            {
                if ( PESection *theSect = this->theSect )
                {
                    LIST_REMOVE( this->sectionNode );

                    this->theSect = NULL;
                }
            }

            inline PESectionDataReference& operator = ( const PESectionDataReference& right ) = delete;
            inline PESectionDataReference& operator = ( PESectionDataReference&& right )
            {
                // This is what can be kind of done by default.
                // Reason why C++ allows different is that you can optimize this.
                this->~PESectionDataReference();

                return *new (this) PESectionDataReference( std::move( right ) );
            }

            inline std::uint32_t GetRVA( void ) const
            {
                if ( this->theSect == NULL )
                {
                    // Zero RVA is valid.
                    return 0;
                }

                return this->theSect->ResolveRVA( this->sectOffset );
            }

            inline std::uint32_t GetDataSize( void ) const
            {
                if ( this->theSect == NULL )
                {
                    return 0;
                }

                return this->dataSize;
            }

        private:
            PESection *theSect;
            std::uint32_t sectOffset;
            std::uint32_t dataSize;

            RwListEntry <PESectionDataReference> sectionNode;
        };

        // We need RVA finalization patches which come in the form of virtual
        // RVA registrations into a section. This can be used to register a VA or RVA for writing.
        struct PEPlacedOffset
        {
            friend struct PESection;

            enum class eOffsetType
            {
                RVA,
                VA_32BIT,
                VA_64BIT
            };

            PEPlacedOffset( std::uint32_t dataOffset, PESection *targetSect, std::uint32_t offsetIntoSect, eOffsetType offType = eOffsetType::RVA );

            inline PEPlacedOffset( PEPlacedOffset&& right )
            {
                PESection *targetSect = right.targetSect;

                this->dataOffset = right.dataOffset;
                this->targetSect = targetSect;
                this->offsetIntoSect = right.offsetIntoSect;
                this->offsetType = right.offsetType;

                if ( targetSect )
                {
                    this->targetNode.moveFrom( std::move( right.targetNode ) );

                    right.targetSect = NULL;
                }
            }

            inline PEPlacedOffset( const PEPlacedOffset& right ) = delete;

            inline ~PEPlacedOffset( void )
            {
                if ( this->targetSect )
                {
                    LIST_REMOVE( this->targetNode );
                }
            }

            inline PEPlacedOffset& operator =( PEPlacedOffset&& right )
            {
                this->~PEPlacedOffset();

                new (this) PEPlacedOffset( std::move( right ) );

                return *this;
            }
            inline PEPlacedOffset& operator =( const PEPlacedOffset& right ) = delete;

            // Management API.
            void WriteIntoData( PEFile *peImage, PESection *writeSect, std::uint64_t imageBase ) const;

        private:
            std::int32_t dataOffset;        // the offset into the section where the RVA has to be written.
            PESection *targetSect;          // before getting a real RVA the section has to be allocated.
            std::int32_t offsetIntoSect;    // we have to add this to the section placement to get real RVA.

            eOffsetType offsetType;         // what kind of offset we should put

            RwListEntry <PEPlacedOffset> targetNode;    // list node inside target section to keep pointer valid.
        };

        std::vector <PEPlacedOffset> placedOffsets;     // list of all RVAs that are in the data of this section.

        RwList <PEPlacedOffset> RVAreferalList;     // list of all our placed RVAs that refer to this section.

        struct PESectionAllocation
        {
            friend struct PESection;

            // TODO: once we begin differing between PE file version we have to be
            // careful about maintaining allocations.

            inline PESectionAllocation( void )
            {
                this->theSection = NULL;
                this->sectOffset = 0;
                this->dataSize = 0;
            }

            inline PESectionAllocation( PESectionAllocation&& right )
                : sectOffset( std::move( right.sectOffset ) ), dataSize( std::move( right.dataSize ) )
            {
                PESection *newSectionHost = right.theSection;

                this->theSection = newSectionHost;

                if ( newSectionHost )
                {
                    // If the section is final, we do not exist
                    // in the list, because final sections do not have to
                    // know about existing chunks.
                    // Keeping a list would over-complicate things.
                    if ( newSectionHost->isFinal == false )
                    {
                        this->sectionBlock.moveFrom( std::move( right.sectionBlock ) );
                    }

                    // Add to general allocation list.
                    this->sectionNode.moveFrom( std::move( right.sectionNode ) );
                }

                // Invalidate the old section.
                right.theSection = NULL;
            }
            inline PESectionAllocation( const PESectionAllocation& right ) = delete;

        private:
            inline void removeFromSection( void )
            {
                // If we are allocated on a section, we want to remove ourselves.
                if ( PESection *sect = this->theSection )
                {
                    if ( sect->isFinal == false )
                    {
                        // Block remove.
                        sect->dataAlloc.RemoveBlock( &this->sectionBlock );
                    }

                    // General list remove.
                    LIST_REMOVE( this->sectionNode );

                    this->theSection = NULL;
                }
            }

        public:
            inline void operator = ( PESectionAllocation&& right )
            {
                // Actually the same as the destructor does.
                this->removeFromSection();

                new (this) PESectionAllocation( std::move( right ) );
            }
            inline void operator = ( const PESectionAllocation& right ) = delete;

            inline ~PESectionAllocation( void )
            {
                this->removeFromSection();
            }

            // Data-access methods for this allocation
            void WriteToSection( const void *dataPtr, std::uint32_t dataSize, std::int32_t dataOff = 0 );
            
            // For allocating placed RVAs into allocated structs.
            void RegisterTargetRVA(
                std::uint32_t patchOffset, PESection *targetSect, std::uint32_t targetOff,
                PEPlacedOffset::eOffsetType offType = PEPlacedOffset::eOffsetType::RVA
            );
            void RegisterTargetRVA(
                std::uint32_t patchOffset, const PESectionAllocation& targetInfo, std::uint32_t targetOff = 0,
                PEPlacedOffset::eOffsetType offType = PEPlacedOffset::eOffsetType::RVA
            );
            void RegisterTargetRVA(
                std::uint32_t patchOffset, const PESectionDataReference& targetInfo, std::uint32_t targetOff = 0,
                PEPlacedOffset::eOffsetType offType = PEPlacedOffset::eOffsetType::RVA
            );

        private:
            PESection *theSection;
            std::uint32_t sectOffset;
            std::uint32_t dataSize;     // if 0 then true size not important/unknown.

        public:
            inline PESection* GetSection( void ) const          { return this->theSection; }
            inline std::uint32_t GetDataSize( void ) const      { return this->dataSize; }

            inline std::uint32_t ResolveInternalOffset( std::uint32_t offsetInto ) const
            {
                if ( this->theSection == NULL )
                {
                    throw std::exception( "attempt to resolve unallocated allocation offset" );
                }

                return ( this->sectOffset + offsetInto );
            }

            inline std::uint32_t ResolveOffset( std::uint32_t offset ) const
            {
                PESection *theSection = this->theSection;

                if ( theSection == NULL )
                {
                    throw std::exception( "attempt to resolve unallocated RVA" );
                }

                return theSection->ResolveRVA( this->sectOffset + offset );
            }

            inline bool IsAllocated( void ) const
            {
                return ( theSection != NULL );
            }

            // Every allocation can ONLY exist on ONE section.

            sectionSpaceAlloc_t::block_t sectionBlock;

            RwListEntry <PESectionAllocation> sectionNode;  // despite having a collision-based list node we need a general node aswell.

            // Write helpers for native numbers.
#define PESECT_WRITEHELPER( typeName, type ) \
            inline void Write##typeName##( type value, std::int32_t dataOff = 0 ) \
            { \
                this->WriteToSection( &value, sizeof(value), dataOff ); \
            }

            PESECT_WRITEHELPER( Int8, std::int8_t );
            PESECT_WRITEHELPER( Int16, std::int16_t );
            PESECT_WRITEHELPER( Int32, std::int32_t );
            PESECT_WRITEHELPER( Int64, std::int64_t );
            PESECT_WRITEHELPER( UInt8, std::uint8_t );
            PESECT_WRITEHELPER( UInt16, std::uint16_t );
            PESECT_WRITEHELPER( UInt32, std::uint32_t );
            PESECT_WRITEHELPER( UInt64, std::uint64_t );
        };

        // API to register RVAs for commit phase.
        void RegisterTargetRVA(
            std::uint32_t patchOffset, PESection *targetSect, std::uint32_t targetOffset,
            PEPlacedOffset::eOffsetType offsetType = PEPlacedOffset::eOffsetType::RVA
        );
        void RegisterTargetRVA(
            std::uint32_t patchOffset, const PESectionAllocation& targetInfo,
            PEPlacedOffset::eOffsetType offsetType = PEPlacedOffset::eOffsetType::RVA
        );

        // General method and initialization.
        void SetPlacementInfo( std::uint32_t virtAddr, std::uint32_t virtSize );

        inline std::uint32_t GetVirtualAddress( void ) const
        {
            if ( this->ownerImage == NULL )
            {
                throw std::exception( "attempt to get virtual address from section unbound to image" );
            }

            return this->virtualAddr;
        }

        inline std::uint32_t GetVirtualSize( void ) const
        {
            if ( this->isFinal == false )
            {
                throw std::exception( "attempt to get virtual size from unfinished section" );
            }

            return this->virtualSize;
        }

        inline bool IsFinal( void ) const       { return this->isFinal; }

        // Allocation methods.
        std::uint32_t Allocate( PESectionAllocation& blockMeta, std::uint32_t allocSize, std::uint32_t alignment = sizeof(std::uint32_t) );
        void SetPlacedMemory( PESectionAllocation& blockMeta, std::uint32_t allocOff, std::uint32_t allocSize = 0u );

        std::uint32_t ResolveRVA( std::uint32_t sectOffset ) const;

        std::uint32_t GetPENativeFlags( void ) const;

        // If we are final, we DO NOT keep a list of allocations.
        // Otherwise we keep a collisionless struct of allocations we made.
        sectionSpaceAlloc_t dataAlloc;

        // List which contains pure data pointers into section data that will
        // be transformed into PEDataStream access objects.
        RwList <PESectionDataReference> dataRefList;

        // List which contains unordered allocated chunks, mostly useful for
        // final sections.
        RwList <PESectionAllocation> dataAllocList;

        inline bool IsEmpty( void ) const
        {
            if ( isFinal )
            {
                return ( this->virtualSize == 0 );
            }
            else
            {
                return ( LIST_EMPTY( this->dataAlloc.blockList.root ) == true );
            }
        }

private:
        // Writing and possibly reading from this data section
        // should be done through this memory stream.
        BasicMemStream::basicMemStreamAllocMan <std::int32_t> streamAllocMan;
public:
        typedef BasicMemStream::basicMemoryBufferStream <std::int32_t> memStream;

        memStream stream;

        // Call just before placing into image.
        void Finalize( void );

        // Node into the list of sections in a PESectionMan.
        RwListEntry <PESection> sectionNode;
        PESectionMan *ownerImage;
    };
    using PEPlacedOffset = PESection::PEPlacedOffset;
    using PESectionDataReference = PESection::PESectionDataReference;
    using PESectionAllocation = PESection::PESectionAllocation;

private:
    // Data inside of a PE file is stored in sections which have special
    // rules if they ought to be "zero padded".
    struct PEDataStream
    {
        inline PEDataStream( void )
        {
            this->accessSection = NULL;
            this->dataOffset = 0;
            this->seek_off = 0;
        }

        inline PEDataStream( PESection *theSection, std::uint32_t dataOffset )
        {
            this->accessSection = theSection;
            this->dataOffset = dataOffset;
            this->seek_off = 0;
        }

        static inline PEDataStream fromDataRef( const PESectionDataReference& dataRef )
        {
            return PEDataStream( dataRef.theSect, dataRef.sectOffset );
        }

        inline void Seek( std::uint32_t offset )
        {
            this->seek_off = offset;
        }

        inline std::uint32_t Tell( void )
        {
            return this->seek_off;
        }

        inline void Read( void *dataBuf, std::uint32_t readCount )
        {
            PESection *theSection = this->accessSection;

            if ( !theSection )
                throw std::exception( "attempt to read from invalid PE data stream" );

            typedef sliceOfData <std::uint32_t> sectionSlice_t;

            // Get the slice of the present data.
            const std::uint32_t sectVirtualAddr = theSection->virtualAddr;
            const std::uint32_t sectVirtualSize = theSection->virtualSize;

            sectionSlice_t dataSlice( 0, theSection->stream.Size() );

            // Get the slice of the zero padding.
            const std::uint32_t validEndPoint = ( sectVirtualSize );

            sectionSlice_t zeroSlice = sectionSlice_t::fromOffsets( dataSlice.GetSliceEndPoint() + 1, validEndPoint );

            // Now the slice of our read operation.
            sectionSlice_t opSlice( ( this->dataOffset + this->seek_off ), readCount );

            // Begin output to buffer operations.
            char *outputPtr = (char*)dataBuf;

            std::uint32_t totalReadCount = 0;

            // First return the amount of data that was requested, if it counts.
            sectionSlice_t retDataSlice;

            if ( opSlice.getSharedRegion( dataSlice, retDataSlice ) )
            {
                std::uint32_t numReadData = retDataSlice.GetSliceSize();

                const void *srcDataPtr = ( (const char*)theSection->stream.Data() + retDataSlice.GetSliceStartPoint() );

                memcpy( outputPtr, srcDataPtr, numReadData );

                outputPtr += numReadData;

                totalReadCount += numReadData;
            }

            // Next see if we have to return any zeroes.
            if ( opSlice.getSharedRegion( zeroSlice, retDataSlice ) )
            {
                std::uint32_t numZeroes = retDataSlice.GetSliceSize();

                memset( outputPtr, 0, numZeroes );

                outputPtr += numZeroes;

                totalReadCount += numZeroes;
            }

            this->seek_off += readCount;

            if ( totalReadCount != readCount )
            {
                throw std::exception( "PE file out-of-bounds section read exception" );
            }
        }

    private:
        PESection *accessSection;
        std::uint32_t dataOffset;
        std::uint32_t seek_off;
    };

    template <typename charType>
    inline static void ReadPEString(
        PEDataStream& stream, std::basic_string <charType>& strOut
    )
    {
        while ( true )
        {
            charType c;
            stream.Read( &c, sizeof(c) );

            if ( c == '\0' )
            {
                break;
            }

            strOut += c;
        }
    }

    struct PESectionMan
    {
        PESectionMan( std::uint32_t sectionAlignment, std::uint32_t imageBase );
        PESectionMan( const PESectionMan& right ) = delete;
        PESectionMan( PESectionMan&& right ) = default;
        ~PESectionMan( void );

        PESectionMan& operator = ( const PESectionMan& right ) = delete;
        PESectionMan& operator = ( PESectionMan&& right ) = default;

        // Private section management API.
        PESection* AddSection( PESection&& theSection );
        PESection* PlaceSection( PESection&& theSection );
        bool RemoveSection( PESection *section );

        std::uint32_t GetSectionAlignment( void )       { return this->sectionAlignment; }
        std::uint32_t GetImageBase( void )              { return this->imageBase; }

        inline std::uint32_t GetImageSize( void )
        {
            // Pretty easy to get because we have an address-sorted list of sections.
            std::uint32_t unalignedMemImageEndOffMax = sectAllocSemantics::GetSpanSize( sectVirtualAllocMan );

            return ALIGN_SIZE( unalignedMemImageEndOffMax, this->sectionAlignment );
        }

        // Function to get the section and the offset into it for a RVA.
        inline bool GetPEDataLocation( std::uint32_t rvirtAddr, std::uint32_t *allocOffOut, PESection **allocSectOut = NULL, std::uint32_t *sectIndexOut = NULL ) const
        {
            typedef sliceOfData <std::uint32_t> memSlice_t;

            // Create a memory slice of the request.
            memSlice_t requestRegion( rvirtAddr, 1 );

            std::uint32_t sectIndex = 0;

            LIST_FOREACH_BEGIN( PESection, this->sectionList.root, sectionNode )
        
                // We only support that for sections whose data is figured out already.
                if ( item->isFinal )
                {
                    // Create a memory slice of this section.
                    std::uint32_t sectAddr, sectSize;
                    {
                        sectAddr = item->virtualAddr;
                        sectSize = item->virtualSize;
                    }

                    memSlice_t sectRegion( sectAddr, sectSize );

                    // Our memory request has to be entirely inside of a section.
                    memSlice_t::eIntersectionResult intResult = requestRegion.intersectWith( sectRegion );

                    if ( intResult == memSlice_t::INTERSECT_INSIDE ||
                         intResult == memSlice_t::INTERSECT_EQUAL )
                    {
                        if ( allocSectOut )
                        {
                            *allocSectOut = item;
                        }

                        if ( allocOffOut )
                        {
                            *allocOffOut = (uint32_t)( rvirtAddr - sectAddr );
                        }

                        if ( sectIndexOut )
                        {
                            *sectIndexOut = sectIndex;
                        }

                        return true;
                    }
                }

                sectIndex++;
        
            LIST_FOREACH_END

            // Not found.
            return false;
        }

        // Function to get a data pointer of data directories.
        inline bool GetPEDataStream(
            std::uint32_t rvirtAddr, PEDataStream& streamOut,
            PESection **allocSectOut = NULL
        )
        {
            // We return a stream into a section.
            std::uint32_t offsetIntoSect;
            PESection *allocSect;

            bool gotLocation = GetPEDataLocation( rvirtAddr, &offsetIntoSect, &allocSect );

            if ( !gotLocation )
                return false;

            streamOut = PEDataStream( allocSect, offsetIntoSect );

            if ( allocSectOut )
            {
                *allocSectOut = allocSect;
            }

            return true;
        }

        inline bool ReadPEData(
            std::uint32_t dataOffset, std::uint32_t dataSize,
            void *dataBuf, PESection **sectionOut
        )
        {
            PEDataStream stream;

            bool gotData = GetPEDataStream( dataOffset, stream, sectionOut );

            if ( !gotData )
            {
                return false;
            }

            stream.Read( dataBuf, dataSize );

            return true;
        }

        inline bool ReadPEString(
            std::uint32_t dataOffset, std::string& strOut,
            PESection **sectionOut
        )
        {
            PEDataStream stream;

            bool gotData = GetPEDataStream( dataOffset, stream, sectionOut );

            if ( !gotData )
                return false;

            PEFile::ReadPEString( stream, strOut );
            return true;
        }

        // Convert RVA to section data reference.
        inline PESectionDataReference ResolveRVAToRef( uint32_t rva )
        {
            if ( rva != 0 )
            {
                PESection *theSect;
                std::uint32_t sectOff;

                bool gotLocation = GetPEDataLocation( rva, &sectOff, &theSect );

                if ( !gotLocation )
                {
                    throw std::exception( "invalid PE relative virtual address resolution" );
                }

                return PESectionDataReference( theSect, sectOff );
            }

            return PESectionDataReference();
        }

    private:
        std::uint32_t sectionAlignment;
        std::uint32_t imageBase;

        struct sectVirtualAllocMan_t
        {
            AINLINE sectVirtualAllocMan_t( void ) = default;
            AINLINE sectVirtualAllocMan_t( const sectVirtualAllocMan_t& right ) = delete;
            AINLINE sectVirtualAllocMan_t( sectVirtualAllocMan_t&& right ) = default;

            AINLINE sectVirtualAllocMan_t& operator = ( const sectVirtualAllocMan_t& right ) = delete;
            AINLINE sectVirtualAllocMan_t& operator = ( sectVirtualAllocMan_t&& right ) = default;

            typedef sliceOfData <decltype(PESection::virtualAddr)> memSlice_t;

            struct blockIter_t
            {
                AINLINE blockIter_t( void )
                {
                    return;
                }

                AINLINE blockIter_t( RwListEntry <PESection>& node )
                {
                    this->node_iter = &node;
                }

                AINLINE void Increment( void )
                {
                    this->node_iter = this->node_iter->next;
                }

            private:
                AINLINE PESection* GetCurrentSection( void ) const
                {
                    return LIST_GETITEM( PESection, this->node_iter, sectionNode );
                }

            public:
                AINLINE memSlice_t GetMemorySlice( void ) const
                {
                    PESection *sect = GetCurrentSection();

                    return memSlice_t( sect->virtualAddr, sect->virtualSize );
                }

                AINLINE PESection* GetNativePointer( void ) const
                {
                    return GetCurrentSection();
                }

                RwListEntry <PESection> *node_iter;
            };

            AINLINE PESectionMan* GetManager( void )
            {
                return (PESectionMan*)( this - offsetof(PESectionMan, sectVirtualAllocMan) );
            }

            AINLINE blockIter_t GetFirstMemoryBlock( void )
            {
                return ( *GetManager()->sectionList.root.next );
            }

            AINLINE blockIter_t GetLastMemoryBlock( void )
            {
                return ( *GetManager()->sectionList.root.prev );
            }

            AINLINE bool HasMemoryBlocks( void )
            {
                return ( LIST_EMPTY( GetManager()->sectionList.root ) == false );
            }

            AINLINE blockIter_t GetRootNode( void )
            {
                return ( GetManager()->sectionList.root );
            }

            AINLINE blockIter_t GetAppendNode( blockIter_t iter )
            {
                return iter;
            }

            AINLINE bool IsEndMemoryBlock( const blockIter_t& iter )
            {
                return ( iter.node_iter == &GetManager()->sectionList.root );
            }

            AINLINE bool IsInAllocationRange( const memSlice_t& memRegion )
            {
                const memSlice_t peFileRegion( 0, std::numeric_limits <std::int32_t>::max() );

                memSlice_t::eIntersectionResult intResult = memRegion.intersectWith( peFileRegion );

                return ( intResult == memSlice_t::INTERSECT_EQUAL || intResult == memSlice_t::INTERSECT_INSIDE );
            }
        };

        sectVirtualAllocMan_t sectVirtualAllocMan;

        typedef FirstPassAllocationSemantics <decltype(PESection::virtualAddr), sectVirtualAllocMan_t> sectAllocSemantics;

    public:
        unsigned int numSections;

        RwList <PESection> sectionList;     // all sections belong to a PEFile MUST have a valid allocation spot.
    };

    PESectionMan sections;

    // We need to know about file-space section allocations.
    // This is mainly used for reflection purposes during PE serialization.
    struct sect_allocInfo
    {
        std::uint32_t alloc_off;
    };

    typedef std::unordered_map <std::uint32_t, sect_allocInfo> sect_allocMap_t;

    struct PEFileSpaceData
    {
        inline PEFileSpaceData( void )
        {
            // We start out without any storage.
            this->storageType = eStorageType::NONE;
        }

        inline PEFileSpaceData( const PEFileSpaceData& right ) = delete;
        inline PEFileSpaceData( PEFileSpaceData&& right ) = default;

        inline PEFileSpaceData& operator = ( const PEFileSpaceData& right ) = delete;
        inline PEFileSpaceData& operator = ( PEFileSpaceData&& right ) = default;

        // Management API.
        void ReadFromFile( PEStream *peStream, const PESectionMan& sections, std::uint32_t rva, std::uint32_t filePtr, std::uint32_t dataSize );

        void ResolveDataPhaseAllocation( std::uint32_t& rvaOut, std::uint32_t& sizeOut ) const;
        std::uint32_t ResolveFinalizationPhase( PEStream *peStream, PEloader::FileSpaceAllocMan& allocMan, const sect_allocMap_t& sectFileAlloc ) const;

        // Call this to check if this storage even needs to be finalized.
        bool NeedsFinalizationPhase( void ) const;

    private:
        struct fileSpaceStreamBufferManager
        {
            inline PEFileSpaceData* GetManager( void )
            {
                return (PEFileSpaceData*)( (char*)this - offsetof(PEFileSpaceData, streamMan) );
            }

            void EstablishBufferView( void*& memPtr, std::int32_t& streamSize, std::int32_t reqSize );
        };

        fileSpaceStreamBufferManager streamMan;

    public:
        typedef memoryBufferStream <std::int32_t, fileSpaceStreamBufferManager, false, false> fileSpaceStream_t;

        // General API about data.
        void ClearData( void );

        // Stream access to this data.
        fileSpaceStream_t OpenStream( bool createNew = false );

    private:
        enum class eStorageType
        {
            SECTION,            // stores data within address space
            FILE,               // stores data appended after the PE file
            NONE                // no storage at all
        };

        eStorageType storageType;
        PESectionAllocation sectRef;    // valid if storageType == SECTION
        std::vector <char> fileRef;     // valid if storageType == FILE
    };

public:
    using fileSpaceStream_t = PEFileSpaceData::fileSpaceStream_t;

    // Generic section management API.
    PESection* AddSection( PESection&& theSection );
    PESection* PlaceSection( PESection&& theSection );
    PESection* FindFirstSectionByName( const char *name );
    PESection* FindFirstAllocatableSection( void );
    PESection* FindSectionByRVA( std::uint32_t rva, std::uint32_t *sectIndexOut = NULL, std::uint32_t *sectOffOut = NULL );
    bool RemoveSection( PESection *section );

    void ForAllSections( std::function <void ( PESection* )> cb );

    // Simple helpers.
    template <typename charType>
    static inline PESectionAllocation WriteZeroTermString( PESection& writeSect, const std::basic_string <charType>& string )
    {
        const std::uint32_t writeCount = (std::uint32_t)( string.size() + 1 );

        const std::uint32_t writeSize = ( writeCount * sizeof(charType) );

        PESectionAllocation allocEntry;
        writeSect.Allocate( allocEntry, writeSize, sizeof(charType) );
                        
        allocEntry.WriteToSection( string.c_str(), writeSize );

        return allocEntry;
    }

    // Data directory business.
    struct PEExportDir
    {
        inline PEExportDir( void )
        {
            this->chars = 0;
            this->timeDateStamp = 0;
            this->majorVersion = 0;
            this->minorVersion = 0;
            this->ordinalBase = 0;
        }

        inline PEExportDir( const PEExportDir& right ) = delete;
        inline PEExportDir( PEExportDir&& right ) = default;

        inline PEExportDir& operator = ( const PEExportDir& right ) = delete;
        inline PEExportDir& operator = ( PEExportDir&& right ) = default;

        std::uint32_t chars;
        std::uint32_t timeDateStamp;
        std::uint16_t majorVersion;
        std::uint16_t minorVersion;
        std::string name;   // NOTE: name table is serialized lexigraphically.
        std::uint32_t ordinalBase;

        PESectionAllocation nameAllocEntry;

        struct func
        {
            // Mandatory valid fields for each function.
            std::uint32_t forwExpFuncOffset;    // might look like an allocation but is NOT.
            PESection *forwExpFuncSection;
            std::string forwarder;
            bool isForwarder;
            
            // Optional fields.
            std::string name;       // is valid if not empty
            bool isNamed;
            PESectionAllocation nameAllocEntry;
            // definition of ordinal: index into function array.
            // thus it is given implicitly.
        };
        std::vector <func> functions;

        PESectionAllocation funcAddressAllocEntry;
        PESectionAllocation funcNamesAllocEntry;
        PESectionAllocation funcOrdinalsAllocEntry;

        PESectionAllocation allocEntry;
    };
    PEExportDir exportDir;

    // Import informations.
    struct PEImportDesc
    {
        inline PEImportDesc( void ) = default;
        inline PEImportDesc( const PEImportDesc& right ) = delete;
        inline PEImportDesc( PEImportDesc&& right ) = default;

        inline PEImportDesc& operator = ( const PEImportDesc& right ) = delete;
        inline PEImportDesc& operator = ( PEImportDesc&& right ) = default;

        struct importFunc
        {
            std::uint16_t ordinal_hint;
            std::string name;
            bool isOrdinalImport;

            PESectionAllocation nameAllocEntry;
        };

        typedef std::vector <importFunc> functions_t;

        // Helper API.
        static functions_t ReadPEImportFunctions( PESectionMan& sections, std::uint32_t rva, PESectionAllocation& allocEntry, bool is64Bit );
        static PESectionAllocation WritePEImportFunctions( PESection& writeSect, functions_t& functionList, bool is64Bit );

        functions_t funcs;
        std::string DLLName;

        PESectionAllocation impNameArrayAllocEntry;
        PESectionAllocation DLLName_allocEntry;
        
        // Meta-information we must keep because it is baked
        // by compilers.
        PESectionDataReference firstThunkRef;
    };
    std::vector <PEImportDesc> imports;

    PESectionAllocation importsAllocEntry;

    // Resource information.
    struct PEResourceItem
    {
        enum class eType
        {
            DIRECTORY,
            DATA
        };

        inline PEResourceItem( eType typeDesc, std::u16string name ) : itemType( typeDesc ), name( std::move( name ) )
        {
            this->identifier = 0;
            this->hasIdentifierName = false;
        }

        virtual ~PEResourceItem( void )
        {
            return;
        }

        eType itemType;
        std::u16string name;        // valid if hasIdentifierName == false
        std::uint16_t identifier;   // valid if hasIdentifierName == true
        bool hasIdentifierName;     // if true then identifier field is valid, name is not
    };

    struct PEResourceInfo : PEResourceItem
    {
        inline PEResourceInfo( std::u16string name, PESection *dataSect, std::uint32_t dataOff, std::uint32_t dataSize )
            : PEResourceItem( eType::DATA, std::move( name ) ),
              sectRef( dataSect, dataOff, dataSize )
        {
            this->codePage = 0;
            this->reserved = 0;
        }

        // Important rule is that resource allocations are
        // stored in the resource section.
        PESectionDataReference sectRef;     // we link resources to data in sections.
        std::uint32_t codePage;
        std::uint32_t reserved;
    };
    
    struct PEResourceDir : PEResourceItem
    {
        inline PEResourceDir( std::u16string name ) : PEResourceItem( eType::DIRECTORY, std::move( name ) )
        {
            this->characteristics = 0;
            this->timeDateStamp = 0;
            this->majorVersion = 0;
            this->minorVersion = 0;
        }

        inline PEResourceDir( const PEResourceDir& right ) = delete;
        inline PEResourceDir( PEResourceDir&& right ) = default;

        inline ~PEResourceDir( void )
        {
            // We need to destroy all our children, because they are
            // dynamically allocated.
            for ( PEResourceItem *item : this->children )
            {
                delete item;
            }

            this->children.clear();
        }

        inline PEResourceDir& operator = ( const PEResourceDir& right ) = delete;
        inline PEResourceDir& operator = ( PEResourceDir&& right ) = default;

        std::uint32_t characteristics;
        std::uint32_t timeDateStamp;
        std::uint16_t majorVersion;
        std::uint16_t minorVersion;
        
        // We contain named and id entries.
        std::vector <PEResourceItem*> children;
    };
    PEResourceDir resourceRoot;
    
    PESectionAllocation resAllocEntry;

    struct PERuntimeFunction
    {
        PESectionDataReference beginAddrRef;
        PESectionDataReference endAddrRef;
        PESectionDataReference unwindInfoRef;
    };
    std::vector <PERuntimeFunction> exceptRFs;

    PESectionAllocation exceptAllocEntry;

    struct PESecurity
    {
        // We just keep the certificate data around for anyone to care about
        PEFileSpaceData certStore;
    };
    PESecurity securityCookie;

    // Base relocations are documented to be per 4K page, so let's take advantage of that.
    static constexpr std::uint32_t baserelocChunkSize = 0x1000;

    struct PEBaseReloc
    {
        inline PEBaseReloc( void ) = default;
        inline PEBaseReloc( const PEBaseReloc& right ) = delete;
        inline PEBaseReloc( PEBaseReloc&& right ) = default;

        inline PEBaseReloc& operator = ( const PEBaseReloc& right ) = delete;
        inline PEBaseReloc& operator = ( PEBaseReloc&& right ) = default;

        std::uint32_t offsetOfReloc;

        enum class eRelocType : std::uint16_t
        {
            ABSOLUTE,
            HIGH,
            LOW,
            HIGHLOW,
            HIGHADJ,
            MACHINE_5,
            RESERVED,
            MACHINE_7,
            MACHINE_8,
            MACHINE_9,
            DIR64
        };

        struct item
        {
            std::uint16_t offset : 12;
            eRelocType type : 4;
        };
        static_assert( sizeof(item) == sizeof(std::uint16_t), "invalid item size" );

        std::vector <item> items;
    };
    std::map <std::uint32_t, PEBaseReloc> baseRelocs;

    PESectionAllocation baseRelocAllocEntry;

    struct PEDebugDesc
    {
        inline PEDebugDesc( void )
        {
            this->characteristics = 0;
            this->timeDateStamp = 0;
            this->majorVer = 0;
            this->minorVer = 0;
            this->type = 0;
        }

        inline PEDebugDesc( const PEDebugDesc& right ) = delete;
        inline PEDebugDesc( PEDebugDesc&& right ) = default;

        inline PEDebugDesc& operator = ( const PEDebugDesc& right ) = delete;
        inline PEDebugDesc& operator = ( PEDebugDesc&& right ) = default;

        std::uint32_t characteristics;
        std::uint32_t timeDateStamp;
        std::uint16_t majorVer, minorVer;
        std::uint32_t type;             // can be any documented or undocumented value

        PEFileSpaceData dataStore;
    };
    std::vector <PEDebugDesc> debugDescs;

    PESectionAllocation debugDescsAlloc;

    struct PEGlobalPtr
    {
        inline PEGlobalPtr( void )
        {
            this->ptrOffset = 0;
        }

        std::uint32_t ptrOffset;
    };
    PEGlobalPtr globalPtr;

    struct PEThreadLocalStorage
    {
        inline PEThreadLocalStorage( void )
        {
            this->sizeOfZeroFill = 0;
            this->characteristics = 0;
        }

        inline PEThreadLocalStorage( const PEThreadLocalStorage& right ) = delete;
        inline PEThreadLocalStorage( PEThreadLocalStorage&& right ) = default;

        inline PEThreadLocalStorage& operator = ( const PEThreadLocalStorage& right ) = delete;
        inline PEThreadLocalStorage& operator = ( PEThreadLocalStorage&& right ) = default;

        inline bool NeedsWriting( void ) const
        {
            if ( this->startOfRawDataRef.GetRVA() != 0 ||
                 this->endOfRawDataRef.GetRVA() != 0 ||
                 this->addressOfIndicesRef.GetRVA() != 0 ||
                 this->addressOfCallbacksRef.GetRVA() != 0 ||
                 this->sizeOfZeroFill != 0 ||
                 this->characteristics != 0 )
            {
                return true;
            }

            return false;
        }

        // For maintenance reasons, we store RVAs in-memory.
        // The serialized PE format actually expects VAs.

        PESectionDataReference startOfRawDataRef;
        PESectionDataReference endOfRawDataRef;
        PESectionDataReference addressOfIndicesRef;
        PESectionDataReference addressOfCallbacksRef;
        std::uint32_t sizeOfZeroFill;
        std::uint32_t characteristics;

        PESectionAllocation allocEntry;
    };
    PEThreadLocalStorage tlsInfo;

    struct PELoadConfig
    {
        inline PELoadConfig( void ) = default;

        inline PELoadConfig( const PELoadConfig& right ) = delete;
        inline PELoadConfig( PELoadConfig&& right ) = default;

        inline PELoadConfig& operator = ( const PELoadConfig& right ) = delete;
        inline PELoadConfig& operator = ( PELoadConfig&& right ) = default;

        std::uint32_t timeDateStamp = 0;
        std::uint16_t majorVersion = 0;
        std::uint16_t minorVersion = 0;
        std::uint32_t globFlagsClear = 0;
        std::uint32_t globFlagsSet = 0;
        std::uint32_t critSecDefTimeOut = 0;
        std::uint64_t deCommitFreeBlockThreshold = 0;
        std::uint64_t deCommitTotalFreeThreshold = 0;
        PESectionDataReference lockPrefixTableRef;
        std::uint64_t maxAllocSize = 0;
        std::uint64_t virtualMemoryThreshold = 0;
        std::uint64_t processAffinityMask = 0;
        std::uint32_t processHeapFlags = 0;
        std::uint16_t CSDVersion = 0;
        std::uint16_t reserved1 = 0;
        PESectionDataReference editListRef;
        PESectionDataReference securityCookieRef;
        PESectionDataReference SEHandlerTableRef;
        std::uint64_t SEHandlerCount = 0;
        PESectionDataReference guardCFCheckFunctionPtrRef;
        std::uint64_t reserved2 = 0;
        PESectionDataReference guardCFFunctionTableRef;
        std::uint64_t guardCFFunctionCount = 0;
        std::uint32_t guardFlags = 0;

        bool isNeeded = false;

        PESectionAllocation allocEntry;
    };
    PELoadConfig loadConfig;

    struct PEBoundImports
    {
        inline PEBoundImports( void ) = default;
        inline PEBoundImports( const PEBoundImports& right ) = delete;
        inline PEBoundImports( PEBoundImports&& right ) = default;
        
        inline PEBoundImports& operator = ( const PEBoundImports& right ) = delete;
        inline PEBoundImports& operator = ( PEBoundImports&& right ) = default;

        std::uint32_t timeDateStamp;
        std::string DLLName;

        struct binding
        {
            std::uint32_t timeDateStamp;
            std::string DLLName;
            std::uint16_t reserved;
        };
        std::vector <binding> bindings;
    };
    std::vector <PEBoundImports> boundImports;

    PESectionAllocation boundImportsAllocEntry;

    struct PEThunkIATStore
    {
        inline PEThunkIATStore( void )
        {
            this->thunkDataStart = 0;
            this->thunkDataSize = 0;
        }

        std::uint32_t thunkDataStart;
        std::uint32_t thunkDataSize;
    };
    PEThunkIATStore iatThunkAll;

    struct PEDelayLoadDesc
    {
        // Uses a similar layout to the PEImportDesc data.
        std::uint32_t attrib;
        std::string DLLName;
        PESectionAllocation DLLName_allocEntry;
        PESectionDataReference DLLHandleRef;
        PESectionDataReference IATRef;
        PEImportDesc::functions_t importNames;
        PESectionAllocation importNamesAllocEntry;
        PESectionDataReference boundImportAddrTableRef;
        PESectionDataReference unloadInfoTableRef;
        std::uint32_t timeDateStamp;
    };
    std::vector <PEDelayLoadDesc> delayLoads;

    PESectionAllocation delayLoadsAllocEntry;

    struct PECommonLanguageRuntimeInfo
    {
        inline PECommonLanguageRuntimeInfo( void )
        {
            this->dataOffset = 0;
            this->dataSize = 0;
        }

        std::uint32_t dataOffset;
        std::uint32_t dataSize;
    };
    PECommonLanguageRuntimeInfo clrInfo;

    // Meta-data.
    bool is64Bit;

    inline static PEResourceDir LoadResourceDirectory( PESectionMan& sections, PEDataStream& rootStream, std::u16string nameOfDir, const PEStructures::IMAGE_RESOURCE_DIRECTORY& serResDir )
    {
        PEResourceDir curDir( std::move( nameOfDir ) );

        // Store general details.
        curDir.characteristics = serResDir.Characteristics;
        curDir.timeDateStamp = serResDir.TimeDateStamp;
        curDir.majorVersion = serResDir.MajorVersion;
        curDir.minorVersion = serResDir.MinorVersion;

        // Read sub entries.
        // Those are planted directly after the directory.
        std::uint16_t numNamedEntries = serResDir.NumberOfNamedEntries;
        std::uint16_t numIDEntries = serResDir.NumberOfIdEntries;

        // Function to read the data behind a resource directory entry.
        auto resDataParser = [&]( std::u16string nameOfItem, const PEStructures::IMAGE_RESOURCE_DIRECTORY_ENTRY& entry ) -> PEResourceItem*
        {
            // Seek to this data entry.
            rootStream.Seek( entry.OffsetToData );

            // Are we a sub-directory or an actual data leaf?
            if ( entry.DataIsDirectory )
            {
                // Get the sub-directory structure.
                PEStructures::IMAGE_RESOURCE_DIRECTORY subDirData;
                rootStream.Read( &subDirData, sizeof(subDirData) );

                PEResourceDir subDir = LoadResourceDirectory( sections, rootStream, std::move( nameOfItem ), subDirData );

                PEResourceDir *subDirItem = new PEResourceDir( std::move( subDir ) );

                return subDirItem;
            }
            else
            {
                // Get the data leaf.
                PEStructures::IMAGE_RESOURCE_DATA_ENTRY itemData;
                rootStream.Read( &itemData, sizeof(itemData) );

                // The data pointer can reside in any section.
                // We want to resolve it properly into a PESectionAllocation-like
                // inline construct.
                PESection *dataSect;
                std::uint32_t sectOff;

                bool gotLocation = sections.GetPEDataLocation( itemData.OffsetToData, &sectOff, &dataSect );

                if ( !gotLocation )
                {
                    throw std::exception( "invalid PE resource item data pointer (could not find section location)" );
                }

                // We dont have to recurse anymore.
                PEResourceInfo resItem(
                    std::move( nameOfItem ),
                    dataSect, std::move( sectOff ), itemData.Size
                );
                resItem.codePage = itemData.CodePage;
                resItem.reserved = itemData.Reserved;

                PEResourceInfo *resItemPtr = new PEResourceInfo( std::move( resItem ) );

                return resItemPtr;
            }
        };

        curDir.children.reserve( numNamedEntries + numIDEntries );

        // Due to us using only one PEDataStream we need to seek to all our entries properly.
        std::uint32_t subDirStartOff = rootStream.Tell();

        for ( std::uint32_t n = 0; n < numNamedEntries; n++ )
        {
            rootStream.Seek( subDirStartOff + n * sizeof(PEStructures::IMAGE_RESOURCE_DIRECTORY_ENTRY) );

            PEStructures::IMAGE_RESOURCE_DIRECTORY_ENTRY namedEntry;
            rootStream.Read( &namedEntry, sizeof(namedEntry) );

            if ( namedEntry.NameIsString == false )
            {
                throw std::exception( "invalid PE resource directory entry: expected named entry" );
            }

            // Load the name.
            std::u16string nameOfItem;
            {
                rootStream.Seek( namedEntry.NameOffset );

                std::uint16_t nameCharCount;
                rootStream.Read( &nameCharCount, sizeof(nameCharCount) );

                nameOfItem.resize( nameCharCount );
                
                rootStream.Read( (char16_t*)nameOfItem.c_str(), nameCharCount );
            }

            // Create a resource item.
            PEResourceItem *resItem = resDataParser( std::move( nameOfItem ), namedEntry );

            resItem->hasIdentifierName = false;

            // Store ourselves.
            curDir.children.push_back( resItem );
        }

        for ( std::uint32_t n = 0; n < numIDEntries; n++ )
        {
            rootStream.Seek( subDirStartOff + ( n + numNamedEntries ) * sizeof(PEStructures::IMAGE_RESOURCE_DIRECTORY_ENTRY) );
            
            PEStructures::IMAGE_RESOURCE_DIRECTORY_ENTRY idEntry;
            rootStream.Read( &idEntry, sizeof(idEntry) );

            if ( idEntry.NameIsString == true )
                throw std::exception( "invalid PE resource directory ID entry" );

            // Create a resource item.
            PEResourceItem *resItem = resDataParser( std::u16string(), idEntry );

            resItem->identifier = idEntry.Id;
            resItem->hasIdentifierName = true;

            // Store it.
            curDir.children.push_back( resItem );
        }

        return curDir;
    }

    // Relocation API.
    void AddRelocation( std::uint32_t rva, PEBaseReloc::eRelocType relocType );
    void RemoveRelocations( std::uint32_t rva, std::uint32_t regionSize );

    void OnWriteAbsoluteVA( PESection *writeSect, std::uint32_t sectOff, bool is64Bit );

    // Debug API.
    PEDebugDesc& AddDebugData( std::uint32_t debugType );
    bool ClearDebugDataOfType( std::uint32_t debugType );

    // Information API.
    inline std::uint64_t GetImageBase( void ) const     { return this->peOptHeader.imageBase; }

private:
    // Helper functions to off-load the duty work from the main
    // serialization function.
    std::uint16_t GetPENativeFileFlags( void );
    std::uint16_t GetPENativeDLLOptFlags( void );

public:
    void CommitDataDirectories( void );
};

#endif //_PELOADER_CORE_