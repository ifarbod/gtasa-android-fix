// Game logic hooks
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  LINK/2012 <dma2012@gmail.com>
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

#ifdef CTN_DEBUG
void LoadWeaponData_accuracy(void* p)
{
    Call(0x5389D0, p);

    for (int i = 0; i < 80; i++)
    {
        MemWrite(0xC8AAB8 + i * 0x70 + 0x38, 10.0f);
    }
}
#endif

static HookFunction hookFunction([]()
{
    // Disable CGameLogic::Update
    MakeRet(0x442AD0);

    // Disable CGameLogic::CalcDistanceToForbiddenTrainCrossing
    MakeRet(0x4418E0);

    // Don't give cigar/beer to player on spawn
    MakeShortJmp(0x5FBA26);

    // Disable CObject::ProcessSamSiteBehaviour
    MakeRet(0x5A07D0);

    // Disable CCheat::DoCheats
    //MakeRet(0x439AF0);
    // Disable CCheat::ToggleCheat
    MakeRet(0x438370);

    // Disable the game's replay system (recording & playing - see CReplay stuff)
    // F1 = Play the last 30 second of gameplay
    // F2 = Save the last 30 second of gameplay to Hard Disk
    // F3 = Play a replay from Hard Disk (replay.rep)
    // F1/F3 = Finish playback (if a replay is playing)
    // CReplay::FinishPlayback
    MakeRet(0x45F050);
    // CReplay::TriggerPlayback
    MakeRet(0x4600F0);
    // CReplay::Update
    MakeRet(0x460500);
    // PlayReplayFromHD
    MakeRet(0x460390);

    // No "JCK_HLP" message
    MakeShortJmp(0x63E8DF);

    // Disable CFont::Initialize
    MakeRet(0x5BA690);
    // Disable CFont::Shutdown
    MakeRet(0x7189B0);

    // Disable CPlayerInfo::MakePlayerSafe
    MakeRet(0x56E870, 8);

    // Disable CInterestingEvents::ScanForNearbyEntities
    MakeRet(0x605A30);

    // Disable CGangWars::Update
    MakeRet(0x446610);

    // Disable CConversations::Update
    MakeRet(0x43C590);
    // Disable CPedToPlayerConversations::Update
    MakeRet(0x43B0F0);

    // Disable ValidateVersion
    // Contains a stupid check for 'grandtheftauto3' string in peds.col
    MakeRet(0x5BA060);

    // Disable CShopping::LoadStats
    MakeRet(0x49B6A0);

    // Disable CEntryExitManager::Update
    MakeRet(0x440D10);

    // Stop CTaskSimpleCarDrive::ProcessPed from exiting passengers with CTaskComplexSequence
    MakeNop(0x644C18);
    MemWrite<u8>(0x644C19, 0xE9);

    // Disable CPlayerInfo::WorkOutEnergyFromHunger (Prevents dying from starvation)
    MakeRet(0x56E610);

    // Disable CFileLoader::LoadPickup
    MakeRet(0x5B47B0);

    // Disable Interior_c::AddPickups
    MakeRet(0x591F90);

    // Make CEventDamage::IsSameEventForAI return false
    MemWrite<u8>(0x4C01F0, 0xB0);
    MemWrite<u8>(0x4C01F1, 0x00);
    MakeNop(0x4C01F2, 3);

    // Avoid GTA setting vehicle primary color to white (1) after setting a new paintjob
    MakeNop(0x6D65C5, 11);

    // Disable CGridRef::Init
    MakeRet(0x71D4E0);

    // Don't change velocity when colliding with peds in a vehicle
    MemWrite<u32>(0x5F12CA, 0x901CC483);
    MakeNop(0x5F12CA + 4, 1);

    // Disable ped vehicles damage when flipped
    MemWrite<u16>(0x6A776B, 0xD8DD);
    MakeNop(0x6A776D, 4);

    // Disable player vehicle damage when flipped
    MemWrite<u16>(0x570E7F, 0xD8DD);
    MakeNop(0x570E81, 4);

    // No vehicle rewards
    MakeNop(0x6D16D6);
    MakeJmp(0x6D16D6 + 1, 0x6D17D5);
    MakeNop(0x6D1999);
    MakeJmp(0x6D1999 + 1, 0x6D1A36);

    // Change money counter format to mobile's one
    CopyStr(0x866C94, "$%d");
    CopyStr(0x866C8C, "$-%d");

    // Increase Streaming_rwObjectInstancesList limit (disables flicker)
    MemWrite<s32>(0x5B8E55, 7500 * 0xC);
    MemWrite<s32>(0x5B8EB0, 7500 * 0xC);

    // Disable SecuromStateDisplay
    MakeRet(0x744AE0);

    // SetWindowText
    MemWrite(0x619608, MOD_NAME);

    // Disable CIniFile::LoadIniFile (gta3.ini)
    MakeRet(0x56D070);

    // Disable CStreaming::ReadIniFile (stream.ini)
    MakeRet(0x5BCCD0);

    // CDecisionMakerTypesFileLoader::LoadDefaultDecisionMaker
    MakeRet(0x5BF400);
    // CPedStats::LoadPedStats
    MakeRet(0x5BB890);
    // Change CPedStats::fHeadingChangeRate (was 15.0)
    MemWrite<f32>(0x5BFA1D + 4, 9.5f);

    // No random hydraulics for cars
    MakeShortJmp(0x6B0BC2);

    // Allow boats to rotate
    MemWrite<u8>(0x6F2089, 0x58);
    MakeNop(0x6F208A, 4);

    // Disable real-time shadows for peds
    MakeShortJmp(0x5E68A0);
    MakeShortJmp(0x542483);
    // Disable CRealTimeShadowManager::Update
    MakeRet(0x706AB0);

    // Stop CPed::ProcessControl from calling CVisibilityPlugins::SetClumpAlpha
    MakeNop(0x5E8E84, 5);

    // Stop CVehicle::UpdateClumpAlpha from calling CVisibilityPlugins::SetClumpAlpha
    MakeNop(0x6D29CB, 5);

    // Disable CVehicle::DoDriveByShootings
    MakeNop(0x741FD0, 3);
    MakeRet(0x741FD0);

    // Disable CTaskSimplePlayerOnFoot::PlayIdleAnimations (ret 4)
    MakeRet(0x6872C0, 4);

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
    MakeRet0(0x72DDD0);

    // Increase tail light corona's intensity
    MemWrite<u8>(0x6E1A22, 0xF0);

    // Disable stealth-kill aiming (holding knife up)
    MakeNop(0x685DFB, 5);
    MemWrite<u8>(0x685DFB, 0x33);
    MemWrite<u8>(0x685DFC, 0xC0);
    MakeNop(0x685C3E, 5);
    MemWrite<u8>(0x685C3E, 0x33);
    MemWrite<u8>(0x685C3F, 0xC0);
    MakeNop(0x685DC4, 5);
    MemWrite<u8>(0x685DC4, 0x33);
    MemWrite<u8>(0x685DC5, 0xC0);
    MakeNop(0x685DE6, 5);
    MemWrite<u8>(0x685DE6, 0x33);
    MemWrite<u8>(0x685DE7, 0xC0);

    // Disable stealth-kill rotation in CTaskSimpleStealthKill::ProcessPed
    // Used to face the dying ped away from the killer.
    MakeNop(0x62E63F, 6);
    MemWrite<u8>(0x62E63F, 0xDD);
    MemWrite<u8>(0x62E640, 0xD8);
    MakeNop(0x62E659, 6);
    MemWrite<u8>(0x62E659, 0xDD);
    MemWrite<u8>(0x62E65A, 0xD8);
    MakeNop(0x62E692, 6);
    MemWrite<u8>(0x62E692, 0xDD);
    MemWrite<u8>(0x62E693, 0xD8);

    // Force the MrWhoopee music to load even if we are not the driver.
    MemWrite<u8>(0x4F9CCE, 0xCE);

#ifdef CTN_DEBUG
    // Test no-recoil weapons
    MakeCall(0x5BEC66, LoadWeaponData_accuracy);

    // Limit fps to 60
    MemWrite(0x619626, 60);
#endif
});
