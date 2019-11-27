# fastype

fastype - a modern text editor for speed.

## What's fastype?

* Friendly: out of box, minimum to learn in easy mode.
* Effective: minimum time cost with maximum job done.
* Performance: fast
* Distributed: split editor to server-client model, server for data, client for rendering.
* Programmable: configurations, extensions, plugins and jobs done via scripts.
* Cross Platform: mainstream system such as windows, macOS, linux, bsd, etc.

## How to install?

Download fastype source code from github and build mannually.

#### Dependencies

|  Platform  |  Manually Installed Dependencies                                  |
|------------|-------------------------------------------------------------------|
|  MacOSX    | clang++, cmake, boost, spdlog, fmt, icu4c                         |
|  Macjaro   | g++, cmake, boost, fmt, icu4c                                     |
|  Ubuntu    | g++, cmake, boost, fmt, icu4c                                     |
|  Windows   | msvc, cmake                                                       |

For windows platform, please build manually:

1. Download prebuilt ICU4C library [here](https://www.npcglib.org/~stathis/blog/precompiled-icu-past/) (choose *ICU v58.2* with *MSVC2017*) and unzip to `/src`.
2. Download boost source code [here](https://www.boost.org/users/history/version_1_70_0.html) (choose `boost_1_70_0.7z`) and unzip to `/src`.
3. Create project with `build.bat` and build with `debug\fastype-parent.sln` and `release\fastype-parent.sln`.


#### Build

|  Platform      |  Build                                                                          |
|----------------|---------------------------------------------------------------------------------|
|  Linux & UNIX  | `git clone https://github.com/fastype/fastype.git && cd fastype && ./build`     |
|  Windows       | `git clone https://github.com/fastype/fastype.git && cd fastype && .\build.bat` |
