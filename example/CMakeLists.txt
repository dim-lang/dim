# Copyright 2019- <coli-lang>
# Apache License Version 2.0

project(coli-example VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if (WIN32)
    include(cmake/msvc.cmake)
elseif (APPLE)
    include(cmake/macos.cmake)
elseif (UNIX AND NOT APPLE)
    if (${CMAKE_SYSTEM_NAME} MATCHES BSD)
        include(cmake/bsd.cmake)
    else ()
        include(cmake/linux.cmake)
    endif (${CMAKE_SYSTEM_NAME} MATCHES BSD)
endif (WIN32)
