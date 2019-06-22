# Fastype

Fastype - A Fast Modern Text Editor

## What's Fastype?

Fastype is a modern text editor, focused on features below.

* Friendly: out of box, nothing to learn in plain mode.
* Effective: minimum time cost while maximum job done.
* Cross Platform: same experience cross windows, macOS, linux, bsd.
* Configurable: configurations via scripts like vim/emacs.
* Scalabble: extensions and plugins via scripts like vim/emacs.
* Distributed: daemon server for data, client editor for rendering.
* Batch Processed: script jobs submitted and processed asynchronously and concurrently.

## How to install?

### Dependencies

* [spdlog](https://github.com/gabime/spdlog)
* [boost](https://github.com/boostorg/boost)
* [icu4c](https://github.com/unicode-org/icu)
* [fmt](https://github.com/fmtlib/fmt)

### Build from source

#### Software Environment

| Platform |                       | |
|----------|-----------------------|-|
|  MacOSX  | `brew install spdlog` | |
|          | `brew install boost`  | |
|          | `brew install icu4c`  | |
|          | `brew install fmt`    | |

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
