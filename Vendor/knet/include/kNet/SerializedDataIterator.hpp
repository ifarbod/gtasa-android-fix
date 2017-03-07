// SerializedDataIterator.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file SerializedDataIterator.h
    @brief The SerializedDataIterator class. */

#include "SharedPtr.hpp"
#include "MessageListParser.hpp"

namespace kNet
{

class SerializedDataIterator : public RefCountable
{
public:
    SerializedDataIterator(const SerializedMessageDesc &desc_)
    :desc(desc_)
    {
        ResetTraversal();
    }

    BasicSerializedDataType NextElementType() const;

    const SerializedElementDesc *NextElementDesc() const;

    void ProceedToNextVariable();

    void ProceedNVariables(int count);

    /// Sets the number of instances in a varying element. When iterating over
    /// the message to insert data into serialized form, this information needs
    /// to be passed to this iterator in order to continue.
    void SetVaryingElemSize(u32 count);

    void ResetTraversal();

private:
    struct ElemInfo
    {
        /// The element we are accessing next.
        SerializedElementDesc *elem;
        /// The index of the elem we are accessing next.
        int nextElem;
        /// The index of the instance we are accessing next.
        int nextIndex;
        /// The total number of instances of this element we are accessing.
        int count;
        /// If this element is a dynamic count -one, then this tracks whether the count has been passed in.
        bool dynamicCountSpecified;
    };

    void ProceedToNextElement();
    void DescendIntoStructure();

    /// Stores the tree traversal progress.
    std::vector<ElemInfo> currentElementStack;
    /// The type of the message we are building.
    const SerializedMessageDesc &desc;

    void operator =(const SerializedDataIterator&);
    SerializedDataIterator(const SerializedDataIterator&);
};

} // ~kNet
