// Launch utils
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

bool CheckRegistryIntegrity();

void Error(const char *pszErrorMessage);
void FatalError(const char *pszErrorMessage);

const char* va(const char* string, ...);

const char * GetAppPath();
const char * GetExePath();

bool CheckNameOfExecutableFromPath(char *szCheckedName, size_t checkedNameSize, char *szPath, size_t pathSize);
bool RemoveExecutableNameFromPath(char *szSrc, size_t srcSize, char *szDst, size_t dstSize);
char GetLastCharacterOfNullTerminatedCString(char *szString, size_t size);

bool ReadRegistryString(HKEY hKeyLocation, const char * szSubKey, const char * szKey, const char * szDefault, char * szData, DWORD dwSize);
bool WriteRegistryString(HKEY hKeyLocation, const char * szSubKey, const char * szKey, char * szData, DWORD dwSize);

DWORD Unprotect(DWORD dwAddress, size_t sSize);
