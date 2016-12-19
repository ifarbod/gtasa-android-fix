// Count references to an object
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include "RefCounted.hpp"

namespace Util
{

RefCounted::RefCounted() :
    refCount_(new RefCount()),
    instantiationType_(INSTANTIATION_NATIVE),
    jsHeapPtr_(0)
{
    // Hold a weak ref to self to avoid possible double delete of the refcount
    (refCount_->weakRefs_)++;
}

RefCounted::~RefCounted()
{
    assert(refCount_);
    assert(refCount_->refs_ == 0);
    assert(refCount_->weakRefs_ > 0);

    // Mark object as expired, release the self weak ref and delete the refcount if no other weak refs exist
    refCount_->refs_ = -1;
    (refCount_->weakRefs_)--;
    if (!refCount_->weakRefs_)
        delete refCount_;

    refCount_ = 0;

    for (unsigned i = 0; i < refCountedDeletedFunctions_.Size(); i++)
        refCountedDeletedFunctions_[i](this);
}

void RefCounted::AddRef()
{
    assert(refCount_->refs_ >= 0);
    (refCount_->refs_)++;

    if (jsHeapPtr_ && refCount_->refs_ == 2)
    {
        for (unsigned i = 0; i < refCountChangedFunctions_.Size(); i++)
        {
            refCountChangedFunctions_[i](this, 2);
        }
    }
}

void RefCounted::ReleaseRef()
{
    assert(refCount_->refs_ > 0);
    (refCount_->refs_)--;

    if (jsHeapPtr_ && refCount_->refs_ == 1)
    {
        for (unsigned i = 0; i < refCountChangedFunctions_.Size(); i++)
        {
            (refCount_->refs_)++;
            refCountChangedFunctions_[i](this, 1);
            if (refCount_->refs_ == 1)
            {
                refCount_->refs_ = 0;
                delete this;
                return;
            }
            (refCount_->refs_)--;
        }
    }

    if (!refCount_->refs_)
        delete this;
}

int RefCounted::Refs() const
{
    return refCount_->refs_;
}

int RefCounted::WeakRefs() const
{
    // Subtract one to not return the internally held reference
    return refCount_->weakRefs_ - 1;
}

PODVector<RefCountChangedFunction> RefCounted::refCountChangedFunctions_;
PODVector<RefCountedDeletedFunction> RefCounted::refCountedDeletedFunctions_;

void RefCounted::AddRefSilent()
{
    assert(refCount_->refs_ >= 0);
    (refCount_->refs_)++;
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
