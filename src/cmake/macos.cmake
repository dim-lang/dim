# Copyright 2019- <fastype.org>
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
    /usr/local/opt/spdlog/include
    /usr/local/opt/fmt/include
    /usr/local/opt/boost/include
    /usr/local/opt/icu4c/include
    /usr/local/opt/catch2/include
    )

set(FLIB
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

add_library(fastypecore STATIC ${FCORE})
target_include_directories(fastypecore PRIVATE ${FINC})
target_link_libraries(fastypecore ${FLIB})
set_target_properties(fastypecore PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype ${FT})
target_include_directories(fastype PRIVATE ${FINC})
target_link_libraries(fastype ${FLIB} fastypecore)
set_target_properties(fastype PROPERTIES VERSION ${PROJECT_VERSION})
