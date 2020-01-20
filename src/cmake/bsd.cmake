# Copyright 2019- <coli-lang>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(coli VERSION 0.1.0 LANGUAGES CXX)
configure_file(Configure.h.in Configure.h)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
find_package(Threads REQUIRED)

set(FINC
    .
    Threads::Threads
    ${CURSES_INCLUDE_DIR}
    spdlog/include
    fmt/include
    /usr/include
    /usr/local/include
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
    )

set(FLIB_DIR
    ${CURSES_LIBRARY_DIRS}
    /usr/lib
    /usr/local/lib
    )

add_definitions(-DFMT_HEADER_ONLY)
include_directories(${FINC})
link_directories(${FLIB_DIR})

add_library(colicore STATIC ${FCORE})
target_include_directories(colicore PRIVATE ${FINC})
target_link_libraries(colicore ${FLIB})
set_target_properties(colicore PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(coli ${FT})
target_include_directories(coli PRIVATE ${FINC})
target_link_libraries(coli ${FLIB} colicore)
set_target_properties(coli PROPERTIES VERSION ${PROJECT_VERSION})
