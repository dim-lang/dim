# Shepherd Programming Languge

Shepherd - a modern, object-oriented, high performance programming language.

## Why shepherd?

Shepherd is a programming language crossed multiple platforms with friendly features and high performance. Shepherd is designed combining object-oriented and functional programming, compact memory layout based on [LLVM](https://llvm.org/) backend, compile or interpret as needed. Here're topics we want to introduce:

#### Object-Oriented
#### Functional Programming
#### Garbage Collection
#### Concurrency

## How to write?

Here is an example:

```
/* declare an interface */
interface People {
    def age(): &int32;
    def name(): &String;
}

/* declare a class implement the interface above */
class Student: People {
    age:int32;
    name:String;
    score:&uint32[];

    def Student() {
        age = 0;
        name = "student"; 
        score = new uint32[10];
        for (var i = 0; i < 10; i++) {
            score[i] = (uint32)i;
        }
    }
    def Student(age:int32, name:String, score:&uint32[]) { 
        this.age = age; 
        this.name = name; 
        this.score = score; 
    }
    def age(): &int32 => &age;
    def name(): &String => &name;
    def score(): &&uint32[] => &score;
}

/* declare a function, which 2nd parameter is another function */
def select(s:&int32[], len:int32, compare:(int32, int32)->boolean):int32 {
    var r = 0;
    for (var i = 0; i < len; i++) {
        r = compare(s[i], s[i+1]) ? s[i] : s[i+1];
    }
    return r;
}

// declare 3 functions, all returns 64 bit signed integers
def max(a:int32, b:int32) => a > b ? a : b;
def min(a:int32, b:int32) => return a < b ? a : b;
def avg(a:int32, b:int32):int32 => return (a + b) / 2;

// declare program routine function main
def main():int32 {
    var s1:&Student = new Student(16, "Jack");
    var s2:Student = Student(17, "Lucy");
    s1.age = 10;
    s2.name = "Tom";
    var x:int64[] = int64[100];
    var y:&int64[] = new int64[100];
    var ss1:Student[] = [ Student(1, "s1"), Student(2, "s2"), Student(3, "s3"), Student(4, "s4") ];
    var ss2:&Student[] = new [ Student(1, "s1"), Student(2, "s2"), Student(3, "s3"), Student(4, "s4") ];
    var ss3:&Student[] = new Student[100];
    ss2 = ss1;
    ss3 = ss2;
    for (var i = 0; i < 4; i++) {
        ss2[i].age = i;
        ss2[i].name = "" + i;
    }
    val mymax:(int64, int64)->int64 = (a:int64, b:int64) => a > b ? a : b;
    val mymin = (a:int64, b:int64) => {
        var x = 0;
        for (var i = 0; i < 10; i++) {
            x += i;
        }
        x;
    };
    val myavg = (a:int64, b:int64) => (a + b) / 2;
    var r = select(y, (a:int32, b:int32) => a > b);
    print("select max: {}", r);
    return 0;
}
```

## How to install?

#### Linux & UNIX dependencies

| Platform          |  Dependencies                                                                                                                 |
|-------------------|-------------------------------------------------------------------------------------------------------------------------------|
| MacOS             | Install Xcode, homebrew, then:<br>`$ brew install cmake flex bison boost llvm`                                                |
| *BSD              | `$ sudo pkg install bash git cmake flex bison boost-all llvm`                                                           |
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
* If git clone is too slow, please download [llvm-project-10.0.0.tar.xz](https://github.com/llvm/llvm-project/releases/tag/llvmorg-10.0.0) from github release for Windows.
* If git clone is too slow, please download [boost_1_70_0.tar.gz](https://sourceforge.net/projects/boost/files/boost/1.70.0/) from sourceforge for Windows.

#### Build

| Platform     | Build                                                                                     |
|--------------|-------------------------------------------------------------------------------------------|
| Linux & UNIX | `$ git clone https://github.com/shepherd-lang/shepherd.git && cd shepherd && ./build -r`  |
| Windows      | `$ git clone https://github.com/shepherd-lang/shepherd.git && cd shepherd && .\build.bat` |
