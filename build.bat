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
echo [fastype] prepare boostorg/boost 1.71.0
if not exist %ROOT%\src\boost (
    cd %ROOT%\src
    git clone -b 1.71.0 --single-branch --depth 1 https://github.com/boostorg/boost.git
    cd boost
    git submodule update --init
    cd %ROOT%
)
echo [fastype] prepare boostorg/boost 1.71.0 - done
echo [fastype] prepare unicode-org/icu release-65-1
if not exist %ROOT%\src\icu (
    cd %ROOT%\src
    git clone -b release-65-1 --single-branch --depth 1 https://github.com/unicode-org/icu 
    cd %ROOT%
)
echo [fastype] prepare unicode-org/icu release-65-1 - done

echo [fastype] prepare msvc project
set DEBUG=msvcd
set RELEASE=msvc

cd %ROOT%
if not exist %DEBUG% md %DEBUG%
cp src\cmake\msvcd.cmake src\CMakeLists.txt
cp test\cmake\msvcd.cmake test\CMakeLists.txt
cp example\cmake\msvcd.cmake example\CMakeLists.txt
cd %DEBUG% && cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_GENERATOR_PLATFORM=x64 --config Debug .. && cd %ROOT%

cd %ROOT%
if not exist %RELEASE% md %RELEASE%
cp src\cmake\msvc.cmake src\CMakeLists.txt
cp test\cmake\msvc.cmake test\CMakeLists.txt
cp example\cmake\msvc.cmake example\CMakeLists.txt
cd %RELEASE% && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_GENERATOR_PLATFORM=x64 --config Release .. && cd %ROOT%
echo [fastype] prepare msvc project - done

echo [fastype] 1. please manually build `icu4c` library by msvc project `src\icu\icu4c\source\allinone\allinone.sln` with option `Debug x64` and `Release x64`
echo [fastype] 2. please manually build `boost` library source code with:
echo [fastype]      $ cd %ROOT%\src\boost
echo [fastype]      $ .\bootstrap.bat
echo [fastype]      $ .\b2 (add option `-j8` to use 8 worker threads to build concurrently if you can)
echo [fastype] 3. please manually build debug version with msvc project `%DEBUG%\fastype-parent.sln` with option `Debug x64` 
echo [fastype] 4. please manually build release version with msvc project `%RELEASE%\fastype-parent.sln` with option `Release x64`
