// Stats system hooks
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/HookingUtils.hpp>
#include <Hooking/HookFunction.hpp>

using namespace Util;

static Util::HookFunction hookFunction([]()
{
    // Disable CStats::LoadActionReactionStats
    MakeRET(0x5599B0);

    // Disable CStats::LoadStatUpdateConditions
    MakeRET(0x559860);

    // Disable CStats::IncrementStat
    MakeRET(0x55C180);

    // Disable CStats::DecrementStat
    MakeRET(0x559FA0);

    // Disable CStats::DisplayScriptStatUpdateMessage
    MakeRET(0x55B980);

    // Disable CStats::CheckForStatsMessage
    MakeRET(0x559760);
});
