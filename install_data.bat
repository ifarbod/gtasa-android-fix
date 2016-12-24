:: Copy data files to Bin/ for debugging
:: Author(s):       iFarbod <ifarbod@outlook.com>
::
:: Copyright (c) 2015-2017 Project CtNorth
::
:: Distributed under the MIT license (See accompanying file LICENSE or copy at
:: https://opensource.org/licenses/MIT)

@echo off

Utils\premake5 install_data

timeout /t 5 /nobreak
