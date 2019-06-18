cmake_minimum_required(VERSION 3.8)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(fastype VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(FT
    fastype.cpp
    Log.cpp
    )
set(FTD
    fastyped.cpp
    Log.cpp
    )
set(FT_INC
    )
set(FT_LIB
    )
set(FT_LIB_DIR
    )

message(CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS})
message(CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD})
message(FT_LIB: ${FT_LIB})
message(FT_LIB_DIR: ${FT_LIB_DIR})

include_directories(.)
link_directories(${FT_LIB_DIR})

target_include_directories(fastype ${FT_INC})
add_executable(fastype ${FT})
target_link_libraries(fastype ${FT_LIB})

target_include_directories(fastyped ${FT_INC})
add_executable(fastyped ${FTD})
target_link_libraries(fastyped ${FT_LIB})
