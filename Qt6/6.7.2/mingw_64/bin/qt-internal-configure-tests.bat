@echo off
setlocal ENABLEDELAYEDEXPANSION ENABLEEXTENSIONS
set script_dir_path=%~dp0
set script_dir_path=%script_dir_path:~0,-1%

rem Extracts the source path, make it native, and put it
rem back again. This is a workaround on Windows LLVM/MINGW
rem to help CMake find source files when doing Unity Build.
set args=%*
set source_dir_path=
set native_source_dir_path=
for %%i in (%args%) do (
    if "%%i"=="-S" (
        set found=true
    ) else if defined found (
        set source_dir_path=%%i
        set native_source_dir_path=%%~dpnxi
        set found=
    )
)

if NOT "%native_source_dir_path%" == "" (
    set args=!args:%source_dir_path%=%native_source_dir_path%!
)

set cmake_scripts_dir=%script_dir_path%

set relative_bin_dir=
if NOT "%relative_bin_dir%" == "" (
set relative_bin_dir="%relative_bin_dir%"\
)

call "%script_dir_path%"\%relative_bin_dir%"qt-cmake.bat" ^
     -DQT_BUILD_STANDALONE_TESTS=ON -DQT_BUILD_EXAMPLES=OFF -DQT_USE_ORIGINAL_COMPILER=ON %args%
