// Game logic hooks
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  LINK/2012 <dma2012@gmail.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
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
    //MakeNOP(0x561FA4, 2);
    MakeRET0(0x561FA0);

    // Disable CWanted::UpdateEachFrame
    MakeNOP(0x53BFF6, 5);

    // Disable the call to CWanted::Update in CPlayerPed::ProcessControl
    //MakeNOP(0x60EBCC, 5);
    MakeRET(0x562C90);

    // Disable CCrime::ReportCrime
    MakeRET(0x532010);

    // Disable military zones (5star wanted level)
    MakeJMP(0x72DF0D);

    // Disable CGameLogic::SetPlayerWantedLevelForForbiddenTerritories
    // May not be necessary, since CWanted::Update and CGameLogic::Update is disabled
    MakeRET(0x441770);

    // Disable CRoadBlocks::Init
    MakeRET(0x461100);

    // Disable CRoadBlocks::GenerateRoadBlocks
    MakeRET(0x4629E0);

    // Disable CRoadBlocks::GenerateRoadBlockPedsForCar
    MakeRET(0x461170);
});
