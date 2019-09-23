#! /usr/bin/env bash
# Copyright 2019- <fastype.org>
# Apache License Version 2.0

function init_macos() {
    echo [fastype] prepare compiler and toolchain for macOS
    brew install gcc cmake automake autoconf
    echo [fastype] prepare compiler and toolchain for macOS - done
    echo [fastype] prepare spdlog, fmt, boost, icu4c, gperftools via homebrew
    brew install spdlog fmt boost icu4c gperftools
    echo [fastype] prepare spdlog, fmt, boost, icu4c, gperftools via homebrew - done
}

function init_apt() {
    sudo echo [fastype] prepare compiler and toolchain || { echo [fastype] sudo not found; exit 3; }
    sudo apt-get install gcc g++ make cmake automake autoconf
    echo [fastype] prepare compiler and toolchain - done
    echo [fastype] prepare boost, icu4c, gperftools
    sudo apt-get install libboost-all-dev libicu-dev libgoogle-perftools-dev valgrind
    echo [fastype] prepare boost, icu4c, gperftools - done
}

function init_dnf() {
    sudo echo [fastype] prepare compiler and toolchain || { echo [fastype] sudo not found; exit 3; }
    sudo dnf install gcc make cmake automake autoconf
    echo [fastype] prepare compiler and toolchain - done
    echo [fastype] prepare boost, icu4c, gperftools
    sudo dnf install boost-devel icu-devel gperftools-devel
    echo [fastype] prepare boost, icu4c, gperftools - done
}

function init_pacman() {
    sudo echo [fastype] prepare compiler and toolchain || { echo [fastype] sudo not found; exit 3; }
    sudo pacman -S g++ make cmake automake autoconf
    echo [fastype] prepare compiler and toolchain - done
    echo [fastype] prepare boost, icu, gperftools
    yes | sudo pacman -S boost icu gperftools valgrind
    echo [fastype] prepare boost, icu, gperftools - done
}

function init_linux() {
    if [ $(uname) == "Linux" ]; then
        echo [fastype] prepare gabime/spdlog v1.3.1
        if [ ! -d src/spdlog ]; then
            cd src && git clone -b 'v1.3.1' --single-branch --depth 1 https://github.com/gabime/spdlog.git && cd ..
        fi
        echo [fastype] prepare gabime/spdlog v1.3.1 - done
        echo [fastype] prepare fmtlib/fmt 5.3.0
        if [ ! -d src/fmt ]; then
            cd src && git clone -b '5.3.0' --single-branch --depth 1 https://github.com/fmtlib/fmt.git && cd ..
        fi
        echo [fastype] prepare fmtlib/fmt 5.3.0 - done
        if cat /etc/*release | grep ^NAME | grep Ubuntu 1>/dev/null 2>&1; then
            init_apt
        elif cat /etc/*release | grep ^NAME | grep Debian 1>/dev/null 2>&1; then
            init_apt
        elif cat /etc/*release | grep ^NAME | grep Fedora 1>/dev/null 2>&1; then
            init_dnf
        elif cat /etc/*release | grep ^NAME | grep Manjaro 1>/dev/null 2>&1; then
            init_pacman
        else
            echo [fastype] Error! Unknown Operating System $(uname) !
            echo [fastype] Please report to https://github.com/fastype/fastype
            exit 3
        fi
    else
        echo [fastype] Error! Unknown Operating System $(uname) !
        echo [fastype] Please report to https://github.com/fastype/fastype
        exit 3
    fi
}

function init_all() {
    echo [fastype] prepare catchorg/Catch2 v2.9.1
    if [ ! -d test/Catch2 ]; then
        cd test && git clone -b 'v2.9.1' --single-branch --depth 1 https://github.com/catchorg/Catch2.git && cd ..
    fi
    echo [fastype] prepare catchorg/Catch2 v2.9.1 - done
    echo [fastype] prepare nlohmann/json v3.7.0
    if [ ! -d src/json ]; then
        cd src && git clone -b 'v3.7.0' --single-branch --depth 1 https://github.com/nlohmann/json.git && cd ..
    fi
    echo [fastype] prepare nlohmann/json v3.7.0 - done
}

init_all
case "$OSTYPE" in
  darwin*)  init_macos ;;
  linux*)   init_linux ;;
  *)        ;;
esac
