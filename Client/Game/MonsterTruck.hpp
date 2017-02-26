// Monster truck vehicle entity
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Automobile.hpp"

namespace ctn::SA
{

class CMonsterTruck : public CAutomobile
{
public:
    f32 m_field988; // +0x988
    f32 m_field98C; // +0x98C
    f32 m_field990; // +0x990
    f32 m_field994; // +0x994
    f32 m_field998; // +0x998
};

//VALIDATE_SIZE(CMonsterTruck, 0x99C);

}
