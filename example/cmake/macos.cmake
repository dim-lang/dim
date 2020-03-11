# Copyright 2019- <coli-lang>
# Apache License Version 2.0

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
find_package(Threads REQUIRED)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_USE_DEBUG_LIBS OFF)
set(Boost_USE_RELEASE_LIBS ON)
find_package(Boost COMPONENTS program_options system filesystem REQUIRED)

set(CINC
    .
    ../src
    spdlog/include
    fmt/include
    Threads::Threads
    /usr/local/opt/icu4c/include
    /usr/local/opt/catch2/include
    ${Boost_INCLUDE_DIRS}
    )
set(CLIB
    Threads::Threads
    ${Boost_LIBRARIES}
    icuuc
    icuio
    icudata
    icui18n
    tcmalloc
    )
set(CLIB_DIR
    .
    /usr/local/opt/icu4c/lib
    /usr/local/opt/gperftools/lib
    ${Boost_LIBRARY_DIRS}
    )

add_definitions(-DFMT_HEADER_ONLY)
include_directories(${CINC})
link_directories(${CLIB_DIR})

add_executable(coli-icu_ustdio icu_ustdio.cpp)
target_include_directories(coli-icu_ustdio PRIVATE ${CINC})
target_link_libraries(coli-icu_ustdio ${CLIB})
set_target_properties(coli-icu_ustdio PROPERTIES VERSION ${PROJECT_VERSION})
