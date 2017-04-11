// Smart pointers
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Container/RefCounted.hpp>

#include <cassert>
#include <cstddef>
#include <utility>

namespace ctn
{

// Shared pointer template class with intrusive reference counting.
template <class T> class SharedPtr
{
public:
    // Construct a null shared pointer.
    SharedPtr() :
        m_ptr(0)
    {
    }

    // Construct a null shared pointer.
    SharedPtr(std::nullptr_t) :
        m_ptr(nullptr)
    {
    }

    // Copy-construct from another shared pointer.
    SharedPtr(const SharedPtr<T>& rhs) :
        m_ptr(rhs.m_ptr)
    {
        AddRef();
    }

    // Copy-construct from another shared pointer allowing implicit upcasting.
    template <class U> SharedPtr(const SharedPtr<U>& rhs) :
        m_ptr(rhs.m_ptr)
    {
        AddRef();
    }

    // Construct from a raw pointer.
    explicit SharedPtr(T* ptr) :
        m_ptr(ptr)
    {
        AddRef();
    }

    // Destruct. Release the object reference.
    ~SharedPtr()
    {
        ReleaseRef();
    }

    // Assign from another shared pointer.
    SharedPtr<T>& operator =(const SharedPtr<T>& rhs)
    {
        if (m_ptr == rhs.m_ptr)
            return *this;

        ReleaseRef();
        m_ptr = rhs.m_ptr;
        AddRef();

        return *this;
    }

    // Assign from another shared pointer allowing implicit upcasting.
    template <class U> SharedPtr<T>& operator =(const SharedPtr<U>& rhs)
    {
        if (m_ptr == rhs.m_ptr)
            return *this;

        ReleaseRef();
        m_ptr = rhs.m_ptr;
        AddRef();

        return *this;
    }

    // Assign from a raw pointer.
    SharedPtr<T>& operator =(T* ptr)
    {
        if (m_ptr == ptr)
            return *this;

        ReleaseRef();
        m_ptr = ptr;
        AddRef();

        return *this;
    }

    // Point to the object.
    T* operator ->() const
    {
        assert(m_ptr);
        return m_ptr;
    }

    // Dereference the object.
    T& operator *() const
    {
        assert(m_ptr);
        return *m_ptr;
    }

    // Subscript the object if applicable.
    T& operator [](const int index)
    {
        assert(m_ptr);
        return m_ptr[index];
    }

    // Test for less than with another shared pointer
    template <class U> bool operator <(const SharedPtr<U>& rhs) const { return m_ptr < rhs.m_ptr; }

    // Test for equality with another shared pointer.
    template <class U> bool operator ==(const SharedPtr<U>& rhs) const { return m_ptr == rhs.m_ptr; }

    // Test for inequality with another shared pointer.
    template <class U> bool operator !=(const SharedPtr<U>& rhs) const { return m_ptr != rhs.m_ptr; }

    // Convert to a raw pointer.
    operator T*() const { return m_ptr; }

    // Reset to null and release the object reference.
    void Reset() { ReleaseRef(); }

    // Detach without destroying the object even if the refcount goes zero.
    // To be used for scripting language interoperation.
    T* Detach()
    {
        T* ptr = m_ptr;
        if (m_ptr)
        {
            RefCount* refCount = RefCountPtr();
            ++refCount->refs_; // 2 refs
            Reset(); // 1 ref
            --refCount->refs_; // 0 refs
        }
        return ptr;
    }

    // Perform a static cast from a shared pointer of another type.
    template <class U> void StaticCast(const SharedPtr<U>& rhs)
    {
        ReleaseRef();
        m_ptr = static_cast<T*>(rhs.Get());
        AddRef();
    }

    // Perform a dynamic cast from a shared pointer of another type.
    template <class U> void DynamicCast(const SharedPtr<U>& rhs)
    {
        ReleaseRef();
        m_ptr = dynamic_cast<T*>(rhs.Get());
        AddRef();
    }

    // Check if the pointer is null.
    bool Null() const { return m_ptr == 0; }

    // Check if the pointer is not null.
    bool NotNull() const { return m_ptr != 0; }

    // Return the raw pointer.
    T* Get() const { return m_ptr; }

    // Return the object's reference count, or 0 if the pointer is null.
    int Refs() const { return m_ptr ? m_ptr->Refs() : 0; }

    // Return the object's weak reference count, or 0 if the pointer is null.
    int WeakRefs() const { return m_ptr ? m_ptr->WeakRefs() : 0; }

    // Return pointer to the RefCount structure.
    RefCount* RefCountPtr() const { return m_ptr ? m_ptr->RefCountPtr() : 0; }

    // Return hash value for HashSet & HashMap.
    unsigned ToHash() const { return (unsigned)((size_t)m_ptr / sizeof(T)); }

private:
    template <class U> friend class SharedPtr;

    // Add a reference to the object pointed to.
    void AddRef()
    {
        if (m_ptr)
            m_ptr->AddRef();
    }

    // Release the object reference and delete it if necessary.
    void ReleaseRef()
    {
        if (m_ptr)
        {
            m_ptr->ReleaseRef();
            m_ptr = 0;
        }
    }

    // Pointer to the object.
    T* m_ptr;
};

// Perform a static cast from one shared pointer type to another.
template <class T, class U> SharedPtr<T> StaticCast(const SharedPtr<U>& ptr)
{
    SharedPtr<T> ret;
    ret.StaticCast(ptr);
    return ret;
}

// Perform a dynamic cast from one weak pointer type to another.
template <class T, class U> SharedPtr<T> DynamicCast(const SharedPtr<U>& ptr)
{
    SharedPtr<T> ret;
    ret.DynamicCast(ptr);
    return ret;
}

// Weak pointer template class with intrusive reference counting. Does not keep the object pointed to alive.
template <class T> class WeakPtr
{
public:
    // Construct a null weak pointer.
    WeakPtr() :
        m_ptr(nullptr),
        m_refCount(0)
    {
    }

    // Construct a null weak pointer.
    WeakPtr(std::nullptr_t) :
        m_ptr(nullptr),
        m_refCount(0)
    {
    }

    // Copy-construct from another weak pointer.
    WeakPtr(const WeakPtr<T>& rhs) :
        m_ptr(rhs.m_ptr),
        m_refCount(rhs.m_refCount)
    {
        AddRef();
    }

    // Copy-construct from another weak pointer allowing implicit upcasting.
    template <class U> WeakPtr(const WeakPtr<U>& rhs) :
        m_ptr(rhs.m_ptr),
        m_refCount(rhs.m_refCount)
    {
        AddRef();
    }

    // Construct from a shared pointer.
    WeakPtr(const SharedPtr<T>& rhs) :
        m_ptr(rhs.Get()),
        m_refCount(rhs.RefCountPtr())
    {
        AddRef();
    }

    // Construct from a raw pointer.
    explicit WeakPtr(T* ptr) :
        m_ptr(ptr),
        m_refCount(ptr ? ptr->RefCountPtr() : 0)
    {
        AddRef();
    }

    // Destruct. Release the weak reference to the object.
    ~WeakPtr()
    {
        ReleaseRef();
    }

    // Assign from a shared pointer.
    WeakPtr<T>& operator =(const SharedPtr<T>& rhs)
    {
        if (m_ptr == rhs.Get() && m_refCount == rhs.RefCountPtr())
            return *this;

        ReleaseRef();
        m_ptr = rhs.Get();
        m_refCount = rhs.RefCountPtr();
        AddRef();

        return *this;
    }

    // Assign from a weak pointer.
    WeakPtr<T>& operator =(const WeakPtr<T>& rhs)
    {
        if (m_ptr == rhs.m_ptr && m_refCount == rhs.m_refCount)
            return *this;

        ReleaseRef();
        m_ptr = rhs.m_ptr;
        m_refCount = rhs.m_refCount;
        AddRef();

        return *this;
    }

    // Assign from another weak pointer allowing implicit upcasting.
    template <class U> WeakPtr<T>& operator =(const WeakPtr<U>& rhs)
    {
        if (m_ptr == rhs.m_ptr && m_refCount == rhs.m_refCount)
            return *this;

        ReleaseRef();
        m_ptr = rhs.m_ptr;
        m_refCount = rhs.m_refCount;
        AddRef();

        return *this;
    }

    // Assign from a raw pointer.
    WeakPtr<T>& operator =(T* ptr)
    {
        RefCount* refCount = ptr ? ptr->RefCountPtr() : nullptr;

        if (m_ptr == ptr && m_refCount == refCount)
            return *this;

        ReleaseRef();
        m_ptr = ptr;
        m_refCount = refCount;
        AddRef();

        return *this;
    }

    // Convert to a shared pointer. If expired, return a null shared pointer.
    SharedPtr<T> Lock() const
    {
        if (Expired())
            return SharedPtr<T>();
        else
            return SharedPtr<T>(m_ptr);
    }

    // Return raw pointer. If expired, return null.
    T* Get() const
    {
        if (Expired())
            return nullptr;
        else
            return m_ptr;
    }

    // Point to the object.
    T* operator ->() const
    {
        T* rawPtr = Get();
        assert(rawPtr);
        return rawPtr;
    }

    // Dereference the object.
    T& operator *() const
    {
        T* rawPtr = Get();
        assert(rawPtr);
        return *rawPtr;
    }

    // Subscript the object if applicable.
    T& operator [](const int index)
    {
        T* rawPtr = Get();
        assert(rawPtr);
        return (*rawPtr)[index];
    }

    // Test for equality with another weak pointer.
    template <class U> bool operator ==(const WeakPtr<U>& rhs) const { return m_ptr == rhs.m_ptr && m_refCount == rhs.m_refCount; }

    // Test for inequality with another weak pointer.
    template <class U> bool operator !=(const WeakPtr<U>& rhs) const { return m_ptr != rhs.m_ptr || m_refCount != rhs.m_refCount; }

    // Test for less than with another weak pointer.
    template <class U> bool operator <(const WeakPtr<U>& rhs) const { return m_ptr < rhs.m_ptr; }

    // Convert to a raw pointer, null if the object is expired.
    operator T*() const { return Get(); }

    // Reset to null and release the weak reference.
    void Reset() { ReleaseRef(); }

    // Perform a static cast from a weak pointer of another type.
    template <class U> void StaticCast(const WeakPtr<U>& rhs)
    {
        ReleaseRef();
        m_ptr = static_cast<T*>(rhs.Get());
        m_refCount = rhs.m_refCount;
        AddRef();
    }

    // Perform a dynamic cast from a weak pointer of another type.
    template <class U> void DynamicCast(const WeakPtr<U>& rhs)
    {
        ReleaseRef();
        m_ptr = dynamic_cast<T*>(rhs.Get());

        if (m_ptr)
        {
            m_refCount = rhs.m_refCount;
            AddRef();
        }
        else
            m_refCount = 0;
    }

    // Check if the pointer is null.
    bool Null() const { return m_refCount == 0; }

    // Check if the pointer is not null.
    bool NotNull() const { return m_refCount != 0; }

    // Return the object's reference count, or 0 if null pointer or if object has expired.
    int Refs() const { return (m_refCount && m_refCount->refs_ >= 0) ? m_refCount->refs_ : 0; }

    // Return the object's weak reference count.
    int WeakRefs() const
    {
        if (!Expired())
            return m_ptr->WeakRefs();
        else
            return m_refCount ? m_refCount->weakRefs_ : 0;
    }

    // Return whether the object has expired. If null pointer, always return true.
    bool Expired() const { return m_refCount ? m_refCount->refs_ < 0 : true; }

    // Return pointer to the RefCount structure.
    RefCount* RefCountPtr() const { return m_refCount; }

    // Return hash value for HashSet & HashMap.
    unsigned ToHash() const { return (unsigned)((size_t)m_ptr / sizeof(T)); }

private:
    template <class U> friend class WeakPtr;

    // Add a weak reference to the object pointed to.
    void AddRef()
    {
        if (m_refCount)
        {
            assert(m_refCount->weakRefs_ >= 0);
            ++(m_refCount->weakRefs_);
        }
    }

    // Release the weak reference. Delete the Refcount structure if necessary.
    void ReleaseRef()
    {
        if (m_refCount)
        {
            assert(m_refCount->weakRefs_ > 0);
            --(m_refCount->weakRefs_);

            if (Expired() && !m_refCount->weakRefs_)
                delete m_refCount;
        }

        m_ptr = 0;
        m_refCount = 0;
    }

    // Pointer to the object.
    T* m_ptr;
    // Pointer to the RefCount structure.
    RefCount* m_refCount;
};

// Perform a static cast from one weak pointer type to another.
template <class T, class U> WeakPtr<T> StaticCast(const WeakPtr<U>& ptr)
{
    WeakPtr<T> ret;
    ret.StaticCast(ptr);
    return ret;
}

// Perform a dynamic cast from one weak pointer type to another.
template <class T, class U> WeakPtr<T> DynamicCast(const WeakPtr<U>& ptr)
{
    WeakPtr<T> ret;
    ret.DynamicCast(ptr);
    return ret;
}

// Delete object of type T. T must be complete. See boost::checked_delete.
template <class T> inline
void CheckedDelete(T* x)
{
    // Intentionally complex - simplification causes regressions
    using type_must_be_complete[sizeof(T) ? 1 : -1] = char;
    (void) sizeof(type_must_be_complete);
    delete x;
}

// Unique pointer template class.
template <class T>
class UniquePtr
{
    // Make non-copyable.
    UniquePtr(const UniquePtr&);
    UniquePtr& operator=(const UniquePtr&) = default; // delete?

public:
    // Construct empty.
    UniquePtr() : m_ptr(nullptr) { }

