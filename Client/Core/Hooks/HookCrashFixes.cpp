// Crash fixes
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/HookingUtils.hpp>
#include <Hooking/HookFunction.hpp>

using namespace Util;

u32 Return_CTaskComplexCarSlowBeDraggedOut__PrepareVehicleForPedExit = 0x6485B2;
u32 Return_CTaskComplexCarSlowBeDraggedOut__PrepareVehicleForPedExit_Invalid = 0x6485E1;
void CTaskComplexCarSlowBeDraggedOut__PrepareVehicleForPedExit_Custom()
{
    //CTN_LOGWARNING("CTaskComplexCarSlowBeDraggedOut race condition");
}

void __declspec(naked) Hook_CTaskComplexCarSlowBeDraggedOut__PrepareVehicleForPedExit()
{
    __asm
    {
        test eax, eax
        jz InvalidVehicle

        mov ecx, [eax + 460h]
        jmp Return_CTaskComplexCarSlowBeDraggedOut__PrepareVehicleForPedExit

    InvalidVehicle:
        pushad
        call CTaskComplexCarSlowBeDraggedOut__PrepareVehicleForPedExit_Custom
        popad
        jmp Return_CTaskComplexCarSlowBeDraggedOut__PrepareVehicleForPedExit_Invalid
    }
}

static Util::HookFunction hookFunction([]()
{
    // Don't create a ped group on player creation (Fixes a crash)
    // TODO: Proper CPlayerPed creation
    MakeNOP(0x60D64D);
    MemWrite<u8>(0x60D64E, 0xE9);

    // Disable the call to FxSystem_c::GetCompositeMatrix in CAEFireAudioEntity::UpdateParameters
    // Which was causing a crash. The crash happens if you create 40 or
    // so vehicles that catch fire (upside down) then delete them, repeating a few times.
    MakeNOP(0x4DCF87, 6);

    // Fixed a crash (race condition triggered when jacking a vehicle)
    MakeNOP(0x6485AC, 6);
    MakeJMP(0x6485AC, Hook_CTaskComplexCarSlowBeDraggedOut__PrepareVehicleForPedExit);

    // Fix mirror crash
    MemWrite<u8>(0x7271CB + 0, 0x85); // test eax, eax
    MemWrite<u8>(0x7271CB + 1, 0xC0);
    MemWrite<u8>(0x7271CB + 2, 0x74); // je 0x727203
    MemWrite<u8>(0x7271CB + 3, 0x34);
    MemWrite<u8>(0x7271CB + 4, 0x83); // add esp, 04
    MemWrite<u8>(0x7271CB + 5, 0xC4);
    MemWrite<u8>(0x7271CB + 6, 0x04);
    MemWrite<u8>(0x7271CB + 7, 0xC6); // mov byte ptr [00C7C728],01

    // No FxMemoryPool_c::Optimize (causes heap corruption)
    MakeNOP(0x5C25D3, 5);

    // Satchel charge crash fix
    MakeNOP(0x738F3A, 83);
});
