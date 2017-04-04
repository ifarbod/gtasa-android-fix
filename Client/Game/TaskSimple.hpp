// Abstract simple task
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Task.hpp"

namespace ctn::SA
{

class CTaskSimple : public CTask
{
public:
    virtual bool ProcessPed(class CPed* ped) = 0;
    virtual bool SetPedPosition(class CPed* ped) = 0;
};

}
