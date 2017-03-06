// Forward-declares all common symbols defined by kNet.
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

namespace kNet
{

class Clock;
class DataDeserializer;
struct Datagram;
class DataSerializer;
struct EndPoint;
class Event;
class EventArray;
class IMessageHandler;
class INetworkServerListener;
class MessageConnection;
class MessageListParser;
class NetException;
class Network;
class NetworkMessage;
class NetworkServer;
struct OverlappedTransferBuffer;
class PolledTimer;
class SerializationStructCompiler;
class SerializedDataIterator;
class Socket;
class StatsEventHierarchyNode;
class TCPMessageConnection;
class Thread;
class UDPMessageConnection;

}
