// Loading screen hooks
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  LINK/2012 <dma2012@gmail.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/HookingUtils.hpp>
#include <Hooking/HookFunction.hpp>

using namespace Util;

void SimulateCopyrightScreen()
{
    // Previous splash index = copyright notice
    *MemoryPointer(0x8D093C).Get<s32>() = 0;
    // Decrease timeSinceLastScreen, so it will change immediately
    *MemoryPointer(0xBAB340).Get<f32>() -= 1000.f;
    // First Loading Splash
    *MemoryPointer(0xBAB31E).Get<s8>() = 1;
}

static HookFunction hookFunction([]()
{
    // Disable CLoadingScreen::LoadSplashes
    MakeRET(0x5900B0);

    // Start the game at state 5
    // Disable gGameState = 0 setting
    MakeNOP(0x747483, 6);
    // Put the game where the user wants (default's to the copyright screen)
    // GS_INIT_ONCE:5
    MemWrite<u32>(0xC8D4C0, 5);

    // Disable Copyright screen
    // Hook the copyright screen fading in/out and simulates that it has happened
    MakeNOP(0x748C2B, 5);
    MakeCALL(0x748C9A, SimulateCopyrightScreen);

    // Disable loading screen rendering
    MakeNOP(0x590D9F, 5);
    MemWrite<u8>(0x590D9F, 0xC3);

    // Skip fading screen rendering
    MakeJMP(0x590AE4, 0x590C9E);

    // Disable loading bar rendering
    MakeNOP(0x5905B4, 5);

    // Disable audio tune from loading screen
    MakeNOP(0x748CF6, 5);
});
