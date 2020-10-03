# Copyright 2019- <nerd-lang>
# Apache License Version 2.0

$ROOT=(Get-Item .).FullName
$OS=Windows
$BuildType=Release
$InstallPath=$ROOT\install

$SPDLOG_VERSION=v1.3.1
$FMTLIB_VERSION=5.3.0
$ENUM_VERSION=0.11.2
$CATCH2_VERSION=v2.9.1
$LLVM_VERSION=v2.9.1
$BOOST_VERSION=boost-1.70.0

Function Build-Dependency {
    Write-Output "[nerd] prepare catchorg/Catch2 $CATCH2_VERSION"
    If (!(Test-Path $ROOT\test\catch2)) {
        Set-Location -Path $ROOT\test
        git clone -b $CATCH2_VERSION --single-branch --depth 1 https://github.com/catchorg/Catch2
    }
    Write-Output "[nerd] prepare catchorg/Catch2 $CATCH2_VERSION - done"
    Write-Output "[nerd] prepare gabime/spdlog $SPDLOG_VERSION"
    If (!(Test-Path $ROOT\src\spdlog)) {
        Set-Location -Path $ROOT\src
        git clone -b $SPDLOG_VERSION --single-branch --depth 1 https://github.com/gabime/spdlog
    }
    Write-Output "[nerd] prepare gabime/spdlog $SPDLOG_VERSION - done"
    Write-Output "[nerd] prepare fmtlib/fmt $FMTLIB_VERSION"
    If (!(Test-Path $ROOT\src\fmt)) {
        Set-Location -Path $ROOT\src
        git clone -b $FMTLIB_VERSION --single-branch --depth 1 https://github.com/fmtlib/fmt
    }
    Write-Output "[nerd] prepare fmtlib/fmt $FMTLIB_VERSION - done"
    Write-Output "[nerd] prepare aantron/better-enums $ENUM_VERSION"
    If (!(Test-Path $ROOT\src\better-enums) {
        Set-Location -Path $ROOT\src
        git clone -b $ENUM_VERSION --single-branch --depth 1 https://github.com/aantron/better-enums
    }
    Write-Output "[nerd] prepare aantron/better-enums $ENUM_VERSION - done"
    Write-Output "[nerd] prepare boostorg/boost $BOOST_VERSION"
    If (!(Test-Path $ROOT\src\boost)) {
        Set-Location -Path $ROOT\src
        git clone -b $BOOST_VERSION --single-branch --depth 1 https://github.com/boostorg/boost
        Set-Location -Path boost
        git submodule update --init
    }
    $BoostVariant=release
    If ("$BuildType" == "Debug") {
        $BoostVariant=debug
    }
    If (!(Test-Path $ROOT\src\boost\$BuildType)) {
        Set-Location -Path $ROOT\src\boost
        cmd /c bootstrap.bat
        b2 address-model=64 variant=$BoostVariant link=static runtime-link=shared threading=multi --with-program_options --with-system --with-filesystem --stagedir=$BuildType stage
    }
    Write-Output "[nerd] prepare boostorg/boost $BOOST_VERSION - done"
    Write-Output "[nerd] prepare llvm/llvm-project $LLVM_VERSION"
    If (!(Test-Path $ROOT\src\llvm-project) {
        Set-Location -Path $ROOT\src
        git clone -b $LLVM_VERSION --single-branch --depth 1 https://github.com/llvm/llvm-project
    }
    $LLVMConfig=CMakeConfigRelease
    If ("$BuildType" == "Debug") {
        $LLVMConfig=CMakeConfigDebug
    }
    If (!(Test-Path $ROOT\src\llvm-project\llvm\$BuildType) {
        Set-Location -Path $ROOT\src\llvm-project\llvm
        cmake -DLLVM_BUILD_EXAMPLES=OFF -DLLVM_INCLUDE_EXAMPLES=OFF -DLLVM_BUILD_TESTS=OFF -DLLVM_INCLUDE_TESTS=OFF -DLLVM_BUILD_BENCHMARKS=OFF -DLLVM_INCLUDE_BENCHMARKS=OFF -DCMAKE_INSTALL_PREFIX="$ROOT\src\llvm-project\llvm\$BuildType" -A x64 -Thost=x64 -B $LLVMConfig
        Set-Location -Path $ROOT\src\llvm-project\llvm\$LLVMConfig
        cmake --build . --config $BuildType --target INSTALL
    }
    Write-Output "[nerd] prepare llvm/llvm-project $LLVM_VERSION - done"
}

Function Build-Make {
    Set-Location -Path $ROOT
    Write-Output "[nerd] build $BuildType"
    $env:Path="$ROOT\src\llvm-project\llvm\$BuildType\bin;" + $env:Path
    Set-Location -Path $ROOT
    cmake -DNERD_BUILD_TYPE=$BuildType -A x64 -Thost=x64 -B $BuildType
    cd $BuildType
    cmake --build . --config $BuildType
    Set-Location -Path $ROOT
    Write-Output "[nerd] build $BuildType - done"
}

Function Build-Routine {
    Write-Output "[nerd] build for $OS, mode=$BuildType"
    Build-Dependency
    Build-Make
}

Function Build-Install {
    Write-Output "[nerd] install for $OS, path=$InstallPath"
    $BuildType=Release
    cd $ROOT\$BuildType
    Write-Output "[nerd] build $BuildType"
    $env:Path="$ROOT\src\llvm-project\llvm\$BuildType\bin;" + $env:Path
    Set-Location -Path $ROOT
    cmake -DNERD_BUILD_TYPE=$BuildType -DCMAKE_INSTALL_PREFIX=$InstallPath -A x64 -Thost=x64 -B $BuildType
    cd $BuildType
    cmake --build . --config $BuildType --target INSTALL
    Set-Location -Path $ROOT
    Write-Output "[nerd] build $BuildType - done"
}

Function Build-Graph {
    Set-Location -Path $ROOT\test\case
    Get-ChildItem "$ROOT\test\case" -Filter *.dot |
    Foreach-Object {
        $FileName=Get-Content $_.FullName
        Write-Output "[nerd] generate $FileName.png for $FileName"
        dot -Tpng $FileName -o $FileName.png
    }
}

Function Build-Help {
    Write-Output "[nerd] build help message:"
    Write-Output "usage:"
    Write-Output "  build -r/--release      build release."
}
