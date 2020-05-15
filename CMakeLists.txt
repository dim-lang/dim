# Copyright 2019- <shepherd-lang>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(shepherd-parent LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
if(WIN32)
    set(CMAKE_C_COMPILER cl)
    set(CMAKE_CXX_COMPILER cl)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14 /W4")
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    find_package(Threads REQUIRED)
endif(WIN32)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(example)
