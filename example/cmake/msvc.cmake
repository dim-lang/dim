# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ".")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14 /W4")

message(CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR})
message(CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR})
message(CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH})
message(CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE})
message(CMAKE_VERBOSE_MAKEFILE: ${CMAKE_VERBOSE_MAKEFILE})

set(F_INC
    spdlog/include
    fmt/include
    boost
    icu/icu4c/include
    )

set(F_LIB
    libboost_program_options-vc141-mt-x64-1_70.lib
    libboost_system-vc141-mt-x64-1_70.lib
    icudt.lib
    icuin.lib
    icuio.lib
    icutu.lib
    icuuc.lib
    )

set(F_LIBD
    libboost_program_options-vc141-mt-gd-x64-1_70.lib
    libboost_system-vc141-mt-gd-x64-1_70.lib
    icudtd.lib
    icuind.lib
    icuiod.lib
    icutud.lib
    icuucd.lib
    )

set(F_LIB_DIR
    boost/stage/lib
    icu/icu4c/lib64
    )

add_definitions(-DFMT_HEADER_ONLY)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
include_directories(${F_INC})
link_directories(${F_LIB_DIR})
