# Dim Programming Languge

Dim - a static typed, multiple paradigms, high performance and friendly programming language.

## Why Dim?

Dim is a static typed, multiple paradigms, high performance and friendly programming language running on multiple platforms.

Dim use a modern and static typed syntax, combines object-oriented and functional programming paradigms inspired by many programming languages such as C++, Java, Scala, Rust, Haskell, etc. Dim use [LLVM](https://llvm.org/) as binary code backend to improve performance. Dim's efficiency and ease to use let you build highly functional system easily.

## How to start?

### Installation

#### Linux & UNIX dependencies

| Platform          |  Dependencies                                                                                                                 |
|-------------------|-------------------------------------------------------------------------------------------------------------------------------|
| MacOS             | Install Xcode, homebrew, then:<br>`$ brew install cmake flex bison boost llvm`                                                |
| *BSD              | `$ sudo pkg install bash git cmake flex bison boost-all llvm`                                                                 |
| Oracle Solaris    | `$ sudo pkg install bash clang git cmake flex bison llvm clang-build llvm-build`                                              |
| Debian/Ubuntu     | `$ sudo apt-get install git clang make cmake flex bison libboost-all-dev llvm-dev`                                            |
| Fedora            | `$ sudo dnf install git clang make cmake flex bison boost-devel llvm-devel llvm-static`                                       |
| CentOS            | `$ sudo yum install git clang make cmake flex bison boost-devel llvm-devel llvm-static llvm-toolset zlib-devel ncurses-devel` |
| Archlinux/Manjaro | `$ sudo pacman -S git clang make cmake flex bison boost llvm`                                                                 |
| OpenSUSE          | `$ sudo zypper install git gcc gcc-c++ clang cmake flex bison libboost*devel llvm-devel`                                      |

* Fix tool chain version on macOS:
    * flex: `$ echo 'export PATH=/usr/local/opt/flex/bin:$PATH' >> ~/.bash_profile`.
    * bison: `$ echo 'export PATH=/usr/local/opt/bison/bin:$PATH' >> ~/.bash_profile`.
    * llvm: `$ echo 'export LLVM_DIR=/usr/local/opt/llvm/lib/cmake' >> ~/.bash_profile`.

#### Windows dependencies

Note: we use PowerShell as default command line instead of legacy cmd on Windows.

* Install [Visual Studio](https://visualstudio.microsoft.com/downloads/) with options:
    * .NET desktop development
    * Visual C++ desktop development
    * Common Windows Platform development
* Install [chocolatey](https://chocolatey.org/), then install third party dependencies through chocolatey (run as administrator), add `git.exe`, `cmake.exe`, `win_flex.exe`, `win_bison.exe`, `python3.exe` to `$env:Path`:
```
$ choco install git cmake winflexbison3 python
```

Nerd installation requires LLVM and Boost library on Windows, please download [llvm-project-10.0.1.tar.xz](https://github.com/llvm/llvm-project/releases/tag/llvmorg-10.0.1) and [boost_1_70_0.tar.gz](https://sourceforge.net/projects/boost/files/boost/1.70.0/), put in `dim/src` if these 2 packages are too large to git clone.

#### Build

| Platform     | Build                                                                         |
|--------------|-------------------------------------------------------------------------------|
| Linux & UNIX | `$ git clone https://github.com/dim-lang/dim.git && cd dim && ./build -r`  |
| Windows      | `$ git clone https://github.com/dim-lang/dim.git && cd dim && .\build.bat` |
