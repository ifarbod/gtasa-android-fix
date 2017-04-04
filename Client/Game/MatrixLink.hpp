// GTA Matrix link
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

namespace ctn::SA
{

class CPlaceable;

class CMatrixLink : public CMatrix
{
public:
    CPlaceable* m_owner; // +0x48
    CMatrixLink* m_prev; // +0x4C
    CMatrixLink* m_next; // +0x50
};
VALIDATE_SIZE(CMatrixLink, 0x54);

}
