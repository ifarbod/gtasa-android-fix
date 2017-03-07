// Vehicle damage manager
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

class CDamageManager
{
    f32 m_wheelDamageEffect;
    u8 m_engineStatus;

    union
    {
        u8 m_wheelsStatus[4];

        struct
        {
            u8 m_rightRearWheelsStatus;
            u8 m_rightFrontWheelsStatus;
            u8 m_leftRearWheelsStatus;
            u8 m_leftFrontWheelsStatus;
        };
    };
    union
    {
        u8 m_doorsStatus[6];

        struct
        {
            u8 m_hoodStatus;
            u8 m_trunkStatus;
            u8 m_leftFrontDoorStatus;
            u8 m_rightFrontDoorStatus;
            u8 m_leftRearDoorStatus;
            u8 m_rightRearDoorStatus;
        };
    };
    union
    {
        u32 m_lightsStatus;

        // 2 bits per light
        struct
        {
            u32 m_leftFrontLightStatus : 2;
            u32 m_rightFrontLightStatus : 2;
            u32 m_rightRearLightStatus : 2;
            u32 m_leftRearLightStatus : 2;
        };
    };
    union
    {
        u32 m_panelsStatus;

        // 4 bits per panel
        // FIXME: Confirm this
        struct
        {
            u8 m_leftFrontWingStatus : 4;
            u8 m_rightFrontWingStatus : 4;
            u8 m_windscreen : 4;
            u8 m_bumpFront : 4;
            u8 m_bumpRear : 4;
        };
    };
};

VALIDATE_SIZE(CDamageManager, 0x18);

}
