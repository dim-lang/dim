# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype VERSION 0.1.0 LANGUAGES CXX)
configure_file(../src/Configure.h.in ../src/Configure.h)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ".")
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(Threads REQUIRED)

message(CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR})
message(CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR})
message(CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH})
message(CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE})
message(CMAKE_VERBOSE_MAKEFILE: ${CMAKE_VERBOSE_MAKEFILE})

set(F_INC
    .
    ../src
    Threads::Threads
    /usr/local/opt/ncurses/include
    /usr/local/opt/spdlog/include
    /usr/local/opt/fmt/include
    /usr/local/opt/boost/include
    /usr/local/opt/icu4c/include
    /usr/local/opt/catch2/include
    Catch2/single_include
    )
set(F_LIB
    Threads::Threads
    ncurses
    panel
    fmt
    boost_program_options-mt
    boost_system-mt
    icuuc
    icuio
    icudata
    icui18n
    tcmalloc
    )
set(F_LIB_DIR
    .
    ..
    ../src
    /usr/local/opt/ncurses/lib
    /usr/local/opt/spdlog/lib
    /usr/local/opt/fmt/lib
    /usr/local/opt/boost/lib
    /usr/local/opt/icu4c/lib
    /usr/local/opt/gperftools/lib
    )

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
link_directories(${F_LIB_DIR})

add_executable(fastype-test ${T_SRC})
target_include_directories(fastype-test PRIVATE ${F_INC})
target_link_libraries(fastype-test ${F_LIB} fastypecore)
set_target_properties(fastype-test PROPERTIES VERSION ${PROJECT_VERSION})
