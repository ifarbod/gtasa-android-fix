// Rotation represented as a four-dimensional normalized vector
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <Math/Matrix3.hpp>

#ifdef CTN_SSE
#include <emmintrin.h>
#endif

namespace ctn
{

// Rotation represented as a four-dimensional normalized vector.
class Quaternion
{
public:
    // Construct an identity quaternion.
    Quaternion()
#ifndef CTN_SSE
        :m_w(1.0f),
        m_x(0.0f),
        m_y(0.0f),
        m_z(0.0f)
#endif
    {
#ifdef CTN_SSE
        _mm_storeu_ps(&m_w, _mm_set_ps(0.f, 0.f, 0.f, 1.f));
#endif
    }

    // Copy-construct from another quaternion.
    Quaternion(const Quaternion& quat)
#if defined(CTN_SSE) && (!defined(_MSC_VER) || _MSC_VER >= 1700) // VS2012 and newer. VS2010 has a bug with these.
    {
        _mm_storeu_ps(&m_w, _mm_loadu_ps(&quat.m_w));
    }
#else
        :m_w(quat.m_w),
        m_x(quat.m_x),
        m_y(quat.m_y),
        m_z(quat.m_z)
    {
    }
#endif

    // Construct from values.
    Quaternion(float w, float x, float y, float z)
#ifndef CTN_SSE
        :m_w(w),
        m_x(x),
        m_y(y),
        m_z(z)
#endif
    {
#ifdef CTN_SSE
        _mm_storeu_ps(&m_w, _mm_set_ps(z, y, x, w));
#endif
    }

    // Construct from a float array.
    explicit Quaternion(const float* data)
#ifndef CTN_SSE
        :m_w(data[0]),
        m_x(data[1]),
        m_y(data[2]),
        m_z(data[3])
#endif
    {
#ifdef CTN_SSE
        _mm_storeu_ps(&m_w, _mm_loadu_ps(data));
#endif
    }

    // Construct from an angle (in degrees) and axis.
    Quaternion(float angle, const Vector3& axis)
    {
        FromAngleAxis(angle, axis);
    }

    // Construct from an angle (in degrees).
    explicit Quaternion(float angle)
    {
        FromAngleAxis(angle, Vector3::FORWARD);
    }

    // Construct from Euler angles (in degrees.)
    Quaternion(float x, float y, float z)
    {
        FromEulerAngles(x, y, z);
    }

    // Construct from the rotation difference between two direction vectors.
    Quaternion(const Vector3& start, const Vector3& end)
    {
        FromRotationTo(start, end);
    }

