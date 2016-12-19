// Static hook installer
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

namespace Util
{

// Initialization function that will be called after the game is loaded.
class HookFunctionBase
{
private:
    HookFunctionBase* next_;

public:
    HookFunctionBase() { Register(); }

    virtual void Run() = 0;

    static void RunAll();
    void Register();
};

class HookFunction : public HookFunctionBase
{
private:
    void (*function_)();

public:
    HookFunction(void (*function)()) { function_ = function; }

    virtual void Run() { function_(); }
};

}
