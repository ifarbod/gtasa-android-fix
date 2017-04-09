// Count references to an object
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include "RefCounted.hpp"

namespace ctn
{

RefCounted::RefCounted() :
    m_refCount(new RefCount()),
    instantiationType_(INSTANTIATION_NATIVE),
    jsHeapPtr_(0)
{
    // Hold a weak ref to self to avoid possible double delete of the refcount
    (m_refCount->weakRefs_)++;
}

RefCounted::~RefCounted()
{
    assert(m_refCount);
    assert(m_refCount->refs_ == 0);
    assert(m_refCount->weakRefs_ > 0);

    // Mark object as expired, release the self weak ref and delete the refcount if no other weak refs exist
    m_refCount->refs_ = -1;
    (m_refCount->weakRefs_)--;
    if (!m_refCount->weakRefs_)
        delete m_refCount;

    m_refCount = 0;

    for (unsigned i = 0; i < refCountedDeletedFunctions_.Size(); i++)
        refCountedDeletedFunctions_[i](this);
}

void RefCounted::AddRef()
{
    assert(m_refCount->refs_ >= 0);
    (m_refCount->refs_)++;

    if (jsHeapPtr_ && m_refCount->refs_ == 2)
    {
        for (unsigned i = 0; i < refCountChangedFunctions_.Size(); i++)
        {
            refCountChangedFunctions_[i](this, 2);
        }
    }
}

void RefCounted::ReleaseRef()
{
    assert(m_refCount->refs_ > 0);
    (m_refCount->refs_)--;

    if (jsHeapPtr_ && m_refCount->refs_ == 1)
    {
        for (unsigned i = 0; i < refCountChangedFunctions_.Size(); i++)
        {
            (m_refCount->refs_)++;
            refCountChangedFunctions_[i](this, 1);
            if (m_refCount->refs_ == 1)
            {
                m_refCount->refs_ = 0;
                delete this;
                return;
            }
            (m_refCount->refs_)--;
        }
    }

    if (!m_refCount->refs_)
        delete this;
}

int RefCounted::Refs() const
{
    return m_refCount->refs_;
}

int RefCounted::WeakRefs() const
{
    // Subtract one to not return the internally held reference
    return m_refCount->weakRefs_ - 1;
}

PODVector<RefCountChangedFunction> RefCounted::refCountChangedFunctions_;
PODVector<RefCountedDeletedFunction> RefCounted::refCountedDeletedFunctions_;

void RefCounted::AddRefSilent()
{
    assert(m_refCount->refs_ >= 0);
    (m_refCount->refs_)++;
}

void RefCounted::AddRefCountChangedFunction(RefCountChangedFunction function)
{
    refCountChangedFunctions_.Push(function);
}

void RefCounted::RemoveRefCountChangedFunction(RefCountChangedFunction function)
{
    refCountChangedFunctions_.Remove(function);
}

void RefCounted::AddRefCountedDeletedFunction(RefCountedDeletedFunction function)
{
    refCountedDeletedFunctions_.Push(function);
}

void RefCounted::RemoveRefCountedDeletedFunction(RefCountedDeletedFunction function)
{
    refCountedDeletedFunctions_.Remove(function);
}

}
