@echo off

echo Generating project files for Visual Studio 15 (2017)...
echo ""
Utils\premake5 vs2017

timeout /t 5 /nobreak
