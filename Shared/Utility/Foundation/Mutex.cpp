// Operating system mutual exclusion primitive
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include <Foundation/Mutex.hpp>

#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

namespace ctn
{

#ifdef _WIN32

Mutex::Mutex() :
    handle_(new CRITICAL_SECTION)
{
    InitializeCriticalSection(reinterpret_cast<CRITICAL_SECTION*>(handle_));
}

Mutex::~Mutex()
{
    CRITICAL_SECTION* cs = reinterpret_cast<CRITICAL_SECTION*>(handle_);
    DeleteCriticalSection(cs);
    delete cs;
    handle_ = nullptr;
}

void Mutex::Acquire()
{
    EnterCriticalSection(reinterpret_cast<CRITICAL_SECTION*>(handle_));
}

bool Mutex::TryAcquire()
{
    return TryEnterCriticalSection(reinterpret_cast<CRITICAL_SECTION*>(handle_)) != FALSE;
}

void Mutex::Release()
{
    LeaveCriticalSection(reinterpret_cast<CRITICAL_SECTION*>(handle_));
}

#else

Mutex::Mutex() :
    handle_(new pthread_mutex_t)
{
    pthread_mutex_t* mutex = reinterpret_cast<pthread_mutex_t*>(handle_);
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(mutex, &attr);
}

Mutex::~Mutex()
{
    pthread_mutex_t* mutex = reinterpret_cast<pthread_mutex_t*>(handle_);
    pthread_mutex_destroy(mutex);
    delete mutex;
    handle_ = nullptr;
}

void Mutex::Acquire()
{
    pthread_mutex_lock(reinterpret_cast<pthread_mutex_t*>(handle_));
}

bool Mutex::TryAcquire()
{
    return pthread_mutex_trylock(reinterpret_cast<pthread_mutex_t*>(handle_)) == 0;
}

void Mutex::Release()
{
    pthread_mutex_unlock(reinterpret_cast<pthread_mutex_t*>(handle_));
}

#endif

MutexLock::MutexLock(Mutex& mutex) :
    mutex_(mutex)
{
    mutex_.Acquire();
}

MutexLock::~MutexLock()
{
    mutex_.Release();
}

}
