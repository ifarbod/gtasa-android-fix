:: Download CEF to vendor/cef3
:: Author(s):       iFarbod <ifarbod@outlook.com>
::
:: Copyright (c) 2015-2017 The San Andreas Online Open Source Project
::
:: Distributed under the MIT license (See accompanying file LICENSE or copy at
:: https://opensource.org/licenses/MIT)

@echo off

:: Download latest CEF binaries
Utils\premake5 install_cef

timeout /t 5 /nobreak
