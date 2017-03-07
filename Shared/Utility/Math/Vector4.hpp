// Four-dimensional vector
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Math/Vector3.hpp>

namespace ctn
{

// Four-dimensional vector.
class Vector4
{
public:
    // Construct a zero vector.
    Vector4() :
        m_x(0.0f),
        m_y(0.0f),
        m_z(0.0f),
        m_w(0.0f)
    {
    }

    // Copy-construct from another vector.
    Vector4(const Vector4& vector) :
        m_x(vector.m_x),
        m_y(vector.m_y),
        m_z(vector.m_z),
        m_w(vector.m_w)
    {
    }

    // Construct from a 3-dimensional vector and the W coordinate.
    Vector4(const Vector3& vector, float w) :
        m_x(vector.m_x),
        m_y(vector.m_y),
        m_z(vector.m_z),
        m_w(w)
    {
    }

    // Construct from coordinates.
    Vector4(float x, float y, float z, float w) :
        m_x(x),
        m_y(y),
        m_z(z),
        m_w(w)
    {
    }

    // Construct from a float array.
    explicit Vector4(const float* data) :
        m_x(data[0]),
        m_y(data[1]),
        m_z(data[2]),
        m_w(data[3])
    {
    }

    // Assign from another vector.
    Vector4& operator =(const Vector4& rhs)
    {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        m_z = rhs.m_z;
        m_w = rhs.m_w;
        return *this;
    }

    // Test for equality with another vector without epsilon.
    bool operator ==(const Vector4& rhs) const { return m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z && m_w == rhs.m_w; }

    // Test for inequality with another vector without epsilon.
    bool operator !=(const Vector4& rhs) const { return m_x != rhs.m_x || m_y != rhs.m_y || m_z != rhs.m_z || m_w != rhs.m_w; }

    // Add a vector.
    Vector4 operator +(const Vector4& rhs) const { return Vector4(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z, m_w + rhs.m_w); }

    // Return negation.
    Vector4 operator -() const { return Vector4(-m_x, -m_y, -m_z, -m_w); }

    // Subtract a vector.
    Vector4 operator -(const Vector4& rhs) const { return Vector4(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z, m_w - rhs.m_w); }

    // Multiply with a scalar.
    Vector4 operator *(float rhs) const { return Vector4(m_x * rhs, m_y * rhs, m_z * rhs, m_w * rhs); }

    // Multiply with a vector.
    Vector4 operator *(const Vector4& rhs) const { return Vector4(m_x * rhs.m_x, m_y * rhs.m_y, m_z * rhs.m_z, m_w * rhs.m_w); }

    // Divide by a scalar.
    Vector4 operator /(float rhs) const { return Vector4(m_x / rhs, m_y / rhs, m_z / rhs, m_w / rhs); }

    // Divide by a vector.
    Vector4 operator /(const Vector4& rhs) const { return Vector4(m_x / rhs.m_x, m_y / rhs.m_y, m_z / rhs.m_z, m_w / rhs.m_w); }

    // Add-assign a vector.
    Vector4& operator +=(const Vector4& rhs)
    {
        m_x += rhs.m_x;
        m_y += rhs.m_y;
        m_z += rhs.m_z;
        m_w += rhs.m_w;
        return *this;
    }

    // Subtract-assign a vector.
    Vector4& operator -=(const Vector4& rhs)
    {
        m_x -= rhs.m_x;
        m_y -= rhs.m_y;
        m_z -= rhs.m_z;
        m_w -= rhs.m_w;
        return *this;
    }

    // Multiply-assign a scalar.
    Vector4& operator *=(float rhs)
    {
        m_x *= rhs;
        m_y *= rhs;
        m_z *= rhs;
        m_w *= rhs;
        return *this;
    }

    // Multiply-assign a vector.
    Vector4& operator *=(const Vector4& rhs)
    {
        m_x *= rhs.m_x;
        m_y *= rhs.m_y;
        m_z *= rhs.m_z;
        m_w *= rhs.m_w;
        return *this;
    }

