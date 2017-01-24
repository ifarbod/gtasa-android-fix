// 2D Sprite
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Sprite2d.hpp"

namespace ctn::SA
{

CSprite2d::CSprite2d() :
    texture_(nullptr)
{
}

CSprite2d::~CSprite2d()
{
    Delete();
}

void CSprite2d::Delete()
{
    if (texture_)
    {
        auto RwTextureDestroy = Hook::Call<0x7F3820, void, struct RwTexture*>;
        RwTextureDestroy(texture_);
        texture_ = nullptr;
    }
}

void CSprite2d::SetTexture(const char* name, const char* mask)
{
    // Destroy the previous texture
    Delete();

    if (name)
    {
        // RwTextureRead
        texture_ = Hook::Call<0x7F3AC0, struct RwTexture*>(name, mask ? mask : nullptr);
    }
}

void CSprite2d::SetAddressing(RwTextureAddressMode addressMode)
{
    if (texture_)
    {
        //RwTextureSetAddressing(texture_, addressMode);
    }
}

void CSprite2d::SetAddressingUV(RwTextureAddressMode addressModeU, RwTextureAddressMode addressModeV)
{
    if (texture_)
    {
        //RwTextureSetAddressingU(texture_, addressModeU);
        //RwTextureSetAddressingV(texture_, addressModeU);
    }
}

void CSprite2d::SetRenderState()
{
    if (texture_)
    {
        //RwRenderStateSet(rwRENDERSTATETEXTURERASTER, RwTextureGetRaster(texture_));
    }
}

void CSprite2d::Draw(CRect& position, CRGBA& color)
{
    Hook::ThisCall<void>(0x728350, this, &position, &color);
}

void CSprite2d::SetRecipNearClip()
{
    //RecipNearClip = 1.0f / Camera->nearPlane;
}

}
