// 3x3 matrix for rotation and scaling
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include <Math/Matrix3.hpp>

namespace Util
{

const Matrix3 Matrix3::ZERO(
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f);

const Matrix3 Matrix3::IDENTITY;

Matrix3 Matrix3::Inverse() const
{
    float det = m00_ * m11_ * m22_ +
        m10_ * m21_ * m02_ +
        m20_ * m01_ * m12_ -
        m20_ * m11_ * m02_ -
        m10_ * m01_ * m22_ -
        m00_ * m21_ * m12_;

    float invDet = 1.0f / det;

    return Matrix3(
        (m11_ * m22_ - m21_ * m12_) * invDet,
        -(m01_ * m22_ - m21_ * m02_) * invDet,
        (m01_ * m12_ - m11_ * m02_) * invDet,
        -(m10_ * m22_ - m20_ * m12_) * invDet,
        (m00_ * m22_ - m20_ * m02_) * invDet,
        -(m00_ * m12_ - m10_ * m02_) * invDet,
        (m10_ * m21_ - m20_ * m11_) * invDet,
        -(m00_ * m21_ - m20_ * m01_) * invDet,
        (m00_ * m11_ - m10_ * m01_) * invDet
    );
}

String Matrix3::ToString() const
{
    return String::Format("%g %g %g %g %g %g %g %g %g", m00_, m01_, m02_, m10_, m11_, m12_, m20_, m21_, m22_);
}

}
