// Convex constructed of 6 planes
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include <Math/Frustum.hpp>

#include <DebugNew.hpp>

namespace ctn
{

inline Vector3 ClipEdgeZ(const Vector3& v0, const Vector3& v1, float clipZ)
{
    return Vector3(
        v1.m_x + (v0.m_x - v1.m_x) * ((clipZ - v1.m_z) / (v0.m_z - v1.m_z)),
        v1.m_y + (v0.m_y - v1.m_y) * ((clipZ - v1.m_z) / (v0.m_z - v1.m_z)),
        clipZ
    );
}

void ProjectAndMergeEdge(Vector3 v0, Vector3 v1, Rect& rect, const Matrix4& projection)
{
    // Check if both vertices behind near plane
    if (v0.m_z < M_MIN_NEARCLIP && v1.m_z < M_MIN_NEARCLIP)
        return;

    // Check if need to clip one of the vertices
    if (v1.m_z < M_MIN_NEARCLIP)
        v1 = ClipEdgeZ(v1, v0, M_MIN_NEARCLIP);
    else if (v0.m_z < M_MIN_NEARCLIP)
        v0 = ClipEdgeZ(v0, v1, M_MIN_NEARCLIP);

    // Project, perspective divide and merge
    Vector3 tV0(projection * v0);
    Vector3 tV1(projection * v1);
    rect.Merge(Vector2(tV0.m_x, tV0.m_y));
    rect.Merge(Vector2(tV1.m_x, tV1.m_y));
}

Frustum::Frustum(const Frustum& frustum)
{
    *this = frustum;
}

Frustum& Frustum::operator =(const Frustum& rhs)
{
    for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
        planes_[i] = rhs.planes_[i];
    for (unsigned i = 0; i < NUM_FRUSTUM_VERTICES; ++i)
        vertices_[i] = rhs.vertices_[i];

    return *this;
}

void Frustum::Define(float fov, float aspectRatio, float zoom, float nearZ, float farZ, const Matrix3x4& transform)
{
    nearZ = Max(nearZ, 0.0f);
    farZ = Max(farZ, nearZ);
    float halfViewSize = tanf(fov * M_DEGTORAD_2) / zoom;
    Vector3 _near, _far;

    _near.m_z = nearZ;
    _near.m_y = _near.m_z * halfViewSize;
    _near.m_x = _near.m_y * aspectRatio;
    _far.m_z = farZ;
    _far.m_y = _far.m_z * halfViewSize;
    _far.m_x = _far.m_y * aspectRatio;

    Define(_near, _far, transform);
}

void Frustum::Define(const Vector3& _near, const Vector3& _far, const Matrix3x4& transform)
{
    vertices_[0] = transform * _near;
    vertices_[1] = transform * Vector3(_near.m_x, -_near.m_y, _near.m_z);
    vertices_[2] = transform * Vector3(-_near.m_x, -_near.m_y, _near.m_z);
    vertices_[3] = transform * Vector3(-_near.m_x, _near.m_y, _near.m_z);
    vertices_[4] = transform * _far;
    vertices_[5] = transform * Vector3(_far.m_x, -_far.m_y, _far.m_z);
    vertices_[6] = transform * Vector3(-_far.m_x, -_far.m_y, _far.m_z);
    vertices_[7] = transform * Vector3(-_far.m_x, _far.m_y, _far.m_z);

    UpdatePlanes();
}

void Frustum::Define(const BoundingBox& box, const Matrix3x4& transform)
{
    vertices_[0] = transform * Vector3(box.max_.m_x, box.max_.m_y, box.min_.m_z);
    vertices_[1] = transform * Vector3(box.max_.m_x, box.min_.m_y, box.min_.m_z);
    vertices_[2] = transform * Vector3(box.min_.m_x, box.min_.m_y, box.min_.m_z);
    vertices_[3] = transform * Vector3(box.min_.m_x, box.max_.m_y, box.min_.m_z);
    vertices_[4] = transform * Vector3(box.max_.m_x, box.max_.m_y, box.max_.m_z);
    vertices_[5] = transform * Vector3(box.max_.m_x, box.min_.m_y, box.max_.m_z);
    vertices_[6] = transform * Vector3(box.min_.m_x, box.min_.m_y, box.max_.m_z);
    vertices_[7] = transform * Vector3(box.min_.m_x, box.max_.m_y, box.max_.m_z);

    UpdatePlanes();
}

void Frustum::Define(const Matrix4& projection)
{
    Matrix4 projInverse = projection.Inverse();

    vertices_[0] = projInverse * Vector3(1.0f, 1.0f, 0.0f);
    vertices_[1] = projInverse * Vector3(1.0f, -1.0f, 0.0f);
    vertices_[2] = projInverse * Vector3(-1.0f, -1.0f, 0.0f);
    vertices_[3] = projInverse * Vector3(-1.0f, 1.0f, 0.0f);
    vertices_[4] = projInverse * Vector3(1.0f, 1.0f, 1.0f);
    vertices_[5] = projInverse * Vector3(1.0f, -1.0f, 1.0f);
    vertices_[6] = projInverse * Vector3(-1.0f, -1.0f, 1.0f);
    vertices_[7] = projInverse * Vector3(-1.0f, 1.0f, 1.0f);

    UpdatePlanes();
}

void Frustum::DefineOrtho(float orthoSize, float aspectRatio, float zoom, float nearZ, float farZ, const Matrix3x4& transform)
{
    nearZ = Max(nearZ, 0.0f);
    farZ = Max(farZ, nearZ);
    float halfViewSize = orthoSize * 0.5f / zoom;
    Vector3 _near, _far;

    _near.m_z = nearZ;
    _far.m_z = farZ;
    _far.m_y = _near.m_y = halfViewSize;
    _far.m_x = _near.m_x = _near.m_y * aspectRatio;

    Define(_near, _far, transform);
}

void Frustum::DefineSplit(const Matrix4& projection, float _near, float _far)
{
    Matrix4 projInverse = projection.Inverse();

    // Figure out depth values for near & far
    Vector4 nearTemp = projection * Vector4(0.0f, 0.0f, _near, 1.0f);
    Vector4 farTemp = projection * Vector4(0.0f, 0.0f, _far, 1.0f);
    float nearZ = nearTemp.m_z / nearTemp.w_;
    float farZ = farTemp.m_z / farTemp.w_;

    vertices_[0] = projInverse * Vector3(1.0f, 1.0f, nearZ);
    vertices_[1] = projInverse * Vector3(1.0f, -1.0f, nearZ);
    vertices_[2] = projInverse * Vector3(-1.0f, -1.0f, nearZ);
    vertices_[3] = projInverse * Vector3(-1.0f, 1.0f, nearZ);
    vertices_[4] = projInverse * Vector3(1.0f, 1.0f, farZ);
    vertices_[5] = projInverse * Vector3(1.0f, -1.0f, farZ);
    vertices_[6] = projInverse * Vector3(-1.0f, -1.0f, farZ);
    vertices_[7] = projInverse * Vector3(-1.0f, 1.0f, farZ);

    UpdatePlanes();
}

void Frustum::Transform(const Matrix3& transform)
{
    for (unsigned i = 0; i < NUM_FRUSTUM_VERTICES; ++i)
        vertices_[i] = transform * vertices_[i];

    UpdatePlanes();
}

void Frustum::Transform(const Matrix3x4& transform)
{
    for (unsigned i = 0; i < NUM_FRUSTUM_VERTICES; ++i)
        vertices_[i] = transform * vertices_[i];

    UpdatePlanes();
}

Frustum Frustum::Transformed(const Matrix3& transform) const
{
    Frustum transformed;
    for (unsigned i = 0; i < NUM_FRUSTUM_VERTICES; ++i)
        transformed.vertices_[i] = transform * vertices_[i];

    transformed.UpdatePlanes();
    return transformed;
}

Frustum Frustum::Transformed(const Matrix3x4& transform) const
{
    Frustum transformed;
    for (unsigned i = 0; i < NUM_FRUSTUM_VERTICES; ++i)
        transformed.vertices_[i] = transform * vertices_[i];

    transformed.UpdatePlanes();
    return transformed;
}

Rect Frustum::Projected(const Matrix4& projection) const
{
    Rect rect;

    ProjectAndMergeEdge(vertices_[0], vertices_[4], rect, projection);
    ProjectAndMergeEdge(vertices_[1], vertices_[5], rect, projection);
    ProjectAndMergeEdge(vertices_[2], vertices_[6], rect, projection);
    ProjectAndMergeEdge(vertices_[3], vertices_[7], rect, projection);
    ProjectAndMergeEdge(vertices_[4], vertices_[5], rect, projection);
    ProjectAndMergeEdge(vertices_[5], vertices_[6], rect, projection);
    ProjectAndMergeEdge(vertices_[6], vertices_[7], rect, projection);
    ProjectAndMergeEdge(vertices_[7], vertices_[4], rect, projection);

    return rect;
}

void Frustum::UpdatePlanes()
{
    planes_[PLANE_NEAR].Define(vertices_[2], vertices_[1], vertices_[0]);
    planes_[PLANE_LEFT].Define(vertices_[3], vertices_[7], vertices_[6]);
    planes_[PLANE_RIGHT].Define(vertices_[1], vertices_[5], vertices_[4]);
    planes_[PLANE_UP].Define(vertices_[0], vertices_[4], vertices_[7]);
    planes_[PLANE_DOWN].Define(vertices_[6], vertices_[5], vertices_[1]);
    planes_[PLANE_FAR].Define(vertices_[5], vertices_[6], vertices_[7]);

    // Check if we ended up with inverted planes (reflected transform) and flip in that case
    if (planes_[PLANE_NEAR].Distance(vertices_[5]) < 0.0f)
    {
        for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
        {
            planes_[i].normal_ = -planes_[i].normal_;
            planes_[i].d_ = -planes_[i].d_;
        }
    }

}

}
