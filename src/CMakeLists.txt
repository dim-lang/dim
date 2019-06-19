# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(fastype VERSION 0.1.0 LANGUAGES CXX)

if (UNIX)
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
endif(UNIX)

find_package(Curses REQUIRED)
message(CURSES_INCLUDE_DIR: ${CURSES_INCLUDE_DIR})
message(CURSES_LIBRARIES: ${CURSES_LIBRARIES})
message(CURSES_LIBRARY_DIR: ${CURSES_LIBRARY_DIR})

set(F
    fastype.cpp
    Log.cpp
    Term.cpp
    NCursesTerm.cpp
    WinTerm.cpp
    )
set(FD
    fastyped.cpp
    Log.cpp
    )
set(F_INC
    .
    spdlog/include
    ${CURSES_INCLUDE_DIR}
    )
set(F_LIB
    ${CURSES_LIBRARIES}
    )
set(F_LIB_DIR
    ${CURSES_LIBRARY_DIR}
    )

message(CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS})
message(CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD})
message(F_LIB: ${F_LIB})
message(F_INC: ${F_INC})
message(F_LIB_DIR: ${F_LIB_DIR})

include_directories(${F_INC})
link_directories(${F_LIB_DIR})

add_executable(fastype ${F})
target_include_directories(fastype PRIVATE ${F_INC})
target_link_libraries(fastype ${F_LIB})

add_executable(fastyped ${FD})
target_include_directories(fastyped PRIVATE ${F_INC})
target_link_libraries(fastyped ${F_LIB})
