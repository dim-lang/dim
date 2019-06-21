@echo off

set ROOT=%cd%
echo "[fastype] Build for Windows"

@REM git submodules
git submodule update --init
cd src\boost && git submodule update --init && cd %ROOT%

@REM third party library
if not exist src\boost\boost (
    echo "[fastype] prepare *Boost* library for Windows"
    cd src\boost && .\bootstrap.bat && .\b2.bat headers && cd %ROOT%
)
if not exist -d src\icu\build (
    echo "[fastype] prepare *ICU4C* library for Windows"
    md src\icu\build && cd src\icu\build && ..\icu4c\source\runConfigureICU Windows && cd %ROOT%
)

@REM build folder
set DEBUG=debug
set RELEASE=release
if not exist %DEBUG% md %DEBUG%
if not exist %RELEASE% md %RELEASE%

@REM build
cd %DEBUG% && cmake --config Debug ..\src && cd %ROOT%
cd %RELEASE% && cmake --config Release ..\src && cd %ROOT%
