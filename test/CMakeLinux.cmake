# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype VERSION 0.1.0 LANGUAGES CXX)
message(input_config: ${CMAKE_CURRENT_SOURCE_DIR})
message(output_config: ${CMAKE_CURRENT_BINARY_DIR})
configure_file(../src/Configure.h.in ../src/Configure.h)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ".")
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

message(CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH})
message(CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE})
message(CMAKE_VERBOSE_MAKEFILE: ${CMAKE_VERBOSE_MAKEFILE})
message(F_OS: ${F_OS})
message(CMakeLinux.cmake)
message(CURSES_LIBRARIES: ${CURSES_LIBRARIES})
message(CURSES_LIBRARY_DIRS: ${CURSES_LIBRARY_DIRS})
message(CURSES_INCLUDE_DIRS: ${CURSES_INCLUDE_DIRS})
message(CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS})
message(CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD})

set(F_INC
    .
    ../src
    ${CURSES_INCLUDE_DIRS}
    Threads::Threads
    spdlog/include
    fmt/include
    /usr/include
    /usr/include/x86_64-linux-gnu
    )
set(F_LIB
    ${CURSES_LIBRARIES}
    Threads::Threads
    boost_program_options
    boost_system
    icuuc
    icuio
    icudata
    icui18n
    panel
    tcmalloc
    )
set(F_LIB_DIR
    ${CURSES_LIBRARY_DIRS}
    /usr/lib
    /usr/lib/x86_64-linux-gnu
    )

add_definitions(-DFMT_HEADER_ONLY)

message(F_INC: ${F_INC})
message(F_LIB_DIR: ${F_LIB_DIR})
message(F_LIB: ${F_LIB})

set(T_SRC
    MainTest.cpp
    LoggingTest.cpp
    LineTest.cpp
    ConfigureTest.cpp
    TimerTest.cpp
    RandomTest.cpp
    FilerTest.cpp
    ApproximateTest.cpp

    exception/NotFoundExceptionTest.cpp
    script/TokenTest.cpp
    script/LexerTest.cpp
    script/ParserTest.cpp
    script/InterpreterTest.cpp
    )


include_directories(${F_INC})
include_directories(../src)
include_directories(../src/spdlog/include)
include_directories(../src/fmt/include)
include_directories(../src/boost)
include_directories(../src/icu/icu4c/source)
include_directories(Catch2/single_include)
link_directories(${F_LIB_DIR})
link_directories(../src)
link_directories(..)
link_directories(.)

add_executable(fastype-test ${T_SRC})
target_include_directories(fastype-test PRIVATE ${F_INC})
target_link_libraries(fastype-test ${F_LIB} fastypecore)
set_target_properties(fastype-test PROPERTIES VERSION ${PROJECT_VERSION})
