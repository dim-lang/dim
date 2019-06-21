# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(fastype VERSION 0.1.0 LANGUAGES CXX)

message(F_OS: ${F_OS})
if (APPLE)
    # MacOSX
    include(CMakeMacOSX.cmake)
elseif (UNIX AND NOT APPLE)
    if (${F_OS} STREQUAL "BSD")
        # BSD
        include(CMakeBSD.cmake)
    elseif (${F_OS} STREQUAL "Linux")
        # Linux
        include(CMakeLinux.cmake)
    else()
        message(FATAL_ERROR "${F_OS} Not Support!")
    endif(${F_OS} STREQUAL "BSD")
elseif (MSVC)
    # Windows
    include(CMakeWindows.cmake)
endif(APPLE)

set(F_SRC
    fastype.cpp
    Log.cpp
    Term.cpp
    NCursesTerm.cpp
    #WinTerm.cpp
    Util.cpp
    )
set(FD_SRC
    fastyped.cpp
    Log.cpp
    )

include_directories(${F_INC})
link_directories(${F_LIB_DIR})

add_executable(fastype ${F_SRC})
target_include_directories(fastype PRIVATE ${F_INC})
target_link_libraries(fastype ${F_LIB})

add_executable(fastyped ${FD_SRC})
target_include_directories(fastyped PRIVATE ${F_INC})
target_link_libraries(fastyped ${F_LIB})
