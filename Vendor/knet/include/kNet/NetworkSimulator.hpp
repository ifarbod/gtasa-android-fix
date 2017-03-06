// NetworkSimulator.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file NetworkSimulator.h
    @brief The NetworkSimulator class, which enables different network conditions testing. */

#include "kNetFwd.hpp"
#include "PolledTimer.hpp"
#include "Types.hpp"
#include <vector>

#include <cstring>

namespace kNet
{

/// A NetworkSimulator is attached to MessageConnections to add in an intermediate layer for
/// network conditions testing.
class NetworkSimulator
{
public:
    NetworkSimulator();
    ~NetworkSimulator();

    /// If false, the network simulator is not being used.
    /// By default, this is always false.
    bool enabled;

    /// Specifies the percentage of messages to drop. This is in the range [0.0, 1.0]. Default: 0 (disabled).
    float packetLossRate;

    /// Specifies a constant delay to add to each packet (msecs). Default: 0.
    float constantPacketSendDelay;

    /// Specifies an amount of uniformly random delay to add to each packet (msecs), [0, uniformRandomPacketSendDelay].  Default: 0.
    float uniformRandomPacketSendDelay;

    /// Specifies the percentage of messages to duplicate. This is in the range [0.0, 1.0]. Default: 0 (disabled).
    float packetDuplicationRate;

    /// Corruption options.
    enum
    {
        CorruptDatagram, ///< The whole datagram is subjected to data corruption. This is the default.
        CorruptPayload, ///< Only kNet message payload (client-side data) will be subjected to corruption.
        CorruptMessageType ///< Only the message payload of a single given message type will be subjected to corruption.
    } corruptionType;

    /// If corruptionType == CorruptMessageType, this field specifies a single specific message type ID which
    /// will be subjected to corruption. Default: 0. No message has the ID 0, i.e. this effectively means "disabled".
    message_id_t corruptMessageId;

    /// Rate in % of the datagrams to corrupt. Default: 0.
    float corruptToggleBitsRate;

    /// The minimum number of bits to corrupt when a datagram is decided to be tampered. Default: 0.
    int corruptMinBits;

    /// The maximum number of bits to corrupt when a datagram is decided to be tampered. Default: 0.
    int corruptMaxBits;

    void SubmitSendBuffer(OverlappedTransferBuffer *buffer, Socket *socket);

    /// Runs a polled update tick on the network simulator. Transfers all expired data.
    void Process();

    /// Discards and frees all currently queued messages.
    void Free();

    /// Performs a random roll against the corruptToggleBitsRate counter, and perhaps corrupts some bits
    /// of the given buffer.
    /// Alters the raw byte buffer contents by flipping some bits according to the currently specified
    /// parameters.
    void MaybeCorruptBufferToggleBits(void *buffer, size_t numBytes) const;

private:
    struct QueuedBuffer
    {
        OverlappedTransferBuffer *buffer;

        /// Stores how long to delay this buffer until transfer.
        PolledTimer timeUntilTransfer;
    };
    std::vector<QueuedBuffer> queuedBuffers;

    MessageConnection *owner;

    friend class MessageConnection;
};

} // ~kNet

