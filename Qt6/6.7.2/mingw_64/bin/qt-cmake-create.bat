@echo off
setlocal
:: The directory of this script is the expanded absolute path of the "$qt_prefix/bin" directory.
set script_dir_path=%~dp0

:: Try to use original cmake, otherwise to make it relocatable, use any cmake found in PATH.
set cmake_path=C:/CMake/bin/cmake.exe
if not exist "%cmake_path%" set cmake_path=cmake

if NOT "%~2" == "" goto :showhelp
if NOT "%~1" == "" (set PROJECT_DIR=%~1) else (set PROJECT_DIR=%cd%)

"%cmake_path%" -DPROJECT_DIR="%PROJECT_DIR%" -P "%script_dir_path%\../lib/cmake/Qt6\QtInitProject.cmake"
exit /b %errorlevel%

:showhelp
echo Usage
echo.    qt-cmake-create <path/to/project>
exit /b 1
