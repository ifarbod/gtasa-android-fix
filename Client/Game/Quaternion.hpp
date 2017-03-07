// Quaternion
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Vector.hpp"

namespace ctn::SA
{

class CQuaternion
{
public:
    union
    {
        struct
        {
            f32 m_x;
            f32 m_y;
            f32 m_z;
        };
        CVector m_imag;
    };
    union
    {
        f32 m_w;
        f32 m_real;
    };
};

VALIDATE_SIZE(CQuaternion, 0x10);

}
