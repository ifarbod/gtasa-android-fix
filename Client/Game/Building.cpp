// Building.cpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Building.hpp"

namespace ctn::SA
{

void* CBuilding::operator new(u32 size)
{
    return nullptr;
}

void CBuilding::operator delete(void* ptr)
{
}

}
