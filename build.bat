@rem Copyright 2019- <coli-lang>
@rem Apache License Version 2.0

@echo off
set ROOT=%cd%
set OS=Windows
set MSVC=msvc

set SPDLOG_VERSION=v1.3.1
set FMTLIB_VERSION=5.3.0
set CATCH2_VERSION=v2.9.1
set ICU_VERSION=release-64-2
set BOOST_VERSION=boost-1.70.0

@rem init third party library
echo [coli] prepare catchorg/Catch2 %CATCH2_VERSION%
if not exist %ROOT%\test\catch2 (
    cd %ROOT%\test
    git clone https://github.com/catchorg/Catch2
    cd Catch2
    git checkout -b %CATCH2_VERSION%
    cd %ROOT%
)
echo [coli] prepare catchorg/Catch2 %CATCH2_VERSION% - done
echo [coli] prepare gabime/spdlog %SPDLOG_VERSION%
if not exist %ROOT%\src\spdlog (
    cd %ROOT%\src
    git clone https://github.com/gabime/spdlog
    cd spdlog
    git checkout -b %SPDLOG_VERSION%
    cd %ROOT%
)
echo [coli] prepare gabime/spdlog %SPDLOG_VERSION% - done
echo [coli] prepare fmtlib/fmt %FMTLIB_VERSION%
if not exist %ROOT%\src\fmt (
    cd %ROOT%\src
    git clone https://github.com/fmtlib/fmt
    cd fmt
    git checkout -b %FMTLIB_VERSION%
    cd %ROOT%
)
echo [coli] prepare fmtlib/fmt %FMTLIB_VERSION% - done
echo [coli] prepare unicode-org/icu %ICU_VERSION%
if not exist %ROOT%\src\icu (
    cd %ROOT%\src
    git clone https://github.com/unicode-org/icu
    cd icu
    git checkout -b %ICU_VERSION%
    cd %ROOT%
)
echo [coli] prepare unicode-org/icu %ICU_VERSION% - done
echo [coli] prepare boostorg/boost %BOOST_VERSION%
if not exist %ROOT%\src\boost (
    cd %ROOT%\src
    git clone https://github.com/boostorg/boost
    cd boost
    git checkout -b %BOOST_VERSION%
    git submodule update --init
    cd %ROOT%
)
if not exist %ROOT%\src\boost\stage (
    cd %ROOT%\src\boost
    cmd /c .\bootstrap.bat
    cmd /c .\b2 link=static threading=multi runtime-link=shared --build-type=complete stage
)
echo [coli] prepare boostorg/boost %BOOST_VERSION% - done

echo [coli] prepare msvc project
cd %ROOT%
if not exist %MSVC% md %MSVC%
cd %MSVC% && cmake -DCMAKE_BUILD_TYPE=Release --config Release .. && cd %ROOT%
echo [coli] prepare msvc project - done

echo [coli] 1. build `icu4c` library:
echo [coli]    suppose coli project location is `C:\\%ROOT%`, move `C:\\%ROOT%\src\icu` to `C:\\` first
echo [coli]    (since icu windows project fail when execution/argument too long in command line)
echo [coli]    $ mv C:\\%ROOT%\src\icu C:\\
echo [coli]    open msvc project `C:\\icu\icu4c\source\allinone\allinone.sln`, remove sub projects `common_uwp`, `i18n_uwp`.
echo [coli]    build `icu4c` library with option `Release x64`, then move `C:\\icu` library back to `C:\\%ROOT%\src`
echo [coli]    $ mv C:\\icu C:\\%ROOT%\src
echo [coli] 2. build msvc project `%MSVC%\coli-parent.sln` with `Release x64`
