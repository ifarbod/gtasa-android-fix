// Im2D Font renderer's Render State
//
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "RGBA.hpp"
#include "Vector2D.hpp"

namespace ctn::SA
{

struct CFontRenderState
{
    CFontRenderState() = default;
    ~CFontRenderState() = default;

    u32 m_field00;             // +0x00
    CVector2D m_position;      // +0x04
    CVector2D m_scale;         // +0x0C
    CRGBA m_color;             // +0x14
    f32 m_field18;             // +0x18
    f32 m_slant;               // +0x1C
    CVector2D m_slantRefPoint; // +0x20
    bool m_forcedColor;        // +0x28
    u8 m_fontAttr;             // +0x29
    bool m_proportional;       // +0x2A
    s8 m_field2B;              // +0x2B  pad?
    s16 m_sprite;              // +0x2C
    u8 m_edgeY;                // +0x2E
    u8 m_field2F;              // +0x2F  pad?
};

// TODO: Confirm size (0x34?)
VALIDATE_SIZE(CFontRenderState, 0x30);

}
