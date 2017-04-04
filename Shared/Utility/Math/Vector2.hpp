// Two-dimensional vector
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Container/Str.hpp>
#include <Math/MathDefs.hpp>

namespace ctn
{

// Two-dimensional vector.
class Vector2
{
public:
    // Construct a zero vector.
    Vector2() :
        m_x(0.0f),
        m_y(0.0f)
    {
    }

    // Copy-construct from another vector.
    Vector2(const Vector2& vector) :
        m_x(vector.m_x),
        m_y(vector.m_y)
    {
    }

    // Construct from coordinates.
    Vector2(float x, float y) :
        m_x(x),
        m_y(y)
    {
    }

    // Construct from a float array.
    explicit Vector2(const float* data) :
        m_x(data[0]),
        m_y(data[1])
    {
    }

    // Assign from another vector.
    Vector2& operator =(const Vector2& rhs)
    {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        return *this;
    }

    // Test for equality with another vector without epsilon.
    bool operator ==(const Vector2& rhs) const { return m_x == rhs.m_x && m_y == rhs.m_y; }

    // Test for inequality with another vector without epsilon.
    bool operator !=(const Vector2& rhs) const { return m_x != rhs.m_x || m_y != rhs.m_y; }

    // Add a vector.
    Vector2 operator +(const Vector2& rhs) const { return Vector2(m_x + rhs.m_x, m_y + rhs.m_y); }

    // Return negation.
    Vector2 operator -() const { return Vector2(-m_x, -m_y); }

    // Subtract a vector.
    Vector2 operator -(const Vector2& rhs) const { return Vector2(m_x - rhs.m_x, m_y - rhs.m_y); }

    // Multiply with a scalar.
    Vector2 operator *(float rhs) const { return Vector2(m_x * rhs, m_y * rhs); }

    // Multiply with a vector.
    Vector2 operator *(const Vector2& rhs) const { return Vector2(m_x * rhs.m_x, m_y * rhs.m_y); }

    // Divide by a scalar.
    Vector2 operator /(float rhs) const { return Vector2(m_x / rhs, m_y / rhs); }

    // Divide by a vector.
    Vector2 operator /(const Vector2& rhs) const { return Vector2(m_x / rhs.m_x, m_y / rhs.m_y); }

    // Add-assign a vector.
    Vector2& operator +=(const Vector2& rhs)
    {
        m_x += rhs.m_x;
        m_y += rhs.m_y;
        return *this;
    }

    // Subtract-assign a vector.
    Vector2& operator -=(const Vector2& rhs)
    {
        m_x -= rhs.m_x;
        m_y -= rhs.m_y;
        return *this;
    }

    // Multiply-assign a scalar.
    Vector2& operator *=(float rhs)
    {
        m_x *= rhs;
        m_y *= rhs;
        return *this;
    }

    // Multiply-assign a vector.
    Vector2& operator *=(const Vector2& rhs)
    {
        m_x *= rhs.m_x;
        m_y *= rhs.m_y;
        return *this;
    }

    // Divide-assign a scalar.
    Vector2& operator /=(float rhs)
    {
        float invRhs = 1.0f / rhs;
        m_x *= invRhs;
        m_y *= invRhs;
        return *this;
    }

    // Divide-assign a vector.
    Vector2& operator /=(const Vector2& rhs)
    {
        m_x /= rhs.m_x;
        m_y /= rhs.m_y;
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
        }
    }

    // Return length.
    float Length() const { return sqrtf(m_x * m_x + m_y * m_y); }

    // Return squared length.
    float LengthSquared() const { return m_x * m_x + m_y * m_y; }

    // Calculate dot product.
    float DotProduct(const Vector2& rhs) const { return m_x * rhs.m_x + m_y * rhs.m_y; }

    // Calculate absolute dot product.
    float AbsDotProduct(const Vector2& rhs) const { return ctn::Abs(m_x * rhs.m_x) + ctn::Abs(m_y * rhs.m_y); }

