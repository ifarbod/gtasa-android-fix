// TCPMessageConnection.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file TCPMessageConnection.h
    @brief The TCPMessageConnection class.*/

#include "MessageConnection.hpp"
#include "RingBuffer.hpp"

/*
TCP stream format:
.Message.
.Message.
...
.Message.

Message format:
u8-u32 Whole Message length (including MessageID + ContentData, but not this field itself)  VLE-encoded 1.7/1.7/16
u8-u32 MessageID                  VLE-encoded 1.7/1.7/16
N bytes Content data

*/

namespace kNet
{

class TCPMessageConnection : public MessageConnection
{
public:
    TCPMessageConnection(Network *owner, NetworkServer *ownerServer, Socket *socket, ConnectionState startingState);
    ~TCPMessageConnection();

private:
    /// Maintains a byte buffer that contains partial messages. [worker thread]
    RingBuffer tcpInboundSocketData;

    /// Reads all available bytes from a stream socket.
    SocketReadResult ReadSocket(size_t &bytesRead); // [worker thread]

    PacketSendResult SendOutPacket(); // [worker thread]
    void SendOutPackets(); // [worker thread]

    void DoUpdateConnection(); // [worker thread]

    unsigned long TimeUntilCanSendPacket() const;

    /// Parses the raw inbound byte stream into messages. [used internally by worker thread]
    void ExtractMessages();

    // The following are temporary data structures used by various internal routines for processing.
    std::vector<NetworkMessage*> serializedMessages; // MessageConnection::TCPSendOutPacket()

    void PerformDisconnection();

    void DumpConnectionStatus() const; // [main thread]
};

} // ~kNet
