// A pair of two values with different/same types
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Container/Hash.hpp>

namespace ctn
{

// Pair template class.
template <class T, class U> class Pair
{
public:
    // Construct undefined.
    Pair() = default;

    // Construct with values.
    Pair(const T& first, const U& second) :
        first_(first),
        second_(second)
    {
    }

    // Test for equality with another pair.
    bool operator ==(const Pair<T, U>& rhs) const { return first_ == rhs.first_ && second_ == rhs.second_; }

    // Test for inequality with another pair.
    bool operator !=(const Pair<T, U>& rhs) const { return first_ != rhs.first_ || second_ != rhs.second_; }

    // Test for less than with another pair.
    bool operator <(const Pair<T, U>& rhs) const
    {
        if (first_ < rhs.first_)
            return true;
        if (first_ != rhs.first_)
            return false;
        return second_ < rhs.second_;
    }

    // Test for greater than with another pair.
    bool operator >(const Pair<T, U>& rhs) const
    {
        if (first_ > rhs.first_)
            return true;
        if (first_ != rhs.first_)
            return false;
        return second_ > rhs.second_;
    }

    // Return hash value for HashSet & HashMap.
    unsigned ToHash() const { return (MakeHash(first_) & 0xFFFF) | (MakeHash(second_) << 16); }

    // First value.
    T first_;
    // Second value.
    U second_;
};

// Construct a pair.
template <class T, class U> Pair<T, U> MakePair(const T& first, const U& second)
{
    return Pair<T, U>(first, second);
}

template <class T> T begin(ctn::Pair<T, T>& range) { return range.first_; }

template <class T> T end(ctn::Pair<T, T>& range) { return range.second_; }

template <class T> T begin(const ctn::Pair<T, T>& range) { return range.first_; }

template <class T> T end(const ctn::Pair<T, T>& range) { return range.second_; }

}
