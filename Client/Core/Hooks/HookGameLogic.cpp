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
    // Disable CGameLogic::Update
    MakeRET(0x442AD0);

    // Disable CPlayerInfo::MakePlayerSafe
    MakeRET(0x56E870, 8);

    // Disable ValidateVersion
    // Contains a stupid check for 'grandtheftauto3' string in peds.col
    MakeRET(0x5BA060);

    // Disable CLoadingScreen::LoadSplashes
    MakeRET(0x5900B0);

    // Disable CShopping::LoadStats
    MakeRET(0x49B6A0);

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

    // Disable Police Mavericks and News choppers at 3+ wanted stars
    // by making CWanted::NumOfHelisRequired always return 0
    //MakeNOP(0x561FA4, 2);
    MakeRET0(0x561FA0);

    // Disable CWanted::UpdateEachFrame
    MakeNOP(0x53BFF6, 5);

    // Disable the call to CWanted::Update in CPlayerPed::ProcessControl
    //MakeNOP(0x60EBCC, 5);
    // Or completely return it out:
    MakeRET(0x562C90);

    // Return out more wanted level related functions
    // Just to make sure player doesn't get a wanted level
    // CCrime::reportCrime
    MakeRET(0x532010);

    // Stop CTaskSimpleCarDrive::ProcessPed from exiting passengers with CTaskComplexSequence
    MakeNOP(0x644C18);
    MemPatch<u8>(0x644C19, 0xE9);

    // Disable CFileLoader::LoadPickup
    MakeRET(0x5B47B0);

    // Disable Interior_c::AddPickups
    MakeRET(0x591F90);

    // Make CEventDamage::IsSameEventForAI return false
    MemPatch<u8>(0x4C01F0, 0xB0);
    MemPatch<u8>(0x4C01F1, 0x00);
    MakeNOP(0x4C01F2, 3);

    // Disable CGridRef::Init
    MakeRET(0x71D4E0);

    // Disable CRoadBlocks::Init
    MakeRET(0x461100);
    // Disable CRoadBlocks::GenerateRoadBlocks
    MakeRET(0x4629E0);
    // Disable CRoadBlocks::GenerateRoadBlockPedsForCar
    MakeRET(0x461170);

    // Don't change velocity when colliding with peds in a vehicle
    MemPatch<u32>(0x5F12CA, 0x901CC483);
    MakeNOP(0x5F12CA + 4, 1);

    // Patch ped vehicles damage when flipped
    MemPatch<u16>(0x6A776B, 0xD8DD);
    MakeNOP(0x6A776D, 4);

    // Patch player vehicle damage when flipped
    MemPatch<u16>(0x570E7F, 0xD8DD);
    MakeNOP(0x570E81, 4);

    // No vehicle rewards
    MakeNOP(0x6D16D6);
    MakeJMP(0x6D16D6 + 1, 0x6D17D5);
    MakeNOP(0x6D1999);
    MakeJMP(0x6D1999 + 1, 0x6D1A36);

    // One hit kill for players getting knocked down
    //MakeRETEx(0x5DF8F0, 0);
});
