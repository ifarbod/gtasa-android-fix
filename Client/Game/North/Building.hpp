// Building.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Entity.hpp"

namespace ctn::SA
{

class CBuilding : public CEntity
{
public:
    void* operator new(u32 size);
    void operator delete(void* ptr);
};

VALIDATE_SIZE(CBuilding, 0x38);

}
