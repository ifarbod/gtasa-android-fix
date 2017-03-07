// SequentialIntegerSet.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file SequentialIntegerSet.h
    @brief The SequentialIntegerSet class. */
#pragma once

#include "kNet/Alignment.hpp"
#include "kNet/NetworkLogging.hpp"

namespace kNet
{

class SequentialIntegerSet
{
public:
    explicit SequentialIntegerSet(int tableSize_)
    :tableSize(tableSize_), tableSizeMask(tableSize_-1)
    {
        assert(IS_POW2(tableSize));
        table = new unsigned long[tableSize];

        // A bit unconventionally, a value of 0xFFFFFFFF denotes an empty spot.
        memset(table, 0xFF, sizeof(unsigned long) * tableSize);

        size = 0;
    }
    ~SequentialIntegerSet()
    {
        delete[] table;
    }

    /// Cannot necessarily return the exact size, but only an upper bound.
    int Size() const { return size; }

    int Capacity() const { return tableSize; }

    /// Recomputes the size of this set, so that Size() returns the exact value.
    void CountSize()
    {
        size = 0;
        for(int i = 0; i < tableSize; ++i)
            if (IsValid(table[i]))
                ++size;
    }

    void Prune()
    {
        unsigned long *newTable = new unsigned long[tableSize];
        size = 0;
        for(int i = 0; i < tableSize; ++i)
            if (IsValid(table[i]))
            {
                Add(newTable, table[i]);
                ++size;
            }
        delete[] table;
        table = newTable;
    }

    /// Returns the index in the table where the given value should exist.
    int Hash(unsigned long value) const { return (int)(value & tableSizeMask); }

    /// Probes the given index linearly and returns the next index to look in.
    int LinearProbe(int idx) const { return (idx + 1) & tableSizeMask; }

    bool IsNull(unsigned long value) const { return value == 0xFFFFFFFF; }

    bool IsValid(unsigned long value) const { return !IsNull(value); }

    void Add(unsigned long value)
    {
        Add(table, value);
    }

    bool Exists(unsigned long value) const
    {
        return table[Hash(value)] == value;
    }

private:
    unsigned long *table;
    int tableSize;
    int tableSizeMask;
    int size;

    void Add(unsigned long *dstTable, int value)
    {
        dstTable[Hash(value)] = value;
    }

    SequentialIntegerSet(const SequentialIntegerSet &); ///\todo Implement.
    void operator =(const SequentialIntegerSet &); ///\todo Implement.

};

} // ~kNet
