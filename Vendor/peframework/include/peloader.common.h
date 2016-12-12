// PEloader common utilities that are globally included inside of the module.

#ifndef _PELOADER_COMMON_HEADER_
#define _PELOADER_COMMON_HEADER_

#include <sdk/MemoryUtils.h>

namespace PEloader
{

struct PEAllocFileAllocProxy
{
    template <typename sliceType>
    AINLINE bool IsInAllocationRange( const sliceType& slice )
    {
        // TODO: add limit checking for 32bit allocatibility here (if required).
        return true;
    }
};

typedef InfiniteCollisionlessBlockAllocator <std::uint32_t> peFileAlloc;

struct FileSpaceAllocMan
{
    inline FileSpaceAllocMan( void )
    {
        return;
    }

    inline ~FileSpaceAllocMan( void )
    {
        // Free all allocations that have not yet been freed (which is every alloc).
        while ( !LIST_EMPTY( this->internalAlloc.blockList.root ) )
        {
            peFileAlloc::block_t *item = LIST_GETITEM( peFileAlloc::block_t, this->internalAlloc.blockList.root.next, node );

            alloc_block_t *allocBlock = LIST_GETITEM( alloc_block_t, item, allocatorEntry );

            // Remove us from registration.
            this->internalAlloc.RemoveBlock( item );

            // Delete us.
            delete allocBlock;
        }
    }

    inline std::uint32_t AllocateAny( std::uint32_t peSize, std::uint32_t peAlignment = sizeof(std::uint32_t) )
    {
        peFileAlloc::allocInfo alloc_data;

        if ( internalAlloc.FindSpace( peSize, alloc_data, peAlignment ) == false )
        {
            throw std::exception( "failed to find PE file space for allocation" );
        }

        alloc_block_t *alloc_savior = new alloc_block_t();
        
        internalAlloc.PutBlock( &alloc_savior->allocatorEntry, alloc_data );

        return alloc_savior->allocatorEntry.slice.GetSliceStartPoint();
    }

    inline void AllocateAt( std::uint32_t peOff, std::uint32_t peSize )
    {
        peFileAlloc::allocInfo alloc_data;

        if ( internalAlloc.ObtainSpaceAt( peOff, peSize, alloc_data ) == false )
        {
            throw std::exception( "failed to obtain PE file space at presignated offset" );
        }

        alloc_block_t *alloc_savior = new alloc_block_t();

        internalAlloc.PutBlock( &alloc_savior->allocatorEntry, alloc_data );
    }

    inline std::uint32_t GetSpanSize( std::uint32_t alignment )
    {
        return ALIGN_SIZE( internalAlloc.GetSpanSize(), alignment );
    }

private:
    peFileAlloc internalAlloc;

    struct alloc_block_t
    {
        peFileAlloc::block_t allocatorEntry;
    };
};

}; //PEloader

#endif //_PELOADER_COMMON_HEADER_