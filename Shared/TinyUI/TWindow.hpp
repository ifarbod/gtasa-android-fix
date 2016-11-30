// Window implementation
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "TinyUI.hpp"
#include "TWindowBase.hpp"
#include <Windows.h>

namespace TinyUI
{

class TINYUI_CLASS TWindow : public TWindowBase
{
public:
	// Constructor.
	TWindow();

	// Destructor.
	virtual ~TWindow() override;

	// Set the location of the window on-screen.
	virtual bool SetLocation(const IntVector2D& location) override;

	// Set the location of the window on-screen.
	virtual IntVector2D GetLocation() override;

	// Set the location of the window on-screen.
	virtual bool SetSize(const IntVector2D& location) override;

	// Set the location of the window on-screen.
	virtual IntVector2D GetSize() override;

	// Create the actual window.
	bool Create(HINSTANCE owner, const TChar* className, const TChar* windowName);

private:
	// Win32 Window handle.
	HWND windowHandle_;

	// Owner instance.
	HINSTANCE owner_;

	// Location.
	IntVector2D location_;

	// Size.
	IntVector2D size_;
};

}
