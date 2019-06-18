@echo off

set DEBUG=debug
set RELEASE=release

if not exist %DEBUG% md %DEBUG%
if not exist %RELEASE% md %RELEASE%

cd %DEBUG% && cmake --config Debug ..\src && cd ..
cd %RELEASE% && cmake --config Release ..\src && cd ..
