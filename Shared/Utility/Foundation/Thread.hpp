// Operating system thread
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#ifndef _WIN32
#include <pthread.h>
using ThreadID = pthread_t;
#else
using ThreadID = unsigned;
#endif

namespace Util
{

// Operating system thread.
class Thread
{
public:
    // Construct. Does not start the thread yet.
    Thread();
    // Destruct. If running, stop and wait for thread to finish.
    virtual ~Thread();

    // The function to run in the thread.
    virtual void ThreadFunction() = 0;

    // Start running the thread. Return true if successful, or false if already running or if can not create the thread.
    bool Run();
    // Set the running flag to false and wait for the thread to finish.
    void Stop();
    // Set thread priority. The thread must have been started first.
    void SetPriority(int priority);

    // Return whether thread exists.
    bool IsStarted() const { return handle_ != 0; }

    // Set the current thread as the main thread.
    static void SetMainThread();
    // Return the current thread's ID.
    static ThreadID GetCurrentThreadID();
    // Return whether is executing in the main thread.
    static bool IsMainThread();

protected:
    // Thread handle.
    void* handle_;
    // Running flag.
    volatile bool shouldRun_;

    // Main thread's thread ID.
    static ThreadID mainThreadID;
};

}