    // Construct from pointer.
    explicit UniquePtr(T* ptr) : m_ptr(ptr) { }

    // Assign from pointer.
    UniquePtr& operator = (T* ptr)
    {
        Reset(ptr);
        return *this;
    }

    // Construct empty.
    UniquePtr(std::nullptr_t) { }

    // Move-construct from UniquePtr.
    UniquePtr(UniquePtr&& up) : m_ptr(up.Detach()) { }

    // Move-assign from UniquePtr.
    UniquePtr& operator = (UniquePtr&& up)
    {
        Reset(up.Detach());
        return *this;
    }

    // Point to the object.
    T* operator ->() const
    {
        assert(m_ptr);
        return m_ptr;
    }

    // Dereference the object.
    T& operator *() const
    {
        assert(m_ptr);
        return *m_ptr;
    }

    // Test for less than with another unique pointer.
    template <class U>
    bool operator <(const UniquePtr<U>& rhs) const { return m_ptr < rhs.m_ptr; }

    // Test for equality with another unique pointer.
    template <class U>
    bool operator ==(const UniquePtr<U>& rhs) const { return m_ptr == rhs.m_ptr; }

    // Test for inequality with another unique pointer.
    template <class U>
    bool operator !=(const UniquePtr<U>& rhs) const { return m_ptr != rhs.m_ptr; }

