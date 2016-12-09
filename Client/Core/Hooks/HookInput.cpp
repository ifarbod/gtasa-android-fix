// Input processing hooks
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/HookingUtils.hpp>
#include <Hooking/HookFunction.hpp>

using namespace Util;

static HookFunction hookFunction([]()
{
    // Disable re-initialization of DirectInput mouse device by the game
    MakeJMP(0x576CCC);
    MakeJMP(0x576EBA);
    MakeJMP(0x576F8A);

    // Make sure DirectInput mouse device is set non-exclusive
    MemPatch<u32>(0x7469A0, 0x9090C030);

    // No DirectInput gamepad
    MakeNOP(0x748813, 5);
});
