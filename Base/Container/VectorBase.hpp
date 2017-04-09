// Vector base
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Container/Swap.hpp>

namespace ctn
{

// Random access iterator.
template <class T>
struct RandomAccessIterator
{
    // Construct.
    RandomAccessIterator() :
        m_ptr(0)
    {
    }

    // Construct with an object pointer.
    explicit RandomAccessIterator(T* ptr) :
        m_ptr(ptr)
    {
    }

    // Point to the object.
    T* operator ->() const { return m_ptr; }

    // Dereference the object.
    T& operator *() const { return *m_ptr; }

    // Preincrement the pointer.
    RandomAccessIterator<T>& operator ++()
    {
        ++m_ptr;
        return *this;
    }

    // Postincrement the pointer.
    RandomAccessIterator<T> operator ++(int)
    {
        RandomAccessIterator<T> it = *this;
        ++m_ptr;
        return it;
    }

    // Predecrement the pointer.
    RandomAccessIterator<T>& operator --()
    {
        --m_ptr;
        return *this;
    }

    // Postdecrement the pointer.
    RandomAccessIterator<T> operator --(int)
    {
        RandomAccessIterator<T> it = *this;
        --m_ptr;
        return it;
    }

    // Add an offset to the pointer.
    RandomAccessIterator<T>& operator +=(int value)
    {
        m_ptr += value;
        return *this;
    }

    // Subtract an offset from the pointer.
    RandomAccessIterator<T>& operator -=(int value)
    {
        m_ptr -= value;
        return *this;
    }

    // Add an offset to the pointer.
    RandomAccessIterator<T> operator +(int value) const { return RandomAccessIterator<T>(m_ptr + value); }

    // Subtract an offset from the pointer.
    RandomAccessIterator<T> operator -(int value) const { return RandomAccessIterator<T>(m_ptr - value); }

    // Calculate offset to another iterator.
    int operator -(const RandomAccessIterator& rhs) const { return (int)(m_ptr - rhs.m_ptr); }

    // Test for equality with another iterator.
    bool operator ==(const RandomAccessIterator& rhs) const { return m_ptr == rhs.m_ptr; }

    // Test for inequality with another iterator.
    bool operator !=(const RandomAccessIterator& rhs) const { return m_ptr != rhs.m_ptr; }

    // Test for less than with another iterator.
    bool operator <(const RandomAccessIterator& rhs) const { return m_ptr < rhs.m_ptr; }

    // Test for greater than with another iterator.
    bool operator >(const RandomAccessIterator& rhs) const { return m_ptr > rhs.m_ptr; }

    // Test for less than or equal with another iterator.
    bool operator <=(const RandomAccessIterator& rhs) const { return m_ptr <= rhs.m_ptr; }

    // Test for greater than or equal with another iterator.
    bool operator >=(const RandomAccessIterator& rhs) const { return m_ptr >= rhs.m_ptr; }

