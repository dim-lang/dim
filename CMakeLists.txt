# Copyright 2019- <coli-lang>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(coli-parent LANGUAGES CXX)

add_subdirectory(src)
if(${COLI_ONLY_RELEASE})
    # skip others when release
    add_subdirectory(test)
    add_subdirectory(example)
endif(${COLI_ONLY_RELEASE})
