// GTA Matrix link
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

namespace ctn::SA
{

class CMatrixLink : public CMatrix
{
public:
    class CPlaceable* owner_;
    CMatrixLink* prev_;
    CMatrixLink* next_;
};
VALIDATE_SIZE(CMatrixLink, 0x54);

}
