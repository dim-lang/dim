cmake_minimum_required(VERSION 3.8)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(fastype VERSION 0.1.0 LANGUAGES CXX)

set(FASTYPE
    fastype.cpp
    Log.cpp
    )

set(FASTYPED
    fastyped.cpp
    Log.cpp
    )

set(FASTYPE_LIB
    )

set(FASTYPE_LIB_DIR
    )

include_directories(.)

target_include_directories(.)
link_directories(${FASTYPE_LIB_DIR})

add_executable(fastype ${FASTYPE})
target_link_libraries(fastype ${FASTYPE_LIB})

add_executable(fastyped ${FASTYPED})
target_link_libraries(fastyped ${FASTYPE_LIB})
