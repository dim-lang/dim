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
echo [fastype] prepare unicode-org/icu release-65-1
if not exist src\icu (
    cd %ROOT%\src
    git clone -b release-65-1 --single-branch --depth 1 https://github.com/unicode-org/icu.git
    cd %ROOT%
)
if not exist src\icu\icu4c\lib64 (
    echo [fastype] please manually build icu4c library
)
echo [fastype] prepare unicode-org/icu release-65-1 - done
echo [fastype] prepare boostorg/boost boost-1.71.0
if not exist src\boost (
    cd %ROOT%\src
    git clone -b boost-1.71.0 --single-branch --depth 1 https://github.com/boostorg/boost.git
    cd boost
    git submodule update --init
    cd %ROOT%
)
if not exist src\boost\stage\lib (
    cd src\boost
    cmd /c .\bootstrap.bat
    cmd /c .\b2 -j4
    cd %ROOT%
)
echo [fastype] prepare boostorg/boost boost-1.71.0 - done

set WINDOWS=windows
cd %ROOT%
if not exist %WINDOWS% md %WINDOWS%
cp src\CMakeWindows.cmake src\CMakeLists.txt
cp test\CMakeWindows.cmake test\CMakeLists.txt
cp example\CMakeWindows.cmake example\CMakeLists.txt

echo [fastype] prepare msvc project
cd %ROOT%
cd %WINDOWS% && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_GENERATOR_PLATFORM=x64 --config Release .. && cd %ROOT%
echo [fastype] prepare msvc project - done

echo [fastype]   1 - please download prebuilt `boost_1_70_0-msvc-14.1-64.exe`(https://sourceforge.net/projects/boost/files/boost-binaries/1.70.0/boost_1_70_0-msvc-14.1-64.exe/download) and extract to `src\boost`
echo [fastype]   2 - please download prebuilt `icu4c-64_2-Win64-MSVC2017.zip`(https://github.com/unicode-org/icu/releases/download/release-64-2/icu4c-64_2-Win64-MSVC2017.zip) and extract to `src\icu4c`
echo [fastype]   3 - please manually add `boost` `icu4c` library to msvc project `windows/fastype-parent.sln` and build fastype
