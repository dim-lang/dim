# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
find_package(Threads REQUIRED)

set(FINC
    Threads::Threads
    /usr/local/opt/ncurses/include
    /usr/local/opt/spdlog/include
    /usr/local/opt/fmt/include
    /usr/local/opt/boost/include
    /usr/local/opt/icu4c/include
    /usr/local/opt/catch2/include
    )
set(FLIB
    Threads::Threads
    ncurses
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
set(FLIB_DIR
    .
    /usr/local/opt/ncurses/lib
    /usr/local/opt/spdlog/lib
    /usr/local/opt/fmt/lib
    /usr/local/opt/boost/lib
    /usr/local/opt/icu4c/lib
    /usr/local/opt/gperftools/lib
    )

include_directories(${FINC})
link_directories(${FLIB_DIR})

add_executable(fastype-ncurses_Panel ncurses_Panel.cpp)
target_include_directories(fastype-ncurses_Panel PRIVATE ${FINC})
target_link_libraries(fastype-ncurses_Panel ${FLIB})
set_target_properties(fastype-ncurses_Panel PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype-ncurses_Term ncurses_Term.cpp)
target_include_directories(fastype-ncurses_Term PRIVATE ${FINC})
target_link_libraries(fastype-ncurses_Term ${FLIB})
set_target_properties(fastype-ncurses_Term PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype-icu_ustdio icu_ustdio.cpp)
target_include_directories(fastype-icu_ustdio PRIVATE ${FINC})
target_link_libraries(fastype-icu_ustdio ${FLIB})
set_target_properties(fastype-icu_ustdio PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype-flex_bison flex_bison.cpp flex.yy.cpp bison.tab.cpp)
target_include_directories(fastype-flex_bison PRIVATE ${FINC})
target_link_libraries(fastype-flex_bison ${FLIB})
set_target_properties(fastype-flex_bison PROPERTIES VERSION ${PROJECT_VERSION})
