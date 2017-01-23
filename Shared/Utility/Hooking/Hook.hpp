// Code injecting utils
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  LINK/2012 <dma2012@hotmail.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Types.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>
#include <forward_list>

namespace ctn
{

namespace Hook
{

union MemoryPointer
{
public:
    // Default constructor.
    MemoryPointer() : ptr_(0) {}
    // Construct from nullptr.
    MemoryPointer(std::nullptr_t) : ptr_(nullptr) {}
    // Copy constructor.
    MemoryPointer(const MemoryPointer& x) = default;
    // Construct from a pointer.
    MemoryPointer(void* x) : ptr_(x) {}
    // Construct from an integral pointer.
    MemoryPointer(uintptr_t x) : a_(x) {}
    // Construct from a pointer with a specified type.
    template <class T> MemoryPointer(T* x) : ptr_(reinterpret_cast<void*>(x)) {}

    // Returns true if the underlying pointer is a nullptr.
    bool IsNull() const { return this->ptr_ != nullptr; }
    // Return the underlying pointer as a uintptr_t.
    uintptr_t AsInt() const { return this->a_; }

    explicit operator bool() const { return IsNull(); }
    explicit operator uintptr_t() const { return this->a_; }

    MemoryPointer Get() const { return *this; }
    template <class T> T* Get() const { return Get(); }
    template <class T> T* GetRaw() { return Get(); }

    template <class T> operator T*() const { return reinterpret_cast<T*>(ptr_); }

    // Comparison
    bool operator==(const MemoryPointer& rhs) const { return this->a_ == rhs.a_; }
    bool operator!=(const MemoryPointer& rhs) const { return this->a_ != rhs.a_; }
    bool operator<(const MemoryPointer& rhs) const { return this->a_ < rhs.a_; }
    bool operator<=(const MemoryPointer& rhs) const { return this->a_ <= rhs.a_; }
    bool operator>(const MemoryPointer& rhs) const { return this->a_ > rhs.a_; }
    bool operator>=(const MemoryPointer& rhs) const { return this->a_ >= rhs.a_; }

    // Operators
    MemoryPointer operator+(const MemoryPointer& rhs) const { return MemoryPointer(this->a_ + rhs.a_); }
    MemoryPointer operator-(const MemoryPointer& rhs) const { return MemoryPointer(this->a_ - rhs.a_); }
    MemoryPointer operator*(const MemoryPointer& rhs) const { return MemoryPointer(this->a_ * rhs.a_); }
    MemoryPointer operator/(const MemoryPointer& rhs) const { return MemoryPointer(this->a_ / rhs.a_); }

    MemoryPointer operator+=(const MemoryPointer& rhs) const { return MemoryPointer(this->a_ + rhs.a_); }
    MemoryPointer operator-=(const MemoryPointer& rhs) const { return MemoryPointer(this->a_ - rhs.a_); }
    MemoryPointer operator*=(const MemoryPointer& rhs) const { return MemoryPointer(this->a_ * rhs.a_); }
    MemoryPointer operator/=(const MemoryPointer& rhs) const { return MemoryPointer(this->a_ / rhs.a_); }

protected:
    // Pointer.
    void* ptr_;
    // Unsigned int32.
    uintptr_t a_;
};

template <uintptr_t addr>
struct LazyPointer
{
public:
    // Returns the final raw pointer
    static MemoryPointer Get() { return xGet().Get(); }

    template <class T> static T* Get() { return Get().Get<T>(); }

private:
    // Returns the final pointer
    static MemoryPointer xGet()
    {
        static void* ptr = nullptr;
        if (!ptr)
            ptr = MemoryPointer(addr).Get();
        return MemoryPointer(ptr);
    }
};

template <uintptr_t addr> inline MemoryPointer LazyPtr() { return LazyPointer<addr>::Get(); }

// Memory protection

inline bool ProtectMemory(MemoryPointer addr, size_t size, DWORD protection)
{
    return VirtualProtect(addr.Get(), size, protection, &protection) != 0;
}

inline bool UnprotectMemory(MemoryPointer addr, size_t size, DWORD& out_oldprotect)
{
    return VirtualProtect(addr.Get(), size, PAGE_EXECUTE_READWRITE, &out_oldprotect) != 0;
}

struct ScopedUnprotect
{
    MemoryPointer addr;
    size_t size;
    DWORD dwOldProtect;
    bool bUnprotected;

    ScopedUnprotect(MemoryPointer addr, size_t size)
    {
        if (size == 0) bUnprotected = false;
        else bUnprotected = UnprotectMemory(this->addr = addr.Get<void>(), this->size = size, dwOldProtect);
    }

