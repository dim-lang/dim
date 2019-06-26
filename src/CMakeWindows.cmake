# Copyright 2019- <fastype.org>
# Apache License Version 2.0

message(CMakeWindows.cmake)
#set(CMAKE_C_COMPILER clang)
#set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
add_definitions(-DFMT_HEADER_ONLY)

message(CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS})
message(CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD})

set(F_INC
    .
    ../src/spdlog/include
    ../src/boost
    ${Boost_INCLUDE_DIRS}
    )

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(F_LIB
        Threads::Threads
        libboost_program_options-vc141-mt-gd-x64-1_70.lib
        )
    set(F_LIB_DIR
        ../src/boost/stage/lib
        )
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(F_LIB
        Threads::Threads
        libboost_program_options-vc141-mt-x64-1_70.lib
        )
    set(F_LIB_DIR
        ../src/boost/stage/lib
        )
else()
    message(FATAL_ERROR "CMAKE_BUILD_TYPE missing!")
endif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")

