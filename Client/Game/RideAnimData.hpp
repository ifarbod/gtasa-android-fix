// Riding animation configuration
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
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
    u32 animGroup_;
    s32 f04_;
    f32 animLean_;
    s32 f0C_;
    f32 f10_;
    f32 handlebarsAngle_;
    f32 animPercentageState_;
};

VALIDATE_SIZE(CRideAnimData, 0x1C);

}
