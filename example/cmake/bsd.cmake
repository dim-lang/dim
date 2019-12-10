# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype VERSION 0.1.0 LANGUAGES CXX)
configure_file(Configure.h.in Configure.h)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ".")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

set(F_INCLUDE_DIR
    ${CURSES_INCLUDE_DIR}
    Threads::Threads
    )

set(F_LIBRARIES
    ${CURSES_LIBRARIES}
    Threads::Threads
    )

set(F_LIBRARY_DIR
    ${UNIX_LIBRARY_DIR}
    ${CURSES_LIBRARY_DIRS}
    )

add_definitions(-DFMT_HEADER_ONLY)
include_directories(${F_INC})
link_directories(${F_LIB_DIR})

add_executable(fastype-ncurses_Panel ncurses_Panel.cpp)
target_include_directories(fastype-ncurses_Panel PRIVATE ${F_INC})
target_link_libraries(fastype-ncurses_Panel ${F_LIB})
set_target_properties(fastype-ncurses_Panel PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype-ncurses_Term ncurses_Term.cpp)
target_include_directories(fastype-ncurses_Term PRIVATE ${F_INC})
target_link_libraries(fastype-ncurses_Term ${F_LIB})
set_target_properties(fastype-ncurses_Term PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype-icu_ustdio icu_ustdio.cpp)
target_include_directories(fastype-icu_ustdio PRIVATE ${F_INC})
target_link_libraries(fastype-icu_ustdio ${F_LIB})
set_target_properties(fastype-icu_ustdio PROPERTIES VERSION ${PROJECT_VERSION})
