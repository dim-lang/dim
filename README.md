# Fastype

Fastype - A Fast Modern Text Editor

## What's Fastype?

Fastype is a modern text editor, focused on characteristics below.

### Simplicity
* out of box: default configuration after installation.
* cross platform: Windows, MacOS, Linux(Ubuntu/ArchLinux), BSD (FreeBSD/OpenBSD).

### Efficiency
* edit mode: normal, key-only.
* key binding: customized key binding.
* batch: process asynchronous concurrent jobs.

### Performance
* front-back seperation: a daemon servier runs background for logic editing, multiple front editors render.
* limited cost: cpu/memory/disk usage is limited.

## How to install?

### Third Party Dependencies
    * [spdlog](https://github.com/gabime/spdlog)
    * [boost](https://github.com/boostorg/boost)
    * [icu4c](https://github.com/unicode-org/icu)
    * [fmt](https://github.com/fmtlib/fmt)

### Manual Build
#### Linux/UNIX

```
$ git clone https://github.com/fastype/fastype.git
$ cd fastype
$ ./build
```

#### Windows

```
$ git clone https://github.com/fastype/fastype.git
$ cd fastype
$ build.bat
```
