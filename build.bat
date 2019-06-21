@echo off

@REM create boost header
if not exist src\boost\boost (
    cd src\boost && .\bootstrap.bat && .\b2.bat headers && cd ..\..
)
if not exist -d src\icu\build (
    md src\icu\build && cd src\icu\build && ..\icu4c\source\runConfigureICU Windows && make -j6 check && cd ..\..\..
)

@REM prepare build folder
set DEBUG=debug
set RELEASE=release
if not exist %DEBUG% md %DEBUG%
if not exist %RELEASE% md %RELEASE%

@REM build
cd %DEBUG% && cmake --config Debug ..\src && cd ..
cd %RELEASE% && cmake --config Release ..\src && cd ..
