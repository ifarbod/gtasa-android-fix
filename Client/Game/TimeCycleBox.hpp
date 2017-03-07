// TimeCycleBox
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Box.hpp"

namespace ctn::SA
{

class CTimeCycleBox : public CBox
{
public:
    s16 m_field18; // +0x18
    u8 m_field1A;  // +0x1A
    u8 m_field1B;  // +0x1B
    s32 m_field1C; // +0x1C
    f32 m_field20; // +0x20
    f32 m_field24; // +0x24
};

}
