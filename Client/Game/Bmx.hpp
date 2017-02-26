// Bmx bike vehicle entity
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Bike.hpp"

namespace ctn::SA
{

class CBmx : public CBike
{
public:
    f32 m_field814;
    f32 m_field818;
    f32 m_field81C;
    f32 m_field820;
    f32 m_field824;
    f32 m_field828;
    f32 m_distanceBetweenWheels;
    f32 m_wheelsBalance;
    u8 m_field834;
    s8 _pad[3];
};

//VALIDATE_SIZE(CBmx, 0x838);

}
