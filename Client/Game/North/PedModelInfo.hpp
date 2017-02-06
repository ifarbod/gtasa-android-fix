// PedModelInfo.hpp
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
    s32 MotionAnimGroup; // AssocGroupID
    s32 DefPedType;
    s32 DefPedStats; // ePedStats

    union
    {
        struct
        {
            u32 drivingMask_;
        };
        struct
        {
            u16 CanDriveCars;
            u16 PedFlags;
        };
    };
    class CColModel* HitColModel;
    u8 FirstRadioStation;
    u8 SecondRadioStation;
    u8 Race;
    u8 f3B_;
    s16 AudioPedType;
    s16 FirstVoice;
    s16 LastVoice;
    s16 NextVoice;
};

VALIDATE_SIZE(CPedModelInfo, 0x44);

}
