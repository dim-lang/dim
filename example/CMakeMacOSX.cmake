# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ".")
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

message(CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR})
message(CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR})
message(CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH})
message(CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE})
message(CMAKE_VERBOSE_MAKEFILE: ${CMAKE_VERBOSE_MAKEFILE})

set(F_INC
    ${CURSES_INCLUDE_DIRS}
    Threads::Threads
    /usr/local/opt/spdlog/include
    /usr/local/opt/fmt/include
    /usr/local/opt/boost/include
    /usr/local/opt/icu4c/include
    /usr/local/opt/catch2/include
    )
set(F_LIB
    ${CURSES_LIBRARIES}
    Threads::Threads
    fmt
    boost_program_options-mt
    boost_system-mt
    icuuc
    icuio
    icudata
    icui18n
    tcmalloc
    panel
    )
set(F_LIB_DIR
    .
    ${CURSES_LIBRARY_DIRS}
    /usr/local/opt/spdlog/lib
    /usr/local/opt/fmt/lib
    /usr/local/opt/boost/lib
    /usr/local/opt/icu4c/lib
    /usr/local/opt/gperftools/lib
    )

message(F_INC: ${F_INC})
message(F_LIB_DIR: ${F_LIB_DIR})
message(F_LIB: ${F_LIB})

include_directories(${F_INC})
link_directories(${F_LIB_DIR})

add_executable(fastype-CursesPanelExample CursesPanelExample.cpp)
target_include_directories(fastype-CursesPanelExample PRIVATE ${F_INC})
target_link_libraries(fastype-CursesPanelExample ${F_LIB})
set_target_properties(fastype-CursesPanelExample PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype-CursesTermExample CursesTermExample.cpp)
target_include_directories(fastype-CursesTermExample PRIVATE ${F_INC})
target_link_libraries(fastype-CursesTermExample ${F_LIB})
set_target_properties(fastype-CursesTermExample PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype-icuu_fgets icuu_fgets.cpp)
target_include_directories(fastype-icuu_fgets PRIVATE ${F_INC})
target_link_libraries(fastype-icuu_fgets ${F_LIB})
set_target_properties(fastype-icuu_fgets PROPERTIES VERSION ${PROJECT_VERSION})
