// Three-dimensional axis-aligned bounding box
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include <Math/Frustum.hpp>
#include <Math/Polyhedron.hpp>

#include <DebugNew.hpp>

namespace ctn
{

void BoundingBox::Define(const Vector3* vertices, unsigned count)
{
    Clear();

    if (!count)
        return;

    Merge(vertices, count);
}

void BoundingBox::Define(const Frustum& frustum)
{
    Clear();
    Define(frustum.vertices_, NUM_FRUSTUM_VERTICES);
}

void BoundingBox::Define(const Polyhedron& poly)
{
    Clear();
    Merge(poly);
}

void BoundingBox::Define(const Sphere& sphere)
{
    const Vector3& center = sphere.center_;
    float radius = sphere.radius_;

    min_ = center + Vector3(-radius, -radius, -radius);
    max_ = center + Vector3(radius, radius, radius);
}

void BoundingBox::Merge(const Vector3* vertices, unsigned count)
{
    while (count--)
        Merge(*vertices++);
}

void BoundingBox::Merge(const Frustum& frustum)
{
    Merge(frustum.vertices_, NUM_FRUSTUM_VERTICES);
}

void BoundingBox::Merge(const Polyhedron& poly)
{
    for (unsigned i = 0; i < poly.faces_.Size(); ++i)
    {
        const PODVector<Vector3>& face = poly.faces_[i];
        if (!face.Empty())
            Merge(&face[0], face.Size());
    }
}

void BoundingBox::Merge(const Sphere& sphere)
{
    const Vector3& center = sphere.center_;
    float radius = sphere.radius_;

    Merge(center + Vector3(radius, radius, radius));
    Merge(center + Vector3(-radius, -radius, -radius));
}

void BoundingBox::Clip(const BoundingBox& box)
{
    if (box.min_.m_x > min_.m_x)
        min_.m_x = box.min_.m_x;
    if (box.max_.m_x < max_.m_x)
        max_.m_x = box.max_.m_x;
    if (box.min_.m_y > min_.m_y)
        min_.m_y = box.min_.m_y;
    if (box.max_.m_y < max_.m_y)
        max_.m_y = box.max_.m_y;
    if (box.min_.m_z > min_.m_z)
        min_.m_z = box.min_.m_z;
    if (box.max_.m_z < max_.m_z)
        max_.m_z = box.max_.m_z;

    if (min_.m_x > max_.m_x || min_.m_y > max_.m_y || min_.m_z > max_.m_z)
    {
        min_ = Vector3(M_INFINITY, M_INFINITY, M_INFINITY);
        max_ = Vector3(-M_INFINITY, -M_INFINITY, -M_INFINITY);
    }
}

void BoundingBox::Transform(const Matrix3& transform)
{
    *this = Transformed(Matrix3x4(transform));
}

void BoundingBox::Transform(const Matrix3x4& transform)
{
    *this = Transformed(transform);
}

BoundingBox BoundingBox::Transformed(const Matrix3& transform) const
{
    return Transformed(Matrix3x4(transform));
}

BoundingBox BoundingBox::Transformed(const Matrix3x4& transform) const
{
#ifdef CTN_SSE
    const __m128 one = _mm_set_ss(1.f);
    __m128 minPt = _mm_movelh_ps(_mm_loadl_pi(_mm_setzero_ps(), (const __m64*)&min_.m_x), _mm_unpacklo_ps(_mm_set_ss(min_.m_z), one));
    __m128 maxPt = _mm_movelh_ps(_mm_loadl_pi(_mm_setzero_ps(), (const __m64*)&max_.m_x), _mm_unpacklo_ps(_mm_set_ss(max_.m_z), one));
    __m128 centerPoint = _mm_mul_ps(_mm_add_ps(minPt, maxPt), _mm_set1_ps(0.5f));
    __m128 halfSize = _mm_sub_ps(centerPoint, minPt);
    __m128 m0 = _mm_loadu_ps(&transform.m00_);
    __m128 m1 = _mm_loadu_ps(&transform.m10_);
    __m128 m2 = _mm_loadu_ps(&transform.m20_);
    __m128 r0 = _mm_mul_ps(m0, centerPoint);
    __m128 r1 = _mm_mul_ps(m1, centerPoint);
    __m128 t0 = _mm_add_ps(_mm_unpacklo_ps(r0, r1), _mm_unpackhi_ps(r0, r1));
    __m128 r2 = _mm_mul_ps(m2, centerPoint);
    const __m128 zero = _mm_setzero_ps();
    __m128 t2 = _mm_add_ps(_mm_unpacklo_ps(r2, zero), _mm_unpackhi_ps(r2, zero));
    __m128 newCenter = _mm_add_ps(_mm_movelh_ps(t0, t2), _mm_movehl_ps(t2, t0));
    const __m128 absMask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));
    __m128 x = _mm_and_ps(absMask, _mm_mul_ps(m0, halfSize));
    __m128 y = _mm_and_ps(absMask, _mm_mul_ps(m1, halfSize));
    __m128 z = _mm_and_ps(absMask, _mm_mul_ps(m2, halfSize));
    t0 = _mm_add_ps(_mm_unpacklo_ps(x, y), _mm_unpackhi_ps(x, y));
    t2 = _mm_add_ps(_mm_unpacklo_ps(z, zero), _mm_unpackhi_ps(z, zero));
    __m128 newDir = _mm_add_ps(_mm_movelh_ps(t0, t2), _mm_movehl_ps(t2, t0));
    return BoundingBox(_mm_sub_ps(newCenter, newDir), _mm_add_ps(newCenter, newDir));
