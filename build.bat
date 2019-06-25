@echo off
@rem Copyright 2019- <fastype.org>
@rem Apache License Version 2.0

set ROOT=%cd%
set OS="Windows"
echo [fastype] Build for %OS%

@rem third party library
cd src
if not exist spdlog (
    echo [fastype] prepare *spdlog* v1.3.1
    git clone https://github.com/gabime/spdlog.git
    cd spdlog && git checkout v1.3.1 && cd %ROOT%\src
    echo [fastype] prepare *spdlog* v1.3.1 - done
)
if not exist fmt (
    echo [fastype] prepare *fmt* v5.3.0
    git clone https://github.com/fmtlib/fmt.git
    cd fmt && git checkout v5.3.0 && cd %ROOT%\src
    echo [fastype] prepare *fmt* v5.3.0 - done
)
if not exist icu (
    echo [fastype] prepare *icu4c* release-64-2
    git clone https://github.com/unicode-org/icu.git
    cd icu && git checkout release-64-2 && cd %ROOT%\src
    echo [fastype] build *icu4c* release-64-2 manually: https://htmlpreview.github.io/?https://github.com/unicode-org/icu/blob/release-64-2/icu4c/readme.html#HowToBuildWindows
)
if not exist boost (
    echo [fastype] prepare *boost* boost-1.70.0
    git clone https://github.com/boostorg/boost.git
    cd boost && git checkout boost-1.70.0 && git submodule update --init && .\bootstrap.bat && .\b2 && cd %ROOT%\src
    echo [fastype] prepare *boost* boost-1.70.0 - done
)

@rem build
cd %ROOT%
set DEBUG=debug
set RELEASE=release
if not exist %DEBUG% md %DEBUG%
if not exist %RELEASE% md %RELEASE%
cd %DEBUG% && cmake -DF_OS=%OS% -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_GENERATOR_PLATFORM=x64 --config Debug ..\src && cd %ROOT%
cd %RELEASE% && cmake -DF_OS=%OS% -DCMAKE_BUILD_TYPE=Release -DCMAKE_GENERATOR_PLATFORM=x64 --config Release ..\src && cd %ROOT%
