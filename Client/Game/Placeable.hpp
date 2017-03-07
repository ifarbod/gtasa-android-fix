// GTA Placeable entity base
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "SimpleTransform.hpp"
#include "MatrixLink.hpp"

namespace ctn::SA
{

class CPlaceable
{
public:
    virtual ~CPlaceable() = 0;

public:
    CSimpleTransform m_position;
    CMatrixLink* m_matrix;
};

VALIDATE_SIZE(CPlaceable, 0x18);

}
