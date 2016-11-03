// Path management
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

namespace Util
{

class String;

// Split a full path to path, filename and extension. The extension will be converted to lowercase by default.
void
SplitPath(const String& fullPath, String& pathName, String& fileName, String& extension, bool lowercaseExtension = true);
// Return the path from a full path.
String GetPath(const String& fullPath);
// Return the filename from a full path.
String GetFileName(const String& fullPath);
// Return the extension from a full path, converted to lowercase by default.
String GetExtension(const String& fullPath, bool lowercaseExtension = true);
// Return the filename and extension from a full path. The case of the extension is preserved by default, so that the file can be opened in case-sensitive operating systems.
String GetFileNameAndExtension(const String& fullPath, bool lowercaseExtension = false);
// Replace the extension of a file name with another.
String ReplaceExtension(const String& fullPath, const String& newExtension);
// Add a slash at the end of the path if missing and convert to internal format (use slashes.)
String AddTrailingSlash(const String& pathName);
// Remove the slash from the end of a path if exists and convert to internal format (use slashes.)
String RemoveTrailingSlash(const String& pathName);
// Return the parent path, or the path itself if not available.
String GetParentPath(const String& pathName);
// Convert a path to internal format (use slashes.)
String GetInternalPath(const String& pathName);
// Convert a path to the format required by the operating system.
String GetNativePath(const String& pathName);
// Convert a path to the format required by the operating system in wide characters.
WString GetWideNativePath(const String& pathName);
// Return whether a path is absolute.
bool IsAbsolutePath(const String& pathName);
// Normalize a path, adding slashes when appropriate
String PathConform(const String& strPath);
// Join two paths
String PathJoin(const String& str1, const String& str2);
// Join three paths
String PathJoin(const String& str1, const String& str2, const String& str3);
// Join four paths
String PathJoin(const String& str1, const String& str2, const String& str3, const String& str4);
// Join five paths
String PathJoin(const String& str1, const String& str2, const String& str3, const String& str4, const String& str5);

// Get GTA Path from our registry key
String GetGTAPath();
// Get SAO Path from our registry key
String GetSAOPath();

String GetGTAPath(const String& pathToJoin);
String GetSAOPath(const String& pathToJoin);

}
