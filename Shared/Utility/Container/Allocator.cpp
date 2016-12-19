// Allocate a memory block
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>
#include "Allocator.hpp"

namespace Util
{

AllocatorBlock* AllocatorReserveBlock(AllocatorBlock* allocator, unsigned nodeSize, unsigned capacity)
{
    if (!capacity)
        capacity = 1;

    unsigned char* blockPtr = new unsigned char[sizeof(AllocatorBlock) + capacity * (sizeof(AllocatorNode) + nodeSize)];
    AllocatorBlock* newBlock = reinterpret_cast<AllocatorBlock*>(blockPtr);
    newBlock->nodeSize_ = nodeSize;
    newBlock->capacity_ = capacity;
    newBlock->free_ = 0;
    newBlock->next_ = 0;

    if (!allocator)
        allocator = newBlock;
    else
    {
        newBlock->next_ = allocator->next_;
        allocator->next_ = newBlock;
    }

    // Initialize the nodes. Free nodes are always chained to the first (parent) allocator
    unsigned char* nodePtr = blockPtr + sizeof(AllocatorBlock);
    AllocatorNode* firstNewNode = reinterpret_cast<AllocatorNode*>(nodePtr);

    for (unsigned i = 0; i < capacity - 1; ++i)
    {
        AllocatorNode* newNode = reinterpret_cast<AllocatorNode*>(nodePtr);
        newNode->next_ = reinterpret_cast<AllocatorNode*>(nodePtr + sizeof(AllocatorNode) + nodeSize);
        nodePtr += sizeof(AllocatorNode) + nodeSize;
    }
    // i == capacity - 1
    {
        AllocatorNode* newNode = reinterpret_cast<AllocatorNode*>(nodePtr);
        newNode->next_ = 0;
    }

    allocator->free_ = firstNewNode;

    return newBlock;
}

AllocatorBlock* AllocatorInitialize(unsigned nodeSize, unsigned initialCapacity)
{
    AllocatorBlock* block = AllocatorReserveBlock(0, nodeSize, initialCapacity);
    return block;
}

void AllocatorUninitialize(AllocatorBlock* allocator)
{
    while (allocator)
    {
        AllocatorBlock* next = allocator->next_;
        delete[] reinterpret_cast<unsigned char*>(allocator);
        allocator = next;
    }
}

void* AllocatorReserve(AllocatorBlock* allocator)
{
    if (!allocator)
        return 0;

    if (!allocator->free_)
    {
        // Free nodes have been exhausted. Allocate a new larger block
        unsigned newCapacity = (allocator->capacity_ + 1) >> 1;
        AllocatorReserveBlock(allocator, allocator->nodeSize_, newCapacity);
        allocator->capacity_ += newCapacity;
    }

    // We should have new free node(s) chained
    AllocatorNode* freeNode = allocator->free_;
    void* ptr = (reinterpret_cast<unsigned char*>(freeNode)) + sizeof(AllocatorNode);
    allocator->free_ = freeNode->next_;
    freeNode->next_ = 0;

    return ptr;
}

void AllocatorFree(AllocatorBlock* allocator, void* ptr)
{
    if (!allocator || !ptr)
        return;

    unsigned char* dataPtr = static_cast<unsigned char*>(ptr);
    AllocatorNode* node = reinterpret_cast<AllocatorNode*>(dataPtr - sizeof(AllocatorNode));

    // Chain the node back to free nodes
    node->next_ = allocator->free_;
    allocator->free_ = node;
}

}
