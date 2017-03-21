// HookGameCore.cpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
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
    // Disable loading default.dat in CGame::Initialize
    MakeCall(0x53BC95, DoNothing);

    // Process test keys
    MakeCall(0x53C090, TestUpdate);

#ifdef CTN_DEBUG
    // Increase rendering priority for streaming objects like trees so you don't crash into them w/ a plane
    MakeNop(0x5557CF, 7);

    // Use Data/peds.ifp instead of anim/peds.ifp
    CopyStr(0x85C718, "data\\ped.ifp");
#endif
});
