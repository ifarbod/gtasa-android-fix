:: Generate VS projects
:: Author(s):       iFarbod <ifarbod@outlook.com>
::
:: Copyright (c) 2015-2017 Project CtNorth
::
:: Distributed under the MIT license (See accompanying file LICENSE or copy at
:: https://opensource.org/licenses/MIT)

@echo off

start /wait /b Vendor\toluapp\src\tool\tolua++.bat %cd%
echo Generating project files for Visual Studio 15 (2017)...
Utils\premake5 vs2017

timeout /t 5 /nobreak
