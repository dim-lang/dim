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

if [ $(uname) == "Linux" ]; then
    if cat /etc/*release | grep ^NAME | grep Ubuntu 1>/dev/null 2>&1; then
        bash ./build_apt.sh
    elif cat /etc/*release | grep ^NAME | grep Debian 1>/dev/null 2>&1; then
        bash ./build_apt.sh
    elif cat /etc/*release | grep ^NAME | grep Fedora 1>/dev/null 2>&1; then
        bash ./build_dnf.sh
    elif cat /etc/*release | grep ^NAME | grep Manjaro 1>/dev/null 2>&1; then
        bash ./build_pacman.sh
    else
        echo [fastype] Error! Unknown Operating System $(uname) !
        echo [fastype] Please report to https://github.com/fastype/fastype
        exit 1
    fi
else
    echo [fastype] Error! Unknown Operating System $(uname) !
    echo [fastype] Please report to https://github.com/fastype/fastype
    exit 1
fi
