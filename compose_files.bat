:: Ready up files for installer script
:: Author(s):       iFarbod <ifarbod@outlook.com>
::
:: Copyright (c) 2015-2017 CtNorth Team
::
:: Distributed under the MIT license (See accompanying file LICENSE or copy at
:: https://opensource.org/licenses/MIT)

@echo off

:: Generate files required for building the installer
Utils\premake5 compose_files

timeout /t 5 /nobreak
