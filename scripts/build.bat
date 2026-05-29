@echo off
setlocal
set "QT_DIR=%USERPROFILE%\Qt\6.8.2\mingw_64"
set "MINGW_DIR=%USERPROFILE%\Qt\Tools\mingw1310_64"
set "PATH=%CD%\build;%QT_DIR%\bin;%MINGW_DIR%\bin;C:\Program Files\CMake\bin;%PATH%"
set "CMAKE_PREFIX_PATH=%QT_DIR%"

if not exist build (
    cmake -B build -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="%QT_DIR%" -DCMAKE_BUILD_TYPE=Release
)
cmake --build build -j 4
"%QT_DIR%\bin\windeployqt.exe" build\SichuanMahjongAssistant.exe
"%QT_DIR%\bin\windeployqt.exe" build\test_shanten.exe
echo.
echo Build complete. Run: build\SichuanMahjongAssistant.exe
