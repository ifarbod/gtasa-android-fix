// ScriptEngine hooks
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/HookingUtils.hpp>
#include <Hooking/HookFunction.hpp>
#include <Math/Vector3.hpp>

using namespace Util;

// TODO: Proper CPlayerPed creation

static bool scriptProcessed = false;
void Hook_CRunningScript__Process()
{
    if (!scriptProcessed)
    {
        // Call RequestModel
        Call<void>(0x4087E0, 106, 2);

        // Call LoadAllRequestedModels
        Call<void>(0x40EA10, false);

        // Change player model ID
        MemPatch<u8>(0x60D5FF + 1, 106);

        // CPlayerPed::SetupPlayerPed
        Call<void>(0x60D790, 0);

        // Set player position
        ThisCall<void>(0x420B80, *(uintptr_t *)0xB7CD98, 2488.562f, -1666.865f, 12.8757f);

        // CStreaming::LoadScene
        Call<void>(0x40EB70, &Vector3(2488.562f, -1666.865f, 12.8757f));

        // First tick processed
        scriptProcessed = true;
    }
}

static HookFunction hookFunction([]()
{
    // Don't load the SCM Script
    MakeJMP(0x468EB5);
    MemPatch<u8>(0x468EB6, 0x32);

    // Hook script process (so we can spawn a local player)
    MakeCALL(0x46A21B, Hook_CRunningScript__Process);
});
