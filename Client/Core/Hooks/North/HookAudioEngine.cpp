// HookAudioEngine.cpp
// Author(s):       iFarbod <>
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

static HookFunction hookFunction([]()
{
    // Disable GTA being able to call CAudioEngine::StopRadio()
    // Well this isn't really CAudioEngine::StopRadio, It's some global class
    // func that StopRadio just jumps to.
    MakeRet(0x4E9820, 8);

    // Disable GTA being able to call CAudioEngine::StartRadio()
    MakeRet(0x4DBEC0, 0);
    MakeRet(0x4EB3C0, 0x10);
});
