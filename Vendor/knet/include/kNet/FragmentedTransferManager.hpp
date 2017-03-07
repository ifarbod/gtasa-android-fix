// FragmentedTransferManager.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file FragmentedTransferManager.h
    @brief The classes \ref kNet::FragmentedSendManager FragmentedSendManager and
    \ref kNet::FragmentedSendManager FragmentedReceiveManager. For managing partial transfers. */

#include <vector>
#include <list>

namespace kNet
{

class NetworkMessage;

/// @internal Manages the allocation of transferIDs to fragmented message transfers and tracks which of the fragments have
/// successfully been sent over to the receiver.
class FragmentedSendManager
{
public:
    struct FragmentedTransfer
    {
        int id;

        /// The total number of fragments in this message.
        size_t totalNumFragments;

        std::list<NetworkMessage*> fragments;

        void AddMessage(NetworkMessage *message);

        /// Returns true if the given message was part of this transfer (which now got removed).
        bool RemoveMessage(NetworkMessage *message);
    };

    typedef std::list<FragmentedTransfer> TransferList;
    TransferList transfers;

    /// Returns a new FragmentedTransfer. A transferID for this transfer will not have been allocated here.
    /// When sending the message is finished, call FreeFragmentedTransfer.
    FragmentedTransfer *AllocateNewFragmentedTransfer();
    void RemoveMessage(FragmentedTransfer *transfer, NetworkMessage *message);

    /// @return True if the allocation succeeded, false otherwise.
    bool AllocateFragmentedTransferID(FragmentedTransfer &transfer);

    void FreeAllTransfers();

private:
    void FreeFragmentedTransfer(FragmentedTransfer *transfer);
};

/// @internal Receives message fragments and assembles fragments to complete messages when they are finished.
class FragmentedReceiveManager
{
public:
    struct ReceiveFragment
    {
        int fragmentIndex;

        std::vector<char> data;
    };

    struct ReceiveTransfer
    {
        int transferID;

        int numTotalFragments;

        std::vector<ReceiveFragment> fragments;
    };

    std::vector<ReceiveTransfer> transfers;

    void NewFragmentStartReceived(int transferID, int numTotalFragments, const char *data, size_t numBytes);
    bool NewFragmentReceived(int transferID, int fragmentNumber, const char *data, size_t numBytes);
    void AssembleMessage(int transferID, std::vector<char> &assembledData);
    void FreeMessage(int transferID);
};

} // ~kNet
