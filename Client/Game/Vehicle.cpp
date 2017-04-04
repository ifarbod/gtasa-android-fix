// Vehicle.cpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Vehicle.hpp"

namespace ctn::SA
{

void* CVehicle::operator new(u32 size)
{
    return nullptr;
}

void CVehicle::operator delete(void* ptr)
{
}

}
