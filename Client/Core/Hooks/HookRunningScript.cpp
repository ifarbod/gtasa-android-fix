// ScriptEngine hooks
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/Hook.hpp>
#include <Hooking/HookFunction.hpp>
#include <Math/Vector3.hpp>

using namespace ctn;
using namespace ctn::Hook;
// TODO: Proper CPlayerPed creation

static bool scriptProcessed = false;
void Hook_CRunningScript__Process()
{
    if (!scriptProcessed)
    {
        // Change player model ID
        MemWrite<u8>(0x60D5FF + 1, 7);

        // CPlayerPed::SetupPlayerPed
        Call(0x60D790, 0);

        // Set player position
        ThisCall(0x420B80, MemRead<u32>(0xB7CD98), 2488.562f, -1666.865f, 12.8757f);

        // CStreaming::LoadScene
        Vector3 scenePosition(2488.562f, -1666.865f, 12.8757f);
        Call(0x40EB70, &scenePosition);

        // First tick processed
        scriptProcessed = true;
    }
}

static HookFunction hookFunction([]()
{
    // Don't load the SCM Script
    MakeShortJmp(0x468EB5, 0x468EE9);

    // Hook script process (so we can spawn a local player)
    MakeCall(0x46A21B, Hook_CRunningScript__Process);
});
