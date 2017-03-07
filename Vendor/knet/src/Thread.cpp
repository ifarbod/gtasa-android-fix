// Thread.cpp
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

/** @file Thread.cpp
    @brief Implements platform-generic Thread functions. */

#include <sstream>

#include "kNet/DebugMemoryLeakCheck.hpp"
#include "kNet/Event.hpp" ///\todo Investigate the inclusion chain of these two files. Is this #include necessary?
#include "kNet/NetworkLogging.hpp"
#include "kNet/Thread.hpp"
#include "kNet/PolledTimer.hpp"

namespace kNet
{

std::string ThreadIdToString(const ThreadId &id)
{
    std::stringstream ss;
    ss << id;
    return ss.str();
}

/// Suspends the thread until 'Resume()' is called. Call this function from the main thread.
void Thread::Hold()
{
    if (threadHoldEvent.Test())
        return;

    threadResumeEvent.Reset();
    threadHoldEvent.Reset();
    threadHoldEventAcked.Reset();

    threadHoldEvent.Set();

    PolledTimer timer;
    while(IsRunning())
    {
        bool success = threadHoldEventAcked.Wait(1000);
        if (success)
            break;
    }
    KNET_LOG(LogWaits, "Thread::Hold: Took %f msecs.", timer.MSecsElapsed());
}

/// Resumes the thread that is being held.
void Thread::Resume()
{
    threadResumeEvent.Set();
    threadHoldEvent.Reset();
    threadHoldEventAcked.Reset();
}

void Thread::CheckHold()
{
    if (threadHoldEvent.Test())
    {
        KNET_LOG(LogVerbose, "Thread::CheckHold(): suspending thread. this: %p.", this);

        PolledTimer timer;
        while(!ShouldQuit())
        {
            threadHoldEventAcked.Set();
            bool success = threadResumeEvent.Wait(1000);
            if (success)
                break;
        }
        KNET_LOG(LogWaits, "Thread::CheckHold: Slept for %f msecs.", timer.MSecsElapsed());
        threadHoldEventAcked.Reset();
    }
}

// This code adapted from http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx "How to: Set a Thread Name in Native Code":
#ifdef _WIN32
const DWORD MS_VC_EXCEPTION=0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
    DWORD dwType; // Must be 0x1000.
    LPCSTR szName; // Pointer to name (in user addr space).
    DWORD dwThreadID; // Thread ID (-1=caller thread).
    DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

#ifdef _MSC_VER
void SetThreadName(DWORD dwThreadID, const char *threadName)
{
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = threadName;
    info.dwThreadID = dwThreadID;
    info.dwFlags = 0;

    __try
    {
        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
//    __except(EXCEPTION_EXECUTE_HANDLER)
    __except(EXCEPTION_CONTINUE_EXECUTION)
    {
    }
}
#endif
#endif

void Thread::SetName(const char *name)
{
// The thread name can only be set when it is ensured that Thread::Id() returns the proper Win32 thread ID
#ifdef _MSC_VER // win32?
    SetThreadName(Id(), name);
#endif
}

} // ~kNet
