// Temporary buffer
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

namespace Util
{

// Heap memory which is auto-deleted on RAII scope exit
template <class T>
class ScopedAlloc
{
    Vector<char> buffer;

public:
    ScopedAlloc(size_t sizeInBytes)
    {
        buffer.Resize(sizeInBytes);
    }

    void Resize(size_t newSizeInBytes)
    {
        buffer.Resize(newSizeInBytes);
    }

    operator T*(void)
    {
        return buffer.Empty() ? nullptr : reinterpret_cast <T*>(&buffer.At(0));
    }
};

}
