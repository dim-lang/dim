# Copyright 2019- <coli-lang>
# Apache License Version 2.0

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
find_package(Threads REQUIRED)

set(CINC
    .
    ../src
    Threads::Threads
    /usr/include
    /usr/include/x86_64-linux-gnu
    ../src/spdlog/include
    ../src/fmt/include
    Catch2/single_include
    )

set(CLIB
    Threads::Threads
    boost_program_options
    boost_system
    boost_filesystem
    icuuc
    icuio
    icudata
    icui18n
    tcmalloc
    )

set(CLIB_DIR
    .
    ../src
    /usr/lib
    /usr/lib/x86_64-linux-gnu
    )

add_definitions(-DFMT_HEADER_ONLY)
include_directories(${CINC})
link_directories(${CLIB_DIR})

add_executable(coli-test ${CTEST})
target_include_directories(coli-test PRIVATE ${CINC})
target_link_libraries(coli-test ${CLIB} colicore)
set_target_properties(coli-test PROPERTIES VERSION ${PROJECT_VERSION})
