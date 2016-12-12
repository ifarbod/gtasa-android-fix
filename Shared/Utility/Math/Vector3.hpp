// Three-dimensional vector
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Math/Vector2.hpp>

namespace Util
{

// Three-dimensional vector.
class Vector3
{
public:
    // Construct a zero vector.
    Vector3() :
        x_(0.0f),
        y_(0.0f),
        z_(0.0f)
    {
    }

    // Copy-construct from another vector.
    Vector3(const Vector3& vector) :
        x_(vector.x_),
        y_(vector.y_),
        z_(vector.z_)
    {
    }

    // Construct from a two-dimensional vector and the Z coordinate.
    Vector3(const Vector2& vector, float z) :
        x_(vector.x_),
        y_(vector.y_),
        z_(z)
    {
    }

    // Construct from a two-dimensional vector.
    Vector3(const Vector2& vector) :
        x_(vector.x_),
        y_(vector.y_),
        z_(0.0f)
    {
    }

    // Construct from coordinates.
    Vector3(float x, float y, float z) :
        x_(x),
        y_(y),
        z_(z)
    {
    }

    // Construct from two-dimensional coordinates.
    Vector3(float x, float y) :
        x_(x),
        y_(y),
        z_(0.0f)
    {
    }

    // Construct from a float array.
    explicit Vector3(const float* data) :
        x_(data[0]),
        y_(data[1]),
        z_(data[2])
    {
    }

    // Assign from another vector.
    Vector3& operator =(const Vector3& rhs)
    {
        x_ = rhs.x_;
        y_ = rhs.y_;
        z_ = rhs.z_;
        return *this;
    }

    // Test for equality with another vector without epsilon.
    bool operator ==(const Vector3& rhs) const { return x_ == rhs.x_ && y_ == rhs.y_ && z_ == rhs.z_; }

    // Test for inequality with another vector without epsilon.
    bool operator !=(const Vector3& rhs) const { return x_ != rhs.x_ || y_ != rhs.y_ || z_ != rhs.z_; }

    // Add a vector.
    Vector3 operator +(const Vector3& rhs) const { return Vector3(x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_); }

    // Return negation.
    Vector3 operator -() const { return Vector3(-x_, -y_, -z_); }

    // Subtract a vector.
    Vector3 operator -(const Vector3& rhs) const { return Vector3(x_ - rhs.x_, y_ - rhs.y_, z_ - rhs.z_); }

    // Multiply with a scalar.
    Vector3 operator *(float rhs) const { return Vector3(x_ * rhs, y_ * rhs, z_ * rhs); }

    // Multiply with a vector.
    Vector3 operator *(const Vector3& rhs) const { return Vector3(x_ * rhs.x_, y_ * rhs.y_, z_ * rhs.z_); }

    // Divide by a scalar.
    Vector3 operator /(float rhs) const { return Vector3(x_ / rhs, y_ / rhs, z_ / rhs); }

    // Divide by a vector.
    Vector3 operator /(const Vector3& rhs) const { return Vector3(x_ / rhs.x_, y_ / rhs.y_, z_ / rhs.z_); }

    // Add-assign a vector.
    Vector3& operator +=(const Vector3& rhs)
    {
        x_ += rhs.x_;
        y_ += rhs.y_;
        z_ += rhs.z_;
        return *this;
    }

    // Subtract-assign a vector.
    Vector3& operator -=(const Vector3& rhs)
    {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        z_ -= rhs.z_;
        return *this;
    }

    // Multiply-assign a scalar.
    Vector3& operator *=(float rhs)
    {
        x_ *= rhs;
        y_ *= rhs;
        z_ *= rhs;
        return *this;
    }

    // Multiply-assign a vector.
    Vector3& operator *=(const Vector3& rhs)
    {
        x_ *= rhs.x_;
        y_ *= rhs.y_;
        z_ *= rhs.z_;
        return *this;
    }

    // Divide-assign a scalar.
    Vector3& operator /=(float rhs)
    {
        float invRhs = 1.0f / rhs;
        x_ *= invRhs;
        y_ *= invRhs;
        z_ *= invRhs;
        return *this;
    }

