@echo off

rem Generate files required for building the installer
Utils\premake5 compose_files

timeout /t 5 /nobreak
