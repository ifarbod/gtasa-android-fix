// Three-dimensional vector
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Math/Vector2.hpp>

namespace ctn
{

// Three-dimensional vector.
class Vector3
{
public:
    // Construct a zero vector.
    Vector3() :
        m_x(0.0f),
        m_y(0.0f),
        m_z(0.0f)
    {
    }

    // Copy-construct from another vector.
    Vector3(const Vector3& vector) :
        m_x(vector.m_x),
        m_y(vector.m_y),
        m_z(vector.m_z)
    {
    }

    // Construct from a two-dimensional vector and the Z coordinate.
    Vector3(const Vector2& vector, float z) :
        m_x(vector.m_x),
        m_y(vector.m_y),
        m_z(z)
    {
    }

    // Construct from a two-dimensional vector.
    Vector3(const Vector2& vector) :
        m_x(vector.m_x),
        m_y(vector.m_y),
        m_z(0.0f)
    {
    }

    // Construct from coordinates.
    Vector3(float x, float y, float z) :
        m_x(x),
        m_y(y),
        m_z(z)
    {
    }

    // Construct from two-dimensional coordinates.
    Vector3(float x, float y) :
        m_x(x),
        m_y(y),
        m_z(0.0f)
    {
    }

    // Construct from a float array.
    explicit Vector3(const float* data) :
        m_x(data[0]),
        m_y(data[1]),
        m_z(data[2])
    {
    }

    // Assign from another vector.
    Vector3& operator =(const Vector3& rhs)
    {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        m_z = rhs.m_z;
        return *this;
    }

    // Test for equality with another vector without epsilon.
    bool operator ==(const Vector3& rhs) const { return m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z; }

    // Test for inequality with another vector without epsilon.
    bool operator !=(const Vector3& rhs) const { return m_x != rhs.m_x || m_y != rhs.m_y || m_z != rhs.m_z; }

    // Add a vector.
    Vector3 operator +(const Vector3& rhs) const { return Vector3(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z); }

    // Return negation.
    Vector3 operator -() const { return Vector3(-m_x, -m_y, -m_z); }

    // Subtract a vector.
    Vector3 operator -(const Vector3& rhs) const { return Vector3(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z); }

    // Multiply with a scalar.
    Vector3 operator *(float rhs) const { return Vector3(m_x * rhs, m_y * rhs, m_z * rhs); }

    // Multiply with a vector.
    Vector3 operator *(const Vector3& rhs) const { return Vector3(m_x * rhs.m_x, m_y * rhs.m_y, m_z * rhs.m_z); }

    // Divide by a scalar.
    Vector3 operator /(float rhs) const { return Vector3(m_x / rhs, m_y / rhs, m_z / rhs); }

    // Divide by a vector.
    Vector3 operator /(const Vector3& rhs) const { return Vector3(m_x / rhs.m_x, m_y / rhs.m_y, m_z / rhs.m_z); }

    // Add-assign a vector.
    Vector3& operator +=(const Vector3& rhs)
    {
        m_x += rhs.m_x;
        m_y += rhs.m_y;
        m_z += rhs.m_z;
        return *this;
    }

    // Subtract-assign a vector.
    Vector3& operator -=(const Vector3& rhs)
    {
        m_x -= rhs.m_x;
        m_y -= rhs.m_y;
        m_z -= rhs.m_z;
        return *this;
    }

    // Multiply-assign a scalar.
    Vector3& operator *=(float rhs)
    {
        m_x *= rhs;
        m_y *= rhs;
        m_z *= rhs;
        return *this;
    }

    // Multiply-assign a vector.
    Vector3& operator *=(const Vector3& rhs)
    {
        m_x *= rhs.m_x;
        m_y *= rhs.m_y;
        m_z *= rhs.m_z;
        return *this;
    }

    // Divide-assign a scalar.
    Vector3& operator /=(float rhs)
    {
        float invRhs = 1.0f / rhs;
        m_x *= invRhs;
        m_y *= invRhs;
        m_z *= invRhs;
        return *this;
    }

    // Divide-assign a vector.
    Vector3& operator /=(const Vector3& rhs)
    {
        m_x /= rhs.m_x;
        m_y /= rhs.m_y;
        m_z /= rhs.m_z;
        return *this;
    }

    // Normalize to unit length.
    void Normalize()
    {
        float lenSquared = LengthSquared();
        if (!ctn::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
        {
            float invLen = 1.0f / sqrtf(lenSquared);
            m_x *= invLen;
            m_y *= invLen;
            m_z *= invLen;
        }
    }

    // Return length.
    float Length() const { return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z); }

    // Return squared length.
    float LengthSquared() const { return m_x * m_x + m_y * m_y + m_z * m_z; }