    // Pointer.
    T* m_ptr;
};

// Random access const iterator.
template <class T> struct RandomAccessConstIterator
{
    // Construct.
    RandomAccessConstIterator() :
        m_ptr(0)
    {
    }

    // Construct with an object pointer.
    explicit RandomAccessConstIterator(T* ptr) :
        m_ptr(ptr)
    {
    }

    // Construct from a non-const iterator.
    RandomAccessConstIterator(const RandomAccessIterator<T>& rhs) :
        m_ptr(rhs.m_ptr)
    {
    }

    // Assign from a non-const iterator.
    RandomAccessConstIterator<T>& operator =(const RandomAccessIterator<T>& rhs)
    {
        m_ptr = rhs.m_ptr;
        return *this;
    }

    // Point to the object.
    const T* operator ->() const { return m_ptr; }

    // Dereference the object.
    const T& operator *() const { return *m_ptr; }

    // Preincrement the pointer.
    RandomAccessConstIterator<T>& operator ++()
    {
        ++m_ptr;
        return *this;
    }

    // Postincrement the pointer.
    RandomAccessConstIterator<T> operator ++(int)
    {
        RandomAccessConstIterator<T> it = *this;
        ++m_ptr;
        return it;
    }

    // Predecrement the pointer.
    RandomAccessConstIterator<T>& operator --()
    {
        --m_ptr;
        return *this;
    }

    // Postdecrement the pointer.
    RandomAccessConstIterator<T> operator --(int)
    {
        RandomAccessConstIterator<T> it = *this;
        --m_ptr;
        return it;
    }

    // Add an offset to the pointer.
    RandomAccessConstIterator<T>& operator +=(int value)
    {
        m_ptr += value;
        return *this;
    }

    // Subtract an offset from the pointer.
    RandomAccessConstIterator<T>& operator -=(int value)
    {
        m_ptr -= value;
        return *this;
    }

    // Add an offset to the pointer.
    RandomAccessConstIterator<T> operator +(int value) const { return RandomAccessConstIterator<T>(m_ptr + value); }

    // Subtract an offset from the pointer.
    RandomAccessConstIterator<T> operator -(int value) const { return RandomAccessConstIterator<T>(m_ptr - value); }

    // Calculate offset to another iterator.
    int operator -(const RandomAccessConstIterator& rhs) const { return (int)(m_ptr - rhs.m_ptr); }

    // Test for equality with another iterator.
    bool operator ==(const RandomAccessConstIterator& rhs) const { return m_ptr == rhs.m_ptr; }

    // Test for inequality with another iterator.
    bool operator !=(const RandomAccessConstIterator& rhs) const { return m_ptr != rhs.m_ptr; }

    // Test for less than with another iterator.
    bool operator <(const RandomAccessConstIterator& rhs) const { return m_ptr < rhs.m_ptr; }

    // Test for greater than with another iterator.
    bool operator >(const RandomAccessConstIterator& rhs) const { return m_ptr > rhs.m_ptr; }

    // Test for less than or equal with another iterator.
    bool operator <=(const RandomAccessConstIterator& rhs) const { return m_ptr <= rhs.m_ptr; }

    // Test for greater than or equal with another iterator.
    bool operator >=(const RandomAccessConstIterator& rhs) const { return m_ptr >= rhs.m_ptr; }

    // Pointer.
    T* m_ptr;
};

// Returns an iterator pointing to the first element in the range [first, last) that is not less than value.
template <class TRandomAccessIterator, class T>
TRandomAccessIterator LowerBound(TRandomAccessIterator first, TRandomAccessIterator last, const T& value)
{
    unsigned count = last - first;

    while (count > 0)
    {
        const unsigned step = count / 2;
        const TRandomAccessIterator it = first + step;
        if (*it < value)
        {
            first = it + 1;
            count -= step + 1;
        }
        else
        {
            count = step;
        }
    }
    return first;
}

// Returns an iterator pointing to the first element in the range [first, last) that is greater than value.
template <class TRandomAccessIterator, class T>
TRandomAccessIterator UpperBound(TRandomAccessIterator first, TRandomAccessIterator last, const T& value)
{
    unsigned count = last - first;

    while (count > 0)
    {
        const unsigned step = count / 2;
        const TRandomAccessIterator it = first + step;
        if (!(value < *it))
        {
            first = it + 1;
            count -= step + 1;
        }
        else
        {
            count = step;
        };
    }
    return first;
}

// Vector base class.
// Note that to prevent extra memory use due to vtable pointer, VectorBase intentionally does not declare a virtual destructor
// and therefore VectorBase pointers should never be used.
class VectorBase
{
public:
    // Construct.
    VectorBase() :
        m_size(0),
        m_capacity(0),
        m_buffer(0)
    {
    }

    // Swap with another vector.
    void Swap(VectorBase& rhs)
    {
        ctn::Swap(m_size, rhs.m_size);
        ctn::Swap(m_capacity, rhs.m_capacity);
        ctn::Swap(m_buffer, rhs.m_buffer);
    }

protected:
    static unsigned char* AllocateBuffer(unsigned size);

    // Size of vector.
    unsigned m_size;
    // Buffer capacity.
    unsigned m_capacity;
    // Buffer.
    unsigned char* m_buffer;
};

}
