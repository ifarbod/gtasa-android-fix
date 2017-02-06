// Plane.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Automobile.hpp"

namespace ctn::SA
{

class CPlane : public CAutomobile
{
public:
    f32 f988_; // multiplier used for vortex?
    s32 f98C_;
    s32 f990_;
    s32 f994_;
    f32 f998_;
    s32 f99C_;
    s32 f9A0_; // some vehicle audio related thingy (sets f7C in CAEVehicleAudioEntity)
    s32 f9A4_;
    f32 f9A8_;
    f32 f9AC_;
    f32 f9B0_;
    f32 f9B4_;
    s32 f9B8_;
    s32 f9BC_;
    u32 startedFlyingTime_;
    s32 bladeSpeed_;
    f32 bladeAngle_;
    float landingGearStatus_;
    s32 f9D0_;
    class FxSystem_c** gunParticles_;
    u8 firingMultiplier_;
    s32 timeOfAutoaimKeyPressed_;
    CVehicle* autoaimedVehicle_;
    s32 f9E4_;
    class FxSystem_c* jettrusParticles_[4];
    class FxSystem_c* m_pSmokeParticle;
    u32 smokeTimer_;
    bool smokeEjectorEnabled_;
    PAD(p, pad, 3);
};

//VALIDATE_SIZE(CPlane, 0xA04);

}