    // Calculate dot product.
    float DotProduct(const Vector3& rhs) const { return m_x * rhs.m_x + m_y * rhs.m_y + m_z * rhs.m_z; }

    // Calculate absolute dot product.
    float AbsDotProduct(const Vector3& rhs) const
    {
        return ctn::Abs(m_x * rhs.m_x) + ctn::Abs(m_y * rhs.m_y) + ctn::Abs(m_z * rhs.m_z);
    }

    // Project vector onto axis.
    float ProjectOntoAxis(const Vector3& axis) const { return DotProduct(axis.Normalized()); }

    // Calculate cross product.
    Vector3 CrossProduct(const Vector3& rhs) const
    {
        return Vector3(
            m_y * rhs.m_z - m_z * rhs.m_y,
            m_z * rhs.m_x - m_x * rhs.m_z,
            m_x * rhs.m_y - m_y * rhs.m_x
        );
    }

    // Return absolute vector.
    Vector3 Abs() const { return Vector3(ctn::Abs(m_x), ctn::Abs(m_y), ctn::Abs(m_z)); }

    // Linear interpolation with another vector.
    Vector3 Lerp(const Vector3& rhs, float t) const { return *this * (1.0f - t) + rhs * t; }

    // Test for equality with another vector with epsilon.
    bool Equals(const Vector3& rhs) const
    {
        return ctn::Equals(m_x, rhs.m_x) && ctn::Equals(m_y, rhs.m_y) && ctn::Equals(m_z, rhs.m_z);
    }

    // Returns the angle between this vector and another vector in degrees.
    float Angle(const Vector3& rhs) const { return ctn::Acos(DotProduct(rhs) / (Length() * rhs.Length())); }

    // Return whether is NaN.
    bool IsNaN() const { return ctn::IsNaN(m_x) || ctn::IsNaN(m_y) || ctn::IsNaN(m_z); }

    // Return normalized to unit length.
    Vector3 Normalized() const
    {
        float lenSquared = LengthSquared();
        if (!ctn::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
        {
            float invLen = 1.0f / sqrtf(lenSquared);
            return *this * invLen;
        }
        else
            return *this;
    }

    // Return clamped linear interpolation.
    Vector3 ClampedLerp(const Vector3& rhs, float t) const
    {
        if (t > 1)
            t = 1;
        else if (t < 0)
            t = 0;
        return *this * (1.0f - t) + rhs * t;
    }

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

    // Zero vector.
    static const Vector3 ZERO;
    // (-1,0,0) vector.
    static const Vector3 LEFT;
    // (1,0,0) vector.
    static const Vector3 RIGHT;
    // (0,1,0) vector.
    static const Vector3 UP;
    // (0,-1,0) vector.
    static const Vector3 DOWN;
    // (0,0,1) vector.
    static const Vector3 FORWARD;
    // (0,0,-1) vector.
    static const Vector3 BACK;
    // (1,1,1) vector.
    static const Vector3 ONE;
};

// Multiply Vector3 with a scalar.
inline Vector3 operator *(float lhs, const Vector3& rhs) { return rhs * lhs; }

// Per-component linear interpolation between two 3-vectors.
inline Vector3 VectorLerp(const Vector3& lhs, const Vector3& rhs, const Vector3& t) { return lhs + (rhs - lhs) * t; }

// Per-component min of two 3-vectors.
inline Vector3 VectorMin(const Vector3& lhs, const Vector3& rhs) { return Vector3(Min(lhs.m_x, rhs.m_x), Min(lhs.m_y, rhs.m_y), Min(lhs.m_z, rhs.m_z)); }

// Per-component max of two 3-vectors.
inline Vector3 VectorMax(const Vector3& lhs, const Vector3& rhs) { return Vector3(Max(lhs.m_x, rhs.m_x), Max(lhs.m_y, rhs.m_y), Max(lhs.m_z, rhs.m_z)); }

// Per-component floor of 3-vector.
inline Vector3 VectorFloor(const Vector3& vec) { return Vector3(Floor(vec.m_x), Floor(vec.m_y), Floor(vec.m_z)); }

// Per-component round of 3-vector.
inline Vector3 VectorRound(const Vector3& vec) { return Vector3(Round(vec.m_x), Round(vec.m_y), Round(vec.m_z)); }

// Per-component ceil of 3-vector.
inline Vector3 VectorCeil(const Vector3& vec) { return Vector3(Ceil(vec.m_x), Ceil(vec.m_y), Ceil(vec.m_z)); }

// Return a random value from [0, 1) from 3-vector seed.
inline float StableRandom(const Vector3& seed) { return StableRandom(Vector2(StableRandom(Vector2(seed.m_x, seed.m_y)), seed.m_z)); }

}
