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
#include <Path.hpp>
#include <Registry.hpp>

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

// Check registry or prompt the user with folder select dialog to detrmine the game's path
String GrabSAPath()
{
    // This will store the final path
    String finalPath;

    // Try HKLM "SOFTWARE\San Andreas Online" "GTAInstallLocation"
    String saoSaPath = GetGTAPath();

    // Try Steam
    String gtasaSteamPath = ReadRegStr(
        HKLM, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Steam App 12120", "InstallLocation");

    // Try retail SA (1.00/1.01/2.00)
    // Returns "gtasaPath\gta_sa.exe"
    // Remove the quotation marks and 'gta_sa.exe'
    String gtasaRetailPath = ReadRegStr(HKLM, "SOFTWARE\\Rockstar Games\\GTA San Andreas\\Installation", "ExePath");
    gtasaRetailPath.Replace('"', '\0');
    gtasaRetailPath.Replace("gta_sa.exe", "");

    finalPath = GetGTAPath();
    if (saoSaPath.Empty())
    {
        finalPath = gtasaSteamPath;

        if (gtasaSteamPath.Empty())
        {
            finalPath = gtasaRetailPath;

            if (gtasaRetailPath.Empty())
            {
                if (MessageBoxW(nullptr,
                    L"Failed to read game directory from registry. Would you like to specify your game path now?",
                    L"SAO Error", MB_ICONERROR | MB_YESNO) == IDYES)
                {
                    String browseForFolderResult = BrowseForSAFolder();

                    if (browseForFolderResult.Empty())
                    {
                        // error
                    }
                }
            }
        }
    }

    return finalPath;
}
