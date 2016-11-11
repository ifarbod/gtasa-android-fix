@echo off

rem Copy data files to Bin\
Utils\premake5 install_data

timeout /t 5 /nobreak
