// Game graphics tweaking
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
    // Reflection Fix
    static const float fRefZVal = 1.0f;
    static const float* const pRefFal = &fRefZVal;
    MemWrite<const void*>(0x6FB97A, &pRefFal);
    MemWrite<u8>(0x6FB9A0, 0);

    // Fix Lightbeams
    MemWrite<u16>(0x6A2E88, 0x0EEB);
    MakeNop(0x6A2E9C, 3);
    MemWrite<u16>(0x6E0F63, 0x0AEB);
    MemWrite<u16>(0x6E0F7C, 0x0BEB);
    MemWrite<u16>(0x6E0F95, 0x0BEB);
    MemWrite<u16>(0x6E0FAF, 0x1AEB);

    MemWrite<u16>(0x6E13D5, 0x09EB);
    MemWrite<u16>(0x6E13ED, 0x17EB);
    MemWrite<u16>(0x6E141F, 0x0AEB);

    MemWrite<u8>(0x6E0FE0, 0x28);
    MemWrite<u8>(0x6E142D, 0x18);
    MemWrite<u8>(0x6E0FDB, 0xC8 - 0x7C);

    // All lights get casted at vehicles
    MemWrite<u8>(0x5D9A88, 8);
    MemWrite<u8>(0x5D9A91, 8);
    MemWrite<u8>(0x5D9F1F, 8);

    // Fixed gun's muzzle flash not showing from last bullet
    MakeNop(0x61ECE4, 2);

    // Weapon icon fix when using a jetpack (rwRENDERSTATEZWRITEENABLE flag being messed up)
    MakeNop(0x58E210, 3);
    MakeNop(0x58EAB7, 3);
    MakeNop(0x58EAE1, 3);
});
