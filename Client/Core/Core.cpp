// Core module
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include "Core.hpp"

#include <Hook.hpp>

using namespace Util;

void SimulateCopyrightScreen()
{
    // Simulate that the copyright screen happened
    *MemPtr(0x8D093C).get<s32>() = 0;           // Previous splash index = copyright notice
    *MemPtr(0xBAB340).get<f32>() -= 1000.f;     // Decrease timeSinceLastScreen, so it will change immediately
    *MemPtr(0xBAB31E).get<s8>() = 1;            // First Loading Splash
}

Core::Core()
{
    // Start the game at state 5
    // Disable gGameState = 0 setting
    MakeNOP(0x747483, 6);
    // Put the game where the user wants (default's to the copyright screen)
    // GS_INIT_ONCE:5
    MemPatch<u32>(0xC8D4C0, 5);

    // Disable Copyright screen
    // Hook the copyright screen fading in/out and simulates that it has happened
    MakeNOP(0x748C2B, 5);
    MakeCALL(0x748C9A, SimulateCopyrightScreen);

    // Disable loading screen rendering
    MakeNOP(0x590D9F, 5);
    MemPatch<u8>(0x590D9F, 0xC3);

    // Skip fading screen rendering
    MakeJMP(0x590AE4, 0x590C9E);

    // Disable loading bar rendering
    MakeNOP(0x5905B4, 5);

    // Disable audio tune from loading screen
    MakeNOP(0x748CF6, 5);

    // Disable re-initialization of DirectInput mouse device by the game - not needed as we disabled the menu
    // And we handle the mouse using RawInput
    MakeJMP(0x576CCC);
    MakeJMP(0x576EBA);
    MakeJMP(0x576F8A);

    // Make sure DirectInput mouse device is set non-exclusive (may not be needed?)
    MemPatch<u32>(0x7469A0, 0x9090C030);

    // Fixed stuck strafing with 2-handed weapons with [FPS > 45]
    static const float fStrafeCheck = 0.1f;
    MemPatch<const void *>(0x61E0C2, &fStrafeCheck);
    MakeNOP(0x61E0CA, 6);

    // Unlocked widescreen resolutions
    MemPatch<u32>(0x745B81, 0x9090587D);
    MemPatch<u32>(0x74596C, 0x9090127D);
    MakeNOP(0x745970, 2);
    MakeNOP(0x745B85, 2);
    MakeNOP(0x7459E1, 2);

    // No framedelay
    MemPatch<u16>(0x53E923, 0x43EB);
    MemPatch<u8>(0x53E99F, 0x10);
    MakeNOP(0x53E9A5);

	// Disable ValidateVersion
	MakeRET(0x5BA060);
}

Core::~Core()
{
}
