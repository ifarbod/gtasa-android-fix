// Input processing hooks
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
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
    // Disable re-initialization of DirectInput mouse device by the game
    MakeShortJmp(0x576CCC);
    MakeShortJmp(0x576EBA);
    MakeShortJmp(0x576F8A);

    // Make sure DirectInput mouse device is set non-exclusive
    MemWrite<u32>(0x7469A0, 0x9090C030);

    // No DirectInput gamepad
    MakeNop(0x748813, 5);

    // No more mouse Y-axis lock during fade-ins
    MemWrite<u16>(0x50FBB4, 0x27EB);
    MemWrite<u16>(0x510512, 0xE990);
    MakeJmp(0x524071, 0x524139);
});
