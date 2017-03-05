// Three-dimensional axis-aligned bounding box
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Math/Rect.hpp>
#include <Math/Vector3.hpp>

#ifdef CTN_SSE
#include <xmmintrin.h>
#endif

namespace ctn
{

class Polyhedron;
class Frustum;
class Matrix3;
class Matrix4;
class Matrix3x4;
class Sphere;

// Three-dimensional axis-aligned bounding box.
class BoundingBox
{
public:
    // Construct with zero size.
    BoundingBox() :
        min_(M_INFINITY, M_INFINITY, M_INFINITY),
        max_(-M_INFINITY, -M_INFINITY, -M_INFINITY)
    {
    }

    // Copy-construct from another bounding box.
    BoundingBox(const BoundingBox& box) :
        min_(box.min_),
        max_(box.max_)
    {
    }

    // Construct from a rect, with the Z dimension left zero.
    BoundingBox(const Rect& rect) :
        min_(Vector3(rect.min_, 0.0f)),
        max_(Vector3(rect.max_, 0.0f))
    {
    }

    // Construct from minimum and maximum vectors.
    BoundingBox(const Vector3& min, const Vector3& max) :
        min_(min),
        max_(max)
    {
    }

    // Construct from minimum and maximum floats (all dimensions same.)
    BoundingBox(float min, float max) :
        min_(Vector3(min, min, min)),
        max_(Vector3(max, max, max))
    {
    }

#ifdef CTN_SSE
    BoundingBox(__m128 min, __m128 max)
    {
        _mm_storeu_ps(&min_.m_x, min);
        _mm_storeu_ps(&max_.m_x, max);
    }
#endif

    // Construct from an array of vertices.
    BoundingBox(const Vector3* vertices, unsigned count) :
        min_(M_INFINITY, M_INFINITY, M_INFINITY),
        max_(-M_INFINITY, -M_INFINITY, -M_INFINITY)
    {
        Define(vertices, count);
    }

    // Construct from a frustum.
    BoundingBox(const Frustum& frustum) :
        min_(M_INFINITY, M_INFINITY, M_INFINITY),
        max_(-M_INFINITY, -M_INFINITY, -M_INFINITY)
    {
        Define(frustum);
    }

    // Construct from a polyhedron.
    BoundingBox(const Polyhedron& poly) :
        min_(M_INFINITY, M_INFINITY, M_INFINITY),
        max_(-M_INFINITY, -M_INFINITY, -M_INFINITY)
    {
        Define(poly);
    }

    // Construct from a sphere.
    BoundingBox(const Sphere& sphere) :
        min_(M_INFINITY, M_INFINITY, M_INFINITY),
        max_(-M_INFINITY, -M_INFINITY, -M_INFINITY)
    {
        Define(sphere);
    }

    // Assign from another bounding box.
    BoundingBox& operator =(const BoundingBox& rhs)
    {
        min_ = rhs.min_;
        max_ = rhs.max_;
        return *this;
    }

    // Assign from a Rect, with the Z dimension left zero.
    BoundingBox& operator =(const Rect& rhs)
    {
        min_ = Vector3(rhs.min_, 0.0f);
        max_ = Vector3(rhs.max_, 0.0f);
        return *this;
    }

    // Test for equality with another bounding box.
    bool operator ==(const BoundingBox& rhs) const { return (min_ == rhs.min_ && max_ == rhs.max_); }

    // Test for inequality with another bounding box.
    bool operator !=(const BoundingBox& rhs) const { return (min_ != rhs.min_ || max_ != rhs.max_); }

    // Define from another bounding box.
    void Define(const BoundingBox& box)
    {
        Define(box.min_, box.max_);
    }

    // Define from a Rect.
    void Define(const Rect& rect)
    {
        Define(Vector3(rect.min_, 0.0f), Vector3(rect.max_, 0.0f));
    }

    // Define from minimum and maximum vectors.
    void Define(const Vector3& min, const Vector3& max)
    {
        min_ = min;
        max_ = max;
    }

    // Define from minimum and maximum floats (all dimensions same.)
    void Define(float min, float max)
    {
        min_ = Vector3(min, min, min);
        max_ = Vector3(max, max, max);
    }

    // Define from a point.
    void Define(const Vector3& point)
    {
        min_ = max_ = point;
    }

    // Merge a point.
    void Merge(const Vector3& point)
    {
#ifdef CTN_SSE
        __m128 vec = _mm_set_ps(1.f, point.m_z, point.m_y, point.m_x);
        _mm_storeu_ps(&min_.m_x, _mm_min_ps(_mm_loadu_ps(&min_.m_x), vec));
        _mm_storeu_ps(&max_.m_x, _mm_max_ps(_mm_loadu_ps(&max_.m_x), vec));
#else
        if (point.m_x < min_.m_x)
            min_.m_x = point.m_x;
        if (point.m_y < min_.m_y)
            min_.m_y = point.m_y;
        if (point.m_z < min_.m_z)
            min_.m_z = point.m_z;
        if (point.m_x > max_.m_x)
            max_.m_x = point.m_x;
        if (point.m_y > max_.m_y)
            max_.m_y = point.m_y;
        if (point.m_z > max_.m_z)
            max_.m_z = point.m_z;
#endif
    }

