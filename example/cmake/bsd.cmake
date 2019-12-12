# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype-example VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ".")
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(Threads REQUIRED)
find_package(Curses REQUIRED)

set(FINC
    Threads::Threads
    ${CURSES_INCLUDE_DIR}
    /usr/include
    /usr/local/include
    ../src/spdlog/include
    ../src/fmt/include
    )
set(FLIB
    Threads::Threads
    ${CURSES_LIBRARIES}
    panel
    boost_program_options
    boost_system
    icuuc
    icuio
    icudata
    icui18n
    )
set(FLIB_DIR
    ${CURSES_LIBRARY_DIRS}
    /usr/lib
    /usr/local/lib
    )

add_definitions(-DFMT_HEADER_ONLY)
include_directories(${FINC})
link_directories(${FLIB_DIR})

add_executable(fastype-ncurses_Panel ncurses_Panel.cpp)
target_link_libraries(fastype-ncurses_Panel ${FLIB})
target_include_directories(fastype-ncurses_Panel PRIVATE ${FINC})
set_target_properties(fastype-ncurses_Panel PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype-ncurses_Term ncurses_Term.cpp)
target_include_directories(fastype-ncurses_Term PRIVATE ${FINC})
target_link_libraries(fastype-ncurses_Term ${FLIB})
set_target_properties(fastype-ncurses_Term PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype-icu_ustdio icu_ustdio.cpp)
target_include_directories(fastype-icu_ustdio PRIVATE ${FINC})
target_link_libraries(fastype-icu_ustdio ${FLIB})
set_target_properties(fastype-icu_ustdio PROPERTIES VERSION ${PROJECT_VERSION})
