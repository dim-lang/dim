# Copyright 2019- <dim-lang>
# Apache License Version 2.0

$ROOT=(Get-Item .).FullName
$OS="Windows"
$BuildType="Release"
$InstallPath=Join-Path -Path $ROOT -ChildPath "install"

$SPDLOG_VERSION="v1.3.1"
$FMTLIB_VERSION="5.3.0"
$ENUM_VERSION="0.11.2"
$CATCH2_VERSION="v2.9.1"
$LLVM_VERSION="v2.9.1"
$BOOST_VERSION="boost-1.70.0"

Function Build-Dependency {
    Write-Output "[dim] prepare catchorg/Catch2 $CATCH2_VERSION"
    If (!(Test-Path $ROOT\test\catch2)) {
        Set-Location -Path $ROOT\test
        git clone -b $CATCH2_VERSION --single-branch --depth 1 https://github.com/catchorg/Catch2
    }
    Write-Output "[dim] prepare catchorg/Catch2 $CATCH2_VERSION - done"
    Write-Output "[dim] prepare gabime/spdlog $SPDLOG_VERSION"
    If (!(Test-Path $ROOT\src\spdlog)) {
        Set-Location -Path $ROOT\src
        git clone -b $SPDLOG_VERSION --single-branch --depth 1 https://github.com/gabime/spdlog
    }
    Write-Output "[dim] prepare gabime/spdlog $SPDLOG_VERSION - done"
    Write-Output "[dim] prepare fmtlib/fmt $FMTLIB_VERSION"
    If (!(Test-Path $ROOT\src\fmt)) {
        Set-Location -Path $ROOT\src
        git clone -b $FMTLIB_VERSION --single-branch --depth 1 https://github.com/fmtlib/fmt
    }
    Write-Output "[dim] prepare fmtlib/fmt $FMTLIB_VERSION - done"
    Write-Output "[dim] prepare aantron/better-enums $ENUM_VERSION"
    If (!(Test-Path $ROOT\src\better-enums)) {
        Set-Location -Path $ROOT\src
        git clone -b $ENUM_VERSION --single-branch --depth 1 https://github.com/aantron/better-enums
    }
    Write-Output "[dim] prepare aantron/better-enums $ENUM_VERSION - done"
    Write-Output "[dim] prepare boostorg/boost $BOOST_VERSION"
    If (!(Test-Path $ROOT\src\boost)) {
        Set-Location -Path $ROOT\src
        git clone -b $BOOST_VERSION --single-branch --depth 1 https://github.com/boostorg/boost
        Set-Location -Path boost
        git submodule update --init
    }
    $BoostVariant=release
    If ($BuildType == "Debug") {
        $BoostVariant=debug
    }
    If (!(Test-Path $ROOT\src\boost\$BuildType)) {
        Set-Location -Path $ROOT\src\boost
        cmd /c bootstrap.bat
        b2 address-model=64 variant=$BoostVariant link=static runtime-link=shared threading=multi --with-program_options --with-system --with-filesystem --stagedir=$BuildType stage
    }
    Write-Output "[dim] prepare boostorg/boost $BOOST_VERSION - done"
    Write-Output "[dim] prepare llvm/llvm-project $LLVM_VERSION"
    If (!(Test-Path $ROOT\src\llvm-project)) {
        Set-Location -Path $ROOT\src
        git clone -b $LLVM_VERSION --single-branch --depth 1 https://github.com/llvm/llvm-project
    }
    $LLVMConfig=CMakeConfigRelease
    If ($BuildType == "Debug") {
        $LLVMConfig=CMakeConfigDebug
    }
    If (!(Test-Path $ROOT\src\llvm-project\llvm\$BuildType)) {
        Set-Location -Path $ROOT\src\llvm-project\llvm
        cmake -DLLVM_BUILD_EXAMPLES=OFF -DLLVM_INCLUDE_EXAMPLES=OFF -DLLVM_BUILD_TESTS=OFF -DLLVM_INCLUDE_TESTS=OFF -DLLVM_BUILD_BENCHMARKS=OFF -DLLVM_INCLUDE_BENCHMARKS=OFF -DCMAKE_INSTALL_PREFIX="$ROOT\src\llvm-project\llvm\$BuildType" -A x64 -Thost=x64 -B $LLVMConfig
        Set-Location -Path $ROOT\src\llvm-project\llvm\$LLVMConfig
        cmake --build . --config $BuildType --target INSTALL
    }
    Write-Output "[dim] prepare llvm/llvm-project $LLVM_VERSION - done"
}

Function Build-Make {
    Set-Location -Path $ROOT
    Write-Output "[dim] build $BuildType"
    $env:Path="$ROOT\src\llvm-project\llvm\$BuildType\bin;" + $env:Path
    Set-Location -Path $ROOT
    cmake -DNERD_BUILD_TYPE=$BuildType -A x64 -Thost=x64 -B $BuildType
    cd $BuildType
    cmake --build . --config $BuildType
    Set-Location -Path $ROOT
    Write-Output "[dim] build $BuildType - done"
}

Function Build-Routine {
    Write-Output "[dim] build for $OS, mode=$BuildType"
    Build-Dependency
    Build-Make
}

Function Build-Install {
    Write-Output "[dim] install for $OS, path=$InstallPath"
    $BuildType="Release"
    cd $ROOT\$BuildType
    Write-Output "[dim] build $BuildType"
    $env:Path="$ROOT\src\llvm-project\llvm\$BuildType\bin;" + $env:Path
    Set-Location -Path $ROOT
    cmake -DNERD_BUILD_TYPE=$BuildType -DCMAKE_INSTALL_PREFIX=$InstallPath -A x64 -Thost=x64 -B $BuildType
    cd $BuildType
    cmake --build . --config $BuildType --target INSTALL
    Set-Location -Path $ROOT
    Write-Output "[dim] build $BuildType - done"
}

Function Build-Graph {
    Set-Location -Path $ROOT\test\case
    Get-ChildItem "$ROOT\test\case" -Filter *.dot |
    Foreach-Object {
        $FileName=Get-Content $_.FullName
        Write-Output "[dim] generate $FileName.png for $FileName"
        dot -Tpng $FileName -o $FileName.png
    }
}

Function Build-Help {
    Write-Output "[dim] build help message:"
    Write-Output "usage:"
    Write-Output "  build -r/--release              build release."
    Write-Output "  build -d/--debug                build debug."
    Write-Output "  build -p/--parser               generate tokenizer and parser."
    Write-Output "  build [option] -i/--install     install with [--path] option."
    Write-Output "option:"
    Write-Output "  --path                          config install path prefix, by default is C:\Program Files\dim."
    Write-Output "flag:"
    Write-Output "  build -h/--help                 show help message."
}

If ($args.Count == 0) {
    $BuildType="Release"
} ElseIf ($args[0] == "-r" -Or $args[0] == "--release") {
    $BuildType="Release"
} ElseIf ($args[0] == "-d" -Or $args[0] == "--debug") {
    $BuildType="Debug"
} ElseIf ($args[0] == "-g" -Or $args[0] == "--graph") {
    Build-Graph
    exit 0
} ElseIf ($args[0] == "-h" -Or $args[0] == "--help") {
    Build-Help
    exit 0
}

Build-Routine
