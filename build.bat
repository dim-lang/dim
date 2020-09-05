@rem Copyright 2019- <nerd-lang>
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
echo [nerd] prepare catchorg/Catch2 %CATCH2_VERSION%
if not exist %ROOT%\test\catch2 (
    cd %ROOT%\test
    git clone -b %CATCH2_VERSION% --single-branch --depth 1 https://github.com/catchorg/Catch2
)
echo [nerd] prepare catchorg/Catch2 %CATCH2_VERSION% - done
echo [nerd] prepare gabime/spdlog %SPDLOG_VERSION%
if not exist %ROOT%\src\spdlog (
    cd %ROOT%\src
    git clone -b %SPDLOG_VERSION% --single-branch --depth 1 https://github.com/gabime/spdlog
)
echo [nerd] prepare gabime/spdlog %SPDLOG_VERSION% - done
echo [nerd] prepare fmtlib/fmt %FMTLIB_VERSION%
if not exist %ROOT%\src\fmt (
    cd %ROOT%\src
    git clone -b %FMTLIB_VERSION% --single-branch --depth 1 https://github.com/fmtlib/fmt
)
echo [nerd] prepare fmtlib/fmt %FMTLIB_VERSION% - done
echo [nerd] prepare aantron/better-enums %ENUM_VERSION%
if not exist %ROOT%\src\better-enums (
    cd %ROOT%\src
    git clone -b %ENUM_VERSION% --single-branch --depth 1 https://github.com/aantron/better-enums
)
echo [nerd] prepare aantron/better-enums %ENUM_VERSION% - done
echo [nerd] prepare boostorg/boost %BOOST_VERSION%
if not exist %ROOT%\src\boost (
    cd %ROOT%\src
    git clone -b %BOOST_VERSION% --single-branch --depth 1 https://github.com/boostorg/boost
    cd boost
    git submodule update --init
)
if not exist %ROOT%\src\boost\stage (
    cd %ROOT%\src\boost
    cmd /c bootstrap.bat
    cmd /c b2 address-model=64 variant=release link=static runtime-link=shared threading=multi --with-program_options --with-system --with-filesystem
)
echo [nerd] prepare boostorg/boost %BOOST_VERSION% - done
echo [nerd] prepare llvm/llvm-project %LLVM_VERSION%
if not exist %ROOT%\src\llvm-project (
    cd %ROOT%\src
    git clone -b %LLVM_VERSION% --single-branch --depth 1 https://github.com/llvm/llvm-project
)
if not exist %ROOT%\src\llvm-project\llvm\cmake_config (
    cd %ROOT%\src\llvm-project\llvm
    cmake -DCMAKE_INSTALL_PREFIX="%ROOT%\src\llvm-project\llvm\build_release" -A x64 -Thost=x64 -B cmake_config
)
if not exist %ROOT%\src\llvm-project\llvm\build_release (
    cd %ROOT%\src\llvm-project\llvm\cmake_config
    cmake --build . --config Release --target INSTALL
)
echo [nerd] prepare llvm/llvm-project %LLVM_VERSION% - done

echo [nerd] build
set PATH=%ROOT%\src\llvm-project\llvm\build_release\bin;%PATH%
cd %ROOT%
cmake -A x64 -Thost=x64 -B %MSVC%
cd %MSVC%
cmake --build . --config Release
cd %ROOT%
echo [nerd] build - done
