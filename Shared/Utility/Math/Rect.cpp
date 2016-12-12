// Two-dimensional bounding rectangle
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include <Math/Rect.hpp>

namespace Util
{

const Rect Rect::FULL(-1.0f, -1.0f, 1.0f, 1.0f);
const Rect Rect::POSITIVE(0.0f, 0.0f, 1.0f, 1.0f);
const Rect Rect::ZERO(0.0f, 0.0f, 0.0f, 0.0f);

const IntRect IntRect::ZERO(0, 0, 0, 0);

String Rect::ToString() const
{
    return String::Format("%g %g %g %g", min_.x_, min_.y_, max_.x_, max_.y_);
}

String IntRect::ToString() const
{
    return String::Format("%d %d %d %d", left_, top_, right_, bottom_);
}

void Rect::Clip(const Rect& rect)
{
    if (rect.min_.x_ > min_.x_)
        min_.x_ = rect.min_.x_;
    if (rect.max_.x_ < max_.x_)
        max_.x_ = rect.max_.x_;
    if (rect.min_.y_ > min_.y_)
        min_.y_ = rect.min_.y_;
    if (rect.max_.y_ < max_.y_)
        max_.y_ = rect.max_.y_;

    if (min_.x_ > max_.x_ || min_.y_ > max_.y_)
    {
        min_ = Vector2(M_INFINITY, M_INFINITY);
        max_ = Vector2(-M_INFINITY, -M_INFINITY);
    }
}

}
