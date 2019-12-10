# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ".")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14 /W4")

set(F_INC
    spdlog/include
    fmt/include
    boost
    icu/icu4c/include
    )

set(F_LIB
    libboost_program_options-vc141-mt-x64-1_70.lib
    libboost_system-vc141-mt-x64-1_70.lib
    icudt.lib
    icuin.lib
    icuio.lib
    icutu.lib
    icuuc.lib
    )

set(F_LIBD
    libboost_program_options-vc141-mt-gd-x64-1_70.lib
    libboost_system-vc141-mt-gd-x64-1_70.lib
    icudt.lib
    icuind.lib
    icuiod.lib
    icutud.lib
    icuucd.lib
    )

set(F_LIB_DIR
    boost/stage/lib
    icu/icu4c/lib64
    )

add_definitions(-DFMT_HEADER_ONLY)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
include_directories(${F_INC})
link_directories(${F_LIB_DIR})

add_executable(fastype-icu_ustdio icu_ustdio.cpp)
target_include_directories(fastype-icu_ustdio PRIVATE ${F_INC})
target_link_libraries(fastype-icu_ustdio debug ${F_LIBD})
target_link_libraries(fastype-icu_ustdio optimized ${F_LIB})
set_target_properties(fastype-icu_ustdio PROPERTIES VERSION ${PROJECT_VERSION})
