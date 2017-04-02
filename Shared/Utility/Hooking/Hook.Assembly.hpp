// Inline assembly injection
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  LINK/2012
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Hooking/Hook.hpp>

namespace Hook
{

struct RegPack
{
    // The ordering is very important, don't change
    // The first field is the last to be pushed and first to be poped

    // PUSHFD / POPFD
    u32 ef;

    // PUSHAD/POPAD -- must be the lastest fields (because of esp)
    union
    {
        u32 arr[8];
        struct
        {
            u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
        };
    };

    enum reg_name
    {
        reg_edi,
        reg_esi,
        reg_ebp,
        reg_esp,
        reg_ebx,
        reg_edx,
        reg_ecx,
        reg_eax
    };

    enum ef_flag
    {
        carry_flag = 0,
        parity_flag = 2,
        adjust_flag = 4,
        zero_flag = 6,
        sign_flag = 7,
        direction_flag = 10,
        overflow_flag = 11
    };

    u32& operator[](size_t i) { return this->arr[i]; }
    const u32& operator[](size_t i) const { return this->arr[i]; }

    template <u32 bit> // bit starts from 0, use ef_flag enum
    bool Flag()
    {
        return (this->ef & (1 << bit)) != 0;
    }

    bool jnb() { return Flag<carry_flag>() == false; }
};

// Lowest level stuff (actual assembly) goes on the following namespace
// PRIVATE! Skip this, not interesting for you.
namespace Asm
{

// Wrapper functor, so the assembly can use some templating
template <class T>
struct Wrapper
{
    static void Call(RegPack* regs)
    {
        T fun;
        fun(*regs);
    }
};

// Constructs a RegPack and calls the wrapper functor
template <class W> // where W is of type wrapper
inline void __declspec(naked) MakeRegPackAndCall()
{
    __asm
    {
        // Construct the RegPack structure on the stack
        // Pushes general purposes registers to RegPack
        pushad
        // Add 4 to RegPack::esp 'cuz of our return pointer, let it be as before this func is called
        add[esp + 12], 4
        // Pushes EFLAGS to RegPack
        pushfd

        // Call wrapper sending RegPack as parameter
        push esp
        Call W::Call
        add esp, 4

        // Destructs the RegPack from the stack
        // Fix RegPack::esp before popping it (doesn't make a difference though) (+4 because eflags)
        sub[esp + 12 + 4], 4

        // Warning: Do not use any instruction that changes EFLAGS after this (-> sub affects EF!! <-)
        popfd

        popad

        // Back to normal flow
        ret
    }
}

}


// Makes inline assembly (but not assembly, an actual functor of type FuncT) at address
template <class FuncT>
void MakeInline(MemoryPointer at)
{
    using functor = Asm::Wrapper<FuncT>;
    if (false)
        functor::Call(nullptr); // To instantiate the template, if not done __asm will fail
    MakeCall(at, Asm::MakeRegPackAndCall<functor>);
}

// Same as above, but it NOPs everything between at and end (exclusive), then performs MakeInline
template <class FuncT>
void MakeInline(MemoryPointer at, MemoryPointer end)
{
    MakeRangedNop(at, end);
    MakeInline<FuncT>(at);
}

// Same as above, but (at,end) are template parameters.
// On this case the functor can be passed as argument since there will be one func instance for each at,end not just for each FuncT
template <uintptr_t at, uintptr_t end, class FuncT>
void MakeInline(FuncT func)
{
    // Stores the func object
    static FuncT static_func = func;
    static_func = func;

    // Encapsulates the call to static_func
    struct Caps
    {
        void operator()(RegPack& regs) { static_func(regs); }
    };

    // Does the actual MakeInline
    return MakeInline<Caps>(LazyPointer<at>::get(), LazyPointer<end>::get());
}


//  MakeInline
//  Same as above, but (end) is calculated by the length of a call instruction
template <uintptr_t at, class FuncT> void MakeInline(FuncT func) { return MakeInline<at, at + 5, FuncT>(func); }

}
