@echo off
@rem Copyright 2019- <fastype.org>
@rem Apache License Version 2.0

set ROOT=%cd%
set OS=Windows
echo [fastype] Build for %OS%

@rem third party library
echo [fastype] prepare spdlog v1.3.1
if not exist %ROOT%\src\spdlog (
    cd %ROOT%\src
    git clone -b 'v1.3.1' --single-branch --depth 1 https://github.com/gabime/spdlog.git
    cd %ROOT%
)
echo [fastype] prepare spdlog v1.3.1 - done
echo [fastype] prepare boost boost-1.70.0
if not exist src\boost (
    cd %ROOT%\src
    git clone -b 'boost-1.70.0' --single-branch --depth 1 https://github.com/boostorg/boost.git
    cd boost
    git submodule update --init
    cd %ROOT%
)
if not exist src\boost\stage\lib (
    cd src\boost
    cmd /c .\bootstrap.bat
    cmd /c .\b2 link=shared runtime-debugging=off runtime-link=shared variant=release -j8
    cd %ROOT%
)
echo [fastype] prepare boost boost-1.70.0 - done
echo [fastype] prepare icu4c release-64-2
if not exist src\icu (
    cd %ROOT%\src
    git clone -b 'release-64-2' --single-branch --depth 1 https://github.com/unicode-org/icu.git
    cd %ROOT%
)
if not exist src\icu\icu4c\lib (
    echo [fastype] build icu4c x64 manually via: src\icu\icu4c\\source\allinone\allinone.sln
    echo [fastype] reference: https://htmlpreview.github.io/?https://github.com/unicode-org/icu/blob/release-64-2/icu4c/readme.html#HowToBuildWindows
)
echo [fastype] prepare icu4c release-64-2 - done

@rem build
set DEBUG=debug
set RELEASE=release
cd %ROOT%
if not exist %DEBUG% md %DEBUG%
if not exist %RELEASE% md %RELEASE%
cd %DEBUG% && cmake -DF_OS=%OS% -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_GENERATOR_PLATFORM=x64 --config Debug .. && cd %ROOT%
cd %RELEASE% && cmake -DF_OS=%OS% -DCMAKE_BUILD_TYPE=Release -DCMAKE_GENERATOR_PLATFORM=x64 --config Release .. && cd %ROOT%
echo [fastype] build manually via msvc project
