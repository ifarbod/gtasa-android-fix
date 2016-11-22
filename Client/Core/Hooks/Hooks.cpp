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

    // Disable re-initialization of DirectInput mouse device by the game
    MakeJMP(0x576CCC);
    MakeJMP(0x576EBA);
    MakeJMP(0x576F8A);

    // Make sure DirectInput mouse device is set non-exclusive
    MemPatch<u32>(0x7469A0, 0x9090C030);

    // Fixed stuck strafing with 2-handed weapons with FPS > 45
    static const float strafeCheck = 0.1f;
    MemPatch(0x61E0C2, &strafeCheck);
    MakeNOP(0x61E0CA, 6);

    // Unlocked widescreen resolutions
    MemPatch<u32>(0x745B81, 0x9090587D);
    MemPatch<u32>(0x74596C, 0x9090127D);
    MakeNOP(0x745970, 2);
    MakeNOP(0x745B85, 2);
    MakeNOP(0x7459E1, 2);

    // No 14ms wait
    MemPatch<u16>(0x53E923, 0x43EB);
    MemPatch<u8>(0x53E99F, 0x10);
    MakeNOP(0x53E9A5);

    // Disable ValidateVersion
	// Contains a stupid check for 'grandtheftauto3' string in peds.col
    MakeRET(0x5BA060);

    // Disable CLoadingScreen::LoadSplashes
    MakeRET(0x5900B0);

    // Disable CShopping::LoadStats
    MakeRET(0x49B6A0);

    // Disable CStats::LoadActionReactionStats
    MakeRET(0x5599B0);

    // Disable CStats::LoadStatUpdateConditions
    MakeRET(0x559860);

    // Disable CGridRef::Init
    MakeRET(0x71D4E0);

    // Disable CRoadBlocks::Init
    MakeRET(0x461100);
    // Disable CRoadBlocks::GenerateRoadBlocks
    MakeRET(0x4629E0);
    // Disable CRoadBlocks::GenerateRoadBlockPedsForCar
    MakeRET(0x461170);
});
