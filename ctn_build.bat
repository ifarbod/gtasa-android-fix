:: Script to build CtNorth
:: Author(s):       iFarbod <ifarbod@outlook.com>
::
:: Copyright (c) 2015-2017 Project CtNorth
::
:: Distributed under the MIT license (See accompanying file LICENSE or copy at
:: https://opensource.org/licenses/MIT)

@echo off

call vcvars32.bat
call genversion.bat
::Utils\premake5 install_cef
Utils\premake5 vs2017
msbuild Build/CtNorth.sln /v:minimal /m /p:Configuration=Release /p:Platform="Win32"
Utils\premake5 install_data
::Utils\premake5 install_resources
Utils\premake5 compose_files
timeout /t 5 /nobreak
