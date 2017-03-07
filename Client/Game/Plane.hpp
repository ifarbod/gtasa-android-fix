// Plane vehicle entity
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Automobile.hpp"

// +0x988 Multiplier used for vortex ?
// +0x9A0 Some vehicle audio related thingy (sets m_field7C in CAEVehicleAudioEntity)

namespace ctn::SA
{

class FxSystem_c;

class CPlane : public CAutomobile
{
public:
    f32 m_field988;                    // +0x988
    s32 m_field98C;                    // +0x98C
    s32 m_field990;                    // +0x990
    s32 m_field994;                    // +0x994
    f32 m_field998;                    // +0x998
    s32 m_field99C;                    // +0x99C
    s32 m_field9A0;                    // +0x9A0
    s32 m_field9A4;                    // +0x9A4
    f32 m_field9A8;                    // +0x9A8
    f32 m_field9AC;                    // +0x9AC
    f32 m_field9B0;                    // +0x9B0
    f32 m_field9B4;                    // +0x9B4
    s32 m_field9B8;                    // +0x9B8
    s32 m_field9BC;                    // +0x9BC
    u32 m_startedFlyingTime;           // +0x9C0
    s32 m_bladeSpeed;                  // +0x9C4
    f32 m_bladeAngle;                  // +0x9C8
    f32 m_landingGearStatus;           // +0x9CC
    s32 m_field9D0;                    // +0x9D0
    FxSystem_c** m_gunParticles;       // +0x9D4
    u8 m_firingMultiplier;             // +0x9D8
    s32 m_timeOfAutoaimKeyPressed;     // +0x9D9
    CVehicle* m_autoaimedVehicle;      // +0x9E0
    s32 m_field9E4;                    // +0x9E4
    FxSystem_c* m_jettrusParticles[4]; // +0x9E8
    FxSystem_c* m_smokeParticle;       // +0x9F8
    u32 m_smokeTimer;                  // +0x9FC
    bool m_smokeEjectorEnabled;        // +0xA00
    u8 m_fieldA01[3];                  // +0xA01
};

//VALIDATE_SIZE(CPlane, 0xA04);

}
