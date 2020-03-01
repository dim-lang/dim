@rem Copyright 2019- <coli-lang>
@rem Apache License Version 2.0

@echo off
set ROOT=%cd%
set OS=Windows
echo [coli] prepare for %OS%

@rem init third party library
echo [coli] prepare catchorg/Catch2 v2.9.1
if not exist %ROOT%\test\catch2 (
    cd %ROOT%\test
    git clone -b v2.9.1 --single-branch --depth 1 https://github.com/catchorg/Catch2.git
    cd %ROOT%
)
echo [coli] prepare catchorg/Catch2 v2.9.1 - done
echo [coli] prepare gabime/spdlog v1.3.1
if not exist %ROOT%\src\spdlog (
    cd %ROOT%\src
    git clone -b v1.3.1 --single-branch --depth 1 https://github.com/gabime/spdlog.git
    cd %ROOT%
)
echo [coli] prepare gabime/spdlog v1.3.1 - done
echo [coli] prepare fmtlib/fmt 5.3.0
if not exist %ROOT%\src\fmt (
    cd %ROOT%\src
    git clone -b 5.3.0 --single-branch --depth 1 https://github.com/fmtlib/fmt.git
    cd %ROOT%
)
echo [coli] prepare fmtlib/fmt 5.3.0 - done

REM echo [coli] prepare unicode-org/icu release-64-2
REM if not exist %ROOT%\src\icu (
    REM cd %ROOT%\src
    REM git clone -b release-64-2 --single-branch --depth 1 https://github.com/unicode-org/icu.git
    REM cd %ROOT%
REM )
REM echo [coli] prepare unicode-org/icu release-64-2 - done

echo [coli] prepare boostorg/boost boost-1.70.0
if not exist %ROOT%\src\boost (
    cd %ROOT%\src
    git clone -b boost-1.70.0 --single-branch --depth 1 https://github.com/boostorg/boost.git
    cd boost
    git submodule update --init
    cd %ROOT%
)
if not exist %ROOT%\src\boost\stage (
    cd %ROOT%\src\boost
    cmd /c .\bootstrap.bat
    cmd /c .\b2 link=static threading=multi runtime-link=shared --build-type=complete stage
)
echo [coli] prepare boostorg/boost boost-1.70.0 - done

echo [coli] prepare msvc project
set MSVC=msvc
cd %ROOT%
if not exist %MSVC% md %MSVC%
cd %MSVC% && cmake -DCMAKE_BUILD_TYPE=Release --config Release .. && cd %ROOT%
echo [coli] prepare msvc project - done

echo [coli] 1. please manually build `icu4c` library:
echo [coli]      $ mv %ROOT%\src\icu \
echo [coli]      $ cd \
echo [coli]    open msvc project `\icu\icu4c\source\allinone\allinone.sln`, remove sub projects `common_uwp`, `i18n_uwp`.
echo [coli]    build `icu4c` library with option `Debug Win32` and `Release Win32`, then move `\icu` library to `%ROOT%\src`
echo [coli]      $ mv \icu %ROOT%\src
echo [coli] 2. please manually build msvc project `%MSVC%\coli-parent.sln` with `Release Win32`
