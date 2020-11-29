# Copyright 2019- <dim-lang>
# Apache License Version 2.0

$ROOT=(Get-Item .).FullName
$OS="Windows"
$HINT="[dim]"

$SPDLOG_VERSION="v1.3.1"
$FMTLIB_VERSION="5.3.0"
$ENUM_VERSION="0.11.2"
$CATCH2_VERSION="v2.9.1"
$LLVM_VERSION="v2.9.1"
$BOOST_VERSION="boost-1.70.0"

$BUILD_TYPE="Release"

function Check-Dependency {
    Write-Output "$HINT prepare catchorg/Catch2 $CATCH2_VERSION"
    if (!(Test-Path $ROOT\test\catch2)) {
        Set-Location -Path $ROOT\test
        git clone -b $CATCH2_VERSION --single-branch --depth 1 https://github.com/catchorg/Catch2
    }
    Write-Output "$HINT prepare catchorg/Catch2 $CATCH2_VERSION - done"
    Write-Output "$HINT prepare gabime/spdlog $SPDLOG_VERSION"
    if (!(Test-Path $ROOT\src\spdlog)) {
        Set-Location -Path $ROOT\src
        git clone -b $SPDLOG_VERSION --single-branch --depth 1 https://github.com/gabime/spdlog
    }
    Write-Output "$HINT prepare gabime/spdlog $SPDLOG_VERSION - done"
    Write-Output "$HINT prepare fmtlib/fmt $FMTLIB_VERSION"
    if (!(Test-Path $ROOT\src\fmt)) {
        Set-Location -Path $ROOT\src
        git clone -b $FMTLIB_VERSION --single-branch --depth 1 https://github.com/fmtlib/fmt
    }
    Write-Output "$HINT prepare fmtlib/fmt $FMTLIB_VERSION - done"
    Write-Output "$HINT prepare aantron/better-enums $ENUM_VERSION"
    if (!(Test-Path $ROOT\src\better-enums)) {
        Set-Location -Path $ROOT\src
        git clone -b $ENUM_VERSION --single-branch --depth 1 https://github.com/aantron/better-enums
    }
    Write-Output "$HINT prepare aantron/better-enums $ENUM_VERSION - done"
    Write-Output "$HINT prepare boostorg/boost $BOOST_VERSION"
    if (!(Test-Path $ROOT\src\boost)) {
        Set-Location -Path $ROOT\src
        git clone -b $BOOST_VERSION --single-branch --depth 1 https://github.com/boostorg/boost
        Set-Location -Path boost
        git submodule update --init
    }
    $BoostVariant="release"
    if ($BUILD_TYPE -eq "Debug") {
        $BoostVariant="debug"
    }
    if (!(Test-Path $ROOT\src\boost\$BUILD_TYPE)) {
        Set-Location -Path $ROOT\src\boost
        cmd /c bootstrap.bat
        .\b2 address-model=64 variant=$BoostVariant link=static runtime-link=shared threading=multi --with-program_options --with-system --with-filesystem --stagedir=$BUILD_TYPE stage
    }
    Write-Output "$HINT prepare boostorg/boost $BOOST_VERSION - done"
    Write-Output "$HINT prepare llvm/llvm-project $LLVM_VERSION"
    if (!(Test-Path $ROOT\src\llvm-project)) {
        Set-Location -Path $ROOT\src
        git clone -b $LLVM_VERSION --single-branch --depth 1 https://github.com/llvm/llvm-project
    }
    $LLVMCMakeConfig="CMakeConfigRelease"
    if ($BUILD_TYPE -eq "Debug") {
        $LLVMCMakeConfig="CMakeConfigDebug"
    }
    if (!(Test-Path $ROOT\src\llvm-project\llvm\$BUILD_TYPE)) {
        Set-Location -Path $ROOT\src\llvm-project\llvm
        cmake -DLLVM_BUILD_EXAMPLES=OFF -DLLVM_INCLUDE_EXAMPLES=OFF -DLLVM_BUILD_TESTS=OFF -DLLVM_INCLUDE_TESTS=OFF -DLLVM_BUILD_BENCHMARKS=OFF -DLLVM_INCLUDE_BENCHMARKS=OFF -DCMAKE_INSTALL_PREFIX="$ROOT\src\llvm-project\llvm\$BUILD_TYPE" -A x64 -Thost=x64 -B $LLVMCMakeConfig
        Set-Location -Path $ROOT\src\llvm-project\llvm\$LLVMCMakeConfig
        cmake --build . --config $BUILD_TYPE --target INSTALL
    }
    Write-Output "$HINT prepare llvm/llvm-project $LLVM_VERSION - done"
}

