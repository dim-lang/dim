# Copyright 2019- <coli-lang>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(coli-test VERSION 0.1.0 LANGUAGES CXX)
configure_file(../src/Configure.h.in ../src/Configure.h)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
find_package(Threads REQUIRED)

set(FINC
    .
    ../src
    Threads::Threads
    /usr/include
    /usr/include/x86_64-linux-gnu
    ../src/spdlog/include
    ../src/fmt/include
    Catch2/single_include
    )

set(FLIB
    Threads::Threads
    boost_program_options
    boost_system
    icuuc
    icuio
    icudata
    icui18n
    tcmalloc
    )

set(FLIB_DIR
    .
    ../src
    /usr/lib
    /usr/lib/x86_64-linux-gnu
    )

add_definitions(-DFMT_HEADER_ONLY)
include_directories(${FINC})
link_directories(${FLIB_DIR})

add_executable(coli-test ${FTEST})
target_include_directories(coli-test PRIVATE ${FINC})
target_link_libraries(coli-test ${FLIB} colicore)
set_target_properties(coli-test PROPERTIES VERSION ${PROJECT_VERSION})
