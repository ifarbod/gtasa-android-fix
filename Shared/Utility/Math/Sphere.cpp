// Sphere in three-dimensional space
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include <Math/Frustum.hpp>
#include <Math/Polyhedron.hpp>

#include <DebugNew.hpp>

namespace ctn
{

void Sphere::Define(const Vector3* vertices, unsigned count)
{
    if (!count)
        return;

    Clear();
    Merge(vertices, count);
}

void Sphere::Define(const BoundingBox& box)
{
    const Vector3& min = box.min_;
    const Vector3& max = box.max_;

    Clear();
    Merge(min);
    Merge(Vector3(max.m_x, min.m_y, min.m_z));
    Merge(Vector3(min.m_x, max.m_y, min.m_z));
    Merge(Vector3(max.m_x, max.m_y, min.m_z));
    Merge(Vector3(min.m_x, min.m_y, max.m_z));
    Merge(Vector3(max.m_x, min.m_y, max.m_z));
    Merge(Vector3(min.m_x, max.m_y, max.m_z));
    Merge(max);
}

void Sphere::Define(const Frustum& frustum)
{
    Define(frustum.vertices_, NUM_FRUSTUM_VERTICES);
}

void Sphere::Define(const Polyhedron& poly)
{
    Clear();
    Merge(poly);
}

void Sphere::Merge(const Vector3* vertices, unsigned count)
{
    while (count--)
        Merge(*vertices++);
}

void Sphere::Merge(const BoundingBox& box)
{
    const Vector3& min = box.min_;
    const Vector3& max = box.max_;

    Merge(min);
    Merge(Vector3(max.m_x, min.m_y, min.m_z));
    Merge(Vector3(min.m_x, max.m_y, min.m_z));
    Merge(Vector3(max.m_x, max.m_y, min.m_z));
    Merge(Vector3(min.m_x, min.m_y, max.m_z));
    Merge(Vector3(max.m_x, min.m_y, max.m_z));
    Merge(Vector3(min.m_x, max.m_y, max.m_z));
    Merge(max);
}

void Sphere::Merge(const Frustum& frustum)
{
    const Vector3* vertices = frustum.vertices_;
    Merge(vertices, NUM_FRUSTUM_VERTICES);
}

void Sphere::Merge(const Polyhedron& poly)
{
    for (unsigned i = 0; i < poly.faces_.Size(); ++i)
    {
        const PODVector<Vector3>& face = poly.faces_[i];
        if (!face.Empty())
            Merge(&face[0], face.Size());
    }
}

void Sphere::Merge(const Sphere& sphere)
{
    if (radius_ < 0.0f)
    {
        center_ = sphere.center_;
        radius_ = sphere.radius_;
        return;
    }

    Vector3 offset = sphere.center_ - center_;
    float dist = offset.Length();

    // If sphere fits inside, do nothing
    if (dist + sphere.radius_ < radius_)
        return;

    // If we fit inside the other sphere, become it
    if (dist + radius_ < sphere.radius_)
    {
        center_ = sphere.center_;
        radius_ = sphere.radius_;
    }
    else
    {
        Vector3 NormalizedOffset = offset / dist;

        Vector3 min = center_ - radius_ * NormalizedOffset;
        Vector3 max = sphere.center_ + sphere.radius_ * NormalizedOffset;
        center_ = (min + max) * 0.5f;
        radius_ = (max - center_).Length();
    }
}

Intersection Sphere::IsInside(const BoundingBox& box) const
{
    float radiusSquared = radius_ * radius_;
    float distSquared = 0;
    float temp;
    Vector3 min = box.min_;
    Vector3 max = box.max_;

    if (center_.m_x < min.m_x)
    {
        temp = center_.m_x - min.m_x;
        distSquared += temp * temp;
    }
    else if (center_.m_x > max.m_x)
    {
        temp = center_.m_x - max.m_x;
        distSquared += temp * temp;
    }
    if (center_.m_y < min.m_y)
    {
        temp = center_.m_y - min.m_y;
        distSquared += temp * temp;
    }
    else if (center_.m_y > max.m_y)
    {
        temp = center_.m_y - max.m_y;
        distSquared += temp * temp;
    }
    if (center_.m_z < min.m_z)
    {
        temp = center_.m_z - min.m_z;
        distSquared += temp * temp;
    }
    else if (center_.m_z > max.m_z)
    {
        temp = center_.m_z - max.m_z;
        distSquared += temp * temp;
    }

    if (distSquared >= radiusSquared)
        return OUTSIDE;

    min -= center_;
    max -= center_;

    Vector3 tempVec = min; // - - -
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec.m_x = max.m_x; // + - -
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec.m_y = max.m_y; // + + -
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec.m_x = min.m_x; // - + -
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec.m_z = max.m_z; // - + +
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec.m_y = min.m_y; // - - +
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec.m_x = max.m_x; // + - +
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec.m_y = max.m_y; // + + +
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;

    return INSIDE;
}

Intersection Sphere::IsInsideFast(const BoundingBox& box) const
{
    float radiusSquared = radius_ * radius_;
    float distSquared = 0;
    float temp;
    Vector3 min = box.min_;
    Vector3 max = box.max_;

    if (center_.m_x < min.m_x)
    {
        temp = center_.m_x - min.m_x;
        distSquared += temp * temp;
    }
    else if (center_.m_x > max.m_x)
    {
        temp = center_.m_x - max.m_x;
        distSquared += temp * temp;
    }
    if (center_.m_y < min.m_y)
    {
        temp = center_.m_y - min.m_y;
        distSquared += temp * temp;
    }
    else if (center_.m_y > max.m_y)
    {
        temp = center_.m_y - max.m_y;
        distSquared += temp * temp;
    }
    if (center_.m_z < min.m_z)
    {
        temp = center_.m_z - min.m_z;
        distSquared += temp * temp;
    }
    else if (center_.m_z > max.m_z)
    {
        temp = center_.m_z - max.m_z;
        distSquared += temp * temp;
    }

    if (distSquared >= radiusSquared)
        return OUTSIDE;
    else
        return INSIDE;
}

}
