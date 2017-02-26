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
    void* m_handling;             // +0x988
    CRideAnimData m_rideAnimData; // +0x98C
    f32 m_field9A8;               // +0x9A8
    s32 m_field9AC;               // +0x9AC
    s32 m_field9B0;               // +0x9B0
    s32 m_field9B4;               // +0x9B4
    u8 m_quadFlags;               // +0x9B8
    u8 m_field9B9[3];             // +0x9B9
};

//VALIDATE_SIZE(CQuadBike, 0x9BC);

}
