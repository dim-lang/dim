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
    /usr/local/opt/spdlog/include
    /usr/local/opt/fmt/include
    /usr/local/opt/boost/include
    /usr/local/opt/icu4c/include
    /usr/local/opt/catch2/include
    )

set(CLIB
    Threads::Threads
    panel
    fmt
    boost_program_options-mt
    boost_system-mt
    icuuc
    icuio
    icudata
    icui18n
    tcmalloc
    )

set(CLIB_DIR
    .
    /usr/local/opt/spdlog/lib
    /usr/local/opt/fmt/lib
    /usr/local/opt/boost/lib
    /usr/local/opt/icu4c/lib
    /usr/local/opt/gperftools/lib
    )

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
