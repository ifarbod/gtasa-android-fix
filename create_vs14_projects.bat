@echo off

echo Generating project files for Visual Studio 14 (2015)...
cls
Utils\premake5 vs2015

timeout /t 5 /nobreak
