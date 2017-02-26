// Ped model info
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "ClumpModelInfo.hpp"

namespace ctn::SA
{

class CPedModelInfo : public CClumpModelInfo
{
public:
    s32 m_motionAnimGroup; // AssocGroupID
    s32 m_defPedType;
    s32 m_defPedStats; // ePedStats

    union
    {
        struct
        {
            u32 m_drivingMask;
        };
        struct
        {
            u16 m_canDriveCars;
            u16 m_pedFlags;
        };
    };
    class CColModel* m_hitColModel;
    u8 m_firstRadioStation;
    u8 m_secondRadioStation;
    u8 m_race;
    u8 m_field3B;
    s16 m_audioPedType;
    s16 m_firstVoice;
    s16 m_lastVoice;
    s16 m_nextVoice;
};

VALIDATE_SIZE(CPedModelInfo, 0x44);

}
