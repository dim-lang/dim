@echo off

@REM create boost header
if not exist src\boost\boost (
    cd src\boost && .\bootstrap.bat && .\b2.bat headers && cd ..\..
)

@REM prepare build folder
set DEBUG=debug
set RELEASE=release
if not exist %DEBUG% md %DEBUG%
if not exist %RELEASE% md %RELEASE%

@REM build
cd %DEBUG% && cmake --config Debug ..\src && cd ..
cd %RELEASE% && cmake --config Release ..\src && cd ..
