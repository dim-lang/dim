# Copyright 2019- <coli-lang>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(coli-example VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
find_package(Threads REQUIRED)

set(CINC
    .
    ../src
    Threads::Threads
    /usr/include
    /usr/include/x86_64-linux-gnu
    ../src/spdlog/include
    ../src/fmt/include
    )
set(CLIB
    Threads::Threads
    boost_program_options
    boost_system
    icuuc
    icuio
    icudata
    icui18n
    tcmalloc
    )
set(CLIB_DIR
    /usr/lib
    /usr/lib/x86_64-linux-gnu
    )

add_definitions(-DFMT_HEADER_ONLY)
include_directories(${CINC})
link_directories(${CLIB_DIR})

add_executable(coli-icu_ustdio icu_ustdio.cpp)
target_include_directories(coli-icu_ustdio PRIVATE ${CINC})
target_link_libraries(coli-icu_ustdio ${CLIB})
set_target_properties(coli-icu_ustdio PROPERTIES VERSION ${PROJECT_VERSION})
