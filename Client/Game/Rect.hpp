// 2D Rectangle
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"

namespace ctn::SA
{

class CRect
{
public:
    CRect();
    CRect(f32 left, f32 top, f32 right, f32 bottom);

public:
    f32 left_;
    f32 bottom_;
    f32 right_;
    f32 top_;
};

VALIDATE_SIZE(CRect, 0x10);

}