    // Project vector onto axis.
    float ProjectOntoAxis(const Vector2& axis) const { return DotProduct(axis.Normalized()); }

    // Returns the angle between this vector and another vector in degrees.
    float Angle(const Vector2& rhs) const { return ctn::Acos(DotProduct(rhs) / (Length() * rhs.Length())); }

    // Return absolute vector.
    Vector2 Abs() const { return Vector2(ctn::Abs(m_x), ctn::Abs(m_y)); }

    // Linear interpolation with another vector.
    Vector2 Lerp(const Vector2& rhs, float t) const { return *this * (1.0f - t) + rhs * t; }

    // Test for equality with another vector with epsilon.
    bool Equals(const Vector2& rhs) const { return ctn::Equals(m_x, rhs.m_x) && ctn::Equals(m_y, rhs.m_y); }

    // Return whether is NaN.
    bool IsNaN() const { return ctn::IsNaN(m_x) || ctn::IsNaN(m_y); }

    // Return normalized to unit length.
    Vector2 Normalized() const
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

    // Return float data.
    const float* Data() const { return &m_x; }

    // Return as string.
    String ToString() const;

    // X coordinate.
    float m_x;
    // Y coordinate.
    float m_y;

    // Zero vector.
    static const Vector2 ZERO;
    // (-1,0) vector.
    static const Vector2 LEFT;
    // (1,0) vector.
    static const Vector2 RIGHT;
    // (0,1) vector.
    static const Vector2 UP;
    // (0,-1) vector.
    static const Vector2 DOWN;
    // (1,1) vector.
    static const Vector2 ONE;
};

// Multiply Vector2 with a scalar
inline Vector2 operator *(float lhs, const Vector2& rhs) { return rhs * lhs; }

// Two-dimensional vector with integer values.
class IntVector2
{
public:
    // Construct a zero vector.
    IntVector2() :
        m_x(0),
        m_y(0)
    {
    }

    // Construct from coordinates.
    IntVector2(int x, int y) :
        m_x(x),
        m_y(y)
    {
    }

    // Construct from an int array.
    IntVector2(const int* data) :
        m_x(data[0]),
        m_y(data[1])
    {
    }

    // Copy-construct from another vector.
    IntVector2(const IntVector2& rhs) :
        m_x(rhs.m_x),
        m_y(rhs.m_y)
    {
    }

    // Assign from another vector.
    IntVector2& operator =(const IntVector2& rhs)
    {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        return *this;
    }

    // Test for equality with another vector.
    bool operator ==(const IntVector2& rhs) const { return m_x == rhs.m_x && m_y == rhs.m_y; }

    // Test for inequality with another vector.
    bool operator !=(const IntVector2& rhs) const { return m_x != rhs.m_x || m_y != rhs.m_y; }

    // Add a vector.
    IntVector2 operator +(const IntVector2& rhs) const { return IntVector2(m_x + rhs.m_x, m_y + rhs.m_y); }

    // Return negation.
    IntVector2 operator -() const { return IntVector2(-m_x, -m_y); }

    // Subtract a vector.
    IntVector2 operator -(const IntVector2& rhs) const { return IntVector2(m_x - rhs.m_x, m_y - rhs.m_y); }

    // Multiply with a scalar.
    IntVector2 operator *(int rhs) const { return IntVector2(m_x * rhs, m_y * rhs); }

    // Divide by a scalar.
    IntVector2 operator /(int rhs) const { return IntVector2(m_x / rhs, m_y / rhs); }

    // Add-assign a vector.
    IntVector2& operator +=(const IntVector2& rhs)
    {
        m_x += rhs.m_x;
        m_y += rhs.m_y;
        return *this;
    }

    // Subtract-assign a vector.
    IntVector2& operator -=(const IntVector2& rhs)
    {
        m_x -= rhs.m_x;
        m_y -= rhs.m_y;
        return *this;
    }

