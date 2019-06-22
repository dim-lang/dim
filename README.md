# Fastype

Fastype - A Fast Modern Text Editor

## What's Fastype?

Fastype is a modern text editor, inherit classical text editors'(vim, emacs, etc) advantages , etc. Focused on characteristics below.

* Friendly: out of box, nothing to learn in normal mode.
* Edit Mode: multiple mode such as normal mode, keyboard-only mode.
* Cross Platform: same experience cross windows, macOS, linux(ubuntu/archLinux), bsd (freeBSD/openBSD).
* Customization: customization via scripts like vim/emacs.
* Programmable: system environment integrated, used as a script language interpreter.
* Batch Process: process multiple files asynchronously and concurrently at once.
* Distributed: daemon server runs background for data, client editor for rendering, thus server and client can be distributed.
* Limited Cost: cpu/memory/disk usage is controlled.

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
