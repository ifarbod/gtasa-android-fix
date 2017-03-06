// NetworkMessage.cpp
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

/** @file NetworkMessage.cpp
    @brief Represents a serializable network message. */

#include <string.h>

#include "kNet/DebugMemoryLeakCheck.hpp"
#include "kNet/NetworkMessage.hpp"

namespace kNet
{

NetworkMessage::NetworkMessage() :
    data(0),
    priority(0),
    id(0),
    contentID(0),
    reliable(true),
    inOrder(true),
    obsolete(false),
    receivedPacketID(0),
    messageNumber(0),
    reliableMessageNumber(0),
    sendCount(0),
    fragmentIndex(0),
    dataCapacity(0),
    dataSize(0),
    transfer(0)
{
}

NetworkMessage::NetworkMessage(const NetworkMessage &rhs)
{
    *this = rhs;
}

NetworkMessage &NetworkMessage::operator=(const NetworkMessage &rhs)
{
    if (this == &rhs)
        return *this;

    Resize(rhs.Size());
    memcpy(data, rhs.data, rhs.Size());
    priority = rhs.priority;
    id = rhs.id;
    contentID = rhs.contentID;
    reliable = rhs.reliable;
    inOrder = rhs.inOrder;
    obsolete = rhs.obsolete;

    // We could also copy the remaining fields messageNumber, reliableMessageNumber, sendCount and fragmentIndex,
    // but those don't have a specified meaning at the moment the message is being crafted, so don't.
    // Once the message has been queued for sending, deep copies of it will not be performed.

    return *this;
}

NetworkMessage::~NetworkMessage()
{
    delete[] data;
}

void NetworkMessage::Resize(size_t newBytes, bool discard)
{
    // Remember how much data is actually being used.
    dataSize = newBytes;

    if (newBytes <= dataCapacity)
        return; // No need to reallocate, we can fit the requested amount of bytes.

    char *newData = new char[newBytes];
    if (!discard)
        memcpy(newData, data, dataCapacity);

    delete[] data;
    data = newData;
    dataCapacity = newBytes;
}

} // ~kNet
