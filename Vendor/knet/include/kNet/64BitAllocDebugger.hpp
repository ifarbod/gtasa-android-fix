// Virtually reserves all memory on Windows in the < 4GB memory area so that all
// pointers used by the application are outside the 32 - bit range.
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#ifdef _WIN64

#include <vector>

class BottomMemoryAllocator
{
public:
    BottomMemoryAllocator();

    ~BottomMemoryAllocator();

    std::vector<void *> virtualAllocated;
    std::vector<void *> heapAllocated;
    std::vector<void *> mallocAllocated;

    void ReserveBottomMemory();
    void FreeBottomMemory();
};

#else

/// On other platforms than 64-bit Windows, this debugging feature is not enabled.
class BottomMemoryAllocator {};

#endif
