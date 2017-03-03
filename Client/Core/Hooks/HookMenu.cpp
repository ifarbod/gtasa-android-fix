// Menu hooks
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

static bool menuFirstProcessed = false;
void ProcessFrontEndMenu()
{
    if (!menuFirstProcessed)
    {
        // Start the game now - resume the timers
        MemWrite<u8>(0xB7CB49, 0);
        MemWrite<u8>(0xBA67A4, 0);
        MemWrite<u32>(0xC8D4C0, 8); // gGameState
        menuFirstProcessed = true;
    }

    // Call the original function - Sets render states
    ThisCall(0x573A60);
}

void SetMultiSamplingLevels(u32 level)
{
    Call(0x7F84F0, 1 << (level - 1)); // _RwD3D9EngineSetMultiSamplingLevels
}

void ChangeMultiSamplingLevels(u32 level)
{
    Call(0x7F8A90, 1 << (level - 1)); // _RwD3D9ChangeMultiSamplingLevels
}

int GetMaxMultiSamplingLevels()
{
    return Call<int>(0x7F84E0); // _RwD3D9EngineGetMaxMultiSamplingLevels
}

static HookFunction hookFunction([]()
{
    // Disable menu by skipping CMenuManager::DrawStandardMenus call in CMenuManager::DrawBackground
    MakeNop(0x57BA57, 6);

    // No background texture drawing in menu
    MakeShortJmp(0x57B9CA);

    // No input process for menu
    MakeNop(0x57B457, 5);

    // No background black rectangle
    MakeNop(0x57B9BB, 5);

    // No more ESC button processing
#ifndef CTN_DEBUG
    MakeShortJmp(0x576B8D);
    MakeShortJmp(0x576BAF);
#endif

    // No frontend texture loading (Disable CMenuManager::LoadAllTextures)
    MakeRet(0x572EC0);

    // Allow widescreen resolutions
    MemWrite<u32>(0x745B81, 0x9090587D);
    MemWrite<u32>(0x74596C, 0x9090127D);
    MakeNop(0x745970, 2);
    MakeNop(0x745B85, 2);
    MakeNop(0x7459E1, 2);

    // Hook menu process
    MakeCall(0x57C2BC, ProcessFrontEndMenu);

    // Allow Alt+Tab without pausing the game
    MakeNop(0x748A8D, 6);

    // Disable menu after focus loss
    MakeRet(0x53BC60);

    // Disable GTA Setting g_bIsForegroundApp to false on focus lost
    MakeNop(0x747FFE, 6);
    MakeNop(0x748054, 10);

    // Anti-Aliasing fix
    //MakeJmp(0x7F6C9B);
    //MakeJmp(0x7F60C6);
    //MemWrite<u16>(0x7F6683, 0xE990);
    //MakeCall(0x746350, SetMultiSamplingLevels);
    // Menu
    //MakeCall(0x5744FE, ChangeMultiSamplingLevels);
    //MakeCall(0x57D163, ChangeMultiSamplingLevels);
    //MakeCall(0x57D2A7, ChangeMultiSamplingLevels);
});
