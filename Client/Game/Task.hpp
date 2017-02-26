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

    // Destruct this task.
    virtual ~CTask() = 0;
    // Clone this task (like a copy constructor).
    virtual class CTask* Clone() = 0;
    // Get sub task.
    virtual class CTask* GetSubTask() = 0;
    // Returns true if this is a CTaskSimple, false if CTaskComplex.
    virtual bool IsSimpleTask() = 0;
    // Return task's ID.
    virtual enum eTaskType GetTaskType() = 0;
    // Stop task's underlying timer.
    virtual void StopTimer(class CEvent* _event) = 0;
    // Abort simple task.
    virtual bool MakeAbortable(class CPed* ped, int priority, class CEvent* _event) = 0;

public:
    // Parent task.
    class CTask* m_parentTask;
};

VALIDATE_SIZE(CTask, 0x8);

}
