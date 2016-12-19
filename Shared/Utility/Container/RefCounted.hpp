// Count references to an object
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Container/Vector.hpp>

namespace Util
{

// Instantation type, native code, JS, or C#
enum InstantiationType
{
    INSTANTIATION_NATIVE = 0,
    INSTANTIATION_JAVASCRIPT = 1,
    INSTANTIATION_NET = 2
};

class RefCounted;

// Function that is called when ref count goes to 1 or 2+, used for script object lifetime
typedef void(*RefCountChangedFunction)(RefCounted*, int refCount);

// Function callback for when a RefCounted is deleted
typedef void(*RefCountedDeletedFunction)(RefCounted*);

typedef const void* ClassID;

// Macro to be included in RefCounted derived classes for efficient RTTI
#define CTN_REFCOUNTED(typeName) \
    public: \
        virtual Util::ClassID GetClassID() const { return GetClassIDStatic(); } \
        static Util::ClassID GetClassIDStatic() { static const int typeID = 0; return (Util::ClassID) &typeID; }


// Reference count structure.
struct RefCount
{
    // Construct.
    RefCount() :
        refs_(0),
        weakRefs_(0)
    {
    }

    // Destruct.
    ~RefCount()
    {
        // Set reference counts below zero to fire asserts if this object is still accessed
        refs_ = -1;
        weakRefs_ = -1;
    }

    // Reference count. If below zero, the object has been destroyed.
    int refs_;
    // Weak reference count.
    int weakRefs_;
};

// Base class for intrusively reference-counted objects. These are noncopyable and non-assignable.
class RefCounted
{
public:
    // Construct. Allocate the reference count structure and set an initial self weak reference.
    RefCounted();
    // Destruct. Mark as expired and also delete the reference count structure if no outside weak references exist.
    virtual ~RefCounted();

    // Increment reference count. Can also be called outside of a SharedPtr for traditional reference counting.
    void AddRef();
    // Decrement reference count and delete self if no more references. Can also be called outside of a SharedPtr for traditional reference counting.
    void ReleaseRef();
    // Return reference count.
    int Refs() const;
    // Return weak reference count.
    int WeakRefs() const;

    // Return pointer to the reference count structure.
    RefCount* RefCountPtr() { return refCount_; }

    virtual bool IsObject() const { return false; }

    // Increment reference count. Do not call any lifetime book keeping
    void AddRefSilent();

    virtual ClassID GetClassID() const = 0;
    static ClassID GetClassIDStatic() { static const int typeID = 0; return (ClassID)&typeID; }

    // JavaScript VM, heap object which can be pushed directly on stack without any lookups
    inline void* JSGetHeapPtr() const { return jsHeapPtr_; }
    inline void  JSSetHeapPtr(void* heapptr) { jsHeapPtr_ = heapptr; }

    inline InstantiationType GetInstantiationType()  const { return instantiationType_; }
    inline void SetInstantiationType(InstantiationType type) { instantiationType_ = type; }

    static void AddRefCountChangedFunction(RefCountChangedFunction function);
    static void RemoveRefCountChangedFunction(RefCountChangedFunction function);

    static void AddRefCountedDeletedFunction(RefCountedDeletedFunction function);
    static void RemoveRefCountedDeletedFunction(RefCountedDeletedFunction function);


private:
    // Prevent copy construction.
    RefCounted(const RefCounted& rhs);
    // Prevent assignment.
    RefCounted& operator =(const RefCounted& rhs) = default; // delete?

    // Pointer to the reference count structure.
    RefCount* refCount_;

    InstantiationType instantiationType_;
    void* jsHeapPtr_;

    static PODVector<RefCountChangedFunction> refCountChangedFunctions_;
    static PODVector<RefCountedDeletedFunction> refCountedDeletedFunctions_;
};

}
