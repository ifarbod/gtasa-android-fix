// SCM disabling hook
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

// TODO: Proper CPlayerPed creation

static bool scriptProcessed = false;
void Hook_CRunningScript__Process()
{
    if (!scriptProcessed)
    {
        // RequestModel
        Call<void>(0x4087E0, 21, 2);

        // LoadAllRequestedModels
        Call<void>(0x40EA10, false);

        // skin
        MemPatch<u8>(0x60D5FF + 1, 21);

        // CPlayerPed::SetupPlayerPed
        Call<void>(0x60D790, 0);

        // setPosn
        ThisCall<void>(0x420B80, *(uintptr_t *)0xB7CD98, 2488.562f, -1666.865f, 12.8757f);

        // CStreaming::loadScene
        //Call<void>(0x40EB70, &SA::CVector(2488.562f, -1666.865f, 12.8757f));

        // First tick processed here, now don't repeat it
        scriptProcessed = true;
    }
}

static HookFunction hookFunction([]()
{
    // Don't load the SCM Script
    MakeJMP(0x468EB5);
    MemPatch<u8>(0x468EB6, 0x32);

    // Hooked script process (so we can spawn a local player)
    MakeCALL(0x46A21B, Hook_CRunningScript__Process);
});
