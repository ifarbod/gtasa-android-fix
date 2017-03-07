// Two-dimensional bounding rectangle
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Math/Vector4.hpp>

namespace ctn
{

// Two-dimensional bounding rectangle.
class Rect
{
public:
    // Construct an undefined rect.
    Rect() :
        min_(M_INFINITY, M_INFINITY),
        max_(-M_INFINITY, -M_INFINITY)
    {
    }

    // Construct from minimum and maximum vectors.
    Rect(const Vector2& min, const Vector2& max) :
        min_(min),
        max_(max)
    {
    }

    // Construct from coordinates.
    Rect(float left, float top, float right, float bottom) :
        min_(left, top),
        max_(right, bottom)
    {
    }

    // Construct from a Vector4.
    Rect(const Vector4& vector) :
        min_(vector.m_x, vector.m_y),
        max_(vector.m_z, vector.m_w)
    {
    }

    // Construct from a float array.
    explicit Rect(const float* data) :
        min_(data[0], data[1]),
        max_(data[2], data[3])
    {
    }

    // Copy-construct from another rect.
    Rect(const Rect& rect) :
        min_(rect.min_),
        max_(rect.max_)
    {
    }

    // Assign from another rect.
    Rect& operator =(const Rect& rhs)
    {
        min_ = rhs.min_;
        max_ = rhs.max_;
        return *this;
    }

    // Test for equality with another rect.
    bool operator ==(const Rect& rhs) const { return min_ == rhs.min_ && max_ == rhs.max_; }

    // Test for inequality with another rect.
    bool operator !=(const Rect& rhs) const { return min_ != rhs.min_ || max_ != rhs.max_; }

    // Define from another rect.
    void Define(const Rect& rect)
    {
        min_ = rect.min_;
        max_ = rect.max_;
    }

    // Define from minimum and maximum vectors.
    void Define(const Vector2& min, const Vector2& max)
    {
        min_ = min;
        max_ = max;
    }

    // Define from a point.
    void Define(const Vector2& point)
    {
        min_ = max_ = point;
    }

    // Merge a point.
    void Merge(const Vector2& point)
    {
        if (point.m_x < min_.m_x)
            min_.m_x = point.m_x;
        if (point.m_x > max_.m_x)
            max_.m_x = point.m_x;
        if (point.m_y < min_.m_y)
            min_.m_y = point.m_y;
        if (point.m_y > max_.m_y)
            max_.m_y = point.m_y;
    }

    // Merge a rect.
    void Merge(const Rect& rect)
    {
        if (rect.min_.m_x < min_.m_x)
            min_.m_x = rect.min_.m_x;
        if (rect.min_.m_y < min_.m_y)
            min_.m_y = rect.min_.m_y;
        if (rect.max_.m_x > max_.m_x)
            max_.m_x = rect.max_.m_x;
        if (rect.max_.m_y > max_.m_y)
            max_.m_y = rect.max_.m_y;
    }

    // Clear to undefined state.
    void Clear()
    {
        min_ = Vector2(M_INFINITY, M_INFINITY);
        max_ = Vector2(-M_INFINITY, -M_INFINITY);
    }

    // Clip with another rect.
    void Clip(const Rect& rect);

    // Return true if this rect is defined via a previous call to Define() or Merge().
    bool Defined() const
    {
        return min_.m_x != M_INFINITY;
    }

    // Return center.
    Vector2 Center() const { return (max_ + min_) * 0.5f; }

    // Return size.
    Vector2 Size() const { return max_ - min_; }

    // Return half-size.
    Vector2 HalfSize() const { return (max_ - min_) * 0.5f; }

    // Test for equality with another rect with epsilon.
    bool Equals(const Rect& rhs) const { return min_.Equals(rhs.min_) && max_.Equals(rhs.max_); }

    // Test whether a point is inside.
    Intersection IsInside(const Vector2& point) const
    {
        if (point.m_x < min_.m_x || point.m_y < min_.m_y || point.m_x > max_.m_x || point.m_y > max_.m_y)
            return OUTSIDE;
        else
            return INSIDE;
    }

    // Test if another rect is inside, outside or intersects.
    Intersection IsInside(const Rect& rect) const
    {
        if (rect.max_.m_x < min_.m_x || rect.min_.m_x > max_.m_x || rect.max_.m_y < min_.m_y || rect.min_.m_y > max_.m_y)
            return OUTSIDE;
        else if (rect.min_.m_x < min_.m_x || rect.max_.m_x > max_.m_x || rect.min_.m_y < min_.m_y || rect.max_.m_y > max_.m_y)
            return INTERSECTS;
        else
            return INSIDE;
    }

    // Return float data.
    const float* Data() const { return &min_.m_x; }

    // Return as a vector.
    Vector4 ToVector4() const { return Vector4(min_.m_x, min_.m_y, max_.m_x, max_.m_y); }

    // Return as string.
    String ToString() const;

    // Minimum vector.
    Vector2 min_;
    // Maximum vector.
    Vector2 max_;

    // Rect in the range (-1, -1) - (1, 1)
    static const Rect FULL;
    // Rect in the range (0, 0) - (1, 1)
    static const Rect POSITIVE;
    // Zero-sized rect.
    static const Rect ZERO;
};

// Two-dimensional bounding rectangle with integer values.
class IntRect
{
public:
    // Construct a zero rect.
    IntRect() :
        left_(0),
        top_(0),
        right_(0),
        bottom_(0)
    {
    }

    // Construct from coordinates.
    IntRect(int left, int top, int right, int bottom) :
        left_(left),
        top_(top),
        right_(right),
        bottom_(bottom)
    {
    }

    // Construct from an int array.
    IntRect(const int* data) :
        left_(data[0]),
        top_(data[1]),
        right_(data[2]),
        bottom_(data[3])
    {
    }

    // Test for equality with another rect.
    bool operator ==(const IntRect& rhs) const
    {
        return left_ == rhs.left_ && top_ == rhs.top_ && right_ == rhs.right_ && bottom_ == rhs.bottom_;
    }

    // Test for inequality with another rect.
    bool operator !=(const IntRect& rhs) const
    {
        return left_ != rhs.left_ || top_ != rhs.top_ || right_ != rhs.right_ || bottom_ != rhs.bottom_;
    }

    // Return size.
    IntVector2 Size() const { return IntVector2(Width(), Height()); }

    // Return width.
    int Width() const { return right_ - left_; }

    // Return height.
    int Height() const { return bottom_ - top_; }

    // Test whether a point is inside.
    Intersection IsInside(const IntVector2& point) const
    {
        if (point.m_x < left_ || point.m_y < top_ || point.m_x >= right_ || point.m_y >= bottom_)
            return OUTSIDE;
        else
            return INSIDE;
    }

    // Return integer data.
    const int* Data() const { return &left_; }

    // Return as string.
    String ToString() const;

    // Left coordinate.
    int left_;
    // Top coordinate.
    int top_;
    // Right coordinate.
    int right_;
    // Bottom coordinate.
    int bottom_;

    // Zero-sized rect.
    static const IntRect ZERO;
};

}
