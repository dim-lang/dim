# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype VERSION 0.1.0 LANGUAGES CXX)
configure_file(../src/Configure.h.in ../src/Configure.h)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
find_package(Threads REQUIRED)

set(FINC
    .
    ../src
    Threads::Threads
    /usr/include
    /usr/local/include
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
    )

set(FLIB_DIR
    .
    ../src
    /usr/lib
    /usr/local/lib
    )

add_definitions(-DFMT_HEADER_ONLY)
include_directories(${FINC})
link_directories(${FLIB_DIR})

add_executable(fastype-test ${FTEST})
target_include_directories(fastype-test PRIVATE ${FINC})
target_link_libraries(fastype-test ${FLIB} fastypecore)
set_target_properties(fastype-test PROPERTIES VERSION ${PROJECT_VERSION})
