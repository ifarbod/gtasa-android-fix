// Dynamically sized buffer that can be read and written to as a stream
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include <IO/VectorBuffer.hpp>

namespace ctn
{

VectorBuffer::VectorBuffer(const PODVector<unsigned char>& data)
{
    SetData(data);
}

VectorBuffer::VectorBuffer(const void* data, unsigned size)
{
    SetData(data, size);
}

VectorBuffer::VectorBuffer(Deserializer& source, unsigned size)
{
    SetData(source, size);
}

unsigned VectorBuffer::Read(void* dest, unsigned size)
{
    if (size + position_ > size_)
        size = size_ - position_;
    if (!size)
        return 0;

    unsigned char* srcPtr = &buffer_[position_];
    unsigned char* destPtr = (unsigned char*)dest;
    position_ += size;

    unsigned copySize = size;
    while (copySize >= sizeof(unsigned))
    {
        *((unsigned*)destPtr) = *((unsigned*)srcPtr);
        srcPtr += sizeof(unsigned);
        destPtr += sizeof(unsigned);
        copySize -= sizeof(unsigned);
    }
    if (copySize & sizeof(unsigned short))
    {
        *((unsigned short*)destPtr) = *((unsigned short*)srcPtr);
        srcPtr += sizeof(unsigned short);
        destPtr += sizeof(unsigned short);
    }
    if (copySize & 1)
        *destPtr = *srcPtr;

    return size;
}

unsigned VectorBuffer::Seek(unsigned position)
{
    if (position > size_)
        position = size_;

    position_ = position;
    return position_;
}

unsigned VectorBuffer::Write(const void* data, unsigned size)
{
    if (!size)
        return 0;

    if (size + position_ > size_)
    {
        size_ = size + position_;
        buffer_.Resize(size_);
    }

    unsigned char* srcPtr = (unsigned char*)data;
    unsigned char* destPtr = &buffer_[position_];
    position_ += size;

    unsigned copySize = size;
    while (copySize >= sizeof(unsigned))
    {
        *((unsigned*)destPtr) = *((unsigned*)srcPtr);
        srcPtr += sizeof(unsigned);
        destPtr += sizeof(unsigned);
        copySize -= sizeof(unsigned);
    }
    if (copySize & sizeof(unsigned short))
    {
        *((unsigned short*)destPtr) = *((unsigned short*)srcPtr);
        srcPtr += sizeof(unsigned short);
        destPtr += sizeof(unsigned short);
    }
    if (copySize & 1)
        *destPtr = *srcPtr;

    return size;
}

void VectorBuffer::SetData(const PODVector<unsigned char>& data)
{
    buffer_ = data;
    position_ = 0;
    size_ = data.Size();
}

void VectorBuffer::SetData(const void* data, unsigned size)
{
    if (!data)
        size = 0;

    buffer_.Resize(size);
    if (size)
        memcpy(&buffer_[0], data, size);

    position_ = 0;
    size_ = size;
}

void VectorBuffer::SetData(Deserializer& source, unsigned size)
{
    buffer_.Resize(size);
    unsigned actualSize = source.Read(&buffer_[0], size);
    if (actualSize != size)
        buffer_.Resize(actualSize);

    position_ = 0;
    size_ = actualSize;
}

void VectorBuffer::Clear()
{
    buffer_.Clear();
    position_ = 0;
    size_ = 0;
}

void VectorBuffer::Resize(unsigned size)
{
    buffer_.Resize(size);
    size_ = size;
    if (position_ > size_)
        position_ = size_;
}

}
