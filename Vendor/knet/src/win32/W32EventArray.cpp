// W32EventArray.cpp
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

/** @file W32EventArray.cpp
    @brief */

#include <cassert>

#include "kNet/DebugMemoryLeakCheck.hpp"

#include "kNet/EventArray.hpp"
#include "kNet/NetworkLogging.hpp"

namespace kNet
{

EventArray::EventArray()
:numAdded(0)
{
}

void EventArray::Clear()
{
    numAdded = 0;
}

int EventArray::Size() const
{
    return numAdded;
}

void EventArray::AddEvent(const Event &e)
{
    if (e.IsNull())
    {
        KNET_LOG(LogError, "EventArray::AddEvent: Error! Tried to add a null event to event array at index %d!", numAdded);
        return;
    }
    if (numAdded >= maxEvents)
    {
        KNET_LOG(LogError, "EventArray::AddEvent: Error! Tried to add too many events to event array! (max: %d)", numAdded);
        return;
    }
    assert(numAdded < maxEvents);
    events[numAdded++] = e.wsaEvent;
}

int EventArray::Wait(int msecs)
{
    if (numAdded == 0)
        return WaitFailed;

    DWORD ret = WSAWaitForMultipleEvents(numAdded, events, FALSE, msecs, TRUE);
    if (ret >= WSA_WAIT_EVENT_0 && ret < WSA_WAIT_EVENT_0 + numAdded)
        return ret - WSA_WAIT_EVENT_0;
    else if (ret == WSA_WAIT_TIMEOUT)
        return WaitTimedOut;
    else
    {
        KNET_LOG(LogError, "EventArray::Wait: WSAWaitForMultipleEvents failed with error code %d (WSAGetLastError: %d)!", (int)ret, (int)WSAGetLastError());
        for(int i = 0; i < numAdded; ++i)
            if (WSAWaitForMultipleEvents(1, events+i, FALSE, 0, FALSE) == WSA_WAIT_FAILED)
                KNET_LOG(LogError, "EventArray::Wait: EventArray event at index %d is not valid!", i);

        return WaitFailed;
    }
}

} // ~kNet
