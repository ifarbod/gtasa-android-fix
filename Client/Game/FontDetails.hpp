// Im2D Font renderer current text sprite details
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

struct CFontDetails
{
    CFontDetails() = default;
    ~CFontDetails() = default;

    CRGBA m_color;             // 00-04
    CVector2D m_scale;         // 04-0C
    f32 m_slant;               // 0C-10
    CVector2D m_slantRefPoint; // 10-18
    u8 m_justify;
    u8 m_centerAlign;
    u8 m_rightAlign;
    u8 m_background;
    u8 m_backgroundTextOnly;
    u8 m_proportional;
    bool m_isBlip;
    f32 m_alphaFade;
    CRGBA m_backgroundColor;
    f32 m_wrapx;
    f32 m_centerSize;
    f32 m_rightJustifyWrap;
    u8 m_fontTextureId;
    u8 m_fontStyle;
    u8 m_shadow;
    CRGBA m_dropColor;
    u8 m_edgeX;
    u8 m_edgeY;
};

// TODO: Confirm size (0x44?)
VALIDATE_SIZE(CFontDetails, 0x40);

}
