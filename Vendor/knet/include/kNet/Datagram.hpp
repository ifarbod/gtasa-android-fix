// Datagram.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file Datagram.h
    @brief The struct \ref kNet::Datagram Datagram. */

namespace kNet
{

static const unsigned long cDatagramBufferSize = 3 * 512;

/// @internal Datagram stores the raw data of a received UDP datagram.
struct Datagram
{
    unsigned char data[cDatagramBufferSize];

    unsigned long size;
};

} // ~kNet
