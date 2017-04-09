// Base for double-linked lists
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Container/Allocator.hpp>
#include <Container/Swap.hpp>

namespace ctn
{

// Doubly-linked list node base class.
struct ListNodeBase
{
    // Construct.
    ListNodeBase() :
        prev_(0),
        next_(0)
    {
    }

    // Previous node.
    ListNodeBase* prev_;
    // Next node.
    ListNodeBase* next_;
};

// Doubly-linked list iterator base class.
struct ListIteratorBase
{
    // Construct.
    ListIteratorBase() :
        m_ptr(0)
    {
    }

    // Construct with a node pointer.
    explicit ListIteratorBase(ListNodeBase* ptr) :
        m_ptr(ptr)
    {
    }

    // Test for equality with another iterator.
    bool operator ==(const ListIteratorBase& rhs) const { return m_ptr == rhs.m_ptr; }

    // Test for inequality with another iterator.
    bool operator !=(const ListIteratorBase& rhs) const { return m_ptr != rhs.m_ptr; }

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
    ListNodeBase* m_ptr;
};

// Doubly-linked list base class.
class ListBase
{
public:
    // Construct.
    ListBase() :
        head_(0),
        tail_(0),
        allocator_(0),
        m_size(0)
    {
    }

    // Swap with another linked list.
    void Swap(ListBase& rhs)
    {
        ctn::Swap(head_, rhs.head_);
        ctn::Swap(tail_, rhs.tail_);
        ctn::Swap(allocator_, rhs.allocator_);
        ctn::Swap(m_size, rhs.m_size);
    }

protected:
    // Head node pointer.
    ListNodeBase* head_;
    // Tail node pointer.
    ListNodeBase* tail_;
    // Node allocator.
    AllocatorBlock* allocator_;
    // Number of nodes.
    unsigned m_size;
};

}
