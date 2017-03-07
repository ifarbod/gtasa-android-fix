// Riding animation configuration
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"

namespace ctn::SA
{

class CRideAnimData
{
public:
    u32 m_animGroup;           // +0x00
    s32 m_field04;             // +0x04
    f32 m_animLean;            // +0x08
    s32 m_field0C;             // +0x0C
    f32 m_field10;             // +0x10
    f32 m_handlebarsAngle;     // +0x14
    f32 m_animPercentageState; // +0x18
};

VALIDATE_SIZE(CRideAnimData, 0x1C);

}
