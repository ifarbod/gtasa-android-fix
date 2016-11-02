// Wrapper for win32's registry function
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"

#include "Registry.hpp"
#include "Buffer.hpp"

#include <Windows.h>

namespace Util
{

bool WriteRegStr(KeyRoot keyRoot, const String& subKey, const String& keyName, const String& value, bool flush)
{
    HKEY hKey;
    WString wSubKey{ subKey };
    WString wKeyName{ keyName };
    WString wValue{ value };

    // Try to open the reg key with all access
    auto openKeyResult = RegOpenKeyExW(reinterpret_cast<HKEY>(keyRoot), wSubKey.CString(), 0, KEY_ALL_ACCESS, &hKey);

    // Does it exists?
    if (openKeyResult == ERROR_FILE_NOT_FOUND || openKeyResult == ERROR_NO_MATCH)
    {
        auto createKeyRes = RegCreateKeyExW(reinterpret_cast<HKEY>(keyRoot), wSubKey.CString(), 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

        if (createKeyRes != ERROR_SUCCESS)
            return false;
    }

    if (hKey)
    {
        auto setValueRes = RegSetValueExW(hKey, wKeyName.CString(), 0, REG_SZ, (LPBYTE)wValue.CString(), (wValue.Length() + 1) * sizeof(wchar_t));

        if (setValueRes != ERROR_SUCCESS)
            return false;

        if (flush)
        {
            auto flushKeyRes = RegFlushKey(hKey);

            if (flushKeyRes != ERROR_SUCCESS)
                return false;
        }

        auto closeKeyRes = RegCloseKey(hKey);

        if (closeKeyRes != ERROR_SUCCESS)
            return false;
    }

    return true;
}

String ReadRegStr(KeyRoot keyRoot, const String& subKey, const String& keyName, bool *result)
{
    String output = "";
    HKEY hKey;
    WString wSubKey{ subKey };
    WString wKeyName{ keyName };

    if (result != nullptr)
        *result = false;

    if (RegOpenKeyExW((HKEY)keyRoot, wSubKey.CString(), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        DWORD bufferSize;

        if (RegQueryValueExW(hKey, wKeyName.CString(), nullptr, nullptr, nullptr, &bufferSize) == ERROR_SUCCESS)
        {
            ScopedAlloc<wchar_t> buffer{ bufferSize + sizeof(wchar_t) };
            //Vector<char> buffer;
            //buffer.Resize(bufferSize + 2);

            if (RegQueryValueExW(hKey, wKeyName.CString(), nullptr, nullptr, reinterpret_cast<LPBYTE>(static_cast<wchar_t *>(buffer)), &bufferSize) == ERROR_SUCCESS)
            {
                buffer[bufferSize / sizeof(wchar_t)] = 0;
                output = static_cast<wchar_t *>(buffer);

                if (result != nullptr)
                    *result = true;
            }
        }
    }

    return output;
}

}
