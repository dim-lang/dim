# Copyright 2019- <fastype.org>
# Apache License Version 2.0

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
include(cmake/test.cmake)

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