    // Construct from orthonormal axes.
    Quaternion(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
    {
        FromAxes(xAxis, yAxis, zAxis);
    }

    // Construct from a rotation matrix.
    explicit Quaternion(const Matrix3& matrix)
    {
        FromRotationMatrix(matrix);
    }

#ifdef CTN_SSE
    explicit Quaternion(__m128 wxyz)
    {
        _mm_storeu_ps(&m_w, wxyz);
    }
#endif

    // Assign from another quaternion.
    Quaternion& operator =(const Quaternion& rhs)
    {
#if defined(CTN_SSE) && (!defined(_MSC_VER) || _MSC_VER >= 1700) // VS2012 and newer. VS2010 has a bug with these.
        _mm_storeu_ps(&m_w, _mm_loadu_ps(&rhs.m_w));
#else
        m_w = rhs.m_w;
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        m_z = rhs.m_z;
#endif
        return *this;
    }

    // Add-assign a quaternion.
    Quaternion& operator +=(const Quaternion& rhs)
    {
#ifdef CTN_SSE
        _mm_storeu_ps(&m_w, _mm_add_ps(_mm_loadu_ps(&m_w), _mm_loadu_ps(&rhs.m_w)));
#else
        m_w += rhs.m_w;
        m_x += rhs.m_x;
        m_y += rhs.m_y;
        m_z += rhs.m_z;
#endif
        return *this;
    }

    // Multiply-assign a scalar.
    Quaternion& operator *=(float rhs)
    {
#ifdef CTN_SSE
        _mm_storeu_ps(&m_w, _mm_mul_ps(_mm_loadu_ps(&m_w), _mm_set1_ps(rhs)));
#else
        m_w *= rhs;
        m_x *= rhs;
        m_y *= rhs;
        m_z *= rhs;
#endif
        return *this;
    }

    // Test for equality with another quaternion without epsilon.
    bool operator ==(const Quaternion& rhs) const
    {
#ifdef CTN_SSE
        __m128 c = _mm_cmpeq_ps(_mm_loadu_ps(&m_w), _mm_loadu_ps(&rhs.m_w));
        c = _mm_and_ps(c, _mm_movehl_ps(c, c));
        c = _mm_and_ps(c, _mm_shuffle_ps(c, c, _MM_SHUFFLE(1, 1, 1, 1)));
        return _mm_cvtsi128_si32(_mm_castps_si128(c)) == -1;
#else
        return m_w == rhs.m_w && m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z;
#endif
    }

    // Test for inequality with another quaternion without epsilon.
    bool operator !=(const Quaternion& rhs) const { return !(*this == rhs); }

    // Multiply with a scalar.
    Quaternion operator *(float rhs) const
    {
#ifdef CTN_SSE
        return Quaternion(_mm_mul_ps(_mm_loadu_ps(&m_w), _mm_set1_ps(rhs)));
#else
        return Quaternion(m_w * rhs, m_x * rhs, m_y * rhs, m_z * rhs);
#endif
    }

    // Return negation.
    Quaternion operator -() const
    {
#ifdef CTN_SSE
        return Quaternion(_mm_xor_ps(_mm_loadu_ps(&m_w), _mm_castsi128_ps(_mm_set1_epi32((int)0x80000000UL))));
#else
        return Quaternion(-m_w, -m_x, -m_y, -m_z);
#endif
    }

    // Add a quaternion.
    Quaternion operator +(const Quaternion& rhs) const
    {
#ifdef CTN_SSE
        return Quaternion(_mm_add_ps(_mm_loadu_ps(&m_w), _mm_loadu_ps(&rhs.m_w)));
#else
        return Quaternion(m_w + rhs.m_w, m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z);
#endif
    }

    // Subtract a quaternion.
    Quaternion operator -(const Quaternion& rhs) const
    {
#ifdef CTN_SSE
        return Quaternion(_mm_sub_ps(_mm_loadu_ps(&m_w), _mm_loadu_ps(&rhs.m_w)));
#else
        return Quaternion(m_w - rhs.m_w, m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z);
#endif
    }

    // Multiply a quaternion.
    Quaternion operator *(const Quaternion& rhs) const
    {
#ifdef CTN_SSE
        __m128 q1 = _mm_loadu_ps(&m_w);
        __m128 q2 = _mm_loadu_ps(&rhs.m_w);
        q2 = _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(0, 3, 2, 1));
        const __m128 signy = _mm_castsi128_ps(_mm_set_epi32((int)0x80000000UL, (int)0x80000000UL, 0, 0));
        const __m128 signx = _mm_shuffle_ps(signy, signy, _MM_SHUFFLE(2, 0, 2, 0));
        const __m128 signz = _mm_shuffle_ps(signy, signy, _MM_SHUFFLE(3, 0, 0, 3));
        __m128 out = _mm_mul_ps(_mm_shuffle_ps(q1, q1, _MM_SHUFFLE(1, 1, 1, 1)), _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(0, 1, 2, 3)));
        out = _mm_add_ps(_mm_mul_ps(_mm_xor_ps(signy, _mm_shuffle_ps(q1, q1, _MM_SHUFFLE(2, 2, 2, 2))), _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(1, 0, 3, 2))), _mm_xor_ps(signx, out));
        out = _mm_add_ps(_mm_mul_ps(_mm_xor_ps(signz, _mm_shuffle_ps(q1, q1, _MM_SHUFFLE(3, 3, 3, 3))), _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(2, 3, 0, 1))), out);
        out = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(q1, q1, _MM_SHUFFLE(0, 0, 0, 0)), q2), out);
        return Quaternion(_mm_shuffle_ps(out, out, _MM_SHUFFLE(2, 1, 0, 3)));
