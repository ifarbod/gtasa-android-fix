// Game logic hooks
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  LINK/2012 <dma2012@gmail.com>
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
    // No DirectPlay dependency - Better compatibility for Windows 8+
    MemWrite<u8>(0x74754A, 0xB8);
    MemWrite<u32>(0x74754B, 0x900);

    // Disable CGameLogic::Update
    MakeRET(0x442AD0);

    // Disable CGameLogic::CalcDistanceToForbiddenTrainCrossing
    MakeRET(0x4418E0);

    // Don't give cigar/beer to player on spawn
    MakeShortJmp(0x5FBA26);

    // Disable CObject::ProcessSamSiteBehaviour
    MakeRET(0x5A07D0);

    // Disable CCheat::DoCheats
    //MakeRET(0x439AF0);
    // Disable CCheat::ToggleCheat
    MakeRET(0x438370);

    // Disable the game's replay system (recording & playing - see CReplay stuff)
    // F1 = Play the last 30 second of gameplay
    // F2 = Save the last 30 second of gameplay to Hard Disk
    // F3 = Play a replay from Hard Disk (replay.rep)
    // F1/F3 = Finish playback (if a replay is playing)
    // CReplay::FinishPlayback
    MakeRET(0x45F050);
    // CReplay::TriggerPlayback
    MakeRET(0x4600F0);
    // CReplay::Update
    MakeRET(0x460500);
    // PlayReplayFromHD
    MakeRET(0x460390);

    // No "JCK_HLP" message
    MakeShortJmp(0x63E8DF);

    // Disable CFont::Initialize
    MakeRET(0x5BA690);
    // Disable CFont::Shutdown
    MakeRET(0x7189B0);

    // Disable CPlayerInfo::MakePlayerSafe
    MakeRET(0x56E870, 8);

    // Disable CInterestingEvents::ScanForNearbyEntities
    MakeRET(0x605A30);

    // Disable CGangWars::Update
    MakeRET(0x446610);

    // Disable CConversations::Update
    MakeRET(0x43C590);
    // Disable CPedToPlayerConversations::Update
    MakeRET(0x43B0F0);

    // Disable ValidateVersion
    // Contains a stupid check for 'grandtheftauto3' string in peds.col
    MakeRET(0x5BA060);

    // Disable CShopping::LoadStats
    MakeRET(0x49B6A0);

    // Stop CTaskSimpleCarDrive::ProcessPed from exiting passengers with CTaskComplexSequence
    MakeNOP(0x644C18);
    MemWrite<u8>(0x644C19, 0xE9);

    // Disable CPlayerInfo::WorkOutEnergyFromHunger (Prevents dying from starvation)
    MakeRET(0x56E610);

    // Disable CFileLoader::LoadPickup
    MakeRET(0x5B47B0);

    // Disable Interior_c::AddPickups
    MakeRET(0x591F90);

    // Make CEventDamage::IsSameEventForAI return false
    MemWrite<u8>(0x4C01F0, 0xB0);
    MemWrite<u8>(0x4C01F1, 0x00);
    MakeNOP(0x4C01F2, 3);

    // Avoid GTA setting vehicle primary color to white (1) after setting a new paintjob
    MakeNOP(0x6D65C5, 11);

    // Disable CGridRef::Init
    MakeRET(0x71D4E0);

    // Don't change velocity when colliding with peds in a vehicle
    MemWrite<u32>(0x5F12CA, 0x901CC483);
    MakeNOP(0x5F12CA + 4, 1);

    // Disable ped vehicles damage when flipped
    MemWrite<u16>(0x6A776B, 0xD8DD);
    MakeNOP(0x6A776D, 4);

    // Disable player vehicle damage when flipped
    MemWrite<u16>(0x570E7F, 0xD8DD);
    MakeNOP(0x570E81, 4);

    // No vehicle rewards
    MakeNOP(0x6D16D6);
    MakeJMP(0x6D16D6 + 1, 0x6D17D5);
    MakeNOP(0x6D1999);
    MakeJMP(0x6D1999 + 1, 0x6D1A36);

    // Change money counter format to mobile's one
    CopyStr(0x866C94, "$%d");
    CopyStr(0x866C8C, "$-%d");

    // Increase Streaming_rwObjectInstancesList limit (disables flicker)
    MemWrite<s32>(0x5B8E55, 7500 * 0xC);
    MemWrite<s32>(0x5B8EB0, 7500 * 0xC);

    // Disable SecuromStateDisplay
    MakeRET(0x744AE0);

    // SetWindowText
    MemWrite(0x619608, MOD_NAME);

    // Disable CIniFile::LoadIniFile (gta3.ini)
    MakeRET(0x56D070);

    // Disable CStreaming::ReadIniFile (stream.ini)
    MakeRET(0x5BCCD0);

    // CDecisionMakerTypesFileLoader::LoadDefaultDecisionMaker
    MakeRET(0x5BF400);
    // CPedStats::LoadPedStats
    MakeRET(0x5BB890);
    // Change CPedStats::fHeadingChangeRate (was 15.0)
    MemWrite<f32>(0x5BFA1D + 4, 9.5f);

    // No random hydraulics for cars
    MakeShortJmp(0x6B0BC2);

    // Allow boats to rotate
    MemWrite<u8>(0x6F2089, 0x58);
    MakeNOP(0x6F208A, 4);

    // Disable real-time shadows for peds
    MakeShortJmp(0x5E68A0);
    MakeShortJmp(0x542483);

    // Stop CPed::ProcessControl from calling CVisibilityPlugins::SetClumpAlpha
    MakeNOP(0x5E8E84, 5);

    // Stop CVehicle::UpdateClumpAlpha from calling CVisibilityPlugins::SetClumpAlpha
    MakeNOP(0x6D29CB, 5);

    // Disable CVehicle::DoDriveByShootings
    MakeNOP(0x741FD0, 3);
    MakeRET(0x741FD0);

    // Disable CTaskSimplePlayerOnFoot::PlayIdleAnimations (ret 4)
    MakeRET(0x6872C0, 4);

    // Hack to make the choke task use 0 time left remaining when he starts to just stand there looking. So he won't do
    // that.
    MemWrite<u8>(0x620607, 0x33);
    MemWrite<u8>(0x620608, 0xC0);

    MemWrite<u8>(0x620618, 0x33);
    MemWrite<u8>(0x620619, 0xC0);
    MemWrite<u8>(0x62061A, 0x90);
    MemWrite<u8>(0x62061B, 0x90);
    MemWrite<u8>(0x62061C, 0x90);

    // Disable speed limits
    //MakeRET0(0x72DDD0);
});
