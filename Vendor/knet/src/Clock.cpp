// Clock.cpp
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

/** @file Clock.cpp
    @brief */

#if defined(__unix__) || defined(__native_client__) || defined(__EMSCRIPTEN__) || defined(__ANDROID__) || defined(__APPLE__) || defined (__CYGWIN__)
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include "kNet/Clock.hpp"
#include "kNet/NetworkLogging.hpp"

namespace kNet
{

#ifdef _WIN32
LARGE_INTEGER Clock::ddwTimerFrequency;
#endif

tick_t Clock::appStartTime = 0;

Clock impl;

void Clock::InitClockData()
{
    if (appStartTime == 0)
        appStartTime = Tick();

#ifdef _WIN32
    if (!QueryPerformanceFrequency(&ddwTimerFrequency))
    {
        KNET_LOG(LogError, "The system doesn't support high-resolution timers!");
        ddwTimerFrequency.HighPart = (unsigned long)-1;
        ddwTimerFrequency.LowPart = (unsigned long)-1;
    }

    if (ddwTimerFrequency.HighPart > 0)
        KNET_LOG(LogError, "Warning: Clock::TicksPerSec will yield invalid timing data!");

    if (appStartTime == 0)
    {
        appStartTime = static_cast<tick_t>(GetTickCount64());
    }

    ///\todo Test here that the return values of QueryPerformanceCounter is nondecreasing.
#endif
}

Clock::Clock()
{
    InitClockData();
}

void Clock::Sleep(int milliseconds)
{
#ifdef WIN8RT
#pragma WARNING(Clock::Sleep has not been implemented!)
#elif defined(_WIN32)
    ::Sleep(milliseconds);
#elif !defined(__native_client__) && !defined(__EMSCRIPTEN__) && !defined(__APPLE__)
    timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds - ts.tv_sec * 1000) * 1000 * 1000;
    int ret = nanosleep(&ts, NULL);
    if (ret == -1)
        KNET_LOG(LogError, "nanosleep returned -1! Reason: %s(%d).", strerror(errno), (int)errno);
#else
#warning Clock::Sleep has not been implemented!
#endif
}

int Clock::Year()
{
#ifdef _WIN32
    SYSTEMTIME s;
    GetSystemTime(&s);
    return s.wYear;
#else
    ///\todo.
    return 0;
#endif
}

int Clock::Month()
{
#ifdef _WIN32
    SYSTEMTIME s;
    GetSystemTime(&s);
    return s.wMonth;
#else
    ///\todo.
    return 0;
#endif
}

int Clock::Day()
{
#ifdef _WIN32
    SYSTEMTIME s;
    GetSystemTime(&s);
    return s.wDay;
#else
    ///\todo.
    return 0;
#endif
}

int Clock::Hour()
{
#ifdef _WIN32
    SYSTEMTIME s;
    GetSystemTime(&s);
    return s.wHour;
#else
    ///\todo.
    return 0;
#endif
}

int Clock::Min()
{
#ifdef _WIN32
    SYSTEMTIME s;
    GetSystemTime(&s);
    return s.wMinute;
#else
    ///\todo.
    return 0;
#endif
}

int Clock::Sec()
{
#ifdef _WIN32
    SYSTEMTIME s;
    GetSystemTime(&s);
    return s.wSecond;
#else
    ///\todo.
    return 0;
#endif
}

unsigned long Clock::SystemTime()
{
#ifdef _WIN32
    return (unsigned long)GetTickCount64();
#else
    return TickU32();
#endif
}
/*
tick_t Clock::ApplicationStartupTick()
{
    return appStartTime;
}
*/
unsigned long Clock::Time()
{
    return (unsigned long)(Tick() - appStartTime);
}

tick_t Clock::Tick()
{
#if defined(__ANDROID__)
    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000000000ULL*res.tv_sec + (tick_t)res.tv_nsec;
#elif defined(__EMSCRIPTEN__)
    // emscripten_get_now() returns a wallclock time as a float in milliseconds (1e-3).
    // scale it to microseconds (1e-6) and return as a tick.
    return (tick_t)(((double)emscripten_get_now()) * 1e3);
//    return (tick_t)clock();
#elif defined(_WIN32)
    LARGE_INTEGER ddwTimer;
    QueryPerformanceCounter(&ddwTimer);
    return ddwTimer.QuadPart;
#elif defined(_POSIX_MONOTONIC_CLOCK)
    timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (tick_t)t.tv_sec * 1000 * 1000 * 1000 + (tick_t)t.tv_nsec;
#elif defined(_POSIX_C_SOURCE) || defined(__APPLE__)
    timeval t;
    gettimeofday(&t, NULL);
    return (tick_t)t.tv_sec * 1000 * 1000 + (tick_t)t.tv_usec;
#else
    return (tick_t)clock();
#endif
}

unsigned long Clock::TickU32()
{
#ifdef _WIN32
    LARGE_INTEGER ddwTimer;
    QueryPerformanceCounter(&ddwTimer);
    return ddwTimer.LowPart;
#else
    return (unsigned long)Tick();
#endif
}

tick_t Clock::TicksPerSec()
{
#if defined(__ANDROID__)
    return 1000000000ULL; // 1e9 == nanoseconds.
#elif defined(__EMSCRIPTEN__)
    return 1000000ULL; // 1e6 == microseconds.
//    return CLOCKS_PER_SEC;
#elif defined(_WIN32)
    return ddwTimerFrequency.QuadPart;
#elif defined(_POSIX_MONOTONIC_CLOCK)
    return 1000 * 1000 * 1000;
#elif defined(_POSIX_C_SOURCE) || defined(__APPLE__)
    return 1000 * 1000;
#else
    return CLOCKS_PER_SEC;
#endif
}

} // ~kNet
