@rem Copyright 2019- <shepherd-lang>
@rem Apache License Version 2.0

@echo off
set ROOT=%cd%
set OS=Windows
set MSVC=msvc

set SPDLOG_VERSION=v1.3.1
set FMTLIB_VERSION=5.3.0
set ENUM_VERSION=0.11.2
set CATCH2_VERSION=v2.9.1
set LLVM_VERSION=llvmorg-9.0.1
set BOOST_VERSION=boost-1.70.0

@rem init third party library
echo [shepherd] prepare catchorg/Catch2 %CATCH2_VERSION%
if not exist %ROOT%\test\catch2 (
    cd %ROOT%\test
    git clone -b %CATCH2_VERSION% --single-branch --depth 1 https://github.com/catchorg/Catch2
)
echo [shepherd] prepare catchorg/Catch2 %CATCH2_VERSION% - done
echo [shepherd] prepare gabime/spdlog %SPDLOG_VERSION%
if not exist %ROOT%\src\spdlog (
    cd %ROOT%\src
    git clone -b %SPDLOG_VERSION% --single-branch --depth 1 https://github.com/gabime/spdlog
)
echo [shepherd] prepare gabime/spdlog %SPDLOG_VERSION% - done
echo [shepherd] prepare fmtlib/fmt %FMTLIB_VERSION%
if not exist %ROOT%\src\fmt (
    cd %ROOT%\src
    git clone -b %FMTLIB_VERSION% --single-branch --depth 1 https://github.com/fmtlib/fmt
)
echo [shepherd] prepare fmtlib/fmt %FMTLIB_VERSION% - done
echo [shepherd] prepare aantron/better-enums %ENUM_VERSION%
if not exist %ROOT%\src\better-enums (
    cd %ROOT%\src
    git clone -b %ENUM_VERSION% --single-branch --depth 1 https://github.com/aantron/better-enums
)
echo [shepherd] prepare aantron/better-enums %ENUM_VERSION% - done
echo [shepherd] prepare boostorg/boost %BOOST_VERSION%
if not exist %ROOT%\src\boost (
    cd %ROOT%\src
    git clone -b %BOOST_VERSION% --single-branch --depth 1 https://github.com/boostorg/boost
    cd boost
    git submodule update --init
    cd %ROOT%
)
if not exist %ROOT%\src\boost\stage (
    cd %ROOT%\src\boost
    cmd /c bootstrap.bat
    cmd /c b2 address-model=64 variant=release link=static runtime-link=shared threading=multi --with-program_options --with-system --with-filesystem
    cd %ROOT%\src\boost\stage\lib
    @rem cp libboost_program_options-vc*-mt-x64-*.lib libboost_program_options-mt-x64.lib
    @rem cp libboost_system-vc*-mt-x64-*.lib libboost_system-mt-x64.lib
    @rem cp libboost_filesystem-vc*-mt-x64-*.lib libboost_filesystem-mt-x64.lib
)
echo [shepherd] prepare boostorg/boost %BOOST_VERSION% - done
echo [shepherd] prepare llvm/llvm-project %LLVM_VERSION%
if not exist %ROOT%\src\llvm-project (
    cd %ROOT%\src
    git clone -b %LLVM_VERSION% --single-branch --depth 1 https://github.com/llvm/llvm-project
)
if not exist %ROOT%\src\llvm-project\llvm\build (
    cd %ROOT%\src\llvm-project\llvm
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="%ROOT%\src\llvm-project\llvm\install" -Thost=x64 ..
)
if not exist %ROOT%\src\llvm-project\llvm\install (
    cd %ROOT%\src\llvm-project\llvm\build
    cmake --build . --config Release --target INSTALL
)
echo [shepherd] prepare llvm/llvm-project %LLVM_VERSION% - done

echo [shepherd] build
cd %ROOT%\src
if exist Token.yy.cpp (
    rm Token.yy.cpp
)
if exist Parser.tab.hpp (
    rm Parser.tab.hpp
)
if exist Parser.tab.cpp (
    rm Parser.tab.cpp
)
win_flex -o Token.yy.cpp Token.l
win_bison -o Parser.tab.cpp --defines=Parser.tab.hpp Parser.y
cd %ROOT%
cmake -DCMAKE_BUILD_TYPE=Release -Thost=x64 -B %MSVC%
cd %MSVC%
cmake --build . --config Release
cd %ROOT%
echo [shepherd] build - done
