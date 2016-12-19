// Condition on which a thread can wait
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include <Foundation/Condition.hpp>

#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

namespace Util
{

#ifdef _WIN32

Condition::Condition() :
    event_(nullptr)
{
    event_ = CreateEvent(0, FALSE, FALSE, 0);
}

Condition::~Condition()
{
    CloseHandle((HANDLE)event_);
    event_ = nullptr;
}

void Condition::Set()
{
    SetEvent((HANDLE)event_);
}

void Condition::Wait()
{
    WaitForSingleObject((HANDLE)event_, INFINITE);
}

#else

Condition::Condition() :
    mutex_(new pthread_mutex_t),
    event_(new pthread_cond_t)
{
    pthread_mutex_init((pthread_mutex_t*)mutex_, 0);
    pthread_cond_init((pthread_cond_t*)event_, 0);
}

Condition::~Condition()
{
    pthread_cond_t* cond = (pthread_cond_t*)event_;
    pthread_mutex_t* mutex = (pthread_mutex_t*)mutex_;

    pthread_cond_destroy(cond);
    pthread_mutex_destroy(mutex);
    delete cond;
    delete mutex;
    event_ = nullptr;
    mutex_ = nullptr;
}

void Condition::Set()
{
    pthread_cond_signal((pthread_cond_t*)event_);
}

void Condition::Wait()
{
    pthread_cond_t* cond = (pthread_cond_t*)event_;
    pthread_mutex_t* mutex = (pthread_mutex_t*)mutex_;

    pthread_mutex_lock(mutex);
    pthread_cond_wait(cond, mutex);
    pthread_mutex_unlock(mutex);
}

#endif

}
