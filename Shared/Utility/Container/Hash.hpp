// Hash stuff
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <cstddef>

namespace Util
{

// Pointer hash function.
template <class T> unsigned MakeHash(T* value)
{
    return (unsigned)((size_t)value / sizeof(T));
}

// Const pointer hash function.
template <class T> unsigned MakeHash(const T* value)
{
    return (unsigned)((size_t)value / sizeof(T));
}

// Generic hash function.
template <class T> unsigned MakeHash(const T& value)
{
    return value.ToHash();
}

// Void pointer hash function.
template <> inline unsigned MakeHash(void* value)
{
    return (unsigned)(size_t)value;
}

// Const void pointer hash function.
template <> inline unsigned MakeHash(const void* value)
{
    return (unsigned)(size_t)value;
}

// Long long hash function.
template <> inline unsigned MakeHash(const long long& value)
{
    return (unsigned)((value >> 32) | (value & 0xffffffff));
}

// Unsigned long long hash function.
template <> inline unsigned MakeHash(const unsigned long long& value)
{
    return (unsigned)((value >> 32) | (value & 0xffffffff));
}

// Int hash function.
template <> inline unsigned MakeHash(const int& value)
{
    return (unsigned)value;
}

// Unsigned hash function.
template <> inline unsigned MakeHash(const unsigned& value)
{
    return value;
}

// Short hash function.
template <> inline unsigned MakeHash(const short& value)
{
    return (unsigned)value;
}

// Unsigned short hash function.
template <> inline unsigned MakeHash(const unsigned short& value)
{
    return value;
}

// Char hash function.
template <> inline unsigned MakeHash(const char& value)
{
    return (unsigned)value;
}

// Unsigned char hash function.
template <> inline unsigned MakeHash(const unsigned char& value)
{
    return value;
}

}
