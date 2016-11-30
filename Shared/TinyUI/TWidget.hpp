// Widget interface
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "TinyUI.hpp"
#include "TWindowBase.hpp"

namespace TinyUI
{

class TINYUI_CLASS TWidget : public TWindowBase
{
public:
    // Destructor.
    virtual ~TWidget() = 0;

    // Set enabled state.
    virtual void SetEnabled(bool enabled) = 0;

    // Get enabled state.
    virtual bool GetEnabled() = 0;

    // Set visibility state.
    virtual void SetVisibility(bool visible) = 0;

    // Get visibility state.
    virtual bool GetVisibility() = 0;
};

}
