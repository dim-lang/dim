# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype VERSION 0.1.0 LANGUAGES CXX)
configure_file(Configure.h.in Configure.h)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ".")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14 /W4")

message(CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR})
message(CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR})
message(CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH})
message(CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE})
message(CMAKE_VERBOSE_MAKEFILE: ${CMAKE_VERBOSE_MAKEFILE})

set(FINC
    .
    spdlog/include
    fmt/include
    boost
    icu/icu4c/include
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
    boost/stage/lib
    icu/icu4c/lib
    )

include(./src.cmake)

add_definitions(-DFMT_HEADER_ONLY)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
include_directories(${FINC})
link_directories(${FLIB_DIR})

add_library(fastypecore STATIC ${FCORE})
target_include_directories(fastypecore PRIVATE ${FINC})
#target_link_libraries(fastypecore debug ${FLIBD})
target_link_libraries(fastypecore ${FLIB})
set_target_properties(fastypecore PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype ${FCLI})
target_include_directories(fastype PRIVATE ${FINC})
#target_link_libraries(fastype debug ${FLIBD} fastypecore)
target_link_libraries(fastype ${FLIB} fastypecore)
set_target_properties(fastype PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastyped ${FSRV})
target_include_directories(fastyped PRIVATE ${FINC})
#target_link_libraries(fastyped debug ${FLIBD} fastypecore)
target_link_libraries(fastyped ${FLIB} fastypecore)
set_target_properties(fastyped PROPERTIES VERSION ${PROJECT_VERSION})
