# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype-example VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ".")
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(Threads REQUIRED)

set(F_INC
    Threads::Threads
    spdlog/include
    fmt/include
    /usr/include
    /usr/include/x86_64-linux-gnu
    )
set(F_LIB
    Threads::Threads
    ncurses
    panel
    boost_program_options
    boost_system
    icuuc
    icuio
    icudata
    icui18n
    tcmalloc
    )
set(F_LIB_DIR
    /usr/lib
    /usr/lib/x86_64-linux-gnu
    )

add_definitions(-DFMT_HEADER_ONLY)
include_directories(${F_INC})
link_directories(${F_LIB_DIR})

add_executable(fastype-CursesPanelExample CursesPanelExample.cpp)
target_link_libraries(fastype-CursesPanelExample ${F_LIB})
target_include_directories(fastype-CursesPanelExample PRIVATE ${F_INC})
set_target_properties(fastype-CursesPanelExample PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype-CursesTermExample CursesTermExample.cpp)
target_include_directories(fastype-CursesTermExample PRIVATE ${F_INC})
target_link_libraries(fastype-CursesTermExample ${F_LIB})
set_target_properties(fastype-CursesTermExample PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype-icuu_fgets icuu_fgets.cpp)
target_include_directories(fastype-icuu_fgets PRIVATE ${F_INC})
target_link_libraries(fastype-icuu_fgets ${F_LIB})
set_target_properties(fastype-icuu_fgets PROPERTIES VERSION ${PROJECT_VERSION})
