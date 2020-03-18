@rem Copyright 2019- <coli-lang>
@rem Apache License Version 2.0

@echo off
set ROOT=%cd%
set OS=Windows
set MSVC=msvc

set SPDLOG_VERSION=v1.3.1
set FMTLIB_VERSION=5.3.0
set CATCH2_VERSION=v2.9.1

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

echo [coli] build
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
if not exist %MSVC% md %MSVC%
cd %MSVC%
cmake -DYY_NO_UNISTD_H -A x64 ..
cmake --build . --config Release
cd %ROOT%
echo [coli] build - done
