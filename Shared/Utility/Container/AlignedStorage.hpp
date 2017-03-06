// AlignedStorage
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

namespace ctn
{

// Convenient template for integral constant types.
template <class T, T Val>
struct IntegralConstant
{
    using ValueType = T;
    using Type = IntegralConstant<T, Val>;

    // Return stored value.
    constexpr operator ValueType() const noexcept
    {
        return value;
    }

    // Return stored value.
    constexpr ValueType operator()() const noexcept
    {
        return value;
    }

    // Stored value.
    static constexpr T value = Val;
};

// Determine alignment of T.
template <class T>
struct AlignmentOf : IntegralConstant<size_t, alignof(T)>
{
};

#define _FITS(T) _Align <= AlignmentOf<T>::value
#define _NEXT_ALIGN(T) typedef typename Aligned<_Len, _Align, T, _FITS(T)>::Type Type

// Union with size _Len bytes and alignment of T.
template <class T, size_t _Len> union AlignType
{
    T _Val;
    char _Pad[_Len];
};

template <size_t _Len, size_t _Align, class T, bool _Ok>
struct Aligned;

// Define type with size _Len and alignment T.
template <size_t _Len, size_t _Align, class T> struct Aligned<_Len, _Align, T, true>
{
    typedef AlignType<T, _Len> Type;
};

// Define type with size _Len and alignment T.
template <size_t _Len, size_t _Align> struct Aligned<_Len, _Align, double, false>
{
    typedef AlignType<max_align_t, _Len> Type;
};

// Define type with size _Len and alignment T.
template <size_t _Len, size_t _Align> struct Aligned<_Len, _Align, int, false>
{
    _NEXT_ALIGN(double);
};

// Define type with size _Len and alignment T.
template <size_t _Len, size_t _Align> struct Aligned<_Len, _Align, short, false>
{
    _NEXT_ALIGN(int);
};

// Define type with size _Len and alignment T.
template <size_t _Len, size_t _Align> struct Aligned<_Len, _Align, char, false>
{
    _NEXT_ALIGN(short);
};

// Define type with size _Len and alignment _Align.
template <size_t _Len, size_t _Align = AlignmentOf<max_align_t>::value>
struct AlignedStorage
{
    typedef typename Aligned<_Len, _Align, char, _FITS(char)>::Type Type;
};

#undef _FITS
#undef _NEXT_ALIGN

}
