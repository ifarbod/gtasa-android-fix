// Input processing hooks
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
    // Disable re-initialization of DirectInput mouse device by the game
    MakeJMP(0x576CCC);
    MakeJMP(0x576EBA);
    MakeJMP(0x576F8A);

    // Make sure DirectInput mouse device is set non-exclusive
    MemPatch<u32>(0x7469A0, 0x9090C030);

    // No DirectInput gamepad
    MakeNOP(0x748813, 5);

    // No more mouse Y-axis lock during fade-ins
    MemPatch<u16>(0x50FBB4, 0x27EB);
    MemPatch<u16>(0x510512, 0xE990);
    MakeJMP(0x524071, 0x524139);
});
