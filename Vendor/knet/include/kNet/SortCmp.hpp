// SortCmp.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file SortCmp.h
    @brief The TriCmp and TriCmpObj constructs are the default sorting predicate objects. */

#include <cassert>

namespace kNet
{
namespace sort
{

template<typename T>
int TriCmp(const T &a, const T &b)
{
//    assume(!(a < b && b < a));
    if (a < b) return -1;
    if (b < a) return 1;
//    assert(a == b); // Require trichotomy.

    return 0;
}

template<typename T>
class TriCmpObj
{
public:
    int operator ()(const T &a, const T &b)
    {
        if (a < b) return -1;
        if (b < a) return 1;
        assert(a == b); // Require trichotomy.
        return 0;
    }
};

} // ~sort
} // ~kNet

