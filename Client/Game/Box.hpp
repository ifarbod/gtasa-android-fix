// Box
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Vector.hpp"

namespace ctn::SA
{

class CBox
{
public:
    // Default constructor
    CBox() :
        m_supremum(CVector::ONE),
        m_infimum(-CVector::ONE)
    {}

    // Initialize this box using two points.
    void Set(const CVector& sup, const CVector& inf);
    // Recalculates box corners - if left > right then Swap(left, right).
    void Recalculate();

private:
    // Supremum vertex (contains largest values)
    CVector m_supremum; // +0x0
    // Infimum vertex (contains smallest values)
    CVector m_infimum;  // +0xC
};

VALIDATE_SIZE(CBox, 0x18);

}
