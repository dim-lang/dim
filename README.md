# Fastype

Fastype - A Fast Modern Text Editor

## What's Fastype?

Fastype is a modern text editor, focused on simplicity, efficiency, performance, configurable and scalability.

For this goal, Fastype introduce below techniques.

#### Simplicity
* out of box: default configuration after installation.
* cross platform: Windows, MacOS, Linux(Ubuntu/ArchLinux), BSD (FreeBSD/OpenBSD).

#### Efficiency
* edit mode: normal, key-only.
* key binding: customized key binding.
* batch: process asynchronous concurrent jobs.

#### Performance
* front-back seperation: a daemon servier runs background for logic editing, multiple front editors render.
* limited cost: cpu/memory/disk usage is limited.

## How to install?

* Third Party Dependencies
    * [spdlog](https://github.com/gabime/spdlog) - v1.3.1
    * [boost](https://github.com/boostorg/boost) - boost-1.70.0
    * [ICU](https://github.com/boostorg/boost) - boost-1.70.0

* Manual Build
    * Linux/UNIX

```
$ git clone https://github.com/fastype/fastype.git
$ cd fastype
$ ./build
```

    * Windows

```
$ git clone https://github.com/fastype/fastype.git
$ cd fastype
$ build.bat
```
