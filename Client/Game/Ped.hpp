// Character entity
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Physical.hpp"
#include "AEPedAudioEntity.hpp"
#include "AEPedSpeechAudioEntity.hpp"
#include "AEPedWeaponAudioEntity.hpp"

namespace ctn::SA
{

class CPed : public CPhysical
{
public:
    // Memory allocation in pool.
    void* operator new(u32 size);
    // Memory deallocation in pool.
    void operator delete(void* ptr);

public:
    CAEPedAudioEntity m_pedAE;             // +0x138
    CAEPedSpeechAudioEntity m_pedSpeechAE; // +0x294
    CAEPedWeaponAudioEntity m_pedWeaponAE; // +0x394
    s32 m_field43C;                        // +0x43C
    s32 m_field440;                        // +0x440
    s32 m_field444;                        // +0x444
    s32 m_field448;                        // +0x448
    s32 m_field44C;                        // +0x44C
    s32 m_field450;                        // +0x450
    s32 m_field454;                        // +0x454
    s32 m_field458;                        // +0x458
    s32 m_field45C;                        // +0x45C
    s32 m_field460;                        // +0x460
    s32 m_field464;                        // +0x464
    s32 m_field468;                        // +0x468
    union
    {
        struct
        {
            //   0  Is ped standing on something
            u32 m_isStanding : 1;
            //   1  Was ped standing on something
            u32 m_wasStanding : 1;
            //   2  Is ped looking at something or in a direction
            u32 m_isLooking : 1;
            //   3  Is ped restoring head postion from a look
            u32 m_isRestoringLook : 1;
            //   4  Is ped aiming gun
            u32 m_isAimingGun : 1;
            //   5  Is ped moving gun back to default position
            u32 m_isRestoringGun : 1;
            //   6  Can ped point gun at target
            u32 m_canPointGunAtTarget : 1;
            //   7  Is ped talking(see Chat())
            u32 m_isTalking : 1;

            //   8  Is in a vehicle
            u32 m_inVehicle : 1;
            //   9  Is in the air
            u32 m_isInTheAir : 1;
            //  10  Is landing after being in the air
            u32 m_isLanding : 1;
            //  11  Has been in a collision last frame
            u32 m_hitSomethingLastFrame : 1;
            //  12  Has been in a collision last frame
            u32 m_isNearCar : 1;
            //  13  Has been in a collision last frame
            u32 m_renderPedInCar : 1;
            //  14  Update ped heading due to heading change during anim sequence
            u32 m_updateAnimHeading : 1;
            //  15  Waiting on AntiSpazTimer to remove head
            u32 m_removeHead : 1;

            //  16  Is pulling trigger
            u32 m_firingWeapon : 1;
            //  17  Does ped possess a camera to document accidents
            u32 m_hasACamera : 1;
            //  18  Ped loses a lot of blood if true
            u32 m_pedIsBleeding : 1;
            //  19  Ped cannot reach target to attack with fist, need to use gun
            u32 m_stopAndShoot : 1;
            //  20  Is ped die animation finished so can dead now
            u32 m_isPedDieAnimPlaying : 1;
            //  21  When set, ped stays put
            u32 m_stayInSamePlace : 1;
            //  22  When set, ped doesn't seek out opponent or cover large distances. Will still shuffle and look for cover
            u32 m_kindaStayInSamePlace : 1;
            //  23  Use nodes for routefind
            u32 m_beingChasedByPolice : 1;

            //  24  Is this ped allowed to duck at all?
            u32 m_notAllowedToDuck : 1;
            //  25  Duck behind cars etc
            u32 m_crouchWhenShooting : 1;
            //  26  Duck behind cars etc
            u32 m_isDucking : 1;
            //  27  Don't want to play getup anim if under something
            u32 m_getUpAnimStarted : 1;
            //  28
            u32 m_doBloodyFootprints : 1;
            //  29
            u32 m_dontDragMeOutCar : 1;
            //  30  Set if the polygon the ped is on is still valid for collision
            u32 m_stillOnValidPoly : 1;
            //  31
            u32 m_allowMedicsToReviveMe : 1;
        };

        u32 m_pedFlags1;
    };
    union
    {
        struct
        {

        };

        u32 m_pedFlags2;
    };
    union
    {
        struct
        {

        };

        u32 m_pedFlags3;
    };
    union
    {
        struct
        {
            //  96
            u32 m_hasBulletProofVest : 1;
            //  97
            u32 m_usingMobilePhone : 1;
            //  98
            u32 m_upperBodyDamageAnimsOnly : 1;
            //  99
            u32 m_stuckUnderCar : 1;
            // 100  If true ped will carry on with task even after cleanup
            u32 m_keepTasksAfterCleanUp : 1;
            // 101
            u32 m_isDyingStuck : 1;
            // 102  set when walking round buildings, reset when task quits
            u32 m_ignoreHeightCheckOnGotoPointTask : 1;
            // 103
            u32 m_forceDieInCar : 1;

            // 104
            u32 m_checkColAboveHead : 1;
            // 105
            u32 m_ignoreWeaponRange : 1;
            // 106
            u32 m_druggedUp : 1;
            // 107  if this is set, the cops will always go after this ped when they are doing a KillCriminal task
            u32 m_wantedByPolice : 1;
            // 108
            u32 m_signalAfterKill : 1;
            // 109
            u32 m_canClimbOntoBoat : 1;
            // 110  useful to store this so that AI knows (normal will still be available)
            u32 m_pedHitWallLastFrame : 1;
            // 111
            u32 m_ignoreHeightDifferenceFollowingNodes : 1;

            // 112
            u32 m_moveAnimSpeedHasBeenSetByTask : 1;
            // 113
            u32 m_getOutUpsideDownCar : 1;
            // 114
            u32 m_justGotOffTrain : 1;
            // 115
            u32 m_deathPickupsPersist : 1;
            // 116
            u32 m_testForShotInVehicle : 1;
            // 117  This ped is controlled by replay and should be removed when replay is done.
            u32 m_usedForReplay : 1;
            // 118
            u32 m_dummyFlag : 2;

            // 120
            u32 m_dummyFlag2 : 8;
        };

        u32 m_pedFlags4;
    };
};

//VALIDATE_SIZE(CPed, 0x79C);

}