    // Multiply-assign a scalar.
    IntVector2& operator *=(int rhs)
    {
        m_x *= rhs;
        m_y *= rhs;
        return *this;
    }

    // Divide-assign a scalar.
    IntVector2& operator /=(int rhs)
    {
        m_x /= rhs;
        m_y /= rhs;
        return *this;
    }

    // Return integer data.
    const int* Data() const { return &m_x; }

    // Return as string.
    String ToString() const;

    // Return hash value for HashSet & HashMap.
    unsigned ToHash() const { return (unsigned)m_x * 31 + (unsigned)m_y; }

    // Return length.
    float Length() const { return sqrtf((float)(m_x * m_x + m_y * m_y)); }

    // X coordinate.
    int m_x;
    // Y coordinate.
    int m_y;

    // Zero vector.
    static const IntVector2 ZERO;
};

// Multiply IntVector2 with a scalar.
inline IntVector2 operator *(int lhs, const IntVector2& rhs) { return rhs * lhs; }

// Per-component linear interpolation between two 2-vectors.
inline Vector2 VectorLerp(const Vector2& lhs, const Vector2& rhs, const Vector2& t) { return lhs + (rhs - lhs) * t; }

// Per-component min of two 2-vectors.
inline Vector2 VectorMin(const Vector2& lhs, const Vector2& rhs) { return Vector2(Min(lhs.m_x, rhs.m_x), Min(lhs.m_y, rhs.m_y)); }

// Per-component max of two 2-vectors.
inline Vector2 VectorMax(const Vector2& lhs, const Vector2& rhs) { return Vector2(Max(lhs.m_x, rhs.m_x), Max(lhs.m_y, rhs.m_y)); }

// Per-component floor of 2-vector.
inline Vector2 VectorFloor(const Vector2& vec) { return Vector2(Floor(vec.m_x), Floor(vec.m_y)); }

// Per-component round of 2-vector.
inline Vector2 VectorRound(const Vector2& vec) { return Vector2(Round(vec.m_x), Round(vec.m_y)); }

// Per-component ceil of 2-vector.
inline Vector2 VectorCeil(const Vector2& vec) { return Vector2(Ceil(vec.m_x), Ceil(vec.m_y)); }

// Per-component floor of 2-vector. Returns IntVector2.
inline IntVector2 VectorFloorToInt(const Vector2& vec) { return IntVector2(FloorToInt(vec.m_x), FloorToInt(vec.m_y)); }

// Per-component round of 2-vector. Returns IntVector2.
inline IntVector2 VectorRoundToInt(const Vector2& vec) { return IntVector2(RoundToInt(vec.m_x), RoundToInt(vec.m_y)); }

// Per-component ceil of 2-vector. Returns IntVector2.
inline IntVector2 VectorCeilToInt(const Vector2& vec) { return IntVector2(CeilToInt(vec.m_x), CeilToInt(vec.m_y)); }

// Per-component min of two 2-vectors.
inline IntVector2 VectorMin(const IntVector2& lhs, const IntVector2& rhs) { return IntVector2(Min(lhs.m_x, rhs.m_x), Min(lhs.m_y, rhs.m_y)); }

// Per-component max of two 2-vectors.
inline IntVector2 VectorMax(const IntVector2& lhs, const IntVector2& rhs) { return IntVector2(Max(lhs.m_x, rhs.m_x), Max(lhs.m_y, rhs.m_y)); }

// Return a random value from [0, 1) from 2-vector seed.
// http://stackoverflow.com/questions/12964279/whats-the-origin-of-this-glsl-rand-one-liner
inline float StableRandom(const Vector2& seed) { return Fract(Sin(seed.DotProduct(Vector2(12.9898f, 78.233f)) * M_RADTODEG) * 43758.5453f); }

// Return a random value from [0, 1) from scalar seed.
inline float StableRandom(float seed) { return StableRandom(Vector2(seed, seed)); }

}
