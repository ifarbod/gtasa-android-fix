// Entity.hpp
// Author(s):       iFarbod <>
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
        struct RwObject* rwObject_;
        struct RpClump* rpClump_;
        struct RpAtomic* rpAtomic_;
    };

    // flags
    u32 flagsTemp_;

    u16 randomSeed_;
    u16 modelIndex_;
    void* references_; // CReference*
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

    //u8 pad37_;
};

VALIDATE_OFFSET(CEntity, rpClump_, 0x18);
VALIDATE_SIZE(CEntity, 0x38);

}
