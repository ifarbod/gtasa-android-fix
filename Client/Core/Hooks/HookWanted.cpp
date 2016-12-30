// Game logic hooks
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

static HookFunction hookFunction([]()
{
    // Disable Police Mavericks and News choppers at 3+ wanted stars
    // by making CWanted::NumOfHelisRequired always return 0
    MakeRET0(0x561FA0);

    // Disable CWanted::UpdateEachFrame
    MakeNOP(0x53BFF6, 5);

    // Disable CWanted::Update
    MakeRET(0x562C90);

    // Disable CCrime::ReportCrime
    MakeRET(0x532010);

    // Disable CWanted::SetWantedLevel
    MakeRET(0x562470);

    // Disable military zones (5star wanted level)
    MakeJMP(0x72DF0D);

    // Disable CGameLogic::SetPlayerWantedLevelForForbiddenTerritories
    // No 4-star wanted level on visiting LV and SF
    MakeRET(0x441770);

    // Disable CRoadBlocks::Init
    MakeRET(0x461100);

    // Disable CRoadBlocks::GenerateRoadBlocks
    MakeRET(0x4629E0);

    // Disable CRoadBlocks::GenerateRoadBlockPedsForCar
    MakeRET(0x461170);
});
