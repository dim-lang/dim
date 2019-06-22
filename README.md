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

### Build from source

#### Software Environment

| Platform |                            |
|----------|----------------------------|
|  MacOSX  | clang++ >=3.4, cmake >=3.4 |
|  Macjaro | g++ >=5, cmake >=3.4       |
|  Ubuntu  | g++ >=5, cmake >=3.4       |
|  Windows | msvc >=2015, cmake >=3.4   |


#### Dependencies

| Platform |                                                                       |
|----------|-----------------------------------------------------------------------|
|  MacOSX  | `brew install spdlog boost icu4c fmt`                                 |
|  Macjaro | `yaourt install spdlog-git && sudo pacman -S fmt boost icu4c`         |
|  Ubuntu  | `sudo apt install libspdlog-dev libfmt-dev libboost-dev libicu4c-dev` |
|  Windows |                                                                       |

#### Build

| Platform |                                                                                 |
|----------|---------------------------------------------------------------------------------|
|  MacOSX  | `git clone https://github.com/fastype/fastype.git && cd fastype && ./build`     |
|  Macjaro | `git clone https://github.com/fastype/fastype.git && cd fastype && ./build`     |
|  Ubuntu  | `git clone https://github.com/fastype/fastype.git && cd fastype && ./build`     |
|  Windows | `git clone https://github.com/fastype/fastype.git && cd fastype && .\build.bat` |
