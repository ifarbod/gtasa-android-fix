// Vehicle entity base
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Physical.hpp"

namespace ctn::SA
{

class CVehicle : public CPhysical
{
public:
    // Memory allocation in pool.
    void* operator new(u32 size);
    // Memory deallocation in pool.
    void operator delete(void* ptr);
};

//VALIDATE_SIZE(CVehicle, 0x5A0);

}