#else
        return Quaternion(
            m_w * rhs.m_w - m_x * rhs.m_x - m_y * rhs.m_y - m_z * rhs.m_z,
            m_w * rhs.m_x + m_x * rhs.m_w + m_y * rhs.m_z - m_z * rhs.m_y,
            m_w * rhs.m_y + m_y * rhs.m_w + m_z * rhs.m_x - m_x * rhs.m_z,
            m_w * rhs.m_z + m_z * rhs.m_w + m_x * rhs.m_y - m_y * rhs.m_x
        );
#endif
    }

    // Multiply a Vector3.
    Vector3 operator *(const Vector3& rhs) const
    {
#ifdef CTN_SSE
        __m128 q = _mm_loadu_ps(&m_w);
        q = _mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 3, 2, 1));
        __m128 v = _mm_set_ps(0.f, rhs.m_z, rhs.m_y, rhs.m_x);
        const __m128 W = _mm_shuffle_ps(q, q, _MM_SHUFFLE(3, 3, 3, 3));
        const __m128 a_yzx = _mm_shuffle_ps(q, q, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 x = _mm_mul_ps(q, _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1)));
        __m128 qxv = _mm_sub_ps(x, _mm_mul_ps(a_yzx, v));
        __m128 Wv = _mm_mul_ps(W, v);
        __m128 s = _mm_add_ps(qxv, _mm_shuffle_ps(Wv, Wv, _MM_SHUFFLE(3, 1, 0, 2)));
        __m128 qs = _mm_mul_ps(q, s);
        __m128 y = _mm_shuffle_ps(qs, qs, _MM_SHUFFLE(3, 1, 0, 2));
        s = _mm_sub_ps(_mm_mul_ps(a_yzx, s), y);
        s = _mm_add_ps(s, s);
        s = _mm_add_ps(s, v);

        return Vector3(
            _mm_cvtss_f32(s),
            _mm_cvtss_f32(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1))),
            _mm_cvtss_f32(_mm_movehl_ps(s, s)));
#else
        Vector3 qVec(m_x, m_y, m_z);
        Vector3 cross1(qVec.CrossProduct(rhs));
        Vector3 cross2(qVec.CrossProduct(cross1));

        return rhs + 2.0f * (cross1 * m_w + cross2);
#endif
    }

    // Define from an angle (in degrees) and axis.
    void FromAngleAxis(float angle, const Vector3& axis);
    // Define from Euler angles (in degrees.)
    void FromEulerAngles(float x, float y, float z);
    // Define from the rotation difference between two direction vectors.
    void FromRotationTo(const Vector3& start, const Vector3& end);
    // Define from orthonormal axes.
    void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
    // Define from a rotation matrix.
    void FromRotationMatrix(const Matrix3& matrix);
    // Define from a direction to look in and an up direction. Return true if successful, or false if would result in a NaN, in which case the current value remains.
    bool FromLookRotation(const Vector3& direction, const Vector3& up = Vector3::UP);

    // Normalize to unit length.
    void Normalize()
    {
#ifdef CTN_SSE
        __m128 q = _mm_loadu_ps(&m_w);
        __m128 n = _mm_mul_ps(q, q);
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(2, 3, 0, 1)));
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(0, 1, 2, 3)));
        __m128 e = _mm_rsqrt_ps(n);
        __m128 e3 = _mm_mul_ps(_mm_mul_ps(e, e), e);
        __m128 half = _mm_set1_ps(0.5f);
        n = _mm_add_ps(e, _mm_mul_ps(half, _mm_sub_ps(e, _mm_mul_ps(n, e3))));
        _mm_storeu_ps(&m_w, _mm_mul_ps(q, n));
#else
        float lenSquared = LengthSquared();
        if (!ctn::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
        {
            float invLen = 1.0f / sqrtf(lenSquared);
            m_w *= invLen;
            m_x *= invLen;
            m_y *= invLen;
            m_z *= invLen;
        }
#endif
    }

    // Return normalized to unit length.
    Quaternion Normalized() const
    {
#ifdef CTN_SSE
        __m128 q = _mm_loadu_ps(&m_w);
        __m128 n = _mm_mul_ps(q, q);
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(2, 3, 0, 1)));
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(0, 1, 2, 3)));
        __m128 e = _mm_rsqrt_ps(n);
        __m128 e3 = _mm_mul_ps(_mm_mul_ps(e, e), e);
        __m128 half = _mm_set1_ps(0.5f);
        n = _mm_add_ps(e, _mm_mul_ps(half, _mm_sub_ps(e, _mm_mul_ps(n, e3))));
        return Quaternion(_mm_mul_ps(q, n));
