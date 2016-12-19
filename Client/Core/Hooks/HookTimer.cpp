// Timer system fixes
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

static HookFunction hookFunction([]()
{
    // No 14ms delay
    MemPatch<u16>(0x53E923, 0x43EB);
    MemPatch<u8>(0x53E99F, 0x10);
    MakeNOP(0x53E9A5);

    // Fixed stuck strafing with 2-handed weapons with FPS > 45
    static const float strafeCheck = 0.1f;
    MemPatch(0x61E0C2, &strafeCheck);
    MakeNOP(0x61E0CA, 6);
});
