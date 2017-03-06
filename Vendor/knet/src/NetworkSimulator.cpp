// NetworkSimulator.cpp
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "kNet/MessageConnection.hpp"

namespace kNet
{

NetworkSimulator::NetworkSimulator() :
    enabled(false),
    packetLossRate(0),
    constantPacketSendDelay(0),
    uniformRandomPacketSendDelay(0),
    packetDuplicationRate(0.f),
    corruptionType(CorruptDatagram),
    corruptMessageId(0),
    corruptToggleBitsRate(0),
    corruptMinBits(0),
    corruptMaxBits(0),
    owner(0)
{
}

NetworkSimulator::~NetworkSimulator()
{
    if (queuedBuffers.size() > 0)
        KNET_LOG(LogError, "NetworkSimulator: Leaked %d buffers with improper NetworkSimulator teardown!", (int)queuedBuffers.size());
}

/// Generates a float in half-open interval [0, 1[.
static float rand01()
{
    assert(RAND_MAX >= 0x7FFF); ///\todo static assert.
    return (float)(rand()&0x7FFF) / 0x8000;
}

/// Generates a float in closed interval [0, 1].
static float rand01incl()
{
    assert(RAND_MAX >= 0x7FFF); ///\todo static assert.
    return (float)(rand()&0x7FFF) / 0x7FFF;
}

void NetworkSimulator::Free()
{
    if (!owner || !owner->GetSocket())
        return;

    for(size_t i = 0; i < queuedBuffers.size(); ++i)
        owner->GetSocket()->AbortSend(queuedBuffers[i].buffer);
    queuedBuffers.clear();
}

void NetworkSimulator::SubmitSendBuffer(kNet::OverlappedTransferBuffer *buffer, Socket *socket)
{
    if (rand01() < packetLossRate)
    {
        if (owner && owner->GetSocket())
            owner->GetSocket()->AbortSend(buffer);
        return; // Dropped this packet!
    }

    // Should we duplicate this packet?
    if (rand01() < packetDuplicationRate)
    {
        QueuedBuffer b;
        assert(socket);
        b.buffer = socket->BeginSend(buffer->bytesContains);
        if (b.buffer)
        {
            assert(b.buffer->buffer.len >= (u32)buffer->bytesContains);
            memcpy(b.buffer->buffer.buf, buffer->buffer.buf, buffer->bytesContains);
            b.buffer->bytesContains = buffer->bytesContains;

            // Should we corrupt the newly created copy?
            if (corruptionType == CorruptDatagram)
                MaybeCorruptBufferToggleBits(b.buffer->buffer.buf, b.buffer->bytesContains);

            b.timeUntilTransfer.StartMSecs(constantPacketSendDelay + rand01incl() * uniformRandomPacketSendDelay);
            queuedBuffers.push_back(b);
        }
    }

    // Should we corrupt this packet?
    if (corruptionType == CorruptDatagram)
        MaybeCorruptBufferToggleBits(buffer->buffer.buf, buffer->bytesContains);

    QueuedBuffer b;
    b.buffer = buffer;
    b.timeUntilTransfer.StartMSecs(constantPacketSendDelay + rand01incl() * uniformRandomPacketSendDelay);
    queuedBuffers.push_back(b);
}

void NetworkSimulator::Process()
{
    for(size_t i = 0; i < queuedBuffers.size(); ++i)
        if (queuedBuffers[i].timeUntilTransfer.Test())
        {
            if (owner && owner->GetSocket())
                owner->GetSocket()->EndSend(queuedBuffers[i].buffer);
            queuedBuffers.erase(queuedBuffers.begin() + i);
            --i;
        }
}

void NetworkSimulator::MaybeCorruptBufferToggleBits(void *buffer, size_t numBytes) const
{
    // Should corrupt this data?
    if (rand01() < corruptToggleBitsRate)
    {
        int numBitsToCorrupt = corruptMinBits + (int)(rand01() * (corruptMaxBits - corruptMinBits + 1));
        for(int i = 0; i < numBitsToCorrupt; ++i)
        {
            int byteIndex = (int)(rand01() * numBytes);
            int bitIndex = rand() % 8;
            int bitMask = (1 << bitIndex);
            ((char*)buffer)[byteIndex] ^= bitMask;
        }
    }
}

} // ~kNet
