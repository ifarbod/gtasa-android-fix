// RingBuffer.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file RingBuffer.h
    @brief The RingBuffer class stores a fast raw byte buffer queue storage. */

#include <vector>

namespace kNet
{

/// Implements a byte-based ring buffer (of sorts) of raw bytes.
class RingBuffer
{
public:
    explicit RingBuffer(int capacity)
    {
        data.resize(capacity);
        start = 0;
        end = 0;
    }

    /// Returns the total number of bytes that this RingBuffer can contain.
    int Capacity() const { return (int)data.size(); }

    /// Returns the number of bytes filled in the ring buffer.
    int Size() const { return end - start; }

    /// Compacts the ring buffer, i.e. moves all bytes to the beginning of the array.
    void Compact()
    {
        // If already compacted, do nothing.
        if (start == 0)
            return;

        const int numBytes = Size();
        for(int i = 0; i < numBytes; ++i)
            data[i] = data[start+i];

        start = 0;
        end = numBytes;
    }

    /// Enlarges the RingBuffer capacity so that it can fit at least the given number of bytes total.
    /// If the capacity of the RingBuffer was greater than this, does nothing.
    void Resize(int newSize)
    {
        assert(newSize > 0);

        if ((size_t)newSize <= data.size())
            return; // No need to resize.
        Compact();
        data.resize(newSize);
    }

    void Clear()
    {
        start = end = 0;
    }

    /// Returns a pointer to the first byte of actual data.
    char *Begin() { return &data[start]; }

    /// Returns a pointer to one past the last byte of actual data.
    char *End() { return &data[end]; }

    int StartIndex() const { return start; }

    /// Call after having inserted the given amount of bytes to the buffer.
    void Inserted(int numBytes)
    {
        end += numBytes;
        assert(end <= (int)data.size());
    }

    /// Call after having processed the given number of bytes from the buffer.
    void Consumed(int numBytes)
    {
        start += numBytes;
        assert(start <= end);
        if (start == end) // Free compact?
            start = end = 0;
    }

    /// Returns the total number of bytes that can be filled in this structure after compacting.
    int TotalFreeBytesLeft() const { return (int)data.size() - Size(); }

    /// Returns the number of bytes that can be added to this structure contiguously, without having to compact.
    int ContiguousFreeBytesLeft() const { return (int)data.size() - end; }

private:
    std::vector<char> data;
    int start; ///< Points to the first used byte.
    int end; ///< Points to the first unused byte.

};

} // ~kNet
