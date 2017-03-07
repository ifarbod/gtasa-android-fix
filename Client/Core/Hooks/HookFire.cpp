// Game logic hooks
// Author(s):       iFarbod <ifarbod@outlook.com>
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

static HookFunction hookFunction([]()
{
    // Disable setting players on fire when they're riding burning BMX's
    MakeShortJmp(0x53A982);

    // Disable setting the occupied's vehicles health to 75.0f when a burning ped enters it
    // in CFire::ProcessFire
    MakeNop(0x53A651, 10);

    // Make CCreepingFire::TryToStartFireAtCoors return the fire pointer rather than a bool
    MemWrite<u8>(0x53A459, 0x33);
    MemWrite<u8>(0x53A568, 0x8B);
    MemWrite<u8>(0x53A4A9, 0x33);
    MakeNop(0x53A55F, 2);
    MemWrite<u8>(0x73EC06, 0x85);

    // Force triggering of the damage event for players on fire
    MakeNop(0x633695, 6);
    MemWrite<u8>(0x633720, 0);
});
