// Dynamic memory management for storing game's entities
// Author(s):       iFarbod <>
//                  GTA Modding Community <http://gtamodding.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"

namespace ctn::SA
{

// Usage flags for each item in the pool.
union tPoolObjectFlags
{
    struct
    {
        u8 id_ : 7;
        bool isFree_ : 1;
    };
    struct
    {
        u8 flags_;
    };
};

VALIDATE_SIZE(tPoolObjectFlags, 1);

// Dynamic memory management for storing game's entities.
template <class T, class U = T>
class CPool
{
public:
    // Type you want to handle (in most cases It's parent type for all types this pool can store)
    using Type = T;
    // Type you want to store (in most cases It's the biggest child of @T)
    using StoreType = U;

    // Default constructor for statically allocated pools.
    CPool() :
        CPool(0)
    {
    }

    // Construct a pool that can hold items.
    CPool(s32 size, const char* poolName = nullptr) :
        objects_(nullptr),
        byteMap_(nullptr),
        firstFree_(-1),
        size_(0),
        ownsAllocations_(false)
    {
        if (size)
        {
            Init(size);
        }
    }

    // Destroy this pool.
    ~CPool()
    {
        Flush();
    }

    // Allocate memory for this pool, call this once.
    void Init(s32 size, void* objects = nullptr, void* infos = nullptr)
    {
        // Should we use pre-allocated objects?
        if (objects && infos)
        {
            objects_ = static_cast<StoreType*>(objects);
            byteMap_ = static_cast<tPoolObjectFlags*>(infos);
            ownsAllocations_ = false;
        }
        // This pool allocates memory on its own
        else
        {
            objects_ = static_cast<StoreType*>(operator new(sizeof(StoreType) * size));
            byteMap_ = static_cast<tPoolObjectFlags*>(operator new(sizeof(tPoolObjectFlags) * size));
            ownsAllocations_ = true;
        }

        size_ = size;
        firstFree_ = -1;

        for (int i = 0; i < size; ++i)
        {
            byteMap_[i].isFree_ = true;
            byteMap_[i].id_ = 0;
        }
    }

    // Removes all items and releases owned memory.
    void Flush()
    {
        if (!size_)
        {
            return;
        }
        if (ownsAllocations_)
        {
            if (objects_)
            {
                operator delete(objects_);
            }
            if (byteMap_)
            {
                operator delete(byteMap_);
            }
        }

        objects_ = nullptr;
        byteMap_ = nullptr;
        size_ = 0;
        firstFree_ = 0;
    }

    // Sets all items as free.
    void Clear()
    {
        for (s32 i = 0; i < size; ++i)
        {
            SetFreeAt(i, true);
        }
    }

    // Checks if the specified index is free.
    bool IsFreeAtIndex(s32 index)
    {
        return byteMap_[index].isFree_;
    }

    // Set specified index as free/used.
    void SetFreeAt(s32 index, bool isFree = true)
    {
        byteMap_[index].isFree_ = isFree;
    }

    // Set specified index as used.
    void SetNotFreeAt(s32 index)
    {
        SetFreeAt(index, false);
    }

    // Returns pointer to object by index.
    Type* GetAt(s32 index) const
    {
        return index >= 0 && index < size_ && !IsFreeAtSlot(index) ? static_cast<Type*>(&objects_[index]) : nullptr;
    }

    // Allocate using first free space.
    Type* New()
    {
        return static_cast<Type*>(&objects_[firstFree_]);
    }

    // Allocate using specific SCM handle.
    Type* New(s32 index)
    {

    }

    void Delete(Type* obj)
    {

    }

    s32 GetJustIndex(Type* obj) const
    {
        return static_cast<StoreType*>(obj) - objects_;
    }

    s32 GetIndex(Type* obj) const
    {
        return static_cast<s32>(GetJustIndex(obj) | byteMap_[GetJustIndex(obj)]->flags_);
    }

private:
    // Holds stored items.
    StoreType* objects_;
    // Specifies object state at index.
    tPoolObjectFlags* byteMap_;
    // Count of objects this pool can hold.
    s32 size_;
    // First free index.
    s32 firstFree_;
    // Free allocated memory on destruction.
    bool ownsAllocations_;
    // Is this pool locked?
    bool locked_;
};

VALIDATE_SIZE(CPool<bool>, 0x14);

}
