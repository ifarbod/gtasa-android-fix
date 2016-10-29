:: version.h & version.nsh generator
:: Author(s):       iFarbod <ifarbod@outlook.com>
::
:: Copyright (c) 2015-2016 The San Andreas Online Open Source Project
::
:: Distributed under the MIT license (See accompanying file LICENSE or copy at
:: https://opensource.org/licenses/MIT)

@echo off

set VERSION_MAJOR=0
set VERSION_MINOR=1
set VERSION_PATCH=0

if defined CI (
set VERSION_PRE_RELEASE_TYPE=ci
) else (
set VERSION_PRE_RELEASE_TYPE=alpha
)

if defined CI (
set VERSION_PRE_RELEASE=%APPVEYOR_BUILD_NUMBER%
) else (
set VERSION_PRE_RELEASE=0
)

:: Client\Version.h
genver Client\Version.h %VERSION_MAJOR% %VERSION_MINOR% %VERSION_PATCH% %VERSION_PRE_RELEASE_TYPE% %VERSION_PRE_RELEASE%

:: Server\Version.h
genver Server\Version.h %VERSION_MAJOR% %VERSION_MINOR% %VERSION_PATCH% %VERSION_PRE_RELEASE_TYPE% %VERSION_PRE_RELEASE%

:: Shared\Installer\Version.nsh
genver Shared\Installer\Version.nsh %VERSION_MAJOR% %VERSION_MINOR% %VERSION_PATCH% %VERSION_PRE_RELEASE_TYPE% %VERSION_PRE_RELEASE%
