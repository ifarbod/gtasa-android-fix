// Condition on which a thread can wait
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

namespace ctn
{

// Condition on which a thread can wait.
class Condition
{
public:
    // Construct.
    Condition();

    // Destruct.
    ~Condition();

    // Set the condition. Will be automatically reset once a waiting thread wakes up.
    void Set();

    // Wait on the condition.
    void Wait();

private:
#ifndef _WIN32
    // Mutex for the event, necessary for pthreads-based implementation.
    void* mutex_;
#endif
    // Operating system specific event.
    void* event_;
};

}
