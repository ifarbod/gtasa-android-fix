// Menu hooks
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

static bool menuFirstProcessed = false;
void ProcessFrontEndMenu()
{
    if (!menuFirstProcessed)
    {
        // Start the game now - resume the timers
        MemPatch<u8>(0xB7CB49, 0);
        MemPatch<u8>(0xBA67A4, 0);
        MemPatch<u32>(0xC8D4C0, 8); // gGameState
        menuFirstProcessed = true;
    }

    // Call the original function - Sets render states
    ThisCall<void>(0x573A60);
}

void SetMultiSamplingLevels(u32 level)
{
    Call<void>(0x7F84F0, 1 << (level - 1)); // _RwD3D9EngineSetMultiSamplingLevels
}

void ChangeMultiSamplingLevels(u32 level)
{
    Call<void>(0x7F8A90, 1 << (level - 1)); // _RwD3D9ChangeMultiSamplingLevels
}

int GetMaxMultiSamplingLevels()
{
    return Call<int>(0x7F84E0); // _RwD3D9EngineGetMaxMultiSamplingLevels
}

static Util::HookFunction hookFunction([]()
{
    // Allow widescreen resolutions
    MemPatch<u32>(0x745B81, 0x9090587D);
    MemPatch<u32>(0x74596C, 0x9090127D);
    MakeNOP(0x745970, 2);
    MakeNOP(0x745B85, 2);
    MakeNOP(0x7459E1, 2);

    // Hook menu process
    MakeCALL(0x57C2BC, ProcessFrontEndMenu);

    // Allow Alt+Tab without pausing the game
    MakeNOP(0x748A8D, 6);

    // Disable Menu After Lost Focus
    //MakeRET(0x53BC60);
    MemPatch<u8>(0x53BC78, 0x00);
    //MemPatch<u8>((0xBA6748) + 0x5C, 0);

    // Disable GTA Setting g_bIsForegroundApp to false on focus lost
    MakeNOP(0x747FFE, 6);
    MakeNOP(0x748054, 10);

    // Anti-Aliasing
    MakeJMP(0x7F6C9B);
    MakeJMP(0x7F60C6);
    MemPatch<u16>(0x7F6683, 0xE990);
    MakeCALL(0x746350, SetMultiSamplingLevels);
    // Menu
    //MakeCALL(0x5744FE, ChangeMultiSamplingLevels);
    //MakeCALL(0x57D163, ChangeMultiSamplingLevels);
    //MakeCALL(0x57D2A7, ChangeMultiSamplingLevels);
});
