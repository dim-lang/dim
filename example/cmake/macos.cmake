# Copyright 2019- <coli-lang>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(coli VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
find_package(Threads REQUIRED)

set(FINC
    .
    ../src
    Threads::Threads
    /usr/local/opt/spdlog/include
    /usr/local/opt/fmt/include
    /usr/local/opt/boost/include
    /usr/local/opt/icu4c/include
    /usr/local/opt/catch2/include
    )
set(FLIB
    Threads::Threads
    fmt
    boost_program_options-mt
    boost_system-mt
    icuuc
    icuio
    icudata
    icui18n
    tcmalloc
    )
set(FLIB_DIR
    .
    /usr/local/opt/spdlog/lib
    /usr/local/opt/fmt/lib
    /usr/local/opt/boost/lib
    /usr/local/opt/icu4c/lib
    /usr/local/opt/gperftools/lib
    )

include_directories(${FINC})
link_directories(${FLIB_DIR})

add_executable(coli-icu_ustdio icu_ustdio.cpp)
target_include_directories(coli-icu_ustdio PRIVATE ${FINC})
target_link_libraries(coli-icu_ustdio ${FLIB})
set_target_properties(coli-icu_ustdio PROPERTIES VERSION ${PROJECT_VERSION})
