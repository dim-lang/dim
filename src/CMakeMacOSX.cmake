# Copyright 2019- <fastype.org>
# Apache License Version 2.0

message(CMakeMacOSX.cmake)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
add_definitions(-DFMT_HEADER_ONLY)

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

message(CURSES_LIBRARIES: ${CURSES_LIBRARIES})
message(CURSES_LIBRARY_DIRS: ${CURSES_LIBRARY_DIRS})
message(CURSES_INCLUDE_DIRS: ${CURSES_INCLUDE_DIRS})
message(CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS})
message(CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD})

set(F_INC
    .
    ${CURSES_INCLUDE_DIRS}
    Threads::Threads
    spdlog/include/
    boost/
    icu/icu4c/source/common
    icu/icu4c/source/i18n
    icu/icu4c/source/io
    )
set(F_LIB
    ${CURSES_LIBRARIES}
    Threads::Threads
    boost_program_options
    icuuc
    icudata
    icuio
    icui18n
    )
set(F_LIB_DIR
    ${CURSES_LIBRARY_DIRS}
    boost/stage/lib
    icu/build/lib
    )
