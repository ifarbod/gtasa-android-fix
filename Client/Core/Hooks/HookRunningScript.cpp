// ScriptEngine hooks
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
        //Call<void>(0x40EB70, &SA::CVector(2488.562f, -1666.865f, 12.8757f));

        MemPatch<u32>(0x438B7A + 1, 343); // Tear gas model load
        MemPatch<u32>(0x438D39 + 1, 343); // Tear gas model unload
        MemPatch<u8>(0x438BB8 + 1, 17); // Tear gas weapon id
        MemPatch<u32>(0x438B3E + 1, 348); // Silenced pistol model load
        MemPatch<u32>(0x438D07 + 1, 348); // Silenced pistol model unload
        MemPatch<u8>(0x438BCF + 1, 24); // Silenced pistol weapon id
        MemPatch<u32>(0x438B6E + 1, 358); // Heat seeker model load
        MemPatch<u32>(0x438D2F + 1, 358); // Heat seeker model unload
        MemPatch<u8>(0x438C31 + 1, 34); // Heat seeker model load

        Call<void>(0x439940); // Weapon skills cheat
        Call<void>(0x4399D0); // Vehicle skills cheat

        for (int i = 0; i < 30; i++)
        {
            Call<void>(0x438B30); // Weapon cheat 3
        }

        // Spawn a car
        Call<void>(0x43A0B0, 411);

        // Force extra sunny weather
        Call<void>(0x72A4F0, 0);

        // Freeze clock on 12AM (CCheat::MidnightCheat)
        Call<void>(0x439510);

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
