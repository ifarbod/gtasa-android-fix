:: Script to build CTNorth
:: Author(s):       iFarbod <ifarbod@outlook.com>
::
:: Copyright (c) 2015-2017 CTNorth Team
::
:: Distributed under the MIT license (See accompanying file LICENSE or copy at
:: https://opensource.org/licenses/MIT)

@echo off

:: Try to find a VS2017 install
set VS15_REG_KEY="HKLM\SOFTWARE\WOW6432Node\Microsoft\VisualStudio\SxS\VS7"
set VS15_REG_SUBKEY="15.0"

:: Check if Visual Studio 15 (2017) is installed
reg query %VS15_REG_KEY% /v %VS15_REG_SUBKEY% > nul 2>&1
if %ERRORLEVEL% == 1 (
	echo Could not find VS SxS install. Make sure you have Visual Studio 2017 installed
    goto end
)

:: Find vcvars32.bat
for /f "skip=2 tokens=2,*" %%a in ('reg query %VS15_REG_KEY% /v %VS15_REG_SUBKEY%') do set VCVARS32PATH="%%bVC\Auxiliary\Build"

if not exist %VCVARS32PATH%\vcvars32.bat (
    echo vcvars32 not found. Make sure you have Visual Studio 2017 installed
    goto end
)

:: Find MSBuild
for /f "skip=2 tokens=2,*" %%a in ('reg query %VS15_REG_KEY% /v %VS15_REG_SUBKEY%') do set MSBUILDPATH="%%bMSBuild\15.0\Bin"

if not exist %MSBUILDPATH%\msbuild.exe (
    echo MSBuild not found. Make sure you have Visual Studio 2017 installed
    goto end
)

:: Find NMake
for /f "skip=2 tokens=2,*" %%a in ('reg query %VS15_REG_KEY% /v %VS15_REG_SUBKEY%') do set NMAKEPATH="%%b\VC\Tools\MSVC\14.10.25017\bin\HostX86\x86"

if not exist %NMAKEPATH%\nmake.exe (
    echo NMake not found. Make sure you have Visual Studio 2017 installed
    goto end
)

echo Found vcvars32.bat at: %VCVARS32PATH%
echo Found MSBuild at: %MSBUILDPATH%
echo Found NMake at: %NMAKEPATH%

echo.
echo.

:: Set MSVC environment variables
cd %VCVARS32PATH%
call vcvars32.bat

:: Install/Update CEF
::Utils\premake5 install_cef

:: Generate VS2017 projects
call Utils\premake5 %* vs2017

:: Build the solution
%MSBUILDPATH%\msbuild Build/CTNorth.sln /v:minimal /m /p:Configuration=Release;Platform=Win32

:: Download and install the required data files
Utils\premake5 install_data

:: Install the game assets
::Utils\premake5 install_resources

:: Compose the required files for building installer
Utils\premake5 compose_files

:: Pause for 5 seconds and auto-close the command window
:end
timeout /t 5 /nobreak
