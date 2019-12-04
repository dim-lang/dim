# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ".")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")

message(CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR})
message(CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR})
message(CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH})
message(CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE})
message(CMAKE_VERBOSE_MAKEFILE: ${CMAKE_VERBOSE_MAKEFILE})

set(F_INC
    spdlog/include
    fmt/include
    boost
    icu/icu4c/include
    )

set(F_LIB
    boost_program_options-vc141-mt-x64-1_70.lib
    )

set(F_LIB_DIR
    boost/stage/lib
    icu/icu4c/lib64
    )

add_definitions(-DFMT_HEADER_ONLY)

message(F_INC: ${F_INC})
message(F_LIB_DIR: ${F_LIB_DIR})
message(F_LIB: ${F_LIB})

include_directories(${F_INC})
link_directories(${F_LIB_DIR})

target_include_directories(fastype-CursesPanelExample PRIVATE ${F_INC})
target_link_libraries(fastype-CursesPanelExample ${F_LIB})
set_target_properties(fastype-CursesPanelExample PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(fastype-CursesTermExample PRIVATE ${F_INC})
target_link_libraries(fastype-CursesTermExample ${F_LIB})
set_target_properties(fastype-CursesTermExample PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(fastype-icuu_fgets PRIVATE ${F_INC})
target_link_libraries(fastype-icuu_fgets ${F_LIB})
set_target_properties(fastype-icuu_fgets PROPERTIES VERSION ${PROJECT_VERSION})