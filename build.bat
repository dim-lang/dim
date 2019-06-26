@echo off
@rem Copyright 2019- <fastype.org>
@rem Apache License Version 2.0

set ROOT=%cd%
set OS="Windows"
echo [fastype] Build for %OS%

@rem third party library
git submodule update --init --recursive
echo [fastype] prepare spdlog v1.3.1 - done
echo [fastype] prepare boost boost-1.70.0
cd %ROOT%\src\boost .\bootstrap.bat && .\b2 && cd %ROOT%
echo [fastype] prepare boost boost-1.70.0 - done
echo [fastype] prepare icu4c release-64-2
cd %ROOT%\src\icu && md build && cd build && ..\icu4c\source\runConfigureICU %OS% && make -j8 && cd %ROOT%
echo [fastype] prepare icu4c release-64-2 - done

@rem build
cd %ROOT%
set DEBUG=debug
set RELEASE=release
if not exist %DEBUG% md %DEBUG%
if not exist %RELEASE% md %RELEASE%
cd %DEBUG% && cmake -DF_OS=%OS% -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=ON --config Debug ..\src && cd %ROOT%
cd %RELEASE% && cmake -DF_OS=%OS% -DCMAKE_BUILD_TYPE=Release --config Release ..\src && cd %ROOT%
