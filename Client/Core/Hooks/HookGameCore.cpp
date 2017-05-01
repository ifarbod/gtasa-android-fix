// HookGameCore.cpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/Hook.hpp>
#include <Hooking/HookFunction.hpp>

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
    auto ControlsMgr = reinterpret_cast<void*>(0xB70198);
    auto GetIsKeyboardKeyJustDown = ThisCall<0x52E450, bool, void*, int>;

    // F2
    if (GetIsKeyboardKeyJustDown(&ControlsMgr, 0x3EA))
    {
        MemWrite<bool>(0xBA6748 + 0x4C, MemRead<bool>(0xBA6748 + 0x4C) ? false : true);
    }
}

static HookFunction hookFunction([]()
{
    // Use our icon
    Hook::MemWrite<u8>(0x7486A5, 1);

    // Patch IsAlreadyRunning
    Hook::MakeRet0(0x7468E0);

    // Don't catch WM_SYSKEYDOWN and WM_SYSKEYUP (fixes Alt+F4)
    MakeJmp(0x748220, 0x748446);
    MemWrite<u8>(0x7481E3, 0x5C); // esi -> ebx
    MemWrite<u8>(0x7481EA, 0x53); // esi -> ebx
    MemWrite<u8>(0x74820D, 0xFB); // esi -> ebx
    MemWrite<s8>(0x7481EF, 0x54 - 0x3C); // use stack space for new lParam
    MemWrite<s8>(0x748200, 0x4C - 0x3C); // use stack space for new lParam
    MemWrite<s8>(0x748214, 0x4C - 0x3C); // use stack space for new lParam

    MakeJmp(0x74826A, 0x748446);
    MemWrite<u8>(0x74822D, 0x5C); // esi -> ebx
    MemWrite<u8>(0x748234, 0x53); // esi -> ebx
    MemWrite<u8>(0x748257, 0xFB); // esi -> ebx
    MemWrite<s8>(0x748239, 0x54 - 0x3C); // use stack space for new lParam
    MemWrite<s8>(0x74824A, 0x4C - 0x3C); // use stack space for new lParam
    MemWrite<s8>(0x74825E, 0x4C - 0x3C); // use stack space for new lParam

    // Disable loading default.dat in CGame::Initialize
    MakeCall(0x53BC95, DoNothing);

    // Process test keys
    MakeCall(0x53C090, TestUpdate);

    // Use Data/peds.ifp instead of anim/peds.ifp
    CopyStr(0x85C718, "data\\ped.ifp");

#ifdef CTN_DEBUG
    // Increase rendering priority for streaming objects like trees so you don't crash into them w/ a plane
    MakeNop(0x5557CF, 7);
#endif
});
