// Animation blending sequence
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"

namespace ctn::SA
{

class CAnimBlendSequence
{
private:
    s32 hash_;
    u16 flags_;
    u16 frameCount_;
    s32 frames_;
};

VALIDATE_SIZE(CAnimBlendSequence, 0xC);

}
