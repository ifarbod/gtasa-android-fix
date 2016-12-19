// Window interface
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "TinyUI.hpp"

namespace TinyUI
{

class TINYUI_CLASS TWindowBase
{
public:
    // Destructor.
    virtual ~TWindowBase() = 0;

    // Set text.
    virtual void SetText(const TChar* text) = 0;

    // Get text.
    virtual const TChar* GetText() = 0;

	// Set position.
	virtual bool SetLocation(const IntVector2D& location) = 0;

	// Get position.
	virtual IntVector2D GetLocation() = 0;

	// Set size.
	virtual bool SetSize(const IntVector2D& size) = 0;

	// Get size.
	virtual IntVector2D GetSize() = 0;
};

}
