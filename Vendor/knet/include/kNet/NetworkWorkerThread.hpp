// NetworkWorkerThread.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file NetworkWorkerThread.h
    @brief The NetworkWorkerThread class. Implements a background thread for responsive
    processing of server and client connections. */

#include "SharedPtr.hpp"

#include "Lockable.hpp"
#include "MessageConnection.hpp"
#include "NetworkServer.hpp"
#include "Thread.hpp"

namespace kNet
{

class NetworkWorkerThread
{
public:
    NetworkWorkerThread();

    void AddConnection(MessageConnection *connection);
    void RemoveConnection(MessageConnection *connection);

    void AddServer(NetworkServer *server);
    void RemoveServer(NetworkServer *server);

    void StartThread();
    void StopThread();

    int NumConnections() const;
    int NumServers() const;

    Thread &ThreadObject() { return workThread; }

private:
    Lockable<std::vector<MessageConnection *> > connections;
    Lockable<std::vector<NetworkServer *> > servers;

    Thread workThread;

    /// The entry point for the work thread, which runs a loop that manages network connections.
    void MainLoop();
};

#ifdef _WIN32
DWORD WINAPI NetworkWorkerThreadMain(LPVOID lpParameter);
#endif

} // ~kNet
