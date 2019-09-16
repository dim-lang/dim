#! /usr/bin/env bash
# Copyright 2019- <fastype.org>
# Apache License Version 2.0


echo [fastype] prepare spdlog v1.3.1
if [ ! -d src/spdlog ]; then
    cd src && git clone -b 'v1.3.1' --single-branch --depth 1 https://github.com/gabime/spdlog.git && cd ..
fi
echo [fastype] prepare spdlog v1.3.1 - done
echo [fastype] prepare fmt 5.3.0
if [ ! -d src/fmt ]; then
    cd src && git clone -b '5.3.0' --single-branch --depth 1 https://github.com/fmtlib/fmt.git && cd ..
fi
echo [fastype] prepare fmt 5.3.0 - done


function build_apt() {
    sudo echo [fastype] prepare compiler and toolchain for linux || { echo [fastype] sudo not found; exit 3; }
    sudo apt-get install gcc g++ make cmake automake autoconf
    echo [fastype] prepare compiler and toolchain for linux - done
    echo [fastype] prepare boost, icu4c, gperftools
    sudo apt-get install libboost-all-dev libicu-dev libgoogle-perftools-dev valgrind
    echo [fastype] prepare boost, icu4c, gperftools - done
}

function build_dnf() {
    sudo echo [fastype] prepare compiler and toolchain for linux || { echo [fastype] sudo not found; exit 3; }
    sudo dnf install gcc make cmake automake autoconf
    echo [fastype] prepare compiler and toolchain for linux - done
    echo [fastype] prepare boost, icu4c, gperftools
    sudo dnf install boost-devel icu-devel gperftools-devel
    echo [fastype] prepare boost, icu4c, gperftools - done
}

function build_pacman() {
    sudo echo [fastype] prepare compiler and toolchain for linux || { echo [fastype] sudo not found; exit 3; }
    sudo pacman -S g++ make cmake automake autoconf
    echo [fastype] prepare compiler and toolchain for linux - done
    echo [fastype] prepare boost, icu, gperftools
    yes | sudo pacman -S boost icu gperftools valgrind
    echo [fastype] prepare boost, icu, gperftools - done
}

if [ $(uname) == "Linux" ]; then
    if cat /etc/*release | grep ^NAME | grep Ubuntu 1>/dev/null 2>&1; then
        build_apt
    elif cat /etc/*release | grep ^NAME | grep Debian 1>/dev/null 2>&1; then
        build_apt
    elif cat /etc/*release | grep ^NAME | grep Fedora 1>/dev/null 2>&1; then
        build_dnf
    elif cat /etc/*release | grep ^NAME | grep Manjaro 1>/dev/null 2>&1; then
        build_pacman
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
