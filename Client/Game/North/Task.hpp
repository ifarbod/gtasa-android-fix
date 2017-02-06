// Abstract task
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"

namespace ctn::SA
{

class CTask
{
public:
    // Constructor.
    CTask() = delete;

    // Memory allocation in pool.
    void* operator new(u32 size);
    // Memory deallocation in pool.
    void operator delete(void* ptr);

    virtual ~CTask() = 0;
    virtual class CTask* Clone() = 0;
    virtual class CTask* GetSubTask() = 0;
    virtual bool IsSimpleTask() = 0;
    virtual enum eTaskType GetTaskType() = 0;
    virtual void StopTimer(class CEvent* _event) = 0;
    virtual bool MakeAbortable(class CPed* ped, int priority, class CEvent* _event) = 0;

public:
    class CTask* parentTask_;
};

VALIDATE_SIZE(CTask, 0x8);

}
