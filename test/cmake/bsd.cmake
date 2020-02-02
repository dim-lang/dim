# Copyright 2019- <coli-lang>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(coli-test VERSION 0.1.0 LANGUAGES CXX)
configure_file(../src/Configure.h.in ../src/Configure.h)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
find_package(Threads REQUIRED)

set(CINC
    .
    ../src
    Threads::Threads
    /usr/include
    /usr/local/include
    ../src/spdlog/include
    ../src/fmt/include
    Catch2/single_include
    )

set(CLIB
    Threads::Threads
    boost_program_options
    boost_system
    boost_filesystem
    icuuc
    icuio
    icudata
    icui18n
    )

set(CLIB_DIR
    .
    ../src
    /usr/lib
    /usr/local/lib
    )

add_definitions(-DFMT_HEADER_ONLY)
include_directories(${CINC})
link_directories(${CLIB_DIR})

add_executable(coli-test ${CTEST})
target_include_directories(coli-test PRIVATE ${CINC})
target_link_libraries(coli-test ${CLIB} colicore)
set_target_properties(coli-test PROPERTIES VERSION ${PROJECT_VERSION})
