// Rotation represented as a four-dimensional normalized vector
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include <Math/Quaternion.hpp>

namespace ctn
{

const Quaternion Quaternion::IDENTITY;

void Quaternion::FromAngleAxis(float angle, const Vector3& axis)
{
    Vector3 normAxis = axis.Normalized();
    angle *= M_DEGTORAD_2;
    float sinAngle = sinf(angle);
    float cosAngle = cosf(angle);

    m_w = cosAngle;
    m_x = normAxis.m_x * sinAngle;
    m_y = normAxis.m_y * sinAngle;
    m_z = normAxis.m_z * sinAngle;
}

void Quaternion::FromEulerAngles(float x, float y, float z)
{
    // Order of rotations: Z first, then X, then Y (mimics typical FPS camera with gimbal lock at top/bottom)
    x *= M_DEGTORAD_2;
    y *= M_DEGTORAD_2;
    z *= M_DEGTORAD_2;
    float sinX = sinf(x);
    float cosX = cosf(x);
    float sinY = sinf(y);
    float cosY = cosf(y);
    float sinZ = sinf(z);
    float cosZ = cosf(z);

    m_w = cosY * cosX * cosZ + sinY * sinX * sinZ;
    m_x = cosY * sinX * cosZ + sinY * cosX * sinZ;
    m_y = sinY * cosX * cosZ - cosY * sinX * sinZ;
    m_z = cosY * cosX * sinZ - sinY * sinX * cosZ;
}

void Quaternion::FromRotationTo(const Vector3& start, const Vector3& end)
{
    Vector3 normStart = start.Normalized();
    Vector3 normEnd = end.Normalized();
    float d = normStart.DotProduct(normEnd);

    if (d > -1.0f + M_EPSILON)
    {
        Vector3 c = normStart.CrossProduct(normEnd);
        float s = sqrtf((1.0f + d) * 2.0f);
        float invS = 1.0f / s;

        m_x = c.m_x * invS;
        m_y = c.m_y * invS;
        m_z = c.m_z * invS;
        m_w = 0.5f * s;
    }
    else
    {
        Vector3 axis = Vector3::RIGHT.CrossProduct(normStart);
        if (axis.Length() < M_EPSILON)
            axis = Vector3::UP.CrossProduct(normStart);

        FromAngleAxis(180.f, axis);
    }
}

void Quaternion::FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
{
    Matrix3 matrix(
        xAxis.m_x, yAxis.m_x, zAxis.m_x,
        xAxis.m_y, yAxis.m_y, zAxis.m_y,
        xAxis.m_z, yAxis.m_z, zAxis.m_z
    );

    FromRotationMatrix(matrix);
}

void Quaternion::FromRotationMatrix(const Matrix3& matrix)
{
    float t = matrix.m00_ + matrix.m11_ + matrix.m22_;

    if (t > 0.0f)
    {
        float invS = 0.5f / sqrtf(1.0f + t);

        m_x = (matrix.m21_ - matrix.m12_) * invS;
        m_y = (matrix.m02_ - matrix.m20_) * invS;
        m_z = (matrix.m10_ - matrix.m01_) * invS;
        m_w = 0.25f / invS;
    }
    else
    {
        if (matrix.m00_ > matrix.m11_ && matrix.m00_ > matrix.m22_)
        {
            float invS = 0.5f / sqrtf(1.0f + matrix.m00_ - matrix.m11_ - matrix.m22_);

            m_x = 0.25f / invS;
            m_y = (matrix.m01_ + matrix.m10_) * invS;
            m_z = (matrix.m20_ + matrix.m02_) * invS;
            m_w = (matrix.m21_ - matrix.m12_) * invS;
        }
        else if (matrix.m11_ > matrix.m22_)
        {
            float invS = 0.5f / sqrtf(1.0f + matrix.m11_ - matrix.m00_ - matrix.m22_);

            m_x = (matrix.m01_ + matrix.m10_) * invS;
            m_y = 0.25f / invS;
            m_z = (matrix.m12_ + matrix.m21_) * invS;
            m_w = (matrix.m02_ - matrix.m20_) * invS;
        }
        else
        {
            float invS = 0.5f / sqrtf(1.0f + matrix.m22_ - matrix.m00_ - matrix.m11_);

            m_x = (matrix.m02_ + matrix.m20_) * invS;
            m_y = (matrix.m12_ + matrix.m21_) * invS;
            m_z = 0.25f / invS;
            m_w = (matrix.m10_ - matrix.m01_) * invS;
        }
    }
}

bool Quaternion::FromLookRotation(const Vector3& direction, const Vector3& upDirection)
{
    Quaternion ret;
    Vector3 forward = direction.Normalized();

    Vector3 v = forward.CrossProduct(upDirection);
    // If direction & upDirection are parallel and crossproduct becomes zero, use FromRotationTo() fallback
    if (v.LengthSquared() >= M_EPSILON)
    {
        v.Normalize();
        Vector3 up = v.CrossProduct(forward);
        Vector3 right = up.CrossProduct(forward);
        ret.FromAxes(right, up, forward);
    }
    else
        ret.FromRotationTo(Vector3::FORWARD, forward);

    if (!ret.IsNaN())
    {
        (*this) = ret;
        return true;
    }
    else
        return false;
}

Vector3 Quaternion::EulerAngles() const
{
    // Derivation from http://www.geometrictools.com/Documentation/EulerAngles.pdf
    // Order of rotations: Z first, then X, then Y
    float check = 2.0f * (-m_y * m_z + m_w * m_x);

    if (check < -0.995f)
    {
        return Vector3(
            -90.0f,
            0.0f,
            -atan2f(2.0f * (m_x * m_z - m_w * m_y), 1.0f - 2.0f * (m_y * m_y + m_z * m_z)) * M_RADTODEG
        );
    }
    else if (check > 0.995f)
    {
        return Vector3(
            90.0f,
            0.0f,
            atan2f(2.0f * (m_x * m_z - m_w * m_y), 1.0f - 2.0f * (m_y * m_y + m_z * m_z)) * M_RADTODEG
        );
    }
    else
    {
        return Vector3(
            asinf(check) * M_RADTODEG,
            atan2f(2.0f * (m_x * m_z + m_w * m_y), 1.0f - 2.0f * (m_x * m_x + m_y * m_y)) * M_RADTODEG,
            atan2f(2.0f * (m_x * m_y + m_w * m_z), 1.0f - 2.0f * (m_x * m_x + m_z * m_z)) * M_RADTODEG
        );
    }
}

float Quaternion::YawAngle() const
{
    return EulerAngles().m_y;
}

float Quaternion::PitchAngle() const
{
    return EulerAngles().m_x;
}

float Quaternion::RollAngle() const
{
    return EulerAngles().m_z;
}

Matrix3 Quaternion::RotationMatrix() const
{
    return Matrix3(
        1.0f - 2.0f * m_y * m_y - 2.0f * m_z * m_z,
        2.0f * m_x * m_y - 2.0f * m_w * m_z,
        2.0f * m_x * m_z + 2.0f * m_w * m_y,
        2.0f * m_x * m_y + 2.0f * m_w * m_z,
        1.0f - 2.0f * m_x * m_x - 2.0f * m_z * m_z,
        2.0f * m_y * m_z - 2.0f * m_w * m_x,
        2.0f * m_x * m_z - 2.0f * m_w * m_y,
        2.0f * m_y * m_z + 2.0f * m_w * m_x,
        1.0f - 2.0f * m_x * m_x - 2.0f * m_y * m_y
    );
}

Quaternion Quaternion::Slerp(Quaternion rhs, float t) const
{
    // Favor accuracy for native code builds
    float cosAngle = DotProduct(rhs);
    // Enable shortest path rotation
    if (cosAngle < 0.0f)
    {
        cosAngle = -cosAngle;
        rhs = -rhs;
    }

    float angle = acosf(cosAngle);
    float sinAngle = sinf(angle);
    float t1, t2;

    if (sinAngle > 0.001f)
    {
        float invSinAngle = 1.0f / sinAngle;
        t1 = sinf((1.0f - t) * angle) * invSinAngle;
        t2 = sinf(t * angle) * invSinAngle;
    }
    else
    {
        t1 = 1.0f - t;
        t2 = t;
    }

    return *this * t1 + rhs * t2;
}

Quaternion Quaternion::Nlerp(Quaternion rhs, float t, bool shortestPath) const
{
    Quaternion result;
    float fCos = DotProduct(rhs);
    if (fCos < 0.0f && shortestPath)
        result = (*this) + (((-rhs) - (*this)) * t);
    else
        result = (*this) + ((rhs - (*this)) * t);
    result.Normalize();
    return result;
}

String Quaternion::ToString() const
{
    return String::Format("%g %g %g %g", m_w, m_x, m_y, m_z);
}

}