#else
    Vector3 newCenter = transform * Center();
    Vector3 oldEdge = Size() * 0.5f;
    Vector3 newEdge = Vector3(
        Abs(transform.m00_) * oldEdge.m_x + Abs(transform.m01_) * oldEdge.m_y + Abs(transform.m02_) * oldEdge.m_z,
        Abs(transform.m10_) * oldEdge.m_x + Abs(transform.m11_) * oldEdge.m_y + Abs(transform.m12_) * oldEdge.m_z,
        Abs(transform.m20_) * oldEdge.m_x + Abs(transform.m21_) * oldEdge.m_y + Abs(transform.m22_) * oldEdge.m_z
    );

    return BoundingBox(newCenter - newEdge, newCenter + newEdge);
#endif
}

Rect BoundingBox::Projected(const Matrix4& projection) const
{
    Vector3 projMin = min_;
    Vector3 projMax = max_;
    if (projMin.m_z < M_MIN_NEARCLIP)
        projMin.m_z = M_MIN_NEARCLIP;
    if (projMax.m_z < M_MIN_NEARCLIP)
        projMax.m_z = M_MIN_NEARCLIP;

    Vector3 vertices[8];
    vertices[0] = projMin;
    vertices[1] = Vector3(projMax.m_x, projMin.m_y, projMin.m_z);
    vertices[2] = Vector3(projMin.m_x, projMax.m_y, projMin.m_z);
    vertices[3] = Vector3(projMax.m_x, projMax.m_y, projMin.m_z);
    vertices[4] = Vector3(projMin.m_x, projMin.m_y, projMax.m_z);
    vertices[5] = Vector3(projMax.m_x, projMin.m_y, projMax.m_z);
    vertices[6] = Vector3(projMin.m_x, projMax.m_y, projMax.m_z);
    vertices[7] = projMax;

    Rect rect;
    for (unsigned i = 0; i < 8; ++i)
    {
        Vector3 projected = projection * vertices[i];
        rect.Merge(Vector2(projected.m_x, projected.m_y));
    }

    return rect;
}

Intersection BoundingBox::IsInside(const Sphere& sphere) const
{
    float distSquared = 0;
    float temp;
    const Vector3& center = sphere.center_;

    if (center.m_x < min_.m_x)
    {
        temp = center.m_x - min_.m_x;
        distSquared += temp * temp;
    }
    else if (center.m_x > max_.m_x)
    {
        temp = center.m_x - max_.m_x;
        distSquared += temp * temp;
    }
    if (center.m_y < min_.m_y)
    {
        temp = center.m_y - min_.m_y;
        distSquared += temp * temp;
    }
    else if (center.m_y > max_.m_y)
    {
        temp = center.m_y - max_.m_y;
        distSquared += temp * temp;
    }
    if (center.m_z < min_.m_z)
    {
        temp = center.m_z - min_.m_z;
        distSquared += temp * temp;
    }
    else if (center.m_z > max_.m_z)
    {
        temp = center.m_z - max_.m_z;
        distSquared += temp * temp;
    }

    float radius = sphere.radius_;
    if (distSquared >= radius * radius)
        return OUTSIDE;
    else if (center.m_x - radius < min_.m_x || center.m_x + radius > max_.m_x || center.m_y - radius < min_.m_y ||
        center.m_y + radius > max_.m_y || center.m_z - radius < min_.m_z || center.m_z + radius > max_.m_z)
        return INTERSECTS;
    else
        return INSIDE;
}

Intersection BoundingBox::IsInsideFast(const Sphere& sphere) const
{
    float distSquared = 0;
    float temp;
    const Vector3& center = sphere.center_;

    if (center.m_x < min_.m_x)
    {
        temp = center.m_x - min_.m_x;
        distSquared += temp * temp;
    }
    else if (center.m_x > max_.m_x)
    {
        temp = center.m_x - max_.m_x;
        distSquared += temp * temp;
    }
    if (center.m_y < min_.m_y)
    {
        temp = center.m_y - min_.m_y;
        distSquared += temp * temp;
    }
    else if (center.m_y > max_.m_y)
    {
        temp = center.m_y - max_.m_y;
        distSquared += temp * temp;
    }
    if (center.m_z < min_.m_z)
    {
        temp = center.m_z - min_.m_z;
        distSquared += temp * temp;
    }
    else if (center.m_z > max_.m_z)
    {
        temp = center.m_z - max_.m_z;
        distSquared += temp * temp;
    }

    float radius = sphere.radius_;
    if (distSquared >= radius * radius)
        return OUTSIDE;
    else
        return INSIDE;
}

String BoundingBox::ToString() const
{
    return min_.ToString() + " - " + max_.ToString();
}

}
