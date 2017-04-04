// Box
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Box.hpp"

namespace ctn::SA
{

void CBox::Set(const CVector& sup, const CVector& inf)
{
    m_supremum = sup;
    m_infimum = inf;
}

void CBox::Recalculate()
{
    if (m_supremum.m_x > m_infimum.m_x)
    {
        Swap<f32>(m_infimum.m_x, m_supremum.m_x);
    }
    if (m_supremum.m_y > m_infimum.m_y)
    {
        Swap<f32>(m_infimum.m_y, m_supremum.m_y);
    }
    if (m_supremum.m_z > m_infimum.m_z)
    {
        Swap<f32>(m_infimum.m_z, m_supremum.m_z);
    }
}

}
