# Minesweeper

[![CMake](https://github.com/nholthaus/minesweeper/actions/workflows/build.yml/badge.svg)](https://github.com/nholthaus/minesweeper/actions/workflows/build.yml) [![GitHub Release](https://img.shields.io/github/release/nholthaus/minesweeper.svg?style=flat)]()

A free C++/Qt implementation of minesweeper, now with dark mode!

Download the latest Windows installer [here](https://github.com/nholthaus/minesweeper/releases/tag/1.0.4). For MacOS or Linux, [build it from source](#build-instructions-macoslinux)

![screenshot_dark](resources/images/screenshot_dark.png)

![screenshot](resources/images/screenshot.png)

![screenshot_lose](resources/images/screenshot_lose.png)

![screenshot_lose](resources/images/screenshot_win.png)

## Build Instructions (MacOS/Linux)

You'll need CMake, a C++20 compiler (e.g. g++13 or clang-18) and Qt 6.7.2 or newer with the `qtscxml` module installed.

```bash
git clone https://github.com/nholthaus/minesweeper.git
cd minesweeper
$env:PATH = "E:\minesweeper\Qt6\Tools\mingw1120_64\bin;E:\minesweeper\Qt6\6.7.2\mingw_64\bin";
mkdir build
cd build
mingw32-make -j8 2>&1
Start-Process "e:\minesweeper\build\bin\minesweeper.exe"
```
