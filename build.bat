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
    cmd /c .\bootstrap.bat
    cmd /c .\b2 link=shared threading=multi runtime-link=shared --with-program_options --with-system --with-filesystem --build-type=complete stage
)
if exist %ROOT%\src\boost\stage\lib (
    cd %ROOT%\src\boost\stage\lib
    xcopy boost_program_options-vc*-mt-x32*.dll boost_filesystem.dll /Y
    xcopy boost_program_options-vc*-mt-x32*.lib boost_filesystem.lib /Y
    xcopy boost_system-vc*-mt-x32*.dll boost_filesystem.dll /Y
    xcopy boost_system-vc*-mt-x32*.lib boost_filesystem.lib /Y
    xcopy boost_filesystem-vc*-mt-x32*.dll boost_filesystem.dll /Y
    xcopy boost_filesystem-vc*-mt-x32*.lib boost_filesystem.lib /Y
)
echo [coli] prepare boostorg/boost %BOOST_VERSION% - done

echo [coli] prepare msvc project
cd %ROOT%
if not exist %MSVC% md %MSVC%
cd %MSVC% && cmake -DCMAKE_BUILD_TYPE=Release --config Release .. && cd %ROOT%
echo [coli] prepare msvc project - done

echo [coli] open msvc project `%MSVC%\coli-parent.sln` and build with configuration `Release Win32`
