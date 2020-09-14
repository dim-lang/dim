@rem Copyright 2019- <nerd-lang>
@rem Apache License Version 2.0

@echo off
set ROOT=%cd%
set OS=Windows
set BUILD_TYPE=Release

set SPDLOG_VERSION=v1.3.1
set FMTLIB_VERSION=5.3.0
set ENUM_VERSION=0.11.2
set CATCH2_VERSION=v2.9.1
set LLVM_VERSION=llvmorg-10.0.1
set BOOST_VERSION=boost-1.70.0

if "%1" == "" (
    set BUILD_TYPE=Release
) else if "%1" == "-d" (
    set BUILD_TYPE=Debug
) else if "%1" == "--debug" (
    set BUILD_TYPE=Debug
) else if "%1" == "-r" (
    set BUILD_TYPE=Release
) else if "%1" == "--release" (
    set BUILD_TYPE=Release
) else if "%1" == "-h" (
    goto :HELP
) else if "%1" == "--help" (
    goto :HELP
) else (
    echo [nerd] unknown build option: "%1"
    goto :EOF
)

echo [nerd] build for %OS%, mode=%BUILD_TYPE%

@rem init third party library
echo [nerd] prepare catchorg/Catch2 %CATCH2_VERSION%
if not exist %ROOT%\test\catch2 (
    cd %ROOT%\test
    git clone -b %CATCH2_VERSION% --single-branch --depth 1 https://github.com/catchorg/Catch2
)
echo [nerd] prepare catchorg/Catch2 %CATCH2_VERSION% - done
echo [nerd] prepare gabime/spdlog %SPDLOG_VERSION%
if not exist %ROOT%\src\spdlog (
    cd %ROOT%\src
    git clone -b %SPDLOG_VERSION% --single-branch --depth 1 https://github.com/gabime/spdlog
)
echo [nerd] prepare gabime/spdlog %SPDLOG_VERSION% - done
echo [nerd] prepare fmtlib/fmt %FMTLIB_VERSION%
if not exist %ROOT%\src\fmt (
    cd %ROOT%\src
    git clone -b %FMTLIB_VERSION% --single-branch --depth 1 https://github.com/fmtlib/fmt
)
echo [nerd] prepare fmtlib/fmt %FMTLIB_VERSION% - done
echo [nerd] prepare aantron/better-enums %ENUM_VERSION%
if not exist %ROOT%\src\better-enums (
    cd %ROOT%\src
    git clone -b %ENUM_VERSION% --single-branch --depth 1 https://github.com/aantron/better-enums
)
echo [nerd] prepare aantron/better-enums %ENUM_VERSION% - done
echo [nerd] prepare boostorg/boost %BOOST_VERSION%
if not exist %ROOT%\src\boost (
    cd %ROOT%\src
    git clone -b %BOOST_VERSION% --single-branch --depth 1 https://github.com/boostorg/boost
    cd boost
    git submodule update --init
)
set BOOST_VARIANT=release
if "%BUILD_TYPE%" == "Debug" (
    set BOOST_VARIANT=debug
)
if not exist %ROOT%\src\boost\%BUILD_TYPE% (
    cd %ROOT%\src\boost
    cmd /c bootstrap.bat
    cmd /c b2 address-model=64 variant=%BOOST_VARIANT% link=static runtime-link=shared threading=multi --with-program_options --with-system --with-filesystem --stagedir=%BUILD_TYPE% stage
)
echo [nerd] prepare boostorg/boost %BOOST_VERSION% - done
echo [nerd] prepare llvm/llvm-project %LLVM_VERSION%
if not exist %ROOT%\src\llvm-project (
    cd %ROOT%\src
    git clone -b %LLVM_VERSION% --single-branch --depth 1 https://github.com/llvm/llvm-project
)
set LLVM_CONFIG=CMakeConfigRelease
if %BUILD_TYPE% == "Debug" (
    set LLVM_CONFIG=CMakeConfigDebug
)
if not exist %ROOT%\src\llvm-project\llvm\%BUILD_TYPE% (
    cd %ROOT%\src\llvm-project\llvm
    cmake -DLLVM_BUILD_EXAMPLES=OFF -DLLVM_INCLUDE_EXAMPLES=OFF -DLLVM_BUILD_TESTS=OFF -DLLVM_INCLUDE_TESTS=OFF -DLLVM_BUILD_BENCHMARKS=OFF -DLLVM_INCLUDE_BENCHMARKS=OFF -DCMAKE_INSTALL_PREFIX="%ROOT%\src\llvm-project\llvm\%BUILD_TYPE%" -A x64 -Thost=x64 -B %LLVM_CONFIG%
    cd %ROOT%\src\llvm-project\llvm\%LLVM_CONFIG%
    cmake --build . --config %BUILD_TYPE% --target INSTALL
)
echo [nerd] prepare llvm/llvm-project %LLVM_VERSION% - done

echo [nerd] build %BUILD_TYPE%
set PATH=%ROOT%\src\llvm-project\llvm\%BUILD_TYPE%\bin;%PATH%
cd %ROOT%
cmake -DNERD_BUILD_TYPE=%BUILD_TYPE% -A x64 -Thost=x64 -B %BUILD_TYPE%
cd %BUILD_TYPE%
cmake --build . --config %BUILD_TYPE%
cd %ROOT%
echo [nerd] build %BUILD_TYPE% - done
goto :EOF

:HELP
echo [nerd] build help message:
echo usage:
echo   build -r/--release              build release.
echo   build -d/--debug                build debug.
echo flag:
echo   build -h/--help                 show help message.

:EOF
