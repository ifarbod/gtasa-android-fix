// RGBA color representation
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "RGBA.hpp"

namespace ctn::SA
{

CRGBA::CRGBA() :
    colorInt_(0)
{
}

CRGBA::CRGBA(u8 r, u8 g, u8 b) :
    red_(r),
    green_(g),
    blue_(b),
    alpha_(255)
{
}

CRGBA::CRGBA(u8 r, u8 g, u8 b, u8 a) :
    red_(r),
    green_(g),
    blue_(b),
    alpha_(a)
{
}

CRGBA::CRGBA(f32 r, f32 g, f32 b) :
    red_(static_cast<u8>(r * 255.0f)),
    green_(static_cast<u8>(g * 255.0f)),
    blue_(static_cast<u8>(b * 255.0f)),
    alpha_(255)
{
}

CRGBA::CRGBA(f32 r, f32 g, f32 b, f32 a) :
    red_(static_cast<u8>(r * 255.0f)),
    green_(static_cast<u8>(g * 255.0f)),
    blue_(static_cast<u8>(b * 255.0f)),
    alpha_(static_cast<u8>(a * 255.0f))
{
}

CRGBA::CRGBA(u32 color) :
    colorInt_(color)
{
}

u32 CRGBA::AsARGB() const
{
    return (blue_ << 0 | green_ << 8 | red_ << 16 | alpha_ << 24);
}

u32 CRGBA::AsBGRA() const
{
    return (alpha_ << 0 | red_ << 8 | green_ << 16 | blue_ << 24);
}

u32 CRGBA::AsRGBA() const
{
    return (alpha_ << 0 | blue_ << 8 | green_ << 16 | red_ << 24);
}

}
