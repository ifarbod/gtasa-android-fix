:: Generate VS projects
:: Author(s):       iFarbod <ifarbod@outlook.com>
::
:: Copyright (c) 2015-2017 CTNorth Team
::
:: Distributed under the MIT license (See accompanying file LICENSE or copy at
:: https://opensource.org/licenses/MIT)

@echo off

:: Install/Update CEF
::Utils\premake5 install_cef

:: Generate VS2017 projects
echo Generating project files for Visual Studio 15 (2017)...
Utils\premake5 %* vs2017

:: Pause for 5 seconds and auto-close the command window
timeout /t 5 /nobreak
