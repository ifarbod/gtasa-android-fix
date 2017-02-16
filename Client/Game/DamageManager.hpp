// DamageManager.hpp
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

class CDamageManager
{
    f32 m_fWheelDamageEffect;
    u8 m_nEngineStatus;
    union
    {
        u8 m_anWheelsStatus[4];
        struct
        {
            u8 m_nRightRearWheelsStatus;
            u8 m_nRightFrontWheelsStatus;
            u8 m_nLeftRearWheelsStatus;
            u8 m_nLeftFrontWheelsStatus;
        };
    };
    union
    {
        u8 m_anDoorsStatus[6];
        struct
        {
            u8 m_nHoodStatus;
            u8 m_nTrunkStatus;
            u8 m_nLeftFrontDoorStatus;
            u8 m_nRightFrontDoorStatus;
            u8 m_nLeftRearDoorStatus;
            u8 m_nRightRearDoorStatus;
        };
    };
    union
    {
        u32 m_dwLightsStatus;
        struct
        {
            u32 m_nLeftFrontLightStatus : 2;
            u32 m_nRightFrontLightStatus : 2;
            u32 m_nRightRearLightStatus : 2;
            u32 m_nLeftRearLightStatus : 2;
        };
    };
    union
    {
        u32 m_dwPanelsStatus;
        struct
        {
            // 4 bytes per panel
            // : 4;
        };
    };
};

VALIDATE_SIZE(CDamageManager, 0x18);

}
