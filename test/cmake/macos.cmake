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
    /usr/local/opt/ncurses/include
    /usr/local/opt/spdlog/include
    /usr/local/opt/fmt/include
    /usr/local/opt/boost/include
    /usr/local/opt/icu4c/include
    /usr/local/opt/catch2/include
    Catch2/single_include
    )

set(FLIB
    Threads::Threads
    ncurses
    panel
    fmt
    boost_program_options-mt
    boost_system-mt
    icuuc
    icuio
    icudata
    icui18n
    tcmalloc
    #fl
    )

set(FLIB_DIR
    .
    ../src
    /usr/local/opt/ncurses/lib
    /usr/local/opt/spdlog/lib
    /usr/local/opt/fmt/lib
    /usr/local/opt/boost/lib
    /usr/local/opt/icu4c/lib
    /usr/local/opt/gperftools/lib
    #/usr/local/opt/flex/lib
    )

include_directories(${FINC})
link_directories(${FLIB_DIR})

add_executable(fastype-test ${FTEST})
target_include_directories(fastype-test PRIVATE ${FINC})
target_link_libraries(fastype-test ${FLIB} fastypecore)
set_target_properties(fastype-test PROPERTIES VERSION ${PROJECT_VERSION})
