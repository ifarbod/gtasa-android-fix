// 2D Sprite
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Rect.hpp"
#include "RGBA.hpp"

namespace ctn::SA
{

// Two-dimensional sprite.
class CSprite2d
{
public:
    // Construct a 2D sprite with a null texture.
    CSprite2d();
    // Destructor.
    ~CSprite2d();
    // Destroy this sprite's texture.
    void Delete();
    // Load texture from the current texture dictionary.
    void SetTexture(const char* name, const char* mask = nullptr);
    // Set texture addressing for this sprite.
    void SetAddressing(enum RwTextureAddressMode _nAddressMode);
    // Set texture UV addressing for this sprite.
    void SetAddressingUV(enum RwTextureAddressMode _nAddressModeU, enum RwTextureAddressMode _nAddressModeV);
    // Set render states for rendering this sprite.
    void SetRenderState();

    // Draw this sprite at the specified position with the desired color.
    void Draw(CRect& position, CRGBA& color);

    // Sets recip near clip for each frame.
    static void SetRecipNearClip();

public:
    // Texture.
    struct RwTexture* m_texture;

    static u32 nextBufferVertex;
    static u32 nextBufferIndex;
    static f32 NearScreenZ;
    static f32 RecipNearClip;
    //static RwIm2DVertex maVertices[8];
};

VALIDATE_SIZE(CSprite2d, 0x4);

}
