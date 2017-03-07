// W32Thread.cpp
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

/** @file W32Thread.cpp
    @brief */

#include <cassert>
#include <exception>

#include "kNet/Thread.hpp"
#include "kNet/NetworkLogging.hpp"
#include "kNet/Clock.hpp"
#include "kNet/NetException.hpp"
#include "kNet/Network.hpp"

#include "kNet/DebugMemoryLeakCheck.hpp"

namespace kNet
{

Thread::Thread()
:threadHandle(NULL),
threadId(0),
threadEnabled(false),
invoker(0)
{
}

Thread::~Thread()
{
    Stop();
    delete invoker;
}

bool Thread::ShouldQuit() const { return threadHandle == NULL || threadEnabled == false; }

bool Thread::IsRunning() const
{
    if (threadHandle == NULL)
        return false;

    DWORD exitCode = 0;
    BOOL result = GetExitCodeThread(threadHandle, &exitCode);

    if (result == 0)
    {
        KNET_LOG(LogError, "Warning: Received error %d from GetExitCodeThread in Thread::IsRunning!", GetLastError());
        return false;
    }

    return exitCode == STILL_ACTIVE;
}

void Thread::Stop()
{
    // Signal that the thread should quit now.
    threadEnabled = false;

    if (threadHandle == NULL)
    {
        threadHoldEvent.Close();
        threadHoldEventAcked.Close();
        threadResumeEvent.Close();

        delete invoker;
        invoker = 0;
        return;
    }

    kNet::Clock::Sleep(10);
    assert(threadHandle != 0);

    int numTries = 100;
    while(numTries-- > 0)
    {
        DWORD exitCode = 0;
        BOOL result = GetExitCodeThread(threadHandle, &exitCode);

        if (result == 0)
        {
            KNET_LOG(LogError, "Warning: Received error %d from GetExitCodeThread in Thread::Stop()!", GetLastError());
            break;
        }
        else if (exitCode != STILL_ACTIVE)
        {
            CloseHandle(threadHandle);
            threadHandle = NULL;
            break;
        }
        kNet::Clock::Sleep(50);
    }

    if (threadHandle != NULL)
    {
        TerminateThread(threadHandle, (DWORD)-1);
//        CloseHandle(threadHandle);
        KNET_LOG(LogError, "Warning: Had to forcibly terminate thread!");
    }

    KNET_LOG(LogInfo, "Thread::Stop() called.");

    threadHandle = NULL;
    threadId = 0;

    delete invoker;
    invoker = 0;

    threadHoldEvent.Close();
    threadHoldEventAcked.Close();
    threadResumeEvent.Close();
}

DWORD WINAPI ThreadEntryPoint(LPVOID lpParameter)
{
    KNET_LOG(LogInfo, "ThreadEntryPoint: Thread started with param 0x%08X.", lpParameter);

    Thread *thread = reinterpret_cast<Thread*>(lpParameter);
    if (!thread)
    {
        KNET_LOG(LogError, "Invalid thread start parameter 0!");
        return (DWORD)-1;
    }
    thread->_ThreadRun();

    return 0;
}

void Thread::_ThreadRun()
{
    try
    {
        if (!threadEnabled)
        {
            KNET_LOG(LogError, "ThreadEntryPoint: Thread immediately requested to quit.");
            return;
        }

        invoker->Invoke();
    } catch(NetException &e)
    {
        KNET_LOG(LogError, "NetException thrown in thread: %s.", e.what());
    } catch(std::exception &e)
    {
        KNET_LOG(LogError, "std::exception thrown in thread: %s.", e.what());
    } catch(...)
    {
        KNET_LOG(LogError, "Unknown exception thrown in thread.");
    }
}

void Thread::StartThread()
{
    if (threadHandle != NULL)
        return;

    threadHoldEvent = CreateNewEvent(EventWaitSignal);
    threadHoldEventAcked = CreateNewEvent(EventWaitSignal);
    threadResumeEvent = CreateNewEvent(EventWaitSignal);

    threadEnabled = true;
    threadHandle = CreateThread(NULL, 0, ThreadEntryPoint, this, 0, &threadId);
    if (threadHandle == NULL)
        throw NetException("Failed to create thread!");
    else
        KNET_LOG(LogInfo, "Thread::Run(): Thread created.");

    SetName("kNet Thread");
}

void Thread::Sleep(int msecs)
{
    ///\todo Allow interruption between sleep.
    Clock::Sleep(msecs);
}

ThreadId Thread::Id()
{
    return threadId;
}

ThreadId Thread::CurrentThreadId()
{
    return GetCurrentThreadId();
}

ThreadId Thread::NullThreadId()
{
    return 0;
}

} // ~kNet
