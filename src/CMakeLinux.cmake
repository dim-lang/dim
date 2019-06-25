# Copyright 2019- <fastype.org>
# Apache License Version 2.0

message(CMakeLinux.cmake)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(fmt REQUIRED)
find_package(Curses REQUIRED)
find_package(Threads REQUIRED)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS program_options REQUIRED)

message(CURSES_LIBRARIES: ${CURSES_LIBRARIES})
message(CURSES_LIBRARY_DIRS: ${CURSES_LIBRARY_DIRS})
message(CURSES_INCLUDE_DIRS: ${CURSES_INCLUDE_DIRS})
message(CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS})
message(CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD})
message(Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS})
message(Boost_LIBRARIES: ${Boost_LIBRARIES})
message(Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS})
message(FMT_LIBRARIES: ${FMT_LIBRARIES})
message(FMT_INCLUDE_DIRS: ${FMT_INCLUDE_DIRS})
message(FMT_LIBRARY_DIRS: ${FMT_LIBRARY_DIRS})

set(F_INC
    .
    ${CURSES_INCLUDE_DIRS}
    fmt::fmt
    ${Boost_INCLUDE_DIRS}
    )
set(F_LIB
    ${CURSES_LIBRARIES}
    Threads::Threads
    Boost::program_options
    fmt::fmt
    )
set(F_LIB_DIR
    ${CURSES_LIBRARY_DIRS}
    )
