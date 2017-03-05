// Two-dimensional bounding rectangle
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include <Math/Rect.hpp>

namespace ctn
{

const Rect Rect::FULL(-1.0f, -1.0f, 1.0f, 1.0f);
const Rect Rect::POSITIVE(0.0f, 0.0f, 1.0f, 1.0f);
const Rect Rect::ZERO(0.0f, 0.0f, 0.0f, 0.0f);

const IntRect IntRect::ZERO(0, 0, 0, 0);

String Rect::ToString() const
{
    return String::Format("%g %g %g %g", min_.m_x, min_.m_y, max_.m_x, max_.m_y);
}

String IntRect::ToString() const
{
    return String::Format("%d %d %d %d", left_, top_, right_, bottom_);
}

void Rect::Clip(const Rect& rect)
{
    if (rect.min_.m_x > min_.m_x)
        min_.m_x = rect.min_.m_x;
    if (rect.max_.m_x < max_.m_x)
        max_.m_x = rect.max_.m_x;
    if (rect.min_.m_y > min_.m_y)
        min_.m_y = rect.min_.m_y;
    if (rect.max_.m_y < max_.m_y)
        max_.m_y = rect.max_.m_y;

    if (min_.m_x > max_.m_x || min_.m_y > max_.m_y)
    {
        min_ = Vector2(M_INFINITY, M_INFINITY);
        max_ = Vector2(-M_INFINITY, -M_INFINITY);
    }
}

}
