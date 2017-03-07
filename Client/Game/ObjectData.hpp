// Dynamic object properties
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
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
    f32 m_mass;
    f32 m_turnMass;
    f32 m_airResistance;
    f32 m_elasticity;
    f32 m_buoyancyConstant;
    f32 m_uprootLimit;
    f32 m_colDamageMultiplier;
    u8 m_colDamageEffect;
    u8 m_specialColResponseCase;
    u8 m_cameraAvoidObject;
    u8 m_causesExplosion;
    u8 m_fxType;
    CVector m_fxOffset;
    class FxSystem_c* m_fxSystem;
    f32 m_smashMultiplier;
    CVector m_breakVelocity;
    f32 m_breakVelocityRand;
    u32 m_gunBreakMode;
    u32 m_sparksOnImpact;
};

VALIDATE_SIZE(CObjectData, 0x50);

}
