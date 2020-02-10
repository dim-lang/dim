# coli

coli - a modern, object oriented, high performance and user friendly programming language.

## Why coli?

Here is an example written in coli-lang.

```
/* declare an interface */
interface People {
    func age(): i64;
    func name(): string;
}

/* declare a class implement the interface above */
class Student => People {
    age:i64;
    name:string;

    func Student() { age = 0; name = "student"; }
    func Student(age:i64, name:string) { this.age = age; this.name = name; }
    func age(): i64 { return age; }
    func name(): string { return name; }
}

/* declare a function, which 2nd parameter is another function */
func select(s:i64[], compare:func(i64, i64):boolean):i64 => {
    let r:i64 = 0;
    for (let i = 0; i < s.length()-1; i++) {
        r = compare(s[i], s[i+1]) ? s[i] : s[i+1];
    }
    return r;
}

// declare 3 functions, all returns 64 bit signed integers

func max(a:i64, b:i64):i64 => a > b ? a : b;
func min(a:i64, b:i64):i64 => return a < b ? a : b;
func avg(a:i64, b:i64):i64 => { return (a + b) / 2; }

// declare program routine function main
func main():i32 {
    let s1:&Student = new Student(16, "Jack");
    let s2 = Student(17, "Lucy");
    s1.age = 10;
    s2.name = "Tom";
    let x:int[] = int[100];
    let y:&int = new int[100];
    let ss1:Student[] = [ Student(1, "s1"), Student(2, "s2"), Student(3, "s3"), Student(4, "s4") ];
    let ss2:&Student[] = new [ Student(1, "s1"), Student(2, "s2"), Student(3, "s3"), Student(4, "s4") ];
    let ss3:&Student[] = new Student[100];
    ss2 = ss1;
    ss3 = ss2;
    for (let i = 0; i < 4; i++) {
        ss2[i].age = i;
        ss2[i].name = "" + i;
    }
    let ss4:&(&Student[])[] = new (&Student[])[10];
    for (let i = 0; i < 10; i++) {
        ss4[i] = new Student[10];
        for (let j = 0; j < 10; j++) {
            ss4[i,j] = Student(i, "" + j);
        }
    }
    let ss5:Student[,] = Student[10,10];
    for (let i = 0; i < 10; i++) {
        for (let j = 0; j < 10; j++) {
            ss5[i,j] = Student(i, "" + j);
        }
    }

    val mymax:func(i64, i64):i64 = func(a:i64, b:i64):i64 => a > b ? a : b;
    val mymin = func(a:i64, b:i64):i64 => { if (a < b) { return a; } else { return b; } };
    val myavg = func(a:i64, b:i64):i64 => (a + b) / 2;
    let r = select(s, func(a, b) => a > b);
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
|  Windows   | Visual Studio 2017, cmake, git, python3                           |

#### Build

|  Platform      |  Build                                                                          |
|----------------|---------------------------------------------------------------------------------|
|  Linux & UNIX  | `git clone https://github.com/coli-lang/coli.git && cd coli && ./build`     |
|  Windows       | `git clone https://github.com/coli-lang/coli.git && cd coli && .\build.bat` |
