# shepherd programming languge

shepherd - a modern, object-oriented, high performance programming language.

## Why shepherd?

shepherd is a programming language crossed multiple platforms with friendly features and high performance. Grammar is designed combining object-oriented and functional programming, compact memory layout based on [LLVM](https://llvm.org/) backend, compile or interpret as needed. Here're topics we want to introduce:

#### Object-Oriented
#### Functional Programming
#### Garbage Collection
#### Concurrency

## How to write?

Here is an example:

```
/* declare an interface */
interface People {
    func age(): int64;
    func name(): String;
}

/* declare a class implement the interface above */
class Student: People {
    age:int64;
    name:String;
    score:&int64[];

    func Student() {
        age = 0; name = "student"; 
        score = new int64[10];
        for (var i = 0; i < 10; i++) {
            score[i] = 100 - i;
        }
    }
    func Student(age:int64, name:String, score:&int64[]) { this.age = age; this.name = name; this.score = score; }
    func age(): int64 { return age; }
    func name(): String { return name; }
    func score(): &int64[] { return score; }
}

/* declare a function, which 2nd parameter is another function */
func select(s:&int64[], len:int64, compare:func(int64, int64):boolean):int64 => {
    var r = 0;
    for (var i = 0; i < len; i++) {
        r = compare(s[i], s[i+1]) ? s[i] : s[i+1];
    }
    return r;
}

// declare 3 functions, all returns 64 bit signed integers
func max(a:int32, b:int32):int32 => a > b ? a : b;
func min(a:int32, b:int32):int32 => return a < b ? a : b;
func avg(a:int32, b:int32):int32 => { return (a + b) / 2; }

// declare program routine function main
func main():int32 {
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
    var ss4:&(&(&Student)[])[] = new (&Student[])[10];
    for (var i = 0; i < 10; i++) {
        ss4[i] = new (&Student)[10];
        for (var j = 0; j < 10; j++) {
            ss4[i][j] = new Student(i, "" + j);
        }
    }
    var ss5:Student[][] = Student[10][10];
    for (var i = 0; i < 10; i++) {
        for (var j = 0; j < 10; j++) {
            ss5[i][j] = Student(i, "" + j);
        }
    }

    val mymax:func(int64, int64):int64 = func(a:int64, b:int64):int64 => a > b ? a : b;
    val mymin = func(a:int64, b:int64):int64 => { if (a < b) { return a; } else { return b; } };
    val myavg = func(a:int64, b:int64):int64 => (a + b) / 2;
    var r = select(y, func(a, b) => a > b);
    print("select max: {}", r);
    return 0;
}
```

## How to install?

#### Install dependencies on Linux & UNIX

| Platform          |  Dependencies                                                                         |
|-------------------|---------------------------------------------------------------------------------------|
| MacOS             | Install Xcode, git, homebrew<br>`$ brew install cmake flex bison boost llvm`          |
| BSD               | Install pkg, bash<br>`$ sudo pkg install -y git cmake flex bison boost-all llvm`      |
| Debian/Ubuntu     | `$ sudo apt-get install -y git clang make cmake flex bison libboost-all-dev llvm-dev` |
| Fedora            | `$ sudo dnf install -y git clang make cmake flex bison boost-devel llvm-devel`        |
| Archlinux/Manjaro | `$ sudo pacman -S git clang make cmake flex bison boost llvm`                         |
| OpenSUSE          | `$ sudo zypper install -y git clang cmake flex bison libboost*devel llvm-devel`       |

#### Install dependencies on windows

* Install [Visual Studio](https://visualstudio.microsoft.com/downloads/) with:
    * .NET desktop development
    * Visual C++ desktop development
    * Common Windows Platform development
* Install [cmake](https://cmake.org/download/) and add `cmake.exe` to `PATH`.
* Install [git](https://git-scm.com/downloads) and enable Linux/UNIX commands(include `git.exe`) from Windows Command Line.
* Install [python3](https://www.python.org/downloads/) and add `python.exe` to `PATH`.
* Install [win_flex_bison3-latest.zip](https://sourceforge.net/projects/winflexbison/files/) and add `win_flex.exe` and `win_bison.exe` to `PATH`.

#### Build

| Platform     | Build                                                                                          |
|--------------|------------------------------------------------------------------------------------------------|
| Linux & UNIX | `$ git clone https://github.com/shepherd-lang/shepherd.git && cd shepherd && ./build init && ./build release` |
| Windows      | `$ git clone https://github.com/shepherd-lang/shepherd.git && cd shepherd && .\build.bat`                     |

Fix issue while installation:
1. Download source code instead of slowly git clone on windows:
    * [llvm-project-9.0.1.tar.xz](https://github.com/llvm/llvm-project/releases/tag/llvmorg-9.0.1) from github release.
    * [boost_1_70_0.tar.gz](https://sourceforge.net/projects/boost/files/boost/1.70.0/) from sourceforge.
2. Fix version incompatible on macOS:
    * flex: `$ echo 'export PATH="/usr/local/opt/flex/bin:$PATH"' >> ~/.bash_profile`.
    * bison: `$ echo 'export PATH="/usr/local/opt/bison/bin:$PATH"' >> ~/.bash_profile`.
    * llvm: `$ echo 'export LLVM_DIR=/usr/local/opt/llvm/lib/cmake' >> ~/.bash_profile`.
