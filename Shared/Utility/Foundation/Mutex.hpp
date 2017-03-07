// Operating system mutual exclusion primitive
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

namespace ctn
{

// Operating system mutual exclusion primitive.
class Mutex
{
public:
    // Construct.
    Mutex();
    // Destruct.
    ~Mutex();

    // Acquire the mutex. Block if already acquired.
    void Acquire();
    // Try to acquire the mutex without locking. Return true if successful.
    bool TryAcquire();
    // Release the mutex.
    void Release();

private:
    // Mutex handle.
    void* handle_;
};

// Lock that automatically acquires and releases a mutex.
class MutexLock
{
public:
    // Construct and acquire the mutex.
    MutexLock(Mutex& mutex);
    // Destruct. Release the mutex.
    ~MutexLock();

private:
    // Prevent copy construction.
    MutexLock(const MutexLock& rhs);
    // Prevent assignment.
    MutexLock& operator =(const MutexLock& rhs) = default; // delete?

    // Mutex reference.
    Mutex& mutex_;
};

}
