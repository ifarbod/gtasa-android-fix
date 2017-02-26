// Static hook installer
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

namespace ctn
{

namespace Hook
{

// Initialization function that will be called after the game is loaded.
class HookFunctionBase
{
private:
    HookFunctionBase* m_next;

public:
    HookFunctionBase() { Register(); }

    virtual void Run() = 0;

    static void RunAll();
    void Register();
};

class HookFunction : public HookFunctionBase
{
private:
    void(*m_function)();

public:
    HookFunction(void(*function)()) { m_function = function; }

    virtual void Run() { m_function(); }
};

}

}
