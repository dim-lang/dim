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
find_package(Curses REQUIRED)

set(F_INC
    Threads::Threads
    ${CURSES_INCLUDE_DIR}
    /usr/include
    /usr/include/ncurses
    /usr/include/x86_64-linux-gnu
    ../src/spdlog/include
    ../src/fmt/include
    )
set(F_LIB
    Threads::Threads
    ${CURSES_LIBRARIES}
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
    ${CURSES_LIBRARY_DIRS}
    /usr/lib
    /usr/lib/x86_64-linux-gnu
    )

add_definitions(-DFMT_HEADER_ONLY)
include_directories(${F_INC})
link_directories(${F_LIB_DIR})

add_executable(fastype-ncurses_Panel ncurses_Panel.cpp)
target_link_libraries(fastype-ncurses_Panel ${F_LIB})
target_include_directories(fastype-ncurses_Panel PRIVATE ${F_INC})
set_target_properties(fastype-ncurses_Panel PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype-ncurses_Term ncurses_Term.cpp)
target_include_directories(fastype-ncurses_Term PRIVATE ${F_INC})
target_link_libraries(fastype-ncurses_Term ${F_LIB})
set_target_properties(fastype-ncurses_Term PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype-icu_ustdio icu_ustdio.cpp)
target_include_directories(fastype-icu_ustdio PRIVATE ${F_INC})
target_link_libraries(fastype-icu_ustdio ${F_LIB})
set_target_properties(fastype-icu_ustdio PROPERTIES VERSION ${PROJECT_VERSION})
