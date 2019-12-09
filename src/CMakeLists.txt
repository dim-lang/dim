# Copyright 2019- <fastype.org>
# Apache License Version 2.0

message(CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME})

if (WIN32)
    include(cmake/msvc.cmake)
elseif (APPLE)
    include(cmake/macos.cmake)
elseif (UNIX AND NOT APPLE)
    include(cmake/linux.cmake)
endif (WIN32)