    ~ScopedUnprotect()
    {
        if (bUnprotected) ProtectMemory(this->addr.Get(), this->size, this->dwOldProtect);
    }
};

// Methods for reading/writing memory

// Gets contents from a memory address
template <class T>
inline T MemRead(MemoryPointer addr)
{
    return *addr.Get<T>();
}

template <class T = void>
inline T& ReadOffset(MemoryPointer ptr, size_t offset)
{
    return *(ptr + offset).Get<T>();
}

inline void MemSet(MemoryPointer addr, s32 value, size_t size)
{
    ScopedUnprotect xprotect(addr, size);
    memset(addr.Get<void>(), value, size);
}

inline void MemCpy(MemoryPointer addr, void const* src, size_t size)
{
    ScopedUnprotect xprotect(addr, size);
    memcpy(addr.Get<void>(), src, size);
}

template <class T>
inline void MemWrite(MemoryPointer addr, T value)
{
    ScopedUnprotect xprotect(addr, sizeof(T));

    if (MemRead<T>(addr) != value)
    {
        *addr.Get<T>() = value;
    }
}

// Searches in the range [@addr, @addr + @max_search] for a pointer in the range [@default_base, @default_end] and replaces
// it with the proper offset in the pointer @replacement_base.
// does memory unprotection if @vp is true.
inline MemoryPointer AdjustPointer(MemoryPointer addr, MemoryPointer replacement_base, MemoryPointer default_base,
    MemoryPointer default_end, size_t max_search = 8)
{
    ScopedUnprotect xprotect(addr, max_search + sizeof(void*));
    for (size_t i = 0; i < max_search; ++i)
    {
        MemoryPointer ptr = MemRead<void*>(addr + i);
        if (ptr >= default_base.Get() && ptr <= default_end.Get())
        {
            auto result = replacement_base + (ptr - default_base.Get());
            MemWrite<void*>(addr + i, result.Get());
            return result;
        }
    }
    return nullptr;
}

inline void MemFill(MemoryPointer addr, u8 value, size_t size = 1)
{
    MemSet(addr, value, size);
}

inline void CopyStr(MemoryPointer addr, char const* value)
{
    ScopedUnprotect xprotect(addr, 1);
    strcpy(addr, value);
}

inline void CopyStrEx(MemoryPointer addr, char const* value, size_t count)
{
    ScopedUnprotect xprotect(addr, count);
    strncpy(addr, value, count);
}

inline void ZeroMem(MemoryPointer at, size_t count = 1)
{
    MemFill(at, 0, count);
}

inline void MakeNop(MemoryPointer at, size_t count = 1)
{
    MemFill(at, 0x90, count);
}

inline void MakeRangedNop(MemoryPointer at, MemoryPointer until)
{
    return MakeNop(at, size_t(until.GetRaw<char>() - at.GetRaw<char>()));
}

// C3 RET
inline void MakeRet(MemoryPointer at)
{
    MemWrite<u8>(at, 0xC3);
}

// C2 RET (2Bytes)
inline void MakeRet(MemoryPointer at, u16 pop)
{
    MemWrite<u8>(at, 0xC2);
    MemWrite<u16>(at + 1, pop);
}

inline void MakeRETEx(MemoryPointer at, u8 ret = 1)
{
    MemWrite<u8>(at, 0xB0); // mov al, @ret
    MemWrite<u8>(0x5DF8F0 + 1, ret);
    MakeRet(0x5DF8F0 + 2, 4);
}

// for making functions return 0
inline void MakeRet0(MemoryPointer at)
{
    MemWrite<u8>(at, 0x33); // xor eax, eax
    MemWrite<u8>(at + 1, 0xC0);
    MakeRet(at + 2);
}

// Uses al instead of eax
inline void MakeRet0Ex(MemoryPointer at)
{
    MemWrite<u8>(at, 0x32); // xor al, al
    MemWrite<u8>(at + 1, 0xC0);
    MakeRet(at + 2);
}

inline MemoryPointer GetAbsoluteOffset(int rel_value, MemoryPointer end_of_instruction)
{
    return end_of_instruction.Get<char>() + rel_value;
}

inline int GetRelativeOffset(MemoryPointer abs_value, MemoryPointer end_of_instruction)
{
    return static_cast<uintptr_t>(abs_value.Get<char>() - end_of_instruction.Get<char>());
}

inline MemoryPointer ReadRelativeOffset(MemoryPointer at, size_t sizeof_addr = 4)
{
    switch (sizeof_addr)
    {
        case 1:
            return (GetAbsoluteOffset(MemRead<s8>(at), at + sizeof_addr));
        case 2:
            return (GetAbsoluteOffset(MemRead<s16>(at), at + sizeof_addr));
        case 4:
            return (GetAbsoluteOffset(MemRead<s32>(at), at + sizeof_addr));
    }
    return nullptr;
}

inline void MakeRelativeOffset(MemoryPointer at, MemoryPointer dest, size_t sizeof_addr = 4)
{
    switch (sizeof_addr)
    {
        case 1:
            MemWrite<s8>(at, static_cast<s8>(GetRelativeOffset(dest, at + sizeof_addr)));
        case 2:
            MemWrite<s16>(at, static_cast<s16>(GetRelativeOffset(dest, at + sizeof_addr)));
        case 4:
            MemWrite<s32>(at, static_cast<s32>(GetRelativeOffset(dest, at + sizeof_addr)));
    }
}

inline MemoryPointer GetBranchDestination(MemoryPointer at)
{
    switch (MemRead<u8>(at))
    {
        // We need to handle other instructions (and prefixes) later...
        case 0xE8: // call rel
        case 0xE9: // jmp rel
            return ReadRelativeOffset(at + 1, 4);
        case 0xEB:
            return ReadRelativeOffset(at + 1, 1);
        case 0xFF:
        {
            switch (MemRead<u8>(at + 1))
            {
                case 0x15: // call dword ptr [addr]
                case 0x25: // jmp dword ptr [addr]
                    return *(MemRead<uintptr_t*>(at + 2));
            }
            break;
        }
    }
    return nullptr;
}

// Jump Near
inline MemoryPointer MakeJmp(MemoryPointer at, MemoryPointer dest)
{
    auto p = GetBranchDestination(at);
    MemWrite<u8>(at, 0xE9);
    MakeRelativeOffset(at + 1, dest, 4);
    return p;
}

inline MemoryPointer MakeCall(MemoryPointer at, MemoryPointer dest)
{
    auto p = GetBranchDestination(at);
    MemWrite<u8>(at, 0xE8);
    MakeRelativeOffset(at + 1, dest, 4);
    return p;
}

// MakeJmp with NOPing @count
inline MemoryPointer MakeJmp(MemoryPointer at, MemoryPointer dest, size_t count)
{
    auto p = GetBranchDestination(at);
    MakeNop(at, count);
    MakeJmp(at, dest);
    return p;
}

// TODO: Handle absolute offsets properly
inline void MakeShortJmp(MemoryPointer at, u8 jmpOffset = 0)
{
    MemWrite<u8>(at, 0xEB);
    if (jmpOffset != 0)
        MemWrite<u8>(at + 1, jmpOffset);
}

inline void MakeShortJmpEx(MemoryPointer at, MemoryPointer dest)
{
    MemWrite<u8>(at, 0xEB);
    MakeRelativeOffset(at + 1, dest, 1);
}

// Gets the virtual method table from the object @self
inline void** GetVMT(const void* self)
{
    return *(void ***)(self);
}

inline MemoryPointer GetVF(MemoryPointer self, size_t index)
{
    return GetVMT(self.Get<void>())[index];
}

// TODO: std::forward-less
template <class Ret = void, class... Args>
inline Ret Call(MemoryPointer p, Args... a)
{
    return reinterpret_cast<Ret(__cdecl*)(Args...)>(p.Get<void>())(std::forward<Args>(a)...);
}

template <u32 addr, class Ret = void, class... Args>
inline Ret Call(Args... a)
{
    return Call(LazyPtr<addr>(), std::forward<Args>(a)...);
}

template <class Ret = void, class ...Args>
inline Ret ThisCall(MemoryPointer p, Args... a)
{
    return reinterpret_cast<Ret(__thiscall*)(Args...)>(p.Get<void>())(std::forward<Args>(a)...);
}

template <u32 addr, class Ret = void, class... Args>
inline Ret ThisCall(Args... a)
{
    return ThisCall(LazyPtr<addr>(), std::forward<Args>(a)...);
}

template <size_t index>
struct Vtbl
{
    template <class Ret, class... Args>
    static Ret Call(Args... a)
    {
        auto obj = MemoryPointer(std::get<0>(std::forward_as_tuple(a...)));
        auto p = MemoryPointer((*obj.template Get<void**>())[i]);
        return ThisCall<Ret>(p, std::forward<Args>(a)...);
    }
};

}

namespace hook = Hook;

}
