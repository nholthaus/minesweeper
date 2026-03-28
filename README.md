# Minesweeper

[![CMake](https://github.com/nholthaus/minesweeper/actions/workflows/build.yml/badge.svg)](https://github.com/nholthaus/minesweeper/actions/workflows/build.yml) [![GitHub Release](https://img.shields.io/github/release/nholthaus/minesweeper.svg?style=flat)]()

A free C++/Qt implementation of minesweeper, now with dark mode!

Download the latest release assets [here](https://github.com/nholthaus/minesweeper/releases/latest). Linux users can either [install from the APT repository](#linux-apt-repository) or [build from source](#build-instructions-macoslinux).

![screenshot_dark](resources/images/screenshot_dark.png)

![screenshot](resources/images/screenshot.png)

![screenshot_lose](resources/images/screenshot_lose.png)

![screenshot_lose](resources/images/screenshot_win.png)

## Build Instructions (MacOS/Linux)

You'll need CMake, a C++20 compiler (e.g. g++13 or clang-18) and Qt 6.7.2 or newer with the `qtscxml` module installed.

```bash
git clone https://github.com/nholthaus/minesweeper.git
cd minesweeper
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --target minesweeper -- -j
./bin/minesweeper
```

## Linux APT Repository

Install the signing key:

```bash
curl -fsSL https://nholthaus.github.io/minesweeper/apt/minesweeper-archive-keyring.asc | gpg --dearmor | sudo tee /usr/share/keyrings/minesweeper-archive-keyring.gpg >/dev/null
```

Add the repository:

```bash
echo "deb [signed-by=/usr/share/keyrings/minesweeper-archive-keyring.gpg] https://nholthaus.github.io/minesweeper/apt stable main" | sudo tee /etc/apt/sources.list.d/minesweeper.list
sudo apt update
```

Install or upgrade Minesweeper:

```bash
sudo apt install minesweeper
```

## Maintainer Notes

The APT repository is published from tag builds to the `gh-pages` branch and is signed with the following GitHub Actions secrets:

- `APT_GPG_PRIVATE_KEY`: ASCII-armored private key used to sign `Release`
- `APT_GPG_KEY_ID`: key ID or fingerprint for the signing key
- `APT_GPG_PASSPHRASE`: passphrase for the private key, if any

GitHub Pages should be configured to serve the `gh-pages` branch from the repository root.