function Make-Proj {
    Set-Location -Path $ROOT
    Write-Output "$HINT build $BUILD_TYPE"
    $env:Path="$ROOT\src\llvm-project\llvm\$BUILD_TYPE\bin;" + $env:Path
    Set-Location -Path $ROOT
    if ($BUILD_TYPE -eq "Debug") {
        cmake -DDIM_BUILD_DEBUG=1 -A x64 -Thost=x64 -B $BUILD_TYPE
        cd $BUILD_TYPE
        cmake --build . --config $BUILD_TYPE
    } else {
        cmake -DDIM_BUILD_DEBUG=0 -A x64 -Thost=x64 -B $BUILD_TYPE
        cd $BUILD_TYPE
        cmake --build . --config $BUILD_TYPE
    }
    Set-Location -Path $ROOT
    Write-Output "$HINT build $BUILD_TYPE - done"
}

function Build-Proj {
    Write-Output "$HINT build for $OS, mode=$BUILD_TYPE"
    Check-Dependency
    Make-Proj
}

function Build-Install {
    $InstallPath=Join-Path -Path $ROOT -ChildPath "install"
    Write-Output "$HINT install for $OS, path=$InstallPath"
    $BUILD_TYPE="Release"
    cd $ROOT\$BUILD_TYPE
    Write-Output "$HINT build $BUILD_TYPE"
    $env:Path="$ROOT\src\llvm-project\llvm\$BUILD_TYPE\bin;" + $env:Path
    Set-Location -Path $ROOT
    cmake -DDIM_BUILD_DEBUG=0 -DCMAKE_INSTALL_PREFIX=$InstallPath -A x64 -Thost=x64 -B $BUILD_TYPE
    cd $BUILD_TYPE
    cmake --build . --config $BUILD_TYPE --target INSTALL
    Set-Location -Path $ROOT
    Write-Output "$HINT build $BUILD_TYPE - done"
}

function Dump-Ast {
    if (Test-Path $ROOT\Debug\test\catch2) {
        Set-Location -Path $ROOT\Debug\test\case
        Get-ChildItem -Path "." -Filter *.dot -File -Name |
        Foreach-Object {
            $FileName=$_
            Write-Output "$HINT generate $FileName.png for $FileName"
            dot -Tpng .\$FileName -o .\$FileName.png
        }
    }
    if (Test-Path $ROOT\Release\test\catch2) {
        Set-Location -Path $ROOT\Release\test\case
        Get-ChildItem -Path "." -Filter *.dot -File -Name |
        Foreach-Object {
            $FileName=$_
            Write-Output "$HINT generate $FileName.png for $FileName"
            dot -Tpng .\$FileName -o .\$FileName.png
        }
    }
}

function Help-Msg {
    Write-Output "$HINT build help message:"
    Write-Output "usage:"
    Write-Output "  build -r/--release              build release."
    Write-Output "  build -d/--debug                build debug."
    Write-Output "  build -p/--parser               generate tokenizer and parser."
    Write-Output "  build -g/--graph                generate abstract syntax tree graph."
    Write-Output "  build [option] -i/--install     install with [--path] option."
    Write-Output "option:"
    Write-Output "  --path                          config install path prefix, by default is 'C:\\Program Files\\'."
    Write-Output "flag:"
    Write-Output "  build -h/--help                 show help message."
}

if ($args.Count -eq 0) {
    $BUILD_TYPE="Release"
} elseif ($args[0] -eq "-h" -Or $args[0] -eq "--help") {
    Help-Msg
    exit 0
} elseif ($args[0] -eq "-g" -Or $args[0] -eq "--graph") {
    Dump-Ast
    exit 0
} elseif ($args[0] -eq "-r" -Or $args[0] -eq "--release") {
    $BUILD_TYPE="Release"
} elseif ($args[0] -eq "-d" -Or $args[0] -eq "--debug") {
    $BUILD_TYPE="Debug"
} else {
    Help-Msg
    exit 0
}

Build-Proj
