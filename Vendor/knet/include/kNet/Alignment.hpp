// Provides functions for working with pointer data alignment computations.
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <cassert>

#include "Types.hpp"

// CtNorth: removed the KNET_UNIX definition
#ifndef _WIN32
#include <stdint.h>
#endif

namespace kNet
{

/// Is value an exact power of 2? i.e. 1,2,4,8,16,...
#define IS_POW2(value) (((value) & ((value)-1)) == 0)

/// Is the given pointer aligned to the pow2-boundary specified by alignment?
inline bool IsPow2Aligned(uintptr_t pointer, u32 alignment)
{
    assert(IS_POW2(alignment));
    return (pointer & (alignment - 1)) == 0;
}

/// @return The given pointer aligned up to the next pow2-boundary specified by alignment. (Alignment must be a pow2)
inline uintptr_t AlignUpPow2(uintptr_t pointer, u32 alignment)
{
    assert(IS_POW2(alignment));
    return (pointer + alignment - 1) & ~((uintptr_t)alignment - 1);
}

/// @return The given pointer aligned down to the previous pow2-boundary specified by alignment. (Alignment must be a pow2)
inline uintptr_t AlignDownPow2(uintptr_t pointer, u32 alignment)
{
    assert(IS_POW2(alignment));
    return pointer & ~((uintptr_t)alignment - 1);
}

inline u32 RoundUpToNextPow2(u32 x)
{
    x = x - 1;
    x = x | (x >>  1);
    x = x | (x >>  2);
    x = x | (x >>  4);
    x = x | (x >>  8);
    x = x | (x >> 16);
    return x + 1;
}

inline u64 RoundUpToNextPow2(u64 x)
{
    x = x - 1;
    x = x | (x >>  1);
    x = x | (x >>  2);
    x = x | (x >>  4);
    x = x | (x >>  8);
    x = x | (x >> 16);
    x = x | (x >> 32);
    return x + 1;
}

} // ~kNet
