// OrderedHashTable.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file OrderedHashTable.h
    @brief The OrderedHashTable<T> template class. */

namespace kNet
{

template<typename T, typename HashFunc>
class OrderedHashTable
{
public:
    struct Node
    {
        Node()
        :next(0), prev(0), hashChain(0), used(false)
        {
        }

        T value;
        bool used;
        Node *next;
        Node *prev;
        Node *hashChain;
    };

    explicit OrderedHashTable(size_t numElems)
    :root(0), numElemsMask(numElems-1), size(0)
    {
        data = new Node[numElems];
        root = 0;
    }
    ~OrderedHashTable()
    {
        delete[] data;
    }

    size_t Size() const { return size; }

    void Insert(const T &value)
    {
        int idx = HashFunc::Hash(value, numElemsMask);
        if (data[idx].used)
            Remove(&data[idx]);
        data[idx].value = value;
        if (root)
        {
            root->prev = &data[idx];
            data[idx].next = root;
        }
        else
            data[idx].next = 0;
        data[idx].prev = 0;
        data[idx].used = true;
        data[idx].hashChain = 0; ///\todo Support item chains.
        root = &data[idx];
        ++size;
    }

    void Clear()
    {
        while(size > 0)
            Remove(First());
    }

    void PopFront()
    {
        Remove(First());
    }

    template<typename KeyType>
    Node *Find(const KeyType &key)
    {
        int idx = HashFunc::Hash(key, numElemsMask);
        if (data[idx].used)
            return &data[idx];
        else
            return 0;
    }

    template<typename KeyType>
    void Remove(const KeyType &key)
    {
        int idx = HashFunc::Hash(key, numElemsMask);
        assert(data[idx].used);
        Remove(&data[idx]);
    }

    void Remove(Node *node)
    {
        if (!node)
            return;
        assert(node->used);

        if (node == root)
            root = node->next;

        node->used = false;
        if (node->prev)
            node->prev->next = node->next;
        if (node->next)
            node->next->prev = node->prev;
        --size;

        assert(root || size == 0);
    }

    Node *FrontNode()
    {
        return root;
    }

    T *Front()
    {
        assert(root || size == 0);
        return root ? &root->value : 0;
    }

    Node *First()
    {
        assert(root || size == 0);
        return root;
    }

    Node *Next(Node *node)
    {
        return node->next;
    }

private:
    Node *root;

    Node *data;
    size_t numElemsMask;
    size_t size;

    void operator =(const OrderedHashTable &); ///< \todo Implement.
    OrderedHashTable(const OrderedHashTable &); ///< \todo Implement.
};

} // ~kNet
