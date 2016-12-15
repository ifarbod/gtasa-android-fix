// Game graphics tweaking
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/HookingUtils.hpp>
#include <Hooking/HookFunction.hpp>

using namespace Util;

static HookFunction hookFunction([]()
{
    // Reflection Fix
    static const float fRefZVal = 1.0f;
    static const float* const pRefFal = &fRefZVal;
    MemPatch<const void*>(0x6FB97A, &pRefFal);
    MemPatch<u8>(0x6FB9A0, 0);

    // Fix Lightbeams
    MemPatch<u16>(0x6A2E88, 0x0EEB);
    MakeNOP(0x6A2E9C, 3);
    MemPatch<u16>(0x6E0F63, 0x0AEB);
    MemPatch<u16>(0x6E0F7C, 0x0BEB);
    MemPatch<u16>(0x6E0F95, 0x0BEB);
    MemPatch<u16>(0x6E0FAF, 0x1AEB);

    MemPatch<u16>(0x6E13D5, 0x09EB);
    MemPatch<u16>(0x6E13ED, 0x17EB);
    MemPatch<u16>(0x6E141F, 0x0AEB);

    MemPatch<u8>(0x6E0FE0, 0x28);
    MemPatch<u8>(0x6E142D, 0x18);
    MemPatch<u8>(0x6E0FDB, 0xC8 - 0x7C);

    // All lights get casted at vehicles
    MemPatch<u8>(0x5D9A88, 8);
    MemPatch<u8>(0x5D9A91, 8);
    MemPatch<u8>(0x5D9F1F, 8);

    // Fixed gun's muzzle flash not showing from last bullet
    MakeNOP(0x61ECE4, 2);

    // Weapon icon fix when using a jetpack (rwRENDERSTATEZWRITEENABLE flag being messed up)
    MakeNOP(0x58E210, 3);
    MakeNOP(0x58EAB7, 3);
    MakeNOP(0x58EAE1, 3);
});