#else
        float lenSquared = LengthSquared();
        if (!ctn::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
        {
            float invLen = 1.0f / sqrtf(lenSquared);
            return *this * invLen;
        }
        else
            return *this;
#endif
    }

    // Return inverse.
    Quaternion Inverse() const
    {
#ifdef CTN_SSE
        __m128 q = _mm_loadu_ps(&m_w);
        __m128 n = _mm_mul_ps(q, q);
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(2, 3, 0, 1)));
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(0, 1, 2, 3)));
        return Quaternion(_mm_div_ps(_mm_xor_ps(q, _mm_castsi128_ps(_mm_set_epi32((int)0x80000000UL, (int)0x80000000UL, (int)0x80000000UL, 0))), n));
#else
        float lenSquared = LengthSquared();
        if (lenSquared == 1.0f)
            return Conjugate();
        else if (lenSquared >= M_EPSILON)
            return Conjugate() * (1.0f / lenSquared);
        else
            return IDENTITY;
#endif
    }

    // Return squared length.
    float LengthSquared() const
    {
#ifdef CTN_SSE
        __m128 q = _mm_loadu_ps(&m_w);
        __m128 n = _mm_mul_ps(q, q);
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(2, 3, 0, 1)));
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(0, 1, 2, 3)));
        return _mm_cvtss_f32(n);
#else
        return m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z;
#endif
    }

    // Calculate dot product.
    float DotProduct(const Quaternion& rhs) const
    {
#ifdef CTN_SSE
        __m128 q1 = _mm_loadu_ps(&m_w);
        __m128 q2 = _mm_loadu_ps(&rhs.m_w);
        __m128 n = _mm_mul_ps(q1, q2);
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(2, 3, 0, 1)));
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(0, 1, 2, 3)));
        return _mm_cvtss_f32(n);
#else
        return m_w * rhs.m_w + m_x * rhs.m_x + m_y * rhs.m_y + m_z * rhs.m_z;
#endif
    }

    // Test for equality with another quaternion with epsilon.
    bool Equals(const Quaternion& rhs) const
    {
        return ctn::Equals(m_w, rhs.m_w) && ctn::Equals(m_x, rhs.m_x) && ctn::Equals(m_y, rhs.m_y) && ctn::Equals(m_z, rhs.m_z);
    }

    // Return whether is NaN.
    bool IsNaN() const { return ctn::IsNaN(m_w) || ctn::IsNaN(m_x) || ctn::IsNaN(m_y) || ctn::IsNaN(m_z); }

    // Return conjugate.
    Quaternion Conjugate() const
    {
#ifdef CTN_SSE
        __m128 q = _mm_loadu_ps(&m_w);
        return Quaternion(_mm_xor_ps(q, _mm_castsi128_ps(_mm_set_epi32((int)0x80000000UL, (int)0x80000000UL, (int)0x80000000UL, 0))));
#else
        return Quaternion(m_w, -m_x, -m_y, -m_z);
#endif
    }

    // Return Euler angles in degrees.
    Vector3 EulerAngles() const;
    // Return yaw angle in degrees.
    float YawAngle() const;
    // Return pitch angle in degrees.
    float PitchAngle() const;
    // Return roll angle in degrees.
    float RollAngle() const;
    // Return the rotation matrix that corresponds to this quaternion.
    Matrix3 RotationMatrix() const;
    // Spherical interpolation with another quaternion.
    Quaternion Slerp(Quaternion rhs, float t) const;
    // Normalized linear interpolation with another quaternion.
    Quaternion Nlerp(Quaternion rhs, float t, bool shortestPath = false) const;

    // Return float data.
    const float* Data() const { return &m_w; }

    // Return as string.
    String ToString() const;

    // W coordinate.
    float m_w;
    // X coordinate.
    float m_x;
    // Y coordinate.
    float m_y;
    // Z coordinate.
    float m_z;

    // Identity quaternion.
    static const Quaternion IDENTITY;
};

}
