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

set WINDOWS=windows
cd %ROOT%
if not exist %WINDOWS% md %WINDOWS%
cp src\CMakeWindows.cmake src\CMakeLists.txt
cp test\CMakeWindows.cmake test\CMakeLists.txt
cp example\CMakeWindows.cmake example\CMakeLists.txt

echo [fastype] prepare microsoft/vcpkg
if not exist vcpkg (
    git clone https://github.com/Microsoft/vcpkg.git
)
cd vcpkg
if not exist downloads (
    git clone --depth 1 https://github.com/fastype/vcpkg-windows-downloads.git downloads
)
cp ..\vcpkg-triplets-x64-windows-vc141.cmake triplets\x64-windows-vc141.cmake
.\bootstrap-vcpkg.bat
echo [fastype] prepare microsoft/vcpkg - done

echo [fastype] prepare unicode-org/icu
.\vcpkg install icu --triplet x64-windows-vc141
echo [fastype] prepare unicode-org/icu - done
echo [fastype] prepare boostorg/boost
.\vcpkg install boost-locale[icu] boost-regex[icu] boost --triplet x64-windows-vc141 --recurse
echo [fastype] prepare boostorg/boost - done

echo [fastype] prepare msvc project
cd %ROOT%
cd %WINDOWS% && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_GENERATOR_PLATFORM=x64 --config Release .. && cd %ROOT%
echo [fastype] prepare msvc project - done

echo [fastype] NOTICE:
echo [fastype]   1 - open msvc project `windows/fastype-parent.sln`
echo [fastype]   2 - build fastype with msvc project `windows/fastype-parent.sln`
