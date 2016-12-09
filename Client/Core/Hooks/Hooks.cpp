// Game hooks
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

static Util::HookFunction hookFunction([]()
{
    // Better than SetWindowText
    MemPatch(0x619608, "San Andreas Online");

    // Unlocked widescreen resolutions
    MemPatch<u32>(0x745B81, 0x9090587D);
    MemPatch<u32>(0x74596C, 0x9090127D);
    MakeNOP(0x745970, 2);
    MakeNOP(0x745B85, 2);
    MakeNOP(0x7459E1, 2);
});
