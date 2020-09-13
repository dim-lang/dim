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
set LLVM_VERSION=llvmorg-9.0.1
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
    echo [nerd] unknown build type: "%1"
    goto :EOF
)

echo [nerd] build for %OS%, mode=%BUILD_TYPE%

@rem init third party library
echo [nerd] prepare catchorg/Catch2 %CATCH2_VERSION%
if not exist %ROOT%\test\catch2 (
    cd %ROOT%\test
    git clone -b %CATCH2_VERSION% --single-branch --depth 1 https://github.com/catchorg/Catch2
    if errorlevel 1 (
        echo [nerd] prepare catchorg/Catch2 %CATCH2_VERSION% - fail
        goto :EOF
    )
)
echo [nerd] prepare catchorg/Catch2 %CATCH2_VERSION% - done
echo [nerd] prepare gabime/spdlog %SPDLOG_VERSION%
if not exist %ROOT%\src\spdlog (
    cd %ROOT%\src
    git clone -b %SPDLOG_VERSION% --single-branch --depth 1 https://github.com/gabime/spdlog
    if errorlevel 1 (
        echo [nerd] prepare gabime/spdlog %SPDLOG_VERSION% - fail
        goto :EOF
    )
)
echo [nerd] prepare gabime/spdlog %SPDLOG_VERSION% - done
echo [nerd] prepare fmtlib/fmt %FMTLIB_VERSION%
if not exist %ROOT%\src\fmt (
    cd %ROOT%\src
    git clone -b %FMTLIB_VERSION% --single-branch --depth 1 https://github.com/fmtlib/fmt
    if errorlevel 1 (
        echo [nerd] prepare fmtlib/fmt %FMTLIB_VERSION% - fail
        goto :EOF
    )
)
echo [nerd] prepare fmtlib/fmt %FMTLIB_VERSION% - done
echo [nerd] prepare aantron/better-enums %ENUM_VERSION%
if not exist %ROOT%\src\better-enums (
    cd %ROOT%\src
    git clone -b %ENUM_VERSION% --single-branch --depth 1 https://github.com/aantron/better-enums
    if errorlevel 1 (
        echo [nerd] prepare aantron/better-enums %ENUM_VERSION% - fail
        goto :EOF
    )
)
echo [nerd] prepare aantron/better-enums %ENUM_VERSION% - done
echo [nerd] prepare boostorg/boost %BOOST_VERSION%
if not exist %ROOT%\src\boost (
    cd %ROOT%\src
    git clone -b %BOOST_VERSION% --single-branch --depth 1 https://github.com/boostorg/boost
    if errorlevel 1 (
        echo [nerd] prepare boostorg/boost %BOOST_VERSION% - fail
        goto :EOF
    )
    cd boost
    git submodule update --init
    if errorlevel 1 (
        echo [nerd] prepare boostorg/boost %BOOST_VERSION% - fail
        goto :EOF
    )
)
set BOOST_STAGE_DIR=stage_release
set BOOST_VARIANT=release
if "%BUILD_TYPE%" == "Debug" (
    set BOOST_STAGE_DIR=stage_debug
    set BOOST_VARIANT=debug
) else (
    set BOOST_STAGE_DIR=stage_release
    set BOOST_VARIANT=release
)
if not exist %ROOT%\src\boost\%BOOST_STAGE_DIR% (
    cd %ROOT%\src\boost
    cmd /c bootstrap.bat
    if errorlevel 1 (
        echo [nerd] prepare boostorg/boost %BOOST_VERSION% - fail
        goto :EOF
    )
    cmd /c b2 address-model=64 variant=%BOOST_VARIANT% link=static runtime-link=shared threading=multi --with-program_options --with-system --with-filesystem --stagedir=%BOOST_STAGE_DIR% stage
    if errorlevel 1 (
        echo [nerd] prepare boostorg/boost %BOOST_VERSION% - fail
        goto :EOF
    )
)
echo [nerd] prepare boostorg/boost %BOOST_VERSION% - done
echo [nerd] prepare llvm/llvm-project %LLVM_VERSION%
if not exist %ROOT%\src\llvm-project (
    cd %ROOT%\src
    git clone -b %LLVM_VERSION% --single-branch --depth 1 https://github.com/llvm/llvm-project
    if errorlevel 1 (
        echo [nerd] prepare llvm/llvm-project %LLVM_VERSION% - fail
        goto :EOF
    )
)
set LLVM_BUILD_CMAKE_DIR=build_cmake_release
set LLVM_BUILD_DIR=build_release
if "%BUILD_TYPE%" == "Debug" (
    set LLVM_BUILD_CMAKE_DIR=build_cmake_debug
    set LLVM_BUILD_DIR=build_debug
) else (
    set LLVM_BUILD_CMAKE_DIR=build_cmake_release
    set LLVM_BUILD_DIR=build_release
)
if not exist %ROOT%\src\llvm-project\llvm\%LLVM_BUILD_CMAKE_DIR% (
    cd %ROOT%\src\llvm-project\llvm
    cmake -DCMAKE_INSTALL_PREFIX="%ROOT%\src\llvm-project\llvm\%LLVM_BUILD_DIR%" -A x64 -Thost=x64 -B %LLVM_BUILD_CMAKE_DIR%
    if errorlevel 1 (
        echo [nerd] prepare llvm/llvm-project %LLVM_VERSION% - fail
        goto :EOF
    )
)
if not exist %ROOT%\src\llvm-project\llvm\%LLVM_BUILD_DIR% (
    cd %ROOT%\src\llvm-project\llvm\%LLVM_BUILD_CMAKE_DIR%
    cmake --build . --config %BUILD_TYPE% --target INSTALL
    if errorlevel 1 (
        echo [nerd] prepare llvm/llvm-project %LLVM_VERSION% - fail
        goto :EOF
    )
)
echo [nerd] prepare llvm/llvm-project %LLVM_VERSION% - done

echo [nerd] build
set PATH=%ROOT%\src\llvm-project\llvm\%LLVM_BUILD_DIR%\bin;%PATH%
cd %ROOT%
cmake -A x64 -Thost=x64 -B %BUILD_TYPE%
cd %BUILD_TYPE%
cmake --build . --config %BUILD_TYPE%
cd %ROOT%
echo [nerd] build - done
goto :EOF

:HELP
echo [nerd] build help message:
echo usage:
echo   build -r/--release              build release.
echo   build -d/--debug                build debug.
echo flag:
echo   build -h/--help                 show help message.

:EOF
