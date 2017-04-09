// Hash set/map base
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Container/Allocator.hpp>
#include <Container/Hash.hpp>
#include <Container/Swap.hpp>

namespace ctn
{

// Hash set/map node base class.
struct HashNodeBase
{
    // Construct.
    HashNodeBase() :
        down_(0),
        prev_(0),
        next_(0)
    {
    }

    // Next node in the bucket.
    HashNodeBase* down_;
    // Previous node.
    HashNodeBase* prev_;
    // Next node.
    HashNodeBase* next_;
};

// Hash set/map iterator base class.
struct HashIteratorBase
{
    // Construct.
    HashIteratorBase() :
        m_ptr(0)
    {
    }

    // Construct with a node pointer.
    explicit HashIteratorBase(HashNodeBase* ptr) :
        m_ptr(ptr)
    {
    }

    // Test for equality with another iterator.
    bool operator ==(const HashIteratorBase& rhs) const { return m_ptr == rhs.m_ptr; }

    // Test for inequality with another iterator.
    bool operator !=(const HashIteratorBase& rhs) const { return m_ptr != rhs.m_ptr; }

    // Go to the next node.
    void GotoNext()
    {
        if (m_ptr)
            m_ptr = m_ptr->next_;
    }

    // Go to the previous node.
    void GotoPrev()
    {
        if (m_ptr)
            m_ptr = m_ptr->prev_;
    }

    // Node pointer.
    HashNodeBase* m_ptr;
};

// Hash set/map base class.
// Note that to prevent extra memory use due to vtable pointer, HashBase intentionally does not declare a virtual destructor
// and therefore HashBase pointers should never be used.
class HashBase
{
public:
    // Initial amount of buckets.
    static const unsigned MIN_BUCKETS = 8;
    // Maximum load factor.
    static const unsigned MAX_LOAD_FACTOR = 4;

    // Construct.
    HashBase() :
        head_(0),
        tail_(0),
        ptrs_(0),
        allocator_(0)
    {
    }

    // Swap with another hash set or map.
    void Swap(HashBase& rhs)
    {
        ctn::Swap(head_, rhs.head_);
        ctn::Swap(tail_, rhs.tail_);
        ctn::Swap(ptrs_, rhs.ptrs_);
        ctn::Swap(allocator_, rhs.allocator_);
    }

    // Return number of elements.
    unsigned Size() const { return ptrs_ ? (reinterpret_cast<unsigned*>(ptrs_))[0] : 0; }

    // Return number of buckets.
    unsigned NumBuckets() const { return ptrs_ ? (reinterpret_cast<unsigned*>(ptrs_))[1] : 0; }

    // Return whether has no elements.
    bool Empty() const { return Size() == 0; }

protected:
    // Allocate bucket head pointers + room for size and bucket count variables.
    void AllocateBuckets(unsigned size, unsigned numBuckets);

    // Reset bucket head pointers.
    void ResetPtrs();

    // Set new size.
    void SetSize(unsigned size) { if (ptrs_) (reinterpret_cast<unsigned*>(ptrs_))[0] = size; }

    // Return bucket head pointers.
    HashNodeBase** Ptrs() const { return ptrs_ ? ptrs_ + 2 : 0; }

    // List head node pointer.
    HashNodeBase* head_;
    // List tail node pointer.
    HashNodeBase* tail_;
    // Bucket head pointers.
    HashNodeBase** ptrs_;
    // Node allocator.
    AllocatorBlock* allocator_;
};

}
