// Timer system fixes
// Author(s):       iFarbod <ifarbod@outlook.com>
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

static HookFunction hookFunction([]()
{
    // No 14ms delay
    MemWrite<u16>(0x53E923, 0x43EB);
    MemWrite<u8>(0x53E99F, 0x10);
    MakeNop(0x53E9A5);

    // Fixed stuck strafing with 2-handed weapons with FPS > 45
    static const float strafeCheck = 0.1f;
    MemWrite(0x61E0C2, &strafeCheck);
    MakeNop(0x61E0CA, 6);
});