    // Cast pointer to bool.
    operator bool() const { return !!m_ptr; }

    // Swap with another UniquePtr.
    void Swap(UniquePtr& up) { Swap(m_ptr, up.m_ptr); }

    // Detach pointer from UniquePtr without destroying.
    T* Detach()
    {
        T* ptr = m_ptr;
        m_ptr = nullptr;
        return ptr;
    }

    // Check if the pointer is null.
    bool Null() const { return m_ptr == nullptr; }

    // Check if the pointer is not null.
    bool NotNull() const { return m_ptr != nullptr; }

    // Return the raw pointer.
    T* Get() const { return m_ptr; }

    // Reset.
    void Reset(T* ptr = nullptr)
    {
        CheckedDelete(m_ptr);
        m_ptr = ptr;
    }

    // Return hash value for HashSet & HashMap.
    unsigned ToHash() const { return (unsigned)((size_t)m_ptr / sizeof(T)); }

    // Destruct.
    ~UniquePtr()
    {
        Reset();
    }

private:
    T* m_ptr;

};

// Swap two UniquePtr-s.
template <class T> void Swap(UniquePtr<T>& first, UniquePtr<T>& second)
{
    first.Swap(second);
}

// Construct UniquePtr.
template <class T, class ... Args> UniquePtr<T> MakeUnique(Args&&... args)
{
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

// Construct SharedPtr.
template <class T, class... Args> SharedPtr<T> MakeShared(Args&&... args)
{
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

}
