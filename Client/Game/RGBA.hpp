// RGBA.hpp
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

class CRGBA
{
public:
    // Default ctor.
    CRGBA();
    // Construct from rgb values and set alpha fully opaque.
    CRGBA(u8 r, u8 g, u8 b);
    // Construct from rgba values.
    CRGBA(u8 r, u8 g, u8 b, u8 a);
    // Construct from floating point rgb values and set alpha fully opaque.
    CRGBA(f32 r, f32 g, f32 b);
    // Construct from floating point rgba values.
    CRGBA(f32 r, f32 g, f32 b, f32 a);
    // Construct from an unsigned int.
    CRGBA(u32 color);

    //operator RwRGBA*() { return rwRGBA_; }

    u32 AsARGB() const;
    u32 AsBGRA() const;
    u32 AsRGBA() const;

public:
    union
    {
        struct
        {
            // Red.
            u8 red_;
            // Green.
            u8 green_;
            // Blue.
            u8 blue_;
            // Alpha.
            u8 alpha_;
        };
        struct
        {
            u32 colorInt_;
        };
        struct
        {
            //RwRGBA rwRGBA_;
        };
    };
};

}
