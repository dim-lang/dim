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
    /usr/local/opt/spdlog/include
    /usr/local/opt/fmt/include
    /usr/local/opt/boost/include
    /usr/local/opt/icu4c/include
    /usr/local/opt/catch2/include
    Catch2/single_include
    )

set(CLIB
    Threads::Threads
    fmt
    boost_program_options-mt
    boost_system-mt
    boost_filesystem-mt
    icuuc
    icuio
    icudata
    icui18n
    tcmalloc
    )

set(CLIB_DIR
    .
    ../src
    /usr/local/opt/spdlog/lib
    /usr/local/opt/fmt/lib
    /usr/local/opt/boost/lib
    /usr/local/opt/icu4c/lib
    /usr/local/opt/gperftools/lib
    )

include_directories(${CINC})
link_directories(${CLIB_DIR})

add_executable(coli-test ${CTEST})
target_include_directories(coli-test PRIVATE ${CINC})
target_link_libraries(coli-test ${CLIB} colicore)
set_target_properties(coli-test PROPERTIES VERSION ${PROJECT_VERSION})
