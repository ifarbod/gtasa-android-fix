// GTA Placeable entity base
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
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
    CSimpleTransform position_;
    CMatrixLink* matrix_;
};
VALIDATE_SIZE(CPlaceable, 0x18);

}
