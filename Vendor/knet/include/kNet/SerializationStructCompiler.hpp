// SerializationStructCompiler.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file SerializationStructCompiler.h
    @brief The SerializationStructCompiler class. */

#include <string>
#include <fstream>

#include "MessageListParser.hpp"

namespace kNet
{

/// Generates .h files out of XML description files for serializable structs and network messages.
class SerializationStructCompiler
{
public:
    void CompileStruct(const SerializedElementDesc &structure, const char *outfile);
    void CompileMessage(const SerializedMessageDesc &message, const char *outfile);

    static std::string ParseToValidCSymbolName(const char *str);

private:
    void WriteFilePreamble(std::ofstream &out);
    void WriteStruct(const SerializedElementDesc &elem, int level, std::ofstream &out);
    void WriteMessage(const SerializedMessageDesc &message, std::ofstream &out);

    void WriteMemberDefinition(const SerializedElementDesc &elem, int level, std::ofstream &out);
    void WriteStructMembers(const SerializedElementDesc &elem, int level, std::ofstream &out);
    void WriteNestedStructs(const SerializedElementDesc &elem, int level, std::ofstream &out);
    void WriteStructSizeMemberFunction(const SerializedElementDesc &elem, int level, std::ofstream &out);
    void WriteSerializeMemberFunction(/*const std::string &className, */const SerializedElementDesc &elem, int level, std::ofstream &out);
    void WriteDeserializeMemberFunction(/*const std::string &className, */const SerializedElementDesc &elem, int level, std::ofstream &out);

    static std::string Indent(int level);
};

} // ~kNet
