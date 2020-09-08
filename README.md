# Nerd Programming Languge

Nerd - a modern, multiple paradigms, high performance and friendly programming language.

## Why Nerd?

Nerd is a modern, multiple paradigms, high performance and friendly programming language running on multiple platforms.

Nerd combines object-oriented and functional programming paradigms, inspired by many modern programming languages such as C++, Java, Scala, Rust and Haskell, backended with [LLVM](https://llvm.org/). Nerd try to balance performance, efficiency and ease, so let you build highly functional system easily.

## How to start?

### Installation

#### Linux & UNIX dependencies

| Platform          |  Dependencies                                                                                                                 |
|-------------------|-------------------------------------------------------------------------------------------------------------------------------|
| MacOS             | Install Xcode, homebrew, then:<br>`$ brew install cmake flex bison boost llvm graphviz`                                       |
| *BSD              | `$ sudo pkg install bash git cmake flex bison boost-all llvm graphviz`                                                        |
| Oracle Solaris    | `$ sudo pkg install bash clang git cmake flex bison llvm clang-build llvm-build graphviz`                                     |
| Debian/Ubuntu     | `$ sudo apt-get install git clang make cmake flex bison libboost-all-dev llvm-dev libgraphviz-dev`                            |
| Fedora            | `$ sudo dnf install git clang make cmake flex bison boost-devel llvm-devel llvm-static graphviz-devel`                        |
| CentOS            | `$ sudo yum install git clang make cmake flex bison boost-devel llvm-devel llvm-static llvm-toolset zlib-devel ncurses-devel` |
| Archlinux/Manjaro | `$ sudo pacman -S git clang make cmake flex bison boost llvm graphviz`                                                        |
| OpenSUSE          | `$ sudo zypper install git gcc gcc-c++ clang cmake flex bison libboost*devel llvm-devel graphviz-devel`                       |

* Fix tool chain version on macOS:
    * flex: `$ echo 'export PATH=/usr/local/opt/flex/bin:$PATH' >> ~/.bash_profile`.
    * bison: `$ echo 'export PATH=/usr/local/opt/bison/bin:$PATH' >> ~/.bash_profile`.
    * llvm: `$ echo 'export LLVM_DIR=/usr/local/opt/llvm/lib/cmake' >> ~/.bash_profile`.

#### windows dependencies

* Install [Visual Studio](https://visualstudio.microsoft.com/downloads/) with:
    * .NET desktop development
    * Visual C++ desktop development
    * Common Windows Platform development
* Install [cmake](https://cmake.org/download/) and add `cmake.exe` to `%PATH%`.
* Install [git](https://git-scm.com/downloads) with options below, `git.exe` and other UNIX/Linux commands such as `cp.exe` `mv.exe` `rm.exe` are enabled in Windows CMD after this step.
    * Use Git and optional Unix tools from the Command Prompt.
    * Use Windows' default console window.
* Install [win_flex_bison3-latest.zip](https://sourceforge.net/projects/winflexbison/files/) and add `win_flex.exe` and `win_bison.exe` to `%PATH%`.
* Install [python3](https://www.python.org/downloads/) and add `python3.exe` and `pip3.exe` to `%PATH%`.
* Please download [llvm-project-9.0.1.tar.xz](https://github.com/llvm/llvm-project/releases/tag/llvmorg-9.0.1) from github release for Windows if git clone is slow.
* Please download [boost_1_70_0.tar.gz](https://sourceforge.net/projects/boost/files/boost/1.70.0/) from sourceforge for Windows if git clone is slow.

#### Build

| Platform     | Build                                                                         |
|--------------|-------------------------------------------------------------------------------|
| Linux & UNIX | `$ git clone https://github.com/nerd-lang/nerd.git && cd nerd && ./build -r`  |
| Windows      | `$ git clone https://github.com/nerd-lang/nerd.git && cd nerd && .\build.bat` |
