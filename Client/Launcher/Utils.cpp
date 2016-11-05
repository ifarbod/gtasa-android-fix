// Launcher specific utils
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// DiStringibuted under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"

#include "Utils.hpp"

#include <Shobjidl.h>
#include <Str.hpp>

using namespace Util;

String BrowseForSAFolder()
{
    String pathToSADir;
    HRESULT hr = S_OK;

    // Initialize COM
    hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED |COINIT_DISABLE_OLE1DDE);

    // Did it succeeded?
    if (SUCCEEDED(hr))
    {
        // Create a new common open file dialog (for browsing folders)
        IFileOpenDialog* folderDialog = nullptr;

        // Create the FileOpenDialog object
        hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&folderDialog));

        if (SUCCEEDED(hr))
        {
            // Set the dialog as a folder picker.
            FILEOPENDIALOGOPTIONS options;
            hr = folderDialog->GetOptions(&options);
            if (SUCCEEDED(hr))
            {
                hr = folderDialog->SetOptions(options | FOS_PICKFOLDERS);
            }

            // Set the title of the dialog.
            if (SUCCEEDED(hr))
            {
                hr = folderDialog->SetTitle(L"Locate your GTA:SA Installation");
            }

            // Show the open file dialog.
            if (SUCCEEDED(hr))
            {
                hr = folderDialog->Show(nullptr);
                if (SUCCEEDED(hr))
                {
                    // Get the selection from the user.
                    IShellItem *shellItemResult = nullptr;
                    hr = folderDialog->GetResult(&shellItemResult);
                    if (SUCCEEDED(hr))
                    {
                        LPWSTR widePath = nullptr;
                        hr = shellItemResult->GetDisplayName(SIGDN_FILESYSPATH, &widePath);
                        if (SUCCEEDED(hr))
                        {
                            // Store the path
                            pathToSADir = widePath;

                            // Release memory
                            CoTaskMemFree(widePath);
                        }
                        shellItemResult->Release();
                    }
                }
                else
                {
                    // Dialog cancelled
                    if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED))
                    {
                        // User cancelled the dialog...
                        //MsgBox::Show("You cancelled the dialog...", MOD_NAME);
                    }
                }
            }

            folderDialog->Release();
        }

        // Report the error
        if (FAILED(hr))
        {
            // If it's not that the user cancelled the dialog, report the error in a
            // message box. Even though, this shouldn't happen at all
            if (hr != HRESULT_FROM_WIN32(ERROR_CANCELLED))
            {
                //MsgBox::Show("CoCreateInstance failed, please try running as admin.", MOD_NAME);
            }
        }

        // Uninitialize COM
        CoUninitialize();
    }

    return pathToSADir;
}
