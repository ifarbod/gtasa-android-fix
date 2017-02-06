// Abstract complex task
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Task.hpp"

namespace ctn::SA
{

class CTaskComplex : public CTask
{
public:
    virtual void SetSubTask(CTask* subTask) = 0;
    virtual CTask* CreateNextSubTask(class CPed* ped) = 0;
    virtual CTask* CreateFirstSubTask(class CPed* ped) = 0;
    virtual CTask* ControlSubTask(class CPed* ped) = 0;

public:
    CTask* subTask_;
};

}
