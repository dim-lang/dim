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
llvm_map_components_to_libnames(llvm_libraries analysis support core object irreader executionengine scalaropts instcombine orcjit runtimedyld)

message(Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS})
message(Boost_LIBRARIES: ${Boost_LIBRARIES})
message(Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS})
message(Boost_DEFINITIONS: ${Boost_DEFINITIONS})
message(LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS})
message(llvm_libraries: ${llvm_libraries})
message(LLVM_LIBRARY_DIRS: ${LLVM_LIBRARY_DIRS})
message(LLVM_DEFINITIONS: ${LLVM_DEFINITIONS})

set(CINC
    .
    spdlog/include
    fmt/include
    Threads::Threads
    ${Boost_INCLUDE_DIRS}
    ${LLVM_INCLUDE_DIRS}
    /usr/include
    /usr/local/include
    )
set(CLIB
    Threads::Threads
    ${Boost_LIBRARIES}
    ${llvm_libraries}
    tcmalloc
    icuuc
    icuio
    icudata
    icui18n
    )
set(CLIB_DIR
    ${Boost_LIBRARY_DIRS}
    ${LLVM_LIBRARY_DIRS}
    /usr/lib
    /usr/local/lib
    )

add_definitions(-DFMT_HEADER_ONLY)
add_definitions(${Boost_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})
include_directories(${CINC})
link_directories(${CLIB_DIR})

add_library(colicore STATIC ${CCORE})
target_include_directories(colicore PRIVATE ${CINC})
target_link_libraries(colicore ${CLIB})
set_target_properties(colicore PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(coli ${CSRC})
target_include_directories(coli PRIVATE ${CINC})
target_link_libraries(coli ${CLIB} colicore)
set_target_properties(coli PROPERTIES VERSION ${PROJECT_VERSION})
