// The main file of kNet that #includes all the commonly used headers for the the
// client application. This file is ideal to be added to a PCH. If you are not using a PCH,
// #including the individual files when necessary is most likely faster.
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "kNetFwd.hpp"

#ifdef _WIN32
#include "kNet/win32/WS2Include.hpp"
#endif

#include "kNet/Clock.hpp"
#include "kNet/DataDeserializer.hpp"
#include "kNet/DataSerializer.hpp"
#include "kNet/EndPoint.hpp"
#include "kNet/Event.hpp"
#include "kNet/EventArray.hpp"
#include "kNet/IMessageHandler.hpp"
#include "kNet/INetworkServerListener.hpp"
#include "kNet/Lockable.hpp"
#include "kNet/MaxHeap.hpp"
#include "kNet/MessageConnection.hpp"
#include "kNet/MessageListParser.hpp"
#include "kNet/NetException.hpp"
#include "kNet/Network.hpp"
#include "kNet/NetworkLogging.hpp"
#include "kNet/NetworkMessage.hpp"
#include "kNet/NetworkServer.hpp"
#include "kNet/PolledTimer.hpp"
#include "kNet/SerializationStructCompiler.hpp"
#include "kNet/SerializedDataIterator.hpp"
#include "kNet/SharedPtr.hpp"
#include "kNet/Socket.hpp"
#include "kNet/Sort.hpp"
#include "kNet/Thread.hpp"
#include "kNet/Types.hpp"
#include "kNet/VLEPacker.hpp"
#include "kNet/WaitFreeQueue.hpp"
#include "kNet/64BitAllocDebugger.hpp"
