# Copyright 2019- <fastype.org>
# Apache License Version 2.0

message(CMakeLinux.cmake)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)
message(CURSES_LIBRARIES: ${CURSES_LIBRARIES})

set(F_INCLUDE_DIR
    ${CURSES_INCLUDE_DIR}
    )

set(F_LIBRARIES
    ${CURSES_LIBRARIES}
    Threads::Threads
    )

set(F_LIBRARY_DIR
    ${UNIX_LIBRARY_DIR}
    )
