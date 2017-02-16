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
    f32 f814_;
    f32 f818_;
    f32 f81C_;
    f32 f820_;
    f32 f824_;
    f32 f828_;
    f32 distanceBetweenWheels_;
    f32 wheelsBalance_;
    u8 f834_;
    s8 _pad[3];
};

//VALIDATE_SIZE(CBmx, 0x838);

}
