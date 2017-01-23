// Path management
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include <PCH.hpp>

#include "Path.hpp"
#include "Registry.hpp"

namespace ctn
{

void SplitPath(const String& fullPath, String& pathName, String& fileName, String& extension, bool lowercaseExtension)
{
    String fullPathCopy = GetInternalPath(fullPath);

    unsigned extPos = fullPathCopy.FindLast('.');
    unsigned pathPos = fullPathCopy.FindLast('/');

    if (extPos != String::NPOS && (pathPos == String::NPOS || extPos > pathPos))
    {
        extension = fullPathCopy.Substring(extPos);
        if (lowercaseExtension)
            extension = extension.ToLower();
        fullPathCopy = fullPathCopy.Substring(0, extPos);
    }
    else
        extension.Clear();

    pathPos = fullPathCopy.FindLast('/');
    if (pathPos != String::NPOS)
    {
        fileName = fullPathCopy.Substring(pathPos + 1);
        pathName = fullPathCopy.Substring(0, pathPos + 1);
    }
    else
    {
        fileName = fullPathCopy;
        pathName.Clear();
    }
}

String GetPath(const String& fullPath)
{
    String path, file, extension;
    SplitPath(fullPath, path, file, extension);
    return path;
}

String GetFileName(const String& fullPath)
{
    String path, file, extension;
    SplitPath(fullPath, path, file, extension);
    return file;
}

String GetExtension(const String& fullPath, bool lowercaseExtension)
{
    String path, file, extension;
    SplitPath(fullPath, path, file, extension, lowercaseExtension);
    return extension;
}

String GetFileNameAndExtension(const String& fileName, bool lowercaseExtension)
{
    String path, file, extension;
    SplitPath(fileName, path, file, extension, lowercaseExtension);
    return file + extension;
}

String ReplaceExtension(const String& fullPath, const String& newExtension)
{
    String path, file, extension;
    SplitPath(fullPath, path, file, extension);
    return path + file + newExtension;
}

String AddTrailingSlash(const String& pathName)
{
    String ret = pathName.Trimmed();
    ret.Replace('\\', '/');
    if (!ret.Empty() && ret.Back() != '/')
        ret += '/';
    return ret;
}

String RemoveTrailingSlash(const String& pathName)
{
    String ret = pathName.Trimmed();
    ret.Replace('\\', '/');
    if (!ret.Empty() && ret.Back() == '/')
        ret.Resize(ret.Length() - 1);
    return ret;
}

String GetParentPath(const String& path)
{
    unsigned pos = RemoveTrailingSlash(path).FindLast('/');
    if (pos != String::NPOS)
        return path.Substring(0, pos + 1);
    else
        return String();
}

String GetInternalPath(const String& pathName)
{
    return pathName.Replaced('\\', '/');
}

String GetNativePath(const String& pathName)
{
#ifdef _WIN32
    return pathName.Replaced('/', '\\');
#else
    return pathName;
#endif
}

WString GetWideNativePath(const String& pathName)
{
    return WString(GetNativePath(pathName));
}

bool IsAbsolutePath(const String& pathName)
{
    if (pathName.Empty())
        return false;

    String path = GetInternalPath(pathName);

    if (path[0] == '/')
        return true;

#ifdef _WIN32__
    if (path.Length() > 1 && IsAlpha(path[0]) && path[1] == ':')
        return true;
#endif

    return false;
}

String PathConform(const String& strPath)
{
    // Make slashes the right way and remove duplicates, except for UNC type indicators
#ifdef WIN32
    String strTemp = strPath.Replaced("/", DIRECTORY_SEPARATOR_CHAR);
#else
    String strTemp = strPath.Replaced("\\", DIRECTORY_SEPARATOR_CHAR);
#endif
    // Remove slash duplicates
    unsigned iFirstDoubleSlash = strTemp.Find(DIRECTORY_SEPARATOR_CHAR DIRECTORY_SEPARATOR_CHAR);
    if (iFirstDoubleSlash == String::NPOS)
    {
        // No duplicates present
        return strTemp;
    }

    // If first double slash is not at the start, then treat as a normal duplicate if:
    // 1. It is not preceeded by a colon, or
    // 2. Another single slash is before it
    if (iFirstDoubleSlash > 0)
    {
        if (iFirstDoubleSlash == 2 && strTemp[1] == ':')
        {
            // Replace all duplicate slashes
            return strTemp.Replaced(DIRECTORY_SEPARATOR_CHAR DIRECTORY_SEPARATOR_CHAR, DIRECTORY_SEPARATOR_CHAR, true);
        }

        if (strTemp.Substring(iFirstDoubleSlash - 1, 1) != ":" || strTemp.Find(DIRECTORY_SEPARATOR_CHAR) < iFirstDoubleSlash)
        {
            // Replace all duplicate slashes
            return strTemp.Replaced(DIRECTORY_SEPARATOR_CHAR DIRECTORY_SEPARATOR_CHAR, DIRECTORY_SEPARATOR_CHAR, true);
        }
    }

    return strTemp.Left(iFirstDoubleSlash + 1) + strTemp.Substring(iFirstDoubleSlash + 1).Replaced(DIRECTORY_SEPARATOR_CHAR DIRECTORY_SEPARATOR_CHAR, DIRECTORY_SEPARATOR_CHAR, true);
}

String PathJoin(const String& str1, const String& str2)
{
    return PathConform(str1 + DIRECTORY_SEPARATOR_CHAR + str2);
}

String PathJoin(const String& str1, const String& str2, const String& str3)
{
    String result = str1 + DIRECTORY_SEPARATOR_CHAR + str2 + DIRECTORY_SEPARATOR_CHAR + str3;
    return PathConform(result);
}

String PathJoin(const String& str1, const String& str2, const String& str3, const String& str4)
{
    String result = str1 + DIRECTORY_SEPARATOR_CHAR + str2 + DIRECTORY_SEPARATOR_CHAR + str3;
    if (str4.Length())
        result += DIRECTORY_SEPARATOR_CHAR + str4;
    return PathConform(result);
}

String PathJoin(const String& str1, const String& str2, const String& str3, const String& str4, const String& str5)
{
    String result = str1 + DIRECTORY_SEPARATOR_CHAR + str2 + DIRECTORY_SEPARATOR_CHAR + str3;
    if (str4.Length())
        result += DIRECTORY_SEPARATOR_CHAR + str4;
    if (str5.Length())
        result += DIRECTORY_SEPARATOR_CHAR + str5;
    return PathConform(result);
}

String GetGTAPath()
{
    static String gtasaPath;
    gtasaPath = RegReadStr("GTAInstallLocation");
    return gtasaPath;
}

String GetCTNPath()
{
    static String ctnPath;
    ctnPath = RegReadStr("InstallLocation");
    return ctnPath;
}

String GetGTAPath(const String& pathToJoin)
{
    return PathJoin(GetGTAPath(), pathToJoin);
}

String GetCTNPath(const String& pathToJoin)
{
    return PathJoin(GetCTNPath(), pathToJoin);
}

}
