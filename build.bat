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
REM echo [coli] prepare unicode-org/icu %ICU_VERSION%
REM if not exist %ROOT%\src\icu (
    REM cd %ROOT%\src
    REM git clone -b %ICU_VERSION% --single-branch --depth 1 https://github.com/unicode-org/icu
    REM cd %ROOT%
REM )
REM echo [coli] prepare unicode-org/icu %ICU_VERSION% - done
echo [coli] prepare boostorg/boost %BOOST_VERSION%
echo [coli] download pre-built %BOOST_VERSION% binaries and install at `C:\local` manually
echo [coli] download url: `https://sourceforge.net/projects/boost/files/boost-binaries/1.70.0/`
echo [coli] select `boost_1_70_0-msvc-{version}-32.exe` where `{version}` matched your msvc version:
echo [coli]   boost_1_70_0-msvc-14.1-32.exe - Visual Studio 2017
echo [coli]   boost_1_70_0-msvc-14.0-32.exe - Visual Studio 2015
echo [coli]   boost_1_70_0-msvc-12.0-32.exe - Visual Studio 2013
echo [coli]   boost_1_70_0-msvc-11.0-32.exe - Visual Studio 2012
echo [coli]   boost_1_70_0-msvc-10.0-32.exe - Visual Studio 2010
echo [coli] prepare boostorg/boost %BOOST_VERSION% - done

echo [coli] prepare msvc project
cd %ROOT%
if not exist %MSVC% md %MSVC%
cd %MSVC% && cmake -DCMAKE_BUILD_TYPE=Release --config Release .. && cd %ROOT%
echo [coli] prepare msvc project - done

echo [coli] open msvc project `%MSVC%\coli-parent.sln` and build with configuration `Release Win32`
