cmake_minimum_required(VERSION 3.8)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(fastype VERSION 0.1.0 LANGUAGES CXX)

if (UNIX)
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
endif(UNIX)

set(F
    fastype.cpp
    Log.cpp
    )
set(FD
    fastyped.cpp
    Log.cpp
    )
set(F_INC
    .
    )
set(F_LIB
    )
set(F_LIB_DIR
    )

message(CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS})
message(CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD})
message(F_LIB: ${F_LIB})
message(F_INC: ${F_INC})
message(F_LIB_DIR: ${F_LIB_DIR})

include_directories(${F_INC})
link_directories(${F_LIB_DIR})

add_executable(fastype ${F})
target_include_directories(fastype PRIVATE ${F_INC})
target_link_libraries(fastype ${F_LIB})

add_executable(fastyped ${FD})
target_include_directories(fastyped PRIVATE ${F_INC})
target_link_libraries(fastyped ${F_LIB})
