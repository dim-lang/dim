@echo off
@rem Copyright 2019- <fastype.org>
@rem Apache License Version 2.0

set ROOT=%cd%
set OS=Windows
echo [fastype] Build for %OS%

@rem third party library
git submodule update --init --recursive
echo [fastype] prepare spdlog v1.3.1
cd %ROOT%\src\spdlog && git checkout tags/v1.3.1 && cd %ROOT%
echo [fastype] prepare spdlog v1.3.1 - done
echo [fastype] prepare boost boost-1.70.0
cd %ROOT%\src\boost && git checkout tags/boost-1.70.0 && cd %ROOT%
if not exist src\boost\stage\lib (
    cd %ROOT%\src\boost && .\bootstrap.bat && call .\b2 link=shared runtime-debugging=off runtime-link=shared variant=release -j8 && cd %ROOT%
)
echo [fastype] prepare boost boost-1.70.0 - done
echo [fastype] prepare icu4c release-64-2
cd %ROOT%\src\icu && git checkout tags/release-64-2 && cd %ROOT%
if not exist src\icu\icu4c\lib (
    echo [fastype] prepare icu4c release-64-2 manually via src\icu\icu4c\\source\allinone\allinone.sln
    echo [fastype] see reference: https://htmlpreview.github.io/?https://github.com/unicode-org/icu/blob/release-64-2/icu4c/readme.html#HowToBuildWindows
)
echo [fastype] prepare icu4c release-64-2 - done

@rem build
echo [fastype] prepare msvc project
set DEBUG=debug
set RELEASE=release
if not exist %DEBUG% md %DEBUG%
if not exist %RELEASE% md %RELEASE%
cd %DEBUG% && cmake -DF_OS=%OS% -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_GENERATOR_PLATFORM=x86 --config Debug ..\src && cd %ROOT%
cd %RELEASE% && cmake -DF_OS=%OS% -DCMAKE_BUILD_TYPE=Release -DCMAKE_GENERATOR_PLATFORM=x86 --config Release ..\src && cd %ROOT%
echo [fastype] prepare msvc project - done
echo [fastype] build manually via msvc project