    // Divide-assign a scalar.
    Vector4& operator /=(float rhs)
    {
        float invRhs = 1.0f / rhs;
        m_x *= invRhs;
        m_y *= invRhs;
        m_z *= invRhs;
        m_w *= invRhs;
        return *this;
    }

    // Divide-assign a vector.
    Vector4& operator /=(const Vector4& rhs)
    {
        m_x /= rhs.m_x;
        m_y /= rhs.m_y;
        m_z /= rhs.m_z;
        m_w /= rhs.m_w;
        return *this;
    }

    // Calculate dot product.
    float DotProduct(const Vector4& rhs) const { return m_x * rhs.m_x + m_y * rhs.m_y + m_z * rhs.m_z + m_w * rhs.m_w; }

    // Calculate absolute dot product.
    float AbsDotProduct(const Vector4& rhs) const
    {
        return ctn::Abs(m_x * rhs.m_x) + ctn::Abs(m_y * rhs.m_y) + ctn::Abs(m_z * rhs.m_z) + ctn::Abs(m_w * rhs.m_w);
    }

    // Project vector onto axis.
    float ProjectOntoAxis(const Vector3& axis) const { return DotProduct(Vector4(axis.Normalized(), 0.0f)); }

    // Return absolute vector.
    Vector4 Abs() const { return Vector4(ctn::Abs(m_x), ctn::Abs(m_y), ctn::Abs(m_z), ctn::Abs(m_w)); }

    // Linear interpolation with another vector.
    Vector4 Lerp(const Vector4& rhs, float t) const { return *this * (1.0f - t) + rhs * t; }

    // Test for equality with another vector with epsilon.
    bool Equals(const Vector4& rhs) const
    {
        return ctn::Equals(m_x, rhs.m_x) && ctn::Equals(m_y, rhs.m_y) && ctn::Equals(m_z, rhs.m_z) && ctn::Equals(m_w, rhs.m_w);
    }

    // Return whether is NaN.
    bool IsNaN() const { return ctn::IsNaN(m_x) || ctn::IsNaN(m_y) || ctn::IsNaN(m_z) || ctn::IsNaN(m_w); }

    // Return float data.
    const float* Data() const { return &m_x; }

    // Return as string.
    String ToString() const;

    // X coordinate.
    float m_x;
    // Y coordinate.
    float m_y;
    // Z coordinate.
    float m_z;
    // W coordinate.
    float m_w;

    // Zero vector.
    static const Vector4 ZERO;
    // (1,1,1) vector.
    static const Vector4 ONE;
};

// Multiply Vector4 with a scalar.
inline Vector4 operator *(float lhs, const Vector4& rhs) { return rhs * lhs; }

// Per-component linear interpolation between two 4-vectors.
inline Vector4 VectorLerp(const Vector4& lhs, const Vector4& rhs, const Vector4& t) { return lhs + (rhs - lhs) * t; }

// Per-component min of two 4-vectors.
inline Vector4 VectorMin(const Vector4& lhs, const Vector4& rhs) { return Vector4(Min(lhs.m_x, rhs.m_x), Min(lhs.m_y, rhs.m_y), Min(lhs.m_z, rhs.m_z), Min(lhs.m_w, rhs.m_w)); }

// Per-component max of two 4-vectors.
inline Vector4 VectorMax(const Vector4& lhs, const Vector4& rhs) { return Vector4(Max(lhs.m_x, rhs.m_x), Max(lhs.m_y, rhs.m_y), Max(lhs.m_z, rhs.m_z), Max(lhs.m_w, rhs.m_w)); }

// Per-component floor of 4-vector.
inline Vector4 VectorFloor(const Vector4& vec) { return Vector4(Floor(vec.m_x), Floor(vec.m_y), Floor(vec.m_z), Floor(vec.m_w)); }

// Per-component round of 4-vector.
inline Vector4 VectorRound(const Vector4& vec) { return Vector4(Round(vec.m_x), Round(vec.m_y), Round(vec.m_z), Round(vec.m_w)); }

// Per-component ceil of 4-vector.
inline Vector4 VectorCeil(const Vector4& vec) { return Vector4(Ceil(vec.m_x), Ceil(vec.m_y), Ceil(vec.m_z), Ceil(vec.m_w)); }

}
