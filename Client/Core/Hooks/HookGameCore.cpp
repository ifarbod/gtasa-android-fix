// HookGameCore.cpp
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

void DoNothing()
{
    // No-op, do nothing
}

static HookFunction hookFunction([]()
{
    // Disable loading default.dat in CGame::Initialize
    MakeCall(0x53BC95, DoNothing);
});
