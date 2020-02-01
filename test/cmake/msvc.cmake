# Copyright 2019- <coli-lang>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(coli-test VERSION 0.1.0 LANGUAGES CXX)
configure_file(../src/Configure.h.in ../src/Configure.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14 /W4")

set(CINC
    .
    ../src
    ../src/spdlog/include
    ../src/fmt/include
    ../src/boost
    ../src/icu/icu4c/include
    Catch2/single_include
    )

set(CLIB
    libboost_program_options-vc141-mt-x32-1_70.lib
    libboost_system-vc141-mt-x32-1_70.lib
    icudt.lib
    icuin.lib
    icuio.lib
    icutu.lib
    icuuc.lib
    )

set(CLIBD
    libboost_program_options-vc141-mt-gd-x32-1_70.lib
    libboost_system-vc141-mt-gd-x32-1_70.lib
    icudt.lib
    icuind.lib
    icuiod.lib
    icutud.lib
    icuucd.lib
    )

set(CLIB_DIR
    .
    ../src
    ../src/boost/stage/lib
    ../src/icu/icu4c/lib
    )

add_definitions(-DFMT_HEADER_ONLY)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
include_directories(${CINC})
link_directories(${CLIB_DIR})

add_executable(coli-test ${CTEST})
target_include_directories(coli-test PRIVATE ${CINC})
#target_link_libraries(coli-test debug ${CLIBD} colicore)
target_link_libraries(coli-test ${CLIB} colicore)
set_target_properties(coli-test PROPERTIES VERSION ${PROJECT_VERSION})
