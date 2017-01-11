// GTA Matrix
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "RWTypes.hpp"
#include "Vector.hpp"

namespace SA
{

class CMatrix
{
public:
    union
    {
        struct
        {
            CVector right_;
            u32 flags_;
            CVector up_;
            u32 pad1_;
            CVector at_;
            u32 pad2_;
            CVector pos_;
            u32 pad3_;
        };

        struct
        {
            RwMatrix matrix_;
        };
    };

    RwMatrix* rwMatrix_;
    b32 removeRwMatrix_;
};
VALIDATE_SIZE(CMatrix, 0x48);

}
