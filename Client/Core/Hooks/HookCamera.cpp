// Camera stuff
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
    // Disable cinematic camera for trains
    MemWrite<u8>(0x52A535, 0);

    // Hack to make non-local players always update their aim on akimbo weapons using camera
    // so they don't freeze when local player doesn't aim
    MakeShortJmp(0x61EFFE);

    // Do not fixate camera behind spectated player if local player is dead
    MemWrite<u8>(0x52A2BB, 0);
    MemWrite<u8>(0x52A4F8, 0);

    // Disable CIdleCam::Process
    MakeRET(0x522C80);

    // Ignore camera fade stage in rendering routine
    MakeNOP(0x53E9C6, 6);

    // Clip camera also outside the world bounds
    MakeNOP(0x41AD12, 2);
    MakeNOP(0x41ADA7, 2);
    MakeNOP(0x41ADF3, 2);
});
