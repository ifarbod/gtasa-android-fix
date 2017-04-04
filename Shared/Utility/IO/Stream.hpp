// A common root class for objects that implement both Serializer and Deserializer.
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <IO/Serializer.hpp
#include <IO/Deserializer.hpp>

namespace ctn
{

// A common root class for objects that implement both Serializer and Deserializer.
class AbstractFile : public Deserializer, public Serializer
{
public:
    // Construct.
    AbstractFile() : Deserializer()
    {
        
    }
    // Construct.
    AbstractFile(unsigned int size) : Deserializer(size)
    {
        
    }
    // Destruct.
    virtual ~AbstractFile()
    {
        
    }
};

};