# Copyright 2019- <coli-lang>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(coli-parent LANGUAGES CXX)

add_subdirectory(src)
message(CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE})
if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release")
    # skip example and test for release
    add_subdirectory(test)
    add_subdirectory(example)
endif(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release")
