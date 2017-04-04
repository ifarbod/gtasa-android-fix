// Dynamically sized buffer that can be read and written to as a stream
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
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
    if (size + m_position > m_size)
        size = m_size - m_position;
    if (!size)
        return 0;

    unsigned char* srcPtr = &m_buffer[m_position];
    unsigned char* destPtr = (unsigned char*)dest;
    m_position += size;

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
    if (position > m_size)
        position = m_size;

    m_position = position;
    return m_position;
}

unsigned VectorBuffer::Write(const void* data, unsigned size)
{
    if (!size)
        return 0;

    if (size + m_position > m_size)
    {
        m_size = size + m_position;
        m_buffer.Resize(m_size);
    }

    unsigned char* srcPtr = (unsigned char*)data;
    unsigned char* destPtr = &m_buffer[m_position];
    m_position += size;

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
    m_buffer = data;
    m_position = 0;
    m_size = data.Size();
}

void VectorBuffer::SetData(const void* data, unsigned size)
{
    if (!data)
        size = 0;

    m_buffer.Resize(size);
    if (size)
        memcpy(&m_buffer[0], data, size);

    m_position = 0;
    m_size = size;
}

void VectorBuffer::SetData(Deserializer& source, unsigned size)
{
    m_buffer.Resize(size);
    unsigned actualSize = source.Read(&m_buffer[0], size);
    if (actualSize != size)
        m_buffer.Resize(actualSize);

    m_position = 0;
    m_size = actualSize;
}

void VectorBuffer::Clear()
{
    m_buffer.Clear();
    m_position = 0;
    m_size = 0;
}

void VectorBuffer::Resize(unsigned size)
{
    m_buffer.Resize(size);
    m_size = size;
    if (m_position > m_size)
        m_position = m_size;
}

}
