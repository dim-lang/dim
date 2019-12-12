# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype VERSION 0.1.0 LANGUAGES CXX)
configure_file(../src/Configure.h.in ../src/Configure.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14 /W4")

set(FINC
    .
    ../src
    ../src/spdlog/include
    ../src/fmt/include
    ../src/boost
    ../src/icu/icu4c/include
    Catch2/single_include
    )

set(FLIB
    libboost_program_options-vc141-mt-x32-1_70.lib
    libboost_system-vc141-mt-x32-1_70.lib
    icudt.lib
    icuin.lib
    icuio.lib
    icutu.lib
    icuuc.lib
    )

set(FLIBD
    libboost_program_options-vc141-mt-gd-x32-1_70.lib
    libboost_system-vc141-mt-gd-x32-1_70.lib
    icudt.lib
    icuind.lib
    icuiod.lib
    icutud.lib
    icuucd.lib
    )

set(FLIB_DIR
    .
    ../src
    ../src/boost/stage/lib
    ../src/icu/icu4c/lib
    )

add_definitions(-DFMT_HEADER_ONLY)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
include_directories(${FINC})
link_directories(${FLIB_DIR})

add_executable(fastype-test ${FTEST})
target_include_directories(fastype-test PRIVATE ${FINC})
#target_link_libraries(fastype-test debug ${FLIBD} fastypecore)
target_link_libraries(fastype-test ${FLIB} fastypecore)
set_target_properties(fastype-test PROPERTIES VERSION ${PROJECT_VERSION})
