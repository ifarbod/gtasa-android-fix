:: Generate VS projects
:: Author(s):       iFarbod <ifarbod@outlook.com>
::
:: Copyright (c) 2015-2017 Project CtNorth
::
:: Distributed under the MIT license (See accompanying file LICENSE or copy at
:: https://opensource.org/licenses/MIT)

@echo off

echo Generating project files for Visual Studio 14 (2015)...
Utils\premake5 vs2015

timeout /t 5 /nobreak
