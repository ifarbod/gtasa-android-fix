// Stats system hooks
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

static HookFunction hookFunction([]()
{
    // Disable CStats::LoadActionReactionStats
    MakeRet(0x5599B0);

    // Disable CStats::LoadStatUpdateConditions
    MakeRet(0x559860);

    // Disable CStats::IncrementStat
    MakeRet(0x55C180);

    // Disable CStats::DecrementStat
    MakeRet(0x559FA0);

    // Disable CStats::DisplayScriptStatUpdateMessage
    MakeRet(0x55B980);

    // Disable CStats::CheckForStatsMessage
    MakeRet(0x559760);
});
