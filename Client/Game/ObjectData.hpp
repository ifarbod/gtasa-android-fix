// Dynamic object properties
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Vector.hpp"

namespace ctn::SA
{

class CObjectData
{
public:
    static void Initialize();
    static void SetObjectData();

private:
    f32 mass_;
    f32 turnMass_;
    f32 fAirResistance_;
    f32 fElasticity_;
    f32 fBuoyancyConstant_;
    f32 fUprootLimit_;
    f32 fColDamageMultiplier_;
    u8 nColDamageEffect_;
    u8 nSpecialColResponseCase_;
    u8 bCameraAvoidObject_;
    u8 bCausesExplosion_;
    u8 nFxType_;
    CVector vFxOffset_;
    class FxSystem_c* pFxSystem_;
    f32 fSmashMultiplier_;
    CVector vBreakVelocity_;
    f32 fBreakVelocityRand_;
    u32 dwGunBreakMode_;
    u32 dwSparksOnImpact_;
};

VALIDATE_SIZE(CObjectData, 0x50);

}
