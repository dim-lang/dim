# Copyright 2019- <coli-lang>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype-example VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
find_package(Threads REQUIRED)

set(FINC
    Threads::Threads
    ${CURSES_INCLUDE_DIR}
    /usr/include
    /usr/include/x86_64-linux-gnu
    ../src/spdlog/include
    ../src/fmt/include
    )
set(FLIB
    Threads::Threads
    ${CURSES_LIBRARIES}
    boost_program_options
    boost_system
    icuuc
    icuio
    icudata
    icui18n
    tcmalloc
    )
set(FLIB_DIR
    ${CURSES_LIBRARY_DIRS}
    /usr/lib
    /usr/lib/x86_64-linux-gnu
    )

add_definitions(-DFMT_HEADER_ONLY)
include_directories(${FINC})
link_directories(${FLIB_DIR})

add_executable(fastype-icu_ustdio icu_ustdio.cpp)
target_include_directories(fastype-icu_ustdio PRIVATE ${FINC})
target_link_libraries(fastype-icu_ustdio ${FLIB})
set_target_properties(fastype-icu_ustdio PROPERTIES VERSION ${PROJECT_VERSION})
