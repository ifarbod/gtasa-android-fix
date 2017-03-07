// Game logic hooks
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CtNorth Team
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
    // Disable Police Mavericks and News choppers at 3+ wanted stars
    // by making CWanted::NumOfHelisRequired always return 0
    MakeRet0(0x561FA0);

    // Disable CWanted::UpdateEachFrame
    MakeNop(0x53BFF6, 5);

    // Disable CWanted::Update
    MakeRet(0x562C90);

    // Disable CCrime::ReportCrime
    MakeRet(0x532010);

    // Disable CWanted::SetWantedLevel
    MakeRet(0x562470);

    // Disable military zones (5star wanted level)
    MakeShortJmp(0x72DF0D);

    // Disable CGameLogic::SetPlayerWantedLevelForForbiddenTerritories
    // No 4-star wanted level on visiting LV and SF
    MakeRet(0x441770);

    // Disable CRoadBlocks::Init
    MakeRet(0x461100);

    // Disable CRoadBlocks::GenerateRoadBlocks
    MakeRet(0x4629E0);

    // Disable CRoadBlocks::GenerateRoadBlockPedsForCar
    MakeRet(0x461170);
});
