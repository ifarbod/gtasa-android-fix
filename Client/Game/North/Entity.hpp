// Entity.hpp
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
        struct RwObject* rwObject_; // +0x18
        struct RpClump* rpClump_;   // +0x18
        struct RpAtomic* rpAtomic_; // +0x18
    };

    union
    {
        struct
        {
            u32 usesCollision_ : 1;      //  0  0x00000001  does entity use collision
            u32 collisionProcessed_ : 1; //  1  0x00000002  has object been processed by a ProcessEntityCollision function
            u32 isStatic_ : 1;           //  2  0x00000004  is entity static
            u32 hasContacted_ : 1;       //  3  0x00000008  has entity processed some contact forces
            u32 isStuck_ : 1;            //  4  0x00000010  is entity stuck
            u32 inSafePosition_ : 1;     //  5  0x00000020  is entity in a collision free safe position
            u32 wasPostponed_ : 1;       //  6  0x00000040  was entity control processing postponed
            u32 isVisible_ : 1;          //  7  0x00000080  is the entity visible
        };
        u32 flags_; // +0x1C
    };

    u16 randomSeed_;
    u16 modelIndex_;
    void* references_;    // CReference*
    void* streamingLink_; // CLink<CEntity*>*
    s16 scanCode_;
    u8 iplSlot_;
    u8 areaCode_;

    // ref @ 0x5B51E0
    union
    {
        class CEntity* lodEntity_;
        s32 lodIndex_;
    };

    u8 numLodChildren_;
    u8 numLodChildrenRendered_;

    union
    {
        u8 typeStatus_;

        struct
        {
            u8 type_ : 3;
            u8 status_ : 5;
        };
    };
};

VALIDATE_SIZE(CEntity, 0x38);

}
