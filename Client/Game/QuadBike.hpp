// Four-wheeled bike vehicle entity
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Automobile.hpp"
#include "RideAnimData.hpp"

namespace ctn::SA
{

class CQuadBike : public CAutomobile
{
public:
    void* handling_;
    CRideAnimData rideAnimData_;
    float f9A8;
    s32 f9AC_;
    s32 f9B0_;
    s32 f9B4_;
    u8 quadFlags_;
    PAD(CQuadBike, pad, 3);
};

//VALIDATE_SIZE(CQuadBike, 0x9BC);

}
