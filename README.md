# coli

coli - a modern, object oriented, high performance programming language.

## Why coli?

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
|  Windows   | Visual Studio, cmake, git, python3, LLVM, flex, bison             |

Install dependencies on windows:
* [Visual Studio](https://visualstudio.microsoft.com/downloads/), install with **.NET desktop development**, **Visual C++ desktop development**, **Common Windows Platform development**.
* [cmake](https://cmake.org/download/), install and add `cmake.exe` to `PATH`.
* [git](https://git-scm.com/downloads), install and add `git.exe` to `PATH`.
* [python3](https://www.python.org/downloads/), install and add `python.exe` to `PATH`.
* [LLVM](https://llvm.org/), install and add `clang.exe` to `PATH`.
* [flex & bison](https://sourceforge.net/projects/winflexbison/files/), install **win_flex_bison3-latest.zip** and add `win_flex.exe` and `win_bison.exe` to `PATH`.

#### Build

|  Platform      |  Build                                                                                              |
|----------------|-----------------------------------------------------------------------------------------------------|
|  Linux & UNIX  | `git clone https://github.com/coli-lang/coli.git && cd coli && ./build init && ./build release`     |
|  Windows       | `git clone https://github.com/coli-lang/coli.git && cd coli && .\build.bat`                         |

Manually build steps on windows:
1. open `coli\msvc\coli-parent.sln` with Visual Studio.
2. build with configuration `Release Win32` or `Release x64`.
