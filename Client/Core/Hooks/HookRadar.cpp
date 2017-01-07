// Radar hooks
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/HookingUtils.hpp>
#include <Hooking/HookFunction.hpp>
#include <Math/Vector2.hpp>

using namespace Util;

f32 SquareRadar(Vector2& pos)
{
    f32 result = pos.Length();

    if (result > Sqrt(2.f) / 2.0)
    {
        pos.x_ /= result * 1 / Sqrt(2.f);
        pos.y_ /= result * 1 / Sqrt(2.f);
    }
    if (pos.x_ > 1.0f)
        pos.x_ = 1.0f;
    else if (-1.0f > pos.x_)
        pos.x_ = -1.0f;
    if (pos.y_ > 1.0f)
        pos.y_ = 1.0f;
    else if (-1.0f > pos.y_)
        pos.y_ = -1.0f;

    return result;
}


static HookFunction hookFunction([]()
{
    // Experimental square radar
#ifdef CTN_DEBUG
    MakeJMP(0x5832F0, SquareRadar);
    MemPatch<f32>(0x585719, 0.0f); // -1.0
    MemPatch<f32>(0x585721, 0.0f); // 1.0
    MemPatch<f32>(0x585729, 0.0f); // 1.0
    MemPatch<f32>(0x585731, 0.0f); // 1.0
    MemPatch<f32>(0x585739, 0.0f); // 1.0
    MemPatch<f32>(0x585741, 0.0f); // -1.0
    MemPatch<f32>(0x585749, 0.0f); // -1.0
    MemPatch<f32>(0x585751, 0.0f); // -1.0
#endif

    // Fix radar disk colors
    MemPatch<u8>(0x58A9A2, 0xFF);
    MemPatch<u8>(0x58A99A, 0xFF);
    MemPatch<u8>(0x58A996, 0xFF);
    MemPatch<u8>(0x58A8EE, 0xFF);
    MemPatch<u8>(0x58A8E6, 0xFF);
    MemPatch<u8>(0x58A8DE, 0xFF);
    MemPatch<u8>(0x58A89A, 0xFF);
    MemPatch<u8>(0x58A896, 0xFF);
    MemPatch<u8>(0x58A894, 0xFF);
    MemPatch<u8>(0x58A798, 0xFF);
    MemPatch<u8>(0x58A790, 0xFF);
    MemPatch<u8>(0x58A78E, 0xFF);
});
