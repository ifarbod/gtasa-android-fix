// LZ4 wrapper
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

namespace ctn
{

class Deserializer;
class Serializer;
class VectorBuffer;

// Estimate and return worst case LZ4 compressed output size in bytes for given input size.
unsigned EstimateCompressBound(unsigned srcSize);
// Compress data using the LZ4 algorithm and return the compressed data size. The needed destination buffer worst-case size is given by EstimateCompressBound().
unsigned CompressData(void* dest, const void* src, unsigned srcSize);
// Uncompress data using the LZ4 algorithm. The uncompressed data size must be known. Return the number of compressed data bytes consumed.
unsigned DecompressData(void* dest, const void* src, unsigned destSize);
// Compress a source stream (from current position to the end) to the destination stream using the LZ4 algorithm. Return true on success.
bool CompressStream(Serializer& dest, Deserializer& src);
// Decompress a compressed source stream produced using CompressStream() to the destination stream. Return true on success.
bool DecompressStream(Serializer& dest, Deserializer& src);
// Compress a VectorBuffer using the LZ4 algorithm and return the compressed result buffer.
VectorBuffer CompressVectorBuffer(VectorBuffer& src);
// Decompress a VectorBuffer produced using CompressVectorBuffer().
VectorBuffer DecompressVectorBuffer(VectorBuffer& src);

}
