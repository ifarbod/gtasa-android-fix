// CefApp implementation
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <cef3/include/cef_app.h>
#include <string>
#include <sstream>
#include "V8Helpers.hpp"

using V8Helpers::V8Handler;

class SAOCefApp : public CefApp, public CefRenderProcessHandler
{
public:
    SAOCefApp() = default;

    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; };

    virtual void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node) override
    {
        if (inputHasFocus_)
        {
            if (node)
                return;

            // Tell SAO that we lost input focus
            auto message = CefProcessMessage::Create("InputFocus");
            message->GetArgumentList()->SetBool(0, false);
            browser->SendProcessMessage(PID_BROWSER, message);

            // Set variable to ensure that the event does not trigger twice
            inputHasFocus_ = false;
            return;
        }
        else
        {
            if (!node)
                return;

            if (node->GetType() == CefDOMNode::Type::DOM_NODE_TYPE_ELEMENT && !node->GetFormControlElementType().empty())
            {
                auto message = CefProcessMessage::Create("InputFocus");
                message->GetArgumentList()->SetBool(0, true);
                browser->SendProcessMessage(PID_BROWSER, message);

                // Set variable to ensure that the event does not trigger twice
                inputHasFocus_ = true;
            }
        }
    }

    virtual void OnRegisterCustomSchemes(CefRefPtr < CefSchemeRegistrar > registrar) override
    {
        // Register custom SAO scheme (has to be called in all proceseses)
        registrar->AddCustomScheme("saolocal", false, false, false);
    }


    // http://magpcss.org/ceforum/apidocs3/projects/(default)/CefRenderProcessHandler.html#OnContextCreated(CefRefPtr%3CCefBrowser%3E,CefRefPtr%3CCefFrame%3E,CefRefPtr%3CCefV8Context%3E) //
    virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override
    {
        // Get global object and create a v8 handler instance
        CefRefPtr<CefV8Value> globalObject = context->GetGlobal();
        CefRefPtr<V8Handler> handler = new V8Handler(frame);

        // Create SAO object
        CefRefPtr<CefV8Value> saoObject = CefV8Value::CreateObject(nullptr, nullptr);

        // Bind V8 --> C++ functions
        V8Helpers::BindV8Function(handler, saoObject, "triggerEvent", Javascript_triggerEvent);

        // Assign saoObject to global object
        globalObject->SetValue("sao", saoObject, V8_PROPERTY_ATTRIBUTE_NONE);
    }

    static void Javascript_triggerEvent(CefRefPtr<CefFrame> frame, const CefV8ValueList& arguments)
    {
        if (arguments.size() == 0)
            return;

        CefRefPtr<CefProcessMessage> message = V8Helpers::SerializeV8Arguments("TriggerLuaEvent", arguments);
        frame->GetBrowser()->SendProcessMessage(PID_BROWSER, message);
    }

public:
    IMPLEMENT_REFCOUNTING(SAOCefApp);

private:
    bool inputHasFocus_ = false;
};
