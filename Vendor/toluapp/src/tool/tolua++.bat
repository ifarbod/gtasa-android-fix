:: Generate tolua++ source files
:: Author(s):       iFarbod <ifarbod@outlook.com>
::
:: Copyright (c) 2015-2017 Project CtNorth
::
:: Distributed under the MIT license (See accompanying file LICENSE or copy at
:: https:::opensource.org/licenses/MIT)

@echo off
cd %1\Vendor\toluapp\src\tool\
tolua++ -o toluabind.c -H toluabind.h -n tolua tolua_scons.pkg
exit