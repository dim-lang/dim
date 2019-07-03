# Copyright 2019- <fastype.org>
# Apache License Version 2.0

message(CMakeLinux.cmake)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

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
    /usr/include
    )
set(F_LIB
    ${CURSES_LIBRARIES}
    Threads::Threads
    fmt
    boost_program_options-mt
    icuuc
    icuio
    icudata
    )
set(F_LIB_DIR
    ${CURSES_LIBRARY_DIRS}
    /usr/lib
    )
