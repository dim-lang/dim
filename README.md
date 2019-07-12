# Fastype

Fastype - A Fast Modern Text Editor

## What's Fastype?

Fastype is a modern text editor, focused on features below.

* Friendly: out of box, minimum to learn in easy mode.
* Effective: minimum time cost with maximum job done.
* Performance: split editor to server-client model, server for data, client for rendering.
* Programmable: configurations, extensions, plugins and jobs done via scripts.
* Cross Platform: mainstream system such as windows, macOS, linux, bsd, etc.

## How to install?

### Build from source

#### Dependencies

|  Platform  |  Dependencies                             |
|------------|-------------------------------------------|
|  MacOSX    | clang++, cmake, boost, spdlog, fmt, icu4c |
|  Macjaro   | g++, cmake, boost, fmt, icu4c             |
|  Ubuntu    | g++, cmake, boost, fmt, icu4c             |
|  Windows   | msvc, cmake                               |

#### Build

|  Platform      |  Build                                                                          |
|----------------|---------------------------------------------------------------------------------|
|  Linux & UNIX  | `git clone https://github.com/fastype/fastype.git && cd fastype && ./build.sh`  |
|  Windows       | `git clone https://github.com/fastype/fastype.git && cd fastype && .\build.bat` |
