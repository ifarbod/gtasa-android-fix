// Vehicle entity base
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Physical.hpp"
#include "AEVehicleAudioEntity.hpp"

namespace ctn::SA
{

class CVehicle : public CPhysical
{
public:
    // Memory allocation in pool.
    void* operator new(u32 size);
    // Memory deallocation in pool.
    void operator delete(void* ptr);

public:
    CAEVehicleAudioEntity m_vehicleAE;      // +0x138
    struct tHandlingData* m_handlingData;   // +0x384
    struct tFlyingHandlingData* m_flyingHandlingData; // +0x388
};

//VALIDATE_SIZE(CVehicle, 0x5A0);

}
