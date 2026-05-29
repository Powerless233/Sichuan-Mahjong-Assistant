@echo off
setlocal
set "QT_DIR=%USERPROFILE%\Qt\6.8.2\mingw_64"
set "MINGW_DIR=%USERPROFILE%\Qt\Tools\mingw1310_64"
set "PATH=%~dp0..\build;%QT_DIR%\bin;%MINGW_DIR%\bin;%PATH%"
cd /d "%~dp0..\build"
start "" SichuanMahjongAssistant.exe
