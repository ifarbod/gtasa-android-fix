// HookGameCore.cpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/Hook.hpp>
#include <Hooking/HookFunction.hpp>

#include "../Game/PlayerPed.hpp"

using namespace ctn;
using namespace ctn::Hook;

void DoNothing()
{
    auto LoadObjectTypes = Call<0x5B8400, void, char*>;
    LoadObjectTypes("data\\default.ide");
    LoadObjectTypes("data\\vehicles.ide");
    LoadObjectTypes("data\\peds.ide");
}

void TestUpdate()
{
    void* ControlsMgr = reinterpret_cast<void*>(0xB70198);
    auto GetIsKeyboardKeyJustDown = ThisCall<0x52E450, bool, void*, int>;

    // F1 - Heat haze
    if (GetIsKeyboardKeyJustDown(&ControlsMgr, 0x3E9))
    {
        MemWrite<u8>(0x72C1B7, MemRead<u8>(0x72C1B7) == 0xEB ? 0x7C : 0xEB);
    }

    // F2 - Frame limiter
    if (GetIsKeyboardKeyJustDown(&ControlsMgr, 0x3EA))
    {
        MemWrite<bool>(0xBA6748 + 0x4C, !MemRead<bool>(0xBA6748 + 0x4C));
    }

    // F6 - Maverick
    if (GetIsKeyboardKeyJustDown(&ControlsMgr, 0x3EE))
    {
        Call(0x43A0B0, 487);
    }

    // F4 - HUD
    if (GetIsKeyboardKeyJustDown(&ControlsMgr, 0x3EC))
    {
        MemWrite<u8>(0x58FAE0, MemRead<u8>(0x58FAE0) == 0xC3 ? 0x80 : 0xC3);
    }

    // F7
    if (GetIsKeyboardKeyJustDown(&ControlsMgr, 0x3EF))
    {
        ctn::SA::CPlayerPed* playerPed = MemRead<ctn::SA::CPlayerPed*>(0xB7CD98);
    }
}

static HookFunction hookFunction([]()
{
    // Disable loading default.dat in CGame::Initialize
    MakeCall(0x53BC95, DoNothing);
    // Process test keys
    MakeCall(0x53C090, TestUpdate);

    // Increase priority for streaming objects like trees so you don't crash into them
    MakeNop(0x5557CF, 7);
});
