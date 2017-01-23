// 2D Sprite
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"

namespace SA
{

// Two-dimensional sprite.
class CSprite2d
{
public:
    CSprite2d() : texture_(nullptr) {}
    ~CSprite2d();

    void Delete();

private:
    // Texture.
    struct RwTexture* texture_;
};
VALIDATE_SIZE(CSprite2d, 0x4);

}
