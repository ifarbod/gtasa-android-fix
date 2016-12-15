// Client Launcher entry point
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2017 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE.md or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"

#include <CommCtrl.h>
#include <shobjidl.h>

#define WIDTH 462
#define HEIGHT 190

static struct
{
    HWND rootWindow;
    HWND topStatic;
    HWND bottomStatic;
    HWND progressBar;
    HWND cancelButton;

    UINT taskbarMsg;

    bool canceled;

    ITaskbarList3* tbList;

    wchar_t topText[512];
    wchar_t bottomText[512];
} g_uui;

void UI_CreateWindow()
{
    g_uui.taskbarMsg = RegisterWindowMessage(L"TaskbarButtonCreated");

    HWND rootWindow = CreateWindowExW(0, L"SAOUpdater", L"37.53% complete - Updating " MOD_NAME,
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, 0, WIDTH, HEIGHT, nullptr, nullptr,
        GetModuleHandle(NULL), nullptr);

    INITCOMMONCONTROLSEX controlSex;
    controlSex.dwSize = sizeof(controlSex);
    controlSex.dwICC = 0x4020;
    InitCommonControlsEx(&controlSex);

    HFONT font = CreateFontW(-12, // Height
        0,                        // Width
        0,                        // escapement
        0,                        // orientation
        0,                        // weight
        0,                        // italic
        0,                        // underline
        0,                        // strikeout
        1,                        // charset
        8,                        // out precision
        0,                        // clip precision
        5,                        // quality
        2,                        // pitch and family
        L"Open Sans");

    HFONT font3 = CreateFontW(-16, // Height
        0,                         // Width
        0,                         // escapement
        0,                         // orientation
        0,                         // weight
        0,                         // italic
        0,                         // underline
        0,                         // strikeout
        1,                         // charset
        8,                         // out precision
        0,                         // clip precision
        5,                         // quality
        2,                         // pitch and family
        L"Open Sans");

    HWND static1 = CreateWindowExW(WS_EX_TRANSPARENT, L"static", L"static1", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
        25, 15, 396, 25, rootWindow, 0, GetModuleHandle(NULL), 0);
    SendMessage(static1, WM_SETFONT, (WPARAM)font, 0);
    SetWindowText(static1, L"Copying 2,417 items from Games to Games");

    HWND static2 = CreateWindowExW(WS_EX_TRANSPARENT, L"static", L"static2", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
        25, 33, 396, 25, rootWindow, 0, GetModuleHandle(NULL), 0);
    SendMessage(static2, WM_SETFONT, (WPARAM)font3, 0);
    SetWindowText(static2, L"Paused - 5% complete");

    HWND progressBar = CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, L"msctls_progress32", 0,
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 25, 63, 396, 15, rootWindow, 0, GetModuleHandle(NULL), 0);
    SendMessage(progressBar, PBM_SETRANGE32, 0, 10000);
    SendMessage(progressBar, PBM_SETPOS, (int)(73 * 100), 0);
    SendMessage(progressBar, PBM_SETSTATE, PBST_PAUSED, 0);

    HWND yesButton = CreateWindowEx(
        0, L"button", L"Yes", WS_CHILD | WS_CLIPSIBLINGS, 270, 85, 75, 23, rootWindow, 0, GetModuleHandle(NULL), 0);
    SendMessage(yesButton, WM_SETFONT, (WPARAM)font, 0);

    HWND cancelNoButton = CreateWindowEx(0, L"button", L"Cancel", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 347, 85, 75,
        23, rootWindow, 0, GetModuleHandle(NULL), 0);
    SendMessage(cancelNoButton, WM_SETFONT, (WPARAM)font, 0);

    HWND static3 = CreateWindowEx(0x20, L"static", L"static3", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 25, 83, 240, 22,
        rootWindow, 0, GetModuleHandle(NULL), 0);
    SendMessage(static3, WM_SETFONT, (WPARAM)font, 0);
    SetWindowText(static3, L"13.00 / 17.89MB (9.7 MB/s)");

    HWND static4 = CreateWindowEx(0x20, L"static", L"static4", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 25, 101, 240,
        22, rootWindow, 0, GetModuleHandle(NULL), 0);
    SendMessage(static4, WM_SETFONT, (WPARAM)font, 0);
    SetWindowText(static4, L"32:57 | -01:37");

    HWND static5 = CreateWindowEx(0x20, L"static", L"static5", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 25, 119, 396,
        22, rootWindow, 0, GetModuleHandle(NULL), 0);
    SendMessage(static5, WM_SETFONT, (WPARAM)font, 0);
    SetWindowText(static5, L"Downloading files required for playing multiplayer...");

    RECT wndRect;
    wndRect.left = 0;
    wndRect.top = 0;
    wndRect.right = WIDTH;
    wndRect.bottom = HEIGHT;

    HWND desktop = GetDesktopWindow();
    HDC dc = GetDC(desktop);
    int width = GetDeviceCaps(dc, 8);
    int height = GetDeviceCaps(dc, 10);

    ReleaseDC(desktop, dc);

    // SetTimer(rootWindow, 0, 20, NULL);

    MoveWindow(rootWindow, (width - WIDTH) / 2, (height - HEIGHT) / 2, wndRect.right - wndRect.left + 1,
        wndRect.bottom - wndRect.top + 1, TRUE);

    ShowWindow(rootWindow, TRUE);
}

LRESULT CALLBACK UI_WndProc(HWND hWnd, UINT uMsg, WPARAM wparam, LPARAM lparam)
{
    switch (uMsg)
    {
        case WM_COMMAND:
            if ((HWND)lparam == g_uui.cancelButton)
            {
                g_uui.canceled = true;
            }

            break;
        case WM_TIMER:
            SetWindowText(g_uui.topStatic, g_uui.topText);
            SetWindowText(g_uui.bottomStatic, g_uui.bottomText);
            break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        default:
        {
            if (uMsg == g_uui.taskbarMsg)
            {
                if (g_uui.tbList)
                {
                    g_uui.tbList->SetProgressState(hWnd, TBPF_NORMAL);
                    g_uui.tbList->SetProgressValue(hWnd, 0, 100);
                }
            }
            break;
        }
    }

    return DefWindowProc(hWnd, uMsg, wparam, lparam);
}

void UI_RegisterClass()
{
    WNDCLASSEX wndClass = {0};
    wndClass.cbSize = sizeof(wndClass);
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = UI_WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = GetModuleHandle(NULL);
    wndClass.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(101));
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wndClass.lpszClassName = L"SAOUpdater";
    wndClass.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(101));

    RegisterClassEx(&wndClass);
}

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR strCmdLine, int nCmdShow)
{
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_uui.tbList));

    UI_RegisterClass();
    UI_CreateWindow();

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
