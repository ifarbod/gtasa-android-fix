// GTA Matrix
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
//#include "RWTypes.hpp"
#include "Vector.hpp"

namespace ctn::SA
{

class CMatrix
{
public:
    union
    {
        struct
        {
            CVector m_right;
            u32 m_flags;
            CVector m_up;
            u32 m_pad1;
            CVector m_at;
            u32 m_pad2;
            CVector m_pos;
            u32 m_pad3;
        };

        struct
        {
            //RwMatrix m_matrix;
        };
    };

    struct RwMatrix* m_rwMatrix;
    b32 m_removeRwMatrix;
};

VALIDATE_SIZE(CMatrix, 0x48);

}
