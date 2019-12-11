@rem Copyright 2019- <fastype.org>
@rem Apache License Version 2.0

@echo off
set ROOT=%cd%
set OS=Windows
echo [fastype] prepare for %OS%

@rem init third party library
echo [fastype] prepare catchorg/Catch2 v2.9.1
if not exist %ROOT%\test\catch2 (
    cd %ROOT%\test
    git clone -b v2.9.1 --single-branch --depth 1 https://github.com/catchorg/Catch2.git
    cd %ROOT%
)
echo [fastype] prepare catchorg/Catch2 v2.9.1 - done
echo [fastype] prepare PDCurses PDCurses_3_6
if not exist %ROOT%\src\PDCurses (
    cd %ROOT%\src
    git clone -b PDCurses_3_6 --single-branch --depth 1 https://github.com/wmcbrine/PDCurses.git
    cd %ROOT%
)
echo [fastype] prepare PDCurses PDCurses_3_6 - done
echo [fastype] prepare gabime/spdlog v1.3.1
if not exist %ROOT%\src\spdlog (
    cd %ROOT%\src
    git clone -b v1.3.1 --single-branch --depth 1 https://github.com/gabime/spdlog.git
    cd %ROOT%
)
echo [fastype] prepare gabime/spdlog v1.3.1 - done
echo [fastype] prepare fmtlib/fmt 5.3.0
if not exist %ROOT%\src\fmt (
    cd %ROOT%\src
    git clone -b 5.3.0 --single-branch --depth 1 https://github.com/fmtlib/fmt.git
    cd %ROOT%
)
echo [fastype] prepare fmtlib/fmt 5.3.0 - done
echo [fastype] prepare nlohmann/json v3.7.0
if not exist %ROOT%\src\json (
    cd %ROOT%\src
    git clone -b v3.7.0 --single-branch --depth 1 https://github.com/nlohmann/json.git
    cd %ROOT%
)
echo [fastype] prepare nlohmann/json v3.7.0 - done
echo [fastype] prepare unicode-org/icu release-64-2
if not exist %ROOT%\src\icu (
    cd %ROOT%\src
    git clone -b release-64-2 --single-branch --depth 1 https://github.com/unicode-org/icu.git
    cd %ROOT%
)
echo [fastype] prepare unicode-org/icu release-64-2 - done
echo [fastype] prepare boostorg/boost boost-1.70.0
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
    cmd /c .\b2 toolset=msvc-14.1 link=static threading=multi runtime-link=shared --build-type=complete stage
)
echo [fastype] prepare boostorg/boost boost-1.70.0 - done

echo [fastype] prepare msvc project
set RELEASE=msvc
cd %ROOT%
if not exist %RELEASE% md %RELEASE%
cd %RELEASE% && cmake -DCMAKE_BUILD_TYPE=Release --config Release .. && cd %ROOT%
echo [fastype] prepare msvc project - done

echo [fastype] 1. please manually build `icu4c` library:
echo [fastype]      $ mv %ROOT%\src\icu \
echo [fastype]      $ cd \
echo [fastype]    open msvc project `\icu\icu4c\source\allinone\allinone.sln`, remove sub projects `common_uwp`, `i18n_uwp`.
echo [fastype]    build `icu4c` library with option `Debug Win32` and `Release Win32`, then move `\icu` library to `%ROOT%\src`
echo [fastype]      $ mv \icu %ROOT%\src
echo [fastype] 2. please manually build msvc project `%DEBUG%\fastype-parent.sln` with `Debug Win32`, `%RELEASE%\fastype-parent.sln` with `Release Win32`
