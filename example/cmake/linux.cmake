# Copyright 2019- <coli-lang>
# Apache License Version 2.0

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
find_package(Threads REQUIRED)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_USE_DEBUG_LIBS OFF)
set(Boost_USE_RELEASE_LIBS ON)
find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(llvm_libs analysis support core object irreader executionengine scalaropts instcombine orcjit runtimedyld)

set(CINC
    .
    ../src
    ../src/spdlog/include
    ../src/fmt/include
    Threads::Threads
    ${Boost_INCLUDE_DIRS}
    ${LLVM_INCLUDE_DIRS}
    )
set(CLIB
    Threads::Threads
    ${Boost_LIBRARIES}
    ${llvm_libraries}
    icuuc
    icuio
    icudata
    icui18n
    tcmalloc
    )
set(CLIB_DIR
    ${Boost_LIBRARY_DIRS}
    ${LLVM_LIBRARY_DIRS}
    )

add_definitions(-DFMT_HEADER_ONLY)
add_definitions(${Boost_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})
include_directories(${CINC})
link_directories(${CLIB_DIR})

add_executable(coli-icu_ustdio icu_ustdio.cpp)
target_include_directories(coli-icu_ustdio PRIVATE ${CINC})
target_link_libraries(coli-icu_ustdio ${CLIB})
set_target_properties(coli-icu_ustdio PROPERTIES VERSION ${PROJECT_VERSION})