    // Merge another bounding box.
    void Merge(const BoundingBox& box)
    {
#ifdef CTN_SSE
        _mm_storeu_ps(&min_.m_x, _mm_min_ps(_mm_loadu_ps(&min_.m_x), _mm_loadu_ps(&box.min_.m_x)));
        _mm_storeu_ps(&max_.m_x, _mm_max_ps(_mm_loadu_ps(&max_.m_x), _mm_loadu_ps(&box.max_.m_x)));
#else
        if (box.min_.m_x < min_.m_x)
            min_.m_x = box.min_.m_x;
        if (box.min_.m_y < min_.m_y)
            min_.m_y = box.min_.m_y;
        if (box.min_.m_z < min_.m_z)
            min_.m_z = box.min_.m_z;
        if (box.max_.m_x > max_.m_x)
            max_.m_x = box.max_.m_x;
        if (box.max_.m_y > max_.m_y)
            max_.m_y = box.max_.m_y;
        if (box.max_.m_z > max_.m_z)
            max_.m_z = box.max_.m_z;
#endif
    }

    // Define from an array of vertices.
    void Define(const Vector3* vertices, unsigned count);
    // Define from a frustum.
    void Define(const Frustum& frustum);
    // Define from a polyhedron.
    void Define(const Polyhedron& poly);
    // Define from a sphere.
    void Define(const Sphere& sphere);
    // Merge an array of vertices.
    void Merge(const Vector3* vertices, unsigned count);
    // Merge a frustum.
    void Merge(const Frustum& frustum);
    // Merge a polyhedron.
    void Merge(const Polyhedron& poly);
    // Merge a sphere.
    void Merge(const Sphere& sphere);
    // Clip with another bounding box. The box can become degenerate (undefined) as a result.
    void Clip(const BoundingBox& box);
    // Transform with a 3x3 matrix.
    void Transform(const Matrix3& transform);
    // Transform with a 3x4 matrix.
    void Transform(const Matrix3x4& transform);

    // Clear to undefined state.
    void Clear()
    {
#ifdef CTN_SSE
        _mm_storeu_ps(&min_.m_x, _mm_set1_ps(M_INFINITY));
        _mm_storeu_ps(&max_.m_x, _mm_set1_ps(-M_INFINITY));
#else
        min_ = Vector3(M_INFINITY, M_INFINITY, M_INFINITY);
        max_ = Vector3(-M_INFINITY, -M_INFINITY, -M_INFINITY);
#endif
    }

    // Return true if this bounding box is defined via a previous call to Define() or Merge().
    bool Defined() const
    {
        return min_.m_x != M_INFINITY;
    }

    // Return center.
    Vector3 Center() const { return (max_ + min_) * 0.5f; }

    // Return size.
    Vector3 Size() const { return max_ - min_; }

    // Return half-size.
    Vector3 HalfSize() const { return (max_ - min_) * 0.5f; }

    // Return transformed by a 3x3 matrix.
    BoundingBox Transformed(const Matrix3& transform) const;
    // Return transformed by a 3x4 matrix.
    BoundingBox Transformed(const Matrix3x4& transform) const;
    // Return projected by a 4x4 projection matrix.
    Rect Projected(const Matrix4& projection) const;

    // Test if a point is inside.
    Intersection IsInside(const Vector3& point) const
    {
        if (point.m_x < min_.m_x || point.m_x > max_.m_x || point.m_y < min_.m_y || point.m_y > max_.m_y ||
            point.m_z < min_.m_z || point.m_z > max_.m_z)
            return OUTSIDE;
        else
            return INSIDE;
    }

    // Test if another bounding box is inside, outside or intersects.
    Intersection IsInside(const BoundingBox& box) const
    {
        if (box.max_.m_x < min_.m_x || box.min_.m_x > max_.m_x || box.max_.m_y < min_.m_y || box.min_.m_y > max_.m_y ||
            box.max_.m_z < min_.m_z || box.min_.m_z > max_.m_z)
            return OUTSIDE;
        else if (box.min_.m_x < min_.m_x || box.max_.m_x > max_.m_x || box.min_.m_y < min_.m_y || box.max_.m_y > max_.m_y ||
            box.min_.m_z < min_.m_z || box.max_.m_z > max_.m_z)
            return INTERSECTS;
        else
            return INSIDE;
    }

    // Test if another bounding box is (partially) inside or outside.
    Intersection IsInsideFast(const BoundingBox& box) const
    {
        if (box.max_.m_x < min_.m_x || box.min_.m_x > max_.m_x || box.max_.m_y < min_.m_y || box.min_.m_y > max_.m_y ||
            box.max_.m_z < min_.m_z || box.min_.m_z > max_.m_z)
            return OUTSIDE;
        else
            return INSIDE;
    }

    // Test if a sphere is inside, outside or intersects.
    Intersection IsInside(const Sphere& sphere) const;
    // Test if a sphere is (partially) inside or outside.
    Intersection IsInsideFast(const Sphere& sphere) const;

    // Return as string.
    String ToString() const;

    // Minimum vector.
    Vector3 min_;
    float dummyMin_; // This is never used, but exists to pad the min_ value to four floats.
    // Maximum vector.
    Vector3 max_;
    float dummyMax_; // This is never used, but exists to pad the max_ value to four floats.

    float data_[6];

    BoundingBox(float* data) :
        min_(Vector3(data[0], data[1], data[2])),
        max_(Vector3(data[3], data[4], data[5]))
    {
    }

    float* Data() const
    {
        float* data = const_cast<BoundingBox*>(this)->data_;

        data[0] = min_.m_x;
        data[1] = min_.m_y;
        data[2] = min_.m_z;
        data[3] = max_.m_x;
        data[4] = max_.m_y;
        data[5] = max_.m_z;
        return data;
    }
};

}
