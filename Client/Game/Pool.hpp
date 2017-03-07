// Dynamic memory management for storing game's entities
// Author(s):       iFarbod <>
//                  GTA Modding Community <http://gtamodding.com>
//
// Copyright (c) 2015-2017 CtNorth Team
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
        u8 m_id : 7;
        bool m_isFree : 1;
    };
    struct
    {
        u8 m_flags;
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
        m_objects(nullptr),
        m_byteMap(nullptr),
        m_firstFree(-1),
        m_size(0),
        m_ownsAllocations(false)
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
            m_objects = static_cast<StoreType*>(objects);
            m_byteMap = static_cast<tPoolObjectFlags*>(infos);
            m_ownsAllocations = false;
        }
        // This pool allocates memory on its own
        else
        {
            m_objects = static_cast<StoreType*>(operator new(sizeof(StoreType) * size));
            m_byteMap = static_cast<tPoolObjectFlags*>(operator new(sizeof(tPoolObjectFlags) * size));
            m_ownsAllocations = true;
        }

        m_size = size;
        m_firstFree = -1;

        for (int i = 0; i < size; ++i)
        {
            m_byteMap[i].m_isFree = true;
            m_byteMap[i].m_id = 0;
        }
    }

    // Removes all items and releases owned memory.
    void Flush()
    {
        if (!m_size)
        {
            return;
        }
        if (m_ownsAllocations)
        {
            if (m_objects)
            {
                operator delete(m_objects);
            }
            if (m_byteMap)
            {
                operator delete(m_byteMap);
            }
        }

        m_objects = nullptr;
        m_byteMap = nullptr;
        m_size = 0;
        m_firstFree = 0;
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
        return m_byteMap[index].m_isFree;
    }

    // Set specified index as free/used.
    void SetFreeAt(s32 index, bool isFree = true)
    {
        m_byteMap[index].m_isFree = isFree;
    }

    // Set specified index as used.
    void SetNotFreeAt(s32 index)
    {
        SetFreeAt(index, false);
    }

    // Returns pointer to object by index.
    Type* GetAt(s32 index) const
    {
        return index >= 0 && index < m_size && !IsFreeAtSlot(index) ? static_cast<Type*>(&m_objects[index]) : nullptr;
    }

    // Allocate using first free space.
    Type* New()
    {
        return static_cast<Type*>(&m_objects[m_firstFree]);
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
        return static_cast<StoreType*>(obj) - m_objects;
    }

    s32 GetIndex(Type* obj) const
    {
        return static_cast<s32>(GetJustIndex(obj) | m_byteMap[GetJustIndex(obj)]->m_flags);
    }

private:
    // Holds stored items.
    StoreType* m_objects;
    // Specifies object state at index.
    tPoolObjectFlags* m_byteMap;
    // Count of objects this pool can hold.
    s32 m_size;
    // First free index.
    s32 m_firstFree;
    // Free allocated memory on destruction.
    bool m_ownsAllocations;
    // Is this pool locked?
    bool m_locked;
};

VALIDATE_SIZE(CPool<bool>, 0x14);

}
