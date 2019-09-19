# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype-example VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ".")
message(CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH})
message(CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE})
message(CMAKE_VERBOSE_MAKEFILE: ${CMAKE_VERBOSE_MAKEFILE})
message(F_OS: ${F_OS})

if (APPLE)
    # MacOSX
    include(../src/CMakeMacOSX.cmake)
elseif (UNIX AND NOT APPLE)
    if (${F_OS} STREQUAL "BSD")
        # BSD
        include(../src/CMakeBSD.cmake)
    elseif (${F_OS} STREQUAL "Linux")
        # Linux
        include(../src/CMakeLinux.cmake)
    else()
        message(FATAL_ERROR "${F_OS} Not Support!")
    endif(${F_OS} STREQUAL "BSD")
elseif (MSVC)
    # Windows
    include(../src/CMakeWindows.cmake)
endif(APPLE)

message(F_INC: ${F_INC})
message(F_LIB_DIR: ${F_LIB_DIR})
message(F_LIB: ${F_LIB})

include_directories(${F_INC})
include_directories(../src)
include_directories(../src/spdlog/include)
include_directories(../src/boost)
include_directories(../src/icu/icu4c/source)
link_directories(${F_LIB_DIR})
link_directories(../src)

add_executable(fastype-NCursesTermExample NCursesTermExample.cpp)
target_include_directories(fastype-NCursesTermExample PRIVATE ${F_INC})
target_link_libraries(fastype-NCursesTermExample ${F_LIB} fastypecore)
set_target_properties(fastype-NCursesTermExample PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype-NCursesPanelExample NCursesPanelExample.cpp)
target_include_directories(fastype-NCursesPanelExample PRIVATE ${F_INC})
target_link_libraries(fastype-NCursesPanelExample ${F_LIB} fastypecore)
set_target_properties(fastype-NCursesPanelExample PROPERTIES VERSION ${PROJECT_VERSION})
