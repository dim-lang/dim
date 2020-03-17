# coli

coli - a modern, object-oriented, high performance programming language.

## Why coli?

coli is a programming language crossed multiple platforms with friendly features and high performance. Grammar is designed combining object-oriented and functional programming, compact memory layout based on [LLVM](https://llvm.org/) backend, compile or interpret as needed. Here're themes we want to introduce coli:

#### Object-Oriented
#### Functional Programming
#### Garbage Collection
#### Thread

## How to write?

Here is an example.

```
/* declare an interface */
interface People {
    func age(): i64;
    func name(): String;
}

/* declare a class implement the interface above */
class Student => People {
    age:i64;
    name:String;
    score:&i64[];

    func Student() {
        age = 0; name = "student"; 
        score = new i64[10];
        for (let i = 0; i < 10; i++) {
            score[i] = 100 - i;
        }
    }
    func Student(age:i64, name:String, score:&i64[]) { this.age = age; this.name = name; this.score = score; }
    func age(): i64 { return age; }
    func name(): String { return name; }
    func score(): &i64[] { return score; }
}

/* declare a function, which 2nd parameter is another function */
func select(s:&i64[], len:i64, compare:func(i64, i64):boolean):i64 => {
    let r = 0;
    for (let i = 0; i < len; i++) {
        r = compare(s[i], s[i+1]) ? s[i] : s[i+1];
    }
    return r;
}

// declare 3 functions, all returns 64 bit signed integers
func max(a:i32, b:i32):i32 => a > b ? a : b;
func min(a:i32, b:i32):i32 => return a < b ? a : b;
func avg(a:i32, b:i32):i32 => { return (a + b) / 2; }

// declare program routine function main
func main():i32 {
    let s1:&Student = new Student(16, "Jack");
    let s2:Student = Student(17, "Lucy");
    s1.age = 10;
    s2.name = "Tom";
    let x:i64[] = i64[100];
    let y:&i64[] = new i64[100];
    let ss1:Student[] = [ Student(1, "s1"), Student(2, "s2"), Student(3, "s3"), Student(4, "s4") ];
    let ss2:&Student[] = new [ Student(1, "s1"), Student(2, "s2"), Student(3, "s3"), Student(4, "s4") ];
    let ss3:&Student[] = new Student[100];
    ss2 = ss1;
    ss3 = ss2;
    for (let i = 0; i < 4; i++) {
        ss2[i].age = i;
        ss2[i].name = "" + i;
    }
    let ss4:&(&(&Student)[])[] = new (&Student[])[10];
    for (let i = 0; i < 10; i++) {
        ss4[i] = new (&Student)[10];
        for (let j = 0; j < 10; j++) {
            ss4[i][j] = new Student(i, "" + j);
        }
    }
    let ss5:Student[][] = Student[10][10];
    for (let i = 0; i < 10; i++) {
        for (let j = 0; j < 10; j++) {
            ss5[i][j] = Student(i, "" + j);
        }
    }

    val mymax:func(i64, i64):i64 = func(a:i64, b:i64):i64 => a > b ? a : b;
    val mymin = func(a:i64, b:i64):i64 => { if (a < b) { return a; } else { return b; } };
    val myavg = func(a:i64, b:i64):i64 => (a + b) / 2;
    let r = select(y, func(a, b) => a > b);
    print("select max: {}", r);
    return 0;
}
```

## How to install?

Download coli source code from github and build mannually.

#### Dependencies

|  Platform  |  Manually Installed Dependencies                                  |
|------------|-------------------------------------------------------------------|
|  MacOSX    | Xcode, git, homebrew                                              |
|  *BSD      | pkg, bash, git                                                    |
|  Manjaro   | git                                                               |
|  Fedora    | git                                                               |
|  Ubuntu    | git                                                               |
|  OpenSUSE  | git                                                               |
|  Windows   | Visual Studio, cmake, git, python3, flex, bison, LLVM             |

Install dependencies on windows:
* Install [Visual Studio](https://visualstudio.microsoft.com/downloads/) with:
    * .NET desktop development
    * Visual C++ desktop development
    * Common Windows Platform development
* Install [cmake](https://cmake.org/download/) and add `cmake.exe` to `PATH`.
* Install [git](https://git-scm.com/downloads) and add `git.exe` to `PATH`.
* Install [python3](https://www.python.org/downloads/) and add `python.exe` to `PATH`.
* Install [win_flex_bison3-latest.zip](https://sourceforge.net/projects/winflexbison/files/) and add `win_flex.exe` and `win_bison.exe` to `PATH`.
* Download [llvm-9.0.1.src.tar.xz](https://github.com/llvm/llvm-project/releases/tag/llvmorg-9.0.1) and build with:
    1. extract `llvm-9.0.1.src.tar.xz` to `coli/src/llvm`.
    2. generate msvc project: `$ cd coli/src/llvm && mkdir build && cd build && cmake -A x64 --config=Release ..`
    3. build `coli/src/llvm/build/LLVM.sln` with both `Debug x64` and `Release x64`.

#### Build

|  Platform      |  Build                                                                                              |
|----------------|-----------------------------------------------------------------------------------------------------|
|  Linux & UNIX  | `git clone https://github.com/coli-lang/coli.git && cd coli && ./build init && ./build release`     |
|  Windows       | `git clone https://github.com/coli-lang/coli.git && cd coli && .\build.bat`                         |

Build msvc project `coli\msvc\coli-parent.sln` with `Release x64`.
