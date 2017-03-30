// 2D Sprite
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Sprite2d.hpp"

namespace ctn::SA
{

CSprite2d::CSprite2d() :
    m_texture(nullptr)
{
}

CSprite2d::~CSprite2d()
{
    Delete();
}

void CSprite2d::Delete()
{
    if (m_texture)
    {
        auto RwTextureDestroy = Hook::Call<0x7F3820, void, struct RwTexture*>;
        RwTextureDestroy(m_texture);
        m_texture = nullptr;
    }
}

void CSprite2d::SetTexture(const char* name, const char* mask)
{
    // Destroy the previous texture
    Delete();

    if (name)
    {
        // RwTextureRead
        m_texture = Hook::Call<0x7F3AC0, struct RwTexture*>(name, mask ? mask : nullptr);
    }
}

void CSprite2d::SetAddressing(RwTextureAddressMode addressMode)
{
    if (m_texture)
    {
        //RwTextureSetAddressing(m_texture, addressMode);
    }
}

void CSprite2d::SetAddressingUV(RwTextureAddressMode addressModeU, RwTextureAddressMode addressModeV)
{
    if (m_texture)
    {
        //RwTextureSetAddressingU(m_texture, addressModeU);
        //RwTextureSetAddressingV(m_texture, addressModeU);
    }
}

void CSprite2d::SetRenderState()
{
    if (m_texture)
    {
        //RwRenderStateSet(rwRENDERSTATETEXTURERASTER, RwTextureGetRaster(m_texture));
    }
}

void CSprite2d::Draw(CRect& position, CRGBA& color)
{
    Hook::ThisCall(0x728350, this, &position, &color);
}

void CSprite2d::SetRecipNearClip()
{
    //RecipNearClip = 1.0f / Camera->nearPlane;
}

}
