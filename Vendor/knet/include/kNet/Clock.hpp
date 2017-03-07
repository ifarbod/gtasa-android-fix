// Clock.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file Clock.h
    @brief The Clock class. Supplies timing facilities. */

#ifdef _WIN32
#include <windows.h>
#endif

namespace kNet
{

/// A tick is the basic unit of the high-resolution timer.
typedef unsigned long long tick_t;

/** @brief High-resolution timing and system time.

    Gives out timing information in various forms. Use this rather than
    any platform-dependent perf-counters or rdtsc or whatever.*/
class Clock
{
public:
    Clock();
//    ~Clock() {}

    /// Sleeps the current thread for the given amount of milliseconds.
    static void Sleep(int milliseconds);

    /// @return The current year.
    static int Year();

    /// @return The current month, [1,12]
    static int Month();

    /// @return The current day of month, [1,31]
    static int Day();

    /// @return The current hour of day, [0,23]
    static int Hour();

    /// @return The current clock minute, [0,59]
    static int Min();

    /// @return The current clock second, [0,59]
    static int Sec();

    /// @return The current system time counter in milliseconds.
    static unsigned long SystemTime();

    /// @return The number of milliseconds since application start.
    static unsigned long Time();

    /// @return The low part of the current tick-time (using whatever high-resolution counter available)
    static unsigned long TickU32();

    /// @return The current tick-time (using whatever high-resolution counter available)
    static tick_t Tick();

    /// @return How many ticks make up a second.
    static tick_t TicksPerSec();

    static inline tick_t TicksPerMillisecond() { return TicksPerSec() / 1000; }

    /// Returns the number of ticks occurring between the two wallclock times.
    static inline tick_t TicksInBetween(tick_t newTick, tick_t oldTick)
    {
        return (tick_t)(newTick - oldTick);
    }

    /// Returns true if newTick represents a later wallclock time than oldTick.
    static inline bool IsNewer(tick_t newTick, tick_t oldTick)
    {
        return TicksInBetween(newTick, oldTick) < ((tick_t)(-1) >> 1);
    }

    static inline float MillisecondsSinceF(tick_t oldTick) { return TimespanToMillisecondsF(oldTick, Tick()); }
    static inline double MillisecondsSinceD(tick_t oldTick) { return TimespanToMillisecondsD(oldTick, Tick()); }

    static inline float SecondsSinceF(tick_t oldTick) { return TimespanToSecondsF(oldTick, Tick()); }
    static inline double SecondsSinceD(tick_t oldTick) { return TimespanToSecondsD(oldTick, Tick()); }

    static inline float TicksToMillisecondsF(tick_t ticks) { return ticks * 1000.f / TicksPerSec(); }
    static inline double TicksToMillisecondsD(tick_t ticks) { return ticks * 1000.0 / TicksPerSec(); }

    static inline float TicksToSecondsF(tick_t ticks) { return ticks / (float)TicksPerSec(); }
    static inline double TicksToSecondsD(tick_t ticks) { return ticks / (double)TicksPerSec(); }

    static inline float TimespanToMillisecondsF(tick_t oldTick, tick_t newTick) { return TicksToMillisecondsF(TicksInBetween(newTick, oldTick)); }
    static inline double TimespanToMillisecondsD(tick_t oldTick, tick_t newTick) { return TicksToMillisecondsD(TicksInBetween(newTick, oldTick)); }

    static inline float TimespanToSecondsF(tick_t oldTick, tick_t newTick) { return TicksToSecondsF(TicksInBetween(newTick, oldTick)); }
    static inline double TimespanToSecondsD(tick_t oldTick, tick_t newTick) { return TicksToSecondsD(TicksInBetween(newTick, oldTick)); }

private:
    static tick_t appStartTime;      ///< Application startup time in ticks.

    /// Initializes clock tick frequency and marks the application startup time.
    static void InitClockData();

#ifdef _WIN32
    static LARGE_INTEGER ddwTimerFrequency; ///< Ticks per second.
    static LARGE_INTEGER ddwTimer;          ///< Temporary storage for Win32 function calls.
#endif
};

} // ~kNet
