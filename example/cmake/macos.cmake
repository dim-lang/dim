# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ".")
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(Threads REQUIRED)
find_package(Curses REQUIRED)

set(F_INC
    Threads::Threads
    ${CURSES_INCLUDE_DIR}
    /usr/local/opt/spdlog/include
    /usr/local/opt/fmt/include
    /usr/local/opt/boost/include
    /usr/local/opt/icu4c/include
    /usr/local/opt/catch2/include
    )
set(F_LIB
    Threads::Threads
    ${CURSES_LIBRARIES}
    panel
    fmt
    boost_program_options-mt
    boost_system-mt
    icuuc
    icuio
    icudata
    icui18n
    tcmalloc
    )
set(F_LIB_DIR
    .
    ${CURSES_LIBRARY_DIRS}
    /usr/local/opt/spdlog/lib
    /usr/local/opt/fmt/lib
    /usr/local/opt/boost/lib
    /usr/local/opt/icu4c/lib
    /usr/local/opt/gperftools/lib
    )

include_directories(${F_INC})
link_directories(${F_LIB_DIR})

add_executable(fastype-ncurses_Panel ncurses_Panel.cpp)
target_include_directories(fastype-ncurses_Panel PRIVATE ${F_INC})
target_link_libraries(fastype-ncurses_Panel ${F_LIB})
set_target_properties(fastype-ncurses_Panel PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype-ncurses_Term ncurses_Term.cpp)
target_include_directories(fastype-ncurses_Term PRIVATE ${F_INC})
target_link_libraries(fastype-ncurses_Term ${F_LIB})
set_target_properties(fastype-ncurses_Term PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype-icu_ustdio icu_ustdio.cpp)
target_include_directories(fastype-icu_ustdio PRIVATE ${F_INC})
target_link_libraries(fastype-icu_ustdio ${F_LIB})
set_target_properties(fastype-icu_ustdio PROPERTIES VERSION ${PROJECT_VERSION})
