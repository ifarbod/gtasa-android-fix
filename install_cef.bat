@echo off

rem Download latest CEF binaries
Utils\premake5 install_cef

timeout /t 5 /nobreak
