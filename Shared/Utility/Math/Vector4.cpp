// Four-dimensional vector
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include <Math/Vector4.hpp>

#include <DebugNew.hpp>

namespace ctn
{

const Vector4 Vector4::ZERO;
const Vector4 Vector4::ONE(1.0f, 1.0f, 1.0f, 1.0f);

String Vector4::ToString() const
{
    return String::Format("%g %g %g %g", m_x, m_y, m_z, m_w);
}

}
