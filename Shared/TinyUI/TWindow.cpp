// Window implementation
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "StdH.hpp"
#include "TWindow.hpp"

namespace TinyUI
{

TWindow::TWindow() :
	windowHandle_(nullptr)
{
}

TWindow::~TWindow()
{
	windowHandle_ = nullptr;
}

bool TWindow::SetLocation(const IntVector2D& location)
{
	return false;
}

IntVector2D TWindow::GetLocation()
{
	return IntVector2D();
}

bool TWindow::SetSize(const IntVector2D& location)
{
	return false;
}

IntVector2D TWindow::GetSize()
{
	return IntVector2D();
}

bool TWindow::Create(HINSTANCE owner, const TChar* className, const TChar* windowName)
{
	if (windowHandle_)
	{
		// A window is already created
		return false;
	}

	HWND window = CreateWindowEx(0, className, windowName, 0, location_.x_, location_.y_, size_.x_, size_.y_, nullptr, nullptr, owner, nullptr);

	// Window created successfully
	if (window)
	{
		windowHandle_ = window;
		return true;
	}

	return false;
}

}
