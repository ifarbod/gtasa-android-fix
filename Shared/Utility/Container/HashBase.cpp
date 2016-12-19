// Hash set/map base
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>
#include "HashBase.hpp"

namespace Util
{

void HashBase::AllocateBuckets(unsigned size, unsigned numBuckets)
{
    if (ptrs_)
        delete[] ptrs_;

    HashNodeBase** ptrs = new HashNodeBase*[numBuckets + 2];
    unsigned* data = reinterpret_cast<unsigned*>(ptrs);
    data[0] = size;
    data[1] = numBuckets;
    ptrs_ = ptrs;

    ResetPtrs();
}

void HashBase::ResetPtrs()
{
    // Reset bucket pointers
    if (!ptrs_)
        return;

    unsigned numBuckets = NumBuckets();
    HashNodeBase** ptrs = Ptrs();
    for (unsigned i = 0; i < numBuckets; ++i)
        ptrs[i] = 0;
}

}
