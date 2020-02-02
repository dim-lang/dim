# Copyright 2019- <coli-lang>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(coli VERSION 0.1.0 LANGUAGES CXX)
configure_file(Configure.h.in Configure.h)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
find_package(Threads REQUIRED)

set(CINC
    .
    Threads::Threads
    spdlog/include
    fmt/include
    /usr/include
    /usr/local/include
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
    /usr/lib
    /usr/local/lib
    )

add_definitions(-DFMT_HEADER_ONLY)
include_directories(${CINC})
link_directories(${CLIB_DIR})

add_library(colicore STATIC ${CCORE})
target_include_directories(colicore PRIVATE ${CINC})
target_link_libraries(colicore ${CLIB})
set_target_properties(colicore PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(coli ${CSRC})
target_include_directories(coli PRIVATE ${CINC})
target_link_libraries(coli ${CLIB} colicore)
set_target_properties(coli PROPERTIES VERSION ${PROJECT_VERSION})
