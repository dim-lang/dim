@echo off

set DEBUG=debug
set RELEASE=release

if not exist %DEBUG% md %DEBUG%
if not exist %RELEASE% md %RELEASE%

cd %DEBUG% && cmake -DCMAKE_BUILD_TYPE=Debug ..\src && cd ..
cd %RELEASE% && cmake -DCMAKE_BUILD_TYPE=Release ..\src && cd ..
