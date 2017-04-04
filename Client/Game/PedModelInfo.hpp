// Ped model info
// Author(s):       iFarbod <>
//                  dkluin <>
//
// Copyright (c) 2015-2017 CTNorth Team
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
    u32 m_animType;           // +0x24
    u32 m_defaultPedType;     // +0x28
    u32 m_defaultPedStatType; // +0x2C

    union
    {
        struct
        {
            union
            {
                struct
                {
                    //  0  0x00000001
                    u16 m_normalCar : 1;
                    //  1  0x00000002
                    u16 m_poorFamilyCar : 1;
                    //  2  0x00000004
                    u16 m_richFamilyCar : 1;
                    //  3  0x00000008
                    u16 m_executiveCar : 1;
                    //  4  0x00000010
                    u16 m_workerCar : 1;
                    //  5  0x00000020
                    u16 m_bigCar : 1;
                    //  6  0x00000040
                    u16 m_taxiCar : 1;
                    //  7  0x00000080
                    u16 m_moped : 1;

                    //  8  0x00000100
                    u16 m_motorBike : 1;
                    //  9  0x00000200
                    u16 m_leisureBoat : 1;
                    // 10  0x00000400
                    u16 m_workerBoat : 1;
                    // 11  0x00000800
                    u16 m_bicycle : 1;
                    // 12  0x00001000
                    u16 m_onFoot : 1;
                };

                u16 m_carsCanDriveMask;     // +0x30
            };

            union
            {
                struct
                {
                    bool m_buysDrugs : 1;
                    bool m_unk1 : 1;
                    bool m_unk2 : 1;
                    bool m_unk3 : 1;
                    bool m_unk4 : 1;
                    bool m_unk5 : 1;
                    bool m_unk6 : 1;
                    bool m_unk7 : 1;
                };

                u16 m_otherFlags;           // +0x32
            };
        };

        u32 m_pedModelFlags;    // +0x30
    };

    CColModel* m_hitColModel;
    u8 m_firstRadioStation;
    u8 m_secondRadioStation;
    u8 m_race;
    s16 m_audioPedType;
    s16 m_firstVoice;
    s16 m_lastVoice;
    s16 m_nextVoice;
};

VALIDATE_SIZE(CPedModelInfo, 0x44);

}
