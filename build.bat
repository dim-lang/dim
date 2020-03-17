@rem Copyright 2019- <coli-lang>
@rem Apache License Version 2.0

@echo off
set ROOT=%cd%
set OS=Windows
set MSVC=msvc

set SPDLOG_VERSION=v1.3.1
set FMTLIB_VERSION=5.3.0
set CATCH2_VERSION=v2.9.1
set BOOST_VERSION=boost-1.70.0
set LLVM_VERSION=llvmorg-9.0.1

@rem init third party library
echo [coli] prepare catchorg/Catch2 %CATCH2_VERSION%
if not exist %ROOT%\test\catch2 (
    cd %ROOT%\test
    git clone -b %CATCH2_VERSION% --single-branch --depth 1 https://github.com/catchorg/Catch2
    cd %ROOT%
)
echo [coli] prepare catchorg/Catch2 %CATCH2_VERSION% - done
echo [coli] prepare gabime/spdlog %SPDLOG_VERSION%
if not exist %ROOT%\src\spdlog (
    cd %ROOT%\src
    git clone -b %SPDLOG_VERSION% --single-branch --depth 1 https://github.com/gabime/spdlog
    cd %ROOT%
)
echo [coli] prepare gabime/spdlog %SPDLOG_VERSION% - done
echo [coli] prepare fmtlib/fmt %FMTLIB_VERSION%
if not exist %ROOT%\src\fmt (
    cd %ROOT%\src
    git clone -b %FMTLIB_VERSION% --single-branch --depth 1 https://github.com/fmtlib/fmt
    cd %ROOT%
)
echo [coli] prepare fmtlib/fmt %FMTLIB_VERSION% - done
echo [coli] prepare boostorg/boost %BOOST_VERSION%
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
    cmd /c b2 link=shared threading=multi runtime-link=shared --with-program_options --with-system --with-filesystem --build-type=complete stage
)
if exist %ROOT%\src\boost\stage\lib (
    cd %ROOT%\src\boost\stage\lib
    cp boost_program_options-vc*-mt-x64-*.dll boost_program_options-mt-x64.dll
    cp boost_program_options-vc*-mt-x64-*.lib boost_program_options-mt-x64.lib
    cp boost_program_options-vc*-mt-gd-x64-*.dll boost_program_options-mt-gd-x64.dll
    cp boost_program_options-vc*-mt-gd-x64-*.lib boost_program_options-mt-gd-x64.lib

    cp boost_system-vc*-mt-x64-*.dll boost_system-mt-x64.dll
    cp boost_system-vc*-mt-x64-*.lib boost_system-mt-x64.lib
    cp boost_system-vc*-mt-gd-x64-*.dll boost_system-mt-gd-x64.dll
    cp boost_system-vc*-mt-gd-x64-*.lib boost_system-mt-gd-x64.lib

    cp boost_filesystem-vc*-mt-x64-*.dll boost_filesystem-mt-x64.dll
    cp boost_filesystem-vc*-mt-x64-*.lib boost_filesystem-mt-x64.lib
    cp boost_filesystem-vc*-mt-gd-x64-*.dll boost_filesystem-mt-gd-x64.dll
    cp boost_filesystem-vc*-mt-gd-x64-*.lib boost_filesystem-mt-gd-x64.lib
)
echo [coli] prepare boostorg/boost %BOOST_VERSION% - done

echo [coli] prepare msvc project
cd %ROOT%
if not exist %MSVC% md %MSVC%
cd %MSVC% && cmake -A x64 -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_BUILD_TYPE=Release --config Release .. && cd %ROOT%
echo [coli] prepare msvc project - done

echo [coli] 1. download `https://sourceforge.net/projects/boost/files/boost/1.70.0/` and extract to `%ROOT%\src\boost` if git clone boostorg/boost too slow
echo [coli] 2. download `https://github.com/llvm/llvm-project/releases/download/llvmorg-9.0.1/llvm-project-9.0.1.tar.xz` and extract to `%ROOT%\src\llvm-project` if git clone llvm/llvm-project too slow
echo [coli] 3. open project `%MSVC%\coli-parent.sln` and build with configuration `Release Win32`
