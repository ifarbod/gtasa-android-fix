// ScriptEngine hooks
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/Hook.hpp>
#include <Hooking/HookFunction.hpp>
#include <Math/Vector3.hpp>

using namespace ctn;
using namespace ctn::Hook;

auto RequestModel = Call<0x4087E0, void, int, int>;
auto LoadAllRequestedModels = Call<0x40EA10, void, bool>;

// TODO: Proper CPlayerPed creation

static bool scriptProcessed = false;
void Hook_CRunningScript__Process()
{
    if (!scriptProcessed)
    {
        RequestModel(106, 2);
        LoadAllRequestedModels(false);

        // Change player model ID
        MemWrite<u8>(0x60D5FF + 1, 106);

        // CPlayerPed::SetupPlayerPed
        Call(0x60D790, 0);

        // Set player position
        ThisCall(0x420B80, *reinterpret_cast<uintptr_t*>(0xB7CD98), 2488.562f, -1666.865f, 12.8757f);

        // CStreaming::LoadScene
        Call(0x40EB70, &Vector3(2488.562f, -1666.865f, 12.8757f));

        // First tick processed
        scriptProcessed = true;
    }
}

static HookFunction hookFunction([]()
{
    // Don't load the SCM Script
    //MakeShortJmp(0x468EB5, 0x32);
    MakeShortJmpEx(0x468EB5, 0x468EE9);

    // Hook script process (so we can spawn a local player)
    MakeCall(0x46A21B, Hook_CRunningScript__Process);
});
