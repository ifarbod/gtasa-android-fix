// Static entity
// Author(s):       iFarbod <>
//                  GTA Modding Community <http://gtamodding.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Placeable.hpp"

namespace ctn::SA
{

class CEntity : public CPlaceable
{
public:
    union
    {
        struct RwObject* m_rwObject; // +0x18
        struct RpClump* m_rpClump;     // +0x18
        struct RpAtomic* m_rpAtomic;   // +0x18
    };

    union
    {
        struct
        {
            u32 m_usesCollision : 1;      //  0  0x00000001  does entity use collision
            u32 m_collisionProcessed : 1; //  1  0x00000002  has object been processed by a ProcessEntityCollision function
            u32 m_isStatic : 1;           //  2  0x00000004  is entity static
            u32 m_hasContacted : 1;       //  3  0x00000008  has entity processed some contact forces
            u32 m_isStuck : 1;            //  4  0x00000010  is entity stuck
            u32 m_inSafePosition : 1;     //  5  0x00000020  is entity in a collision free safe position
            u32 m_wasPostponed : 1;       //  6  0x00000040  was entity control processing postponed
            u32 m_isVisible : 1;          //  7  0x00000080  is the entity visible
        };
        u32 m_flags; // +0x1C
    };

    u16 m_randomSeed;
    u16 m_modelIndex;
    void* m_references;    // CReference*
    void* m_streamingLink; // CLink<CEntity*>*
    s16 m_scanCode;
    u8 m_iplSlot;
    u8 m_areaCode;

    // ref @ 0x5B51E0
    union
    {
        class CEntity* m_lodEntity;
        s32 m_lodIndex;
    };

    u8 m_numLodChildren;
    u8 m_numLodChildrenRendered;

    union
    {
        u8 m_typeStatus;

        struct
        {
            u8 m_type : 3;
            u8 m_status : 5;
        };
    };
};

VALIDATE_SIZE(CEntity, 0x38);

}
