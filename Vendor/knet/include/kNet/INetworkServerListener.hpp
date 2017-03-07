// INetworkServerListener.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file INetworkServerListener.h
    @brief The \ref kNet::INetworkServerListener INetworkServerListener interface. Implementable by the client application. */

#include "Types.hpp"

namespace kNet
{

struct EndPoint;

/// An interface implementable by the owner of a network server to receive notifications about connection-related events.
/// @note Do not call NetworkServer::Process from any of these methods. This might cause infinite recursion.
class INetworkServerListener
{
public:
    /// Called to query whether the new connection should be accepted or not, but only if the server operates in UDP mode.
    /// @return If the implementor of this listener decides the connection should be accepted, it should return true.
    ///       If it returns false, the connection attempt will be ignored.
    virtual bool NewConnectionAttempt(const EndPoint & UNUSED(endPoint), const char * UNUSED(data), size_t UNUSED(numBytes))
    {
        /// @note The default implementation of this method is to accept the connection. Be sure to override this if
        ///       custom access control is needed.
        return true;
    }

    /// Called to notify the listener that a new connection has been established.
    /// The application is expected to at least register a message listener for the new connection.
    virtual void NewConnectionEstablished(MessageConnection *connection) = 0;

    /// Called to notify the listener that the given client has disconnected.
    virtual void ClientDisconnected(MessageConnection * UNUSED(connection))
    {
        /// The default action is to not do anything.
    }
};

} // ~kNet
