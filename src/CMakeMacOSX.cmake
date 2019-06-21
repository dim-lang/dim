# Copyright 2019- <fastype.org>
# Apache License Version 2.0

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
add_definitions(-DFMT_HEADER_ONLY)

find_package(spdlog REQUIRED)
find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

message(CURSES_LIBRARIES: ${CURSES_LIBRARIES})
message(CURSES_LIBRARY_DIR: ${CURSES_LIBRARY_DIR})
message(CURSES_INCLUDE_DIR: ${CURSES_INCLUDE_DIR})
message(CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS})
message(CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD})

set(F_INC
    .
    ${CURSES_INCLUDE_DIR}
    spdlog::spdlog
    )
set(F_LIB
    ${CURSES_LIBRARIES}
    Threads::Threads
    spdlog::spdlog
    )
set(F_LIB_DIR
    ${CURSES_LIBRARY_DIR}
    )
