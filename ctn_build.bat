:: Script to build CtNorth
:: Author(s):       iFarbod <ifarbod@outlook.com>
::
:: Copyright (c) 2015-2017 Project CtNorth
::
:: Distributed under the MIT license (See accompanying file LICENSE or copy at
:: https://opensource.org/licenses/MIT)

@echo off

:: Change the working directory to VS common tools
:: Value for a normal install: C:\Program Files (x86)\Microsoft Visual Studio 15.0\Common7\Tools
:: TODO: Change to VS150
cd %VS140COMNTOOLS%

:: Set MSVC environment variables
call vcvars32.bat

:: Generate version files
call genversion.bat

:: Install/Update CEF
::Utils\premake5 install_cef

:: Generate VS2017 projects
echo Generating project files for Visual Studio 15 (2017)...
Utils\premake5 %* vs2017

:: Build the solution
msbuild Build/CtNorth.sln /v:minimal /m /p:Configuration=Release;Platform=Win32

:: Download and install the required data files
Utils\premake5 install_data

:: Install the game assets
::Utils\premake5 install_resources

:: Compose the required files for building installer
Utils\premake5 compose_files

:: Pause for 5 seconds and auto-close the command window
timeout /t 5 /nobreak
