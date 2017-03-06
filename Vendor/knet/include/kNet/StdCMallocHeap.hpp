// StdCMallocHeap.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file StdCMallocHeap.h
    @brief Implements the allocator object for allocating memory using malloc and free. */

#include <cstdlib>
#include <cassert>

#include "Allocator.hpp"
#include "Alignment.hpp"

namespace kNet
{
/** Allocates raw memory using the std C malloc and free. Note that the free/used -block tracking
    is left at the burden of the stdlib implementation, so don't rely on this allocator in performance
    critical sections. */
class StdCAlloc
{
public:
//    StdCAlloc(){}
//    StdCAlloc(const StdCAlloc &);
//    ~StdCAlloc() {}

    static inline void *Alloc(StdCAlloc *, size_t size, size_t alignment, const char * /*nameTag*/ = 0, AllocFlags /*flags*/ = AFAllocLow)
    {
        assert(IS_POW2(alignment));
#ifdef _MSC_VER
        void *ptr = _aligned_malloc(size, alignment);
#else
        void *ptr = malloc(size); ///\todo aligned_malloc on unix?
#endif
        return ptr;
    }
    static inline void Free(StdCAlloc *, void *ptr)
    {
#ifdef _MSC_VER
        _aligned_free(ptr);
#else
        free(ptr);
#endif
    }

    ///\todo Perhaps support Resize(void *ptr, size_t newSize); ?
    static inline size_t Size(StdCAlloc *, void *ptr)
    {
#ifdef _MSC_VER
        return ::_msize(ptr);
#else
        assert(false && "N/I");
        return 0;
#endif
    }
};

} // ~kNet
