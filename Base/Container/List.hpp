// Doubly-linked list
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Container/ListBase.hpp>

#include <initializer_list>

namespace ctn
{

// Doubly-linked list template class.
template <class T> class List : public ListBase
{
public:
    // List node.
    struct Node : public ListNodeBase
    {
        // Construct undefined.
        Node() = default;

        // Construct with value.
        Node(const T& value) :
            value_(value)
        {
        }

        // Node value.
        T value_;

        // Return next node.
        Node* Next() const { return static_cast<Node*>(next_); }

        // Return previous node.
        Node* Prev() { return static_cast<Node*>(prev_); }
    };

    // List iterator.
    struct Iterator : public ListIteratorBase
    {
        // Construct.
        Iterator() = default;

        // Construct with a node pointer.
        explicit Iterator(Node* ptr) :
            ListIteratorBase(ptr)
        {
        }

        // Preincrement the pointer.
        Iterator& operator ++()
        {
            GotoNext();
            return *this;
        }

        // Postincrement the pointer.
        Iterator operator ++(int)
        {
            Iterator it = *this;
            GotoNext();
            return it;
        }

        // Predecrement the pointer.
        Iterator& operator --()
        {
            GotoPrev();
            return *this;
        }

        // Postdecrement the pointer.
        Iterator operator --(int)
        {
            Iterator it = *this;
            GotoPrev();
            return it;
        }

        // Point to the node value.
        T* operator ->() const { return &(static_cast<Node*>(m_ptr))->value_; }

        // Dereference the node value.
        T& operator *() const { return (static_cast<Node*>(m_ptr))->value_; }
    };

    // List const iterator.
    struct ConstIterator : public ListIteratorBase
    {
        // Construct.
        ConstIterator() = default;

        // Construct with a node pointer.
        explicit ConstIterator(Node* ptr) :
            ListIteratorBase(ptr)
        {
        }

        // Construct from a non-const iterator.
        ConstIterator(const Iterator& rhs) :
            ListIteratorBase(rhs.m_ptr)
        {
        }

        // Assign from a non-const iterator.
        ConstIterator& operator =(const Iterator& rhs)
        {
            m_ptr = rhs.m_ptr;
            return *this;
        }

        // Preincrement the pointer.
        ConstIterator& operator ++()
        {
            GotoNext();
            return *this;
        }

        // Postincrement the pointer.
        ConstIterator operator ++(int)
        {
            ConstIterator it = *this;
            GotoNext();
            return it;
        }

        // Predecrement the pointer.
        ConstIterator& operator --()
        {
            GotoPrev();
            return *this;
        }

        // Postdecrement the pointer.
        ConstIterator operator --(int)
        {
            ConstIterator it = *this;
            GotoPrev();
            return it;
        }

        // Point to the node value.
        const T* operator ->() const { return &(static_cast<Node*>(m_ptr))->value_; }

        // Dereference the node value.
        const T& operator *() const { return (static_cast<Node*>(m_ptr))->value_; }
    };

    // Construct empty.
    List()
    {
        allocator_ = AllocatorInitialize((unsigned)sizeof(Node));
        head_ = tail_ = ReserveNode();
    }

    // Construct from another list.
    List(const List<T>& list)
    {
        // Reserve the tail node + initial capacity according to the list's size
        allocator_ = AllocatorInitialize((unsigned)sizeof(Node), list.Size() + 1);
        head_ = tail_ = ReserveNode();
        *this = list;
    }

    // Aggregate initialization constructor.
    List(const std::initializer_list<T>& list) : List()
    {
        for (auto it = list.begin(); it != list.end(); it++)
        {
            Push(*it);
        }
    }

    // Destruct.
    ~List()
    {
        Clear();
        FreeNode(Tail());
        AllocatorUninitialize(allocator_);
    }

    // Assign from another list.
    List& operator =(const List<T>& rhs)
    {
        // Clear, then insert the nodes of the other list. In case of self-assignment do nothing
        if (&rhs != this)
        {
            Clear();
            Insert(End(), rhs);
        }
        return *this;
    }

    // Add-assign an element.
    List& operator +=(const T& rhs)
    {
        Push(rhs);
        return *this;
    }

    // Add-assign a list.
    List& operator +=(const List<T>& rhs)
    {
        Insert(End(), rhs);
        return *this;
    }

    // Test for equality with another list.
    bool operator ==(const List<T>& rhs) const
    {
        if (rhs.m_size != m_size)
            return false;

        ConstIterator i = Begin();
        ConstIterator j = rhs.Begin();
        while (i != End())
        {
            if (*i != *j)
                return false;
            ++i;
            ++j;
        }

        return true;
    }

    // Test for inequality with another list.
    bool operator !=(const List<T>& rhs) const
    {
        if (rhs.m_size != m_size)
            return true;

        ConstIterator i = Begin();
        ConstIterator j = rhs.Begin();
        while (i != End())
        {
            if (*i != *j)
                return true;
            ++i;
            ++j;
        }

        return false;
    }

    // Insert an element to the end.
    void Push(const T& value) { InsertNode(Tail(), value); }

    // Insert an element to the beginning.
    void PushFront(const T& value) { InsertNode(Head(), value); }

    // Insert an element at position.
    void Insert(const Iterator& dest, const T& value) { InsertNode(static_cast<Node*>(dest.m_ptr), value); }

    // Insert a list at position.
    void Insert(const Iterator& dest, const List<T>& list)
    {
        Node* destNode = static_cast<Node*>(dest.m_ptr);
        ConstIterator it = list.Begin();
        ConstIterator end = list.End();
        while (it != end)
            InsertNode(destNode, *it++);
    }

    // Insert elements by iterators.
    void Insert(const Iterator& dest, const ConstIterator& start, const ConstIterator& end)
    {
        Node* destNode = static_cast<Node*>(dest.m_ptr);
        ConstIterator it = start;
        while (it != end)
            InsertNode(destNode, *it++);
    }

    // Insert elements.
    void Insert(const Iterator& dest, const T* start, const T* end)
    {
        Node* destNode = static_cast<Node*>(dest.m_ptr);
        const T* ptr = start;
        while (ptr != end)
            InsertNode(destNode, *ptr++);
    }

    // Erase the last element.
    void Pop()
    {
        if (m_size)
            Erase(--End());
    }

    // Erase the first element.
    void PopFront()
    {
        if (m_size)
            Erase(Begin());
    }

    // Erase an element by iterator. Return iterator to the next element.
    Iterator Erase(Iterator it)
    {
        return Iterator(EraseNode(static_cast<Node*>(it.m_ptr)));
    }

    // Erase a range by iterators. Return an iterator to the next element.
    Iterator Erase(const Iterator& start, const Iterator& end)
    {
        Iterator it = start;
        while (it != end)
            it = Erase(it);

        return it;
    }

    // Clear the list.
    void Clear()
    {
        if (Size())
        {
            for (Iterator i = Begin(); i != End();)
            {
                FreeNode(static_cast<Node*>(i++.m_ptr));
                i.m_ptr->prev_ = 0;
            }

            head_ = tail_;
            m_size = 0;
        }
    }

    // Resize the list by removing or adding items at the end.
    void Resize(unsigned newSize)
    {
        while (m_size > newSize)
            Pop();

        while (m_size < newSize)
            InsertNode(Tail(), T());
    }

    // Return iterator to value, or to the end if not found.
    Iterator Find(const T& value)
    {
        Iterator it = Begin();
        while (it != End() && *it != value)
            ++it;
        return it;
    }

    // Return const iterator to value, or to the end if not found.
    ConstIterator Find(const T& value) const
    {
        ConstIterator it = Begin();
        while (it != End() && *it != value)
            ++it;
        return it;
    }

    // Return whether contains a specific value.
    bool Contains(const T& value) const { return Find(value) != End(); }

    // Return iterator to the first element.
    Iterator Begin() { return Iterator(Head()); }

    // Return iterator to the first element.
    ConstIterator Begin() const { return ConstIterator(Head()); }

    // Return iterator to the end.
    Iterator End() { return Iterator(Tail()); }

    // Return iterator to the end.
    ConstIterator End() const { return ConstIterator(Tail()); }

    // Return first element.
    T& Front() { return *Begin(); }

    // Return const first element.
    const T& Front() const { return *Begin(); }

    // Return last element.
    T& Back() { return *(--End()); }

    // Return const last element.
    const T& Back() const { return *(--End()); }

    // Return number of elements.
    unsigned Size() const { return m_size; }

    // Return whether list is empty.
    bool Empty() const { return m_size == 0; }

private:
    // Return the head node.
    Node* Head() const { return static_cast<Node*>(head_); }

    // Return the tail node.
    Node* Tail() const { return static_cast<Node*>(tail_); }

    // Allocate and insert a node into the list.
    void InsertNode(Node* dest, const T& value)
    {
        if (!dest)
            return;

        Node* newNode = ReserveNode(value);
        Node* prev = dest->Prev();
        newNode->next_ = dest;
        newNode->prev_ = prev;
        if (prev)
            prev->next_ = newNode;
        dest->prev_ = newNode;

        // Reassign the head node if necessary
        if (dest == Head())
            head_ = newNode;

        ++m_size;
    }

    // Erase and free a node. Return pointer to the next node, or to the end if could not erase.
    Node* EraseNode(Node* node)
    {
        // The tail node can not be removed
        if (!node || node == tail_)
            return Tail();

        Node* prev = node->Prev();
        Node* next = node->Next();
        if (prev)
            prev->next_ = next;
        next->prev_ = prev;

        // Reassign the head node if necessary
        if (node == Head())
            head_ = next;

        FreeNode(node);
        --m_size;

        return next;
    }

    // Reserve a node.
    Node* ReserveNode()
    {
        Node* newNode = static_cast<Node*>(AllocatorReserve(allocator_));
        new(newNode) Node();
        return newNode;
    }

    // Reserve a node with initial value.
    Node* ReserveNode(const T& value)
    {
        Node* newNode = static_cast<Node*>(AllocatorReserve(allocator_));
        new(newNode) Node(value);
        return newNode;
    }

    // Free a node.
    void FreeNode(Node* node)
    {
        (node)->~Node();
        AllocatorFree(allocator_, node);
    }
};

template <class T> typename ctn::List<T>::ConstIterator begin(const ctn::List<T>& v) { return v.Begin(); }

template <class T> typename ctn::List<T>::ConstIterator end(const ctn::List<T>& v) { return v.End(); }

template <class T> typename ctn::List<T>::Iterator begin(ctn::List<T>& v) { return v.Begin(); }

template <class T> typename ctn::List<T>::Iterator end(ctn::List<T>& v) { return v.End(); }

}