    // Divide-assign a vector.
    Vector3& operator /=(const Vector3& rhs)
    {
        x_ /= rhs.x_;
        y_ /= rhs.y_;
        z_ /= rhs.z_;
        return *this;
    }

    // Normalize to unit length.
    void Normalize()
    {
        float lenSquared = LengthSquared();
        if (!Util::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
        {
            float invLen = 1.0f / sqrtf(lenSquared);
            x_ *= invLen;
            y_ *= invLen;
            z_ *= invLen;
        }
    }

    // Return length.
    float Length() const { return sqrtf(x_ * x_ + y_ * y_ + z_ * z_); }

    // Return squared length.
    float LengthSquared() const { return x_ * x_ + y_ * y_ + z_ * z_; }

    // Calculate dot product.
    float DotProduct(const Vector3& rhs) const { return x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_; }

    // Calculate absolute dot product.
    float AbsDotProduct(const Vector3& rhs) const
    {
        return Util::Abs(x_ * rhs.x_) + Util::Abs(y_ * rhs.y_) + Util::Abs(z_ * rhs.z_);
    }

    // Project vector onto axis.
    float ProjectOntoAxis(const Vector3& axis) const { return DotProduct(axis.Normalized()); }

    // Calculate cross product.
    Vector3 CrossProduct(const Vector3& rhs) const
    {
        return Vector3(
            y_ * rhs.z_ - z_ * rhs.y_,
            z_ * rhs.x_ - x_ * rhs.z_,
            x_ * rhs.y_ - y_ * rhs.x_
        );
    }

    // Return absolute vector.
    Vector3 Abs() const { return Vector3(Util::Abs(x_), Util::Abs(y_), Util::Abs(z_)); }

    // Linear interpolation with another vector.
    Vector3 Lerp(const Vector3& rhs, float t) const { return *this * (1.0f - t) + rhs * t; }

    // Test for equality with another vector with epsilon.
    bool Equals(const Vector3& rhs) const
    {
        return Util::Equals(x_, rhs.x_) && Util::Equals(y_, rhs.y_) && Util::Equals(z_, rhs.z_);
    }

    // Returns the angle between this vector and another vector in degrees.
    float Angle(const Vector3& rhs) const { return Util::Acos(DotProduct(rhs) / (Length() * rhs.Length())); }

    // Return whether is NaN.
    bool IsNaN() const { return Util::IsNaN(x_) || Util::IsNaN(y_) || Util::IsNaN(z_); }

    // Return normalized to unit length.
    Vector3 Normalized() const
    {
        float lenSquared = LengthSquared();
        if (!Util::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
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
    const float* Data() const { return &x_; }

    // Return as string.
    String ToString() const;

    // X coordinate.
    float x_;
    // Y coordinate.
    float y_;
    // Z coordinate.
    float z_;

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
inline Vector3 VectorMin(const Vector3& lhs, const Vector3& rhs) { return Vector3(Min(lhs.x_, rhs.x_), Min(lhs.y_, rhs.y_), Min(lhs.z_, rhs.z_)); }

// Per-component max of two 3-vectors.
inline Vector3 VectorMax(const Vector3& lhs, const Vector3& rhs) { return Vector3(Max(lhs.x_, rhs.x_), Max(lhs.y_, rhs.y_), Max(lhs.z_, rhs.z_)); }

// Per-component floor of 3-vector.
inline Vector3 VectorFloor(const Vector3& vec) { return Vector3(Floor(vec.x_), Floor(vec.y_), Floor(vec.z_)); }

// Per-component round of 3-vector.
inline Vector3 VectorRound(const Vector3& vec) { return Vector3(Round(vec.x_), Round(vec.y_), Round(vec.z_)); }

// Per-component ceil of 3-vector.
inline Vector3 VectorCeil(const Vector3& vec) { return Vector3(Ceil(vec.x_), Ceil(vec.y_), Ceil(vec.z_)); }

// Return a random value from [0, 1) from 3-vector seed.
inline float StableRandom(const Vector3& seed) { return StableRandom(Vector2(StableRandom(Vector2(seed.x_, seed.y_)), seed.z_)); }

}
