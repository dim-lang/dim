# Copyright 2019- <coli-lang>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype VERSION 0.1.0 LANGUAGES CXX)
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

add_library(fastypecore STATIC ${FCORE})
target_include_directories(fastypecore PRIVATE ${FINC})
target_link_libraries(fastypecore ${FLIB})
set_target_properties(fastypecore PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype ${FT})
target_include_directories(fastype PRIVATE ${FINC})
target_link_libraries(fastype ${FLIB} fastypecore)
set_target_properties(fastype PROPERTIES VERSION ${PROJECT_VERSION})
