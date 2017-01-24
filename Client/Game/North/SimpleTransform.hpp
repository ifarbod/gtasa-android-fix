// Simple transform
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Vector.hpp"

namespace ctn::SA
{

class CSimpleTransform
{
public:
    CVector position_;
    f32 a_;
};

VALIDATE_SIZE(CSimpleTransform, 0x10);

}
