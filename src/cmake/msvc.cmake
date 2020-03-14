# Copyright 2019- <coli-lang>
# Apache License Version 2.0

set(CMAKE_C_COMPILER cl)
set(CMAKE_CXX_COMPILER cl)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14 /W4")
set(Boost_DETAILED_FAILURE_MSG ON)
set(Boost_THREADAPI win32)
set(Boost_ROOT "boost/stage")
set(Boost_LIBRARY_DIRS "boost/stage/lib")
set(Boost_INCLUDE_DIRS "boost/stage/include")
set(Boost_COMPILER "-vc")
set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_USE_DEBUG_LIBS OFF)
set(Boost_USE_RELEASE_LIBS ON)
find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(llvm_libraries analysis support core object irreader executionengine scalaropts instcombine orcjit runtimedyld)

set(CINC
    .
    spdlog/include
    fmt/include
    ${LLVM_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    icu/icu4c/include
    )

set(CLIB
    ${Boost_LIBRARIES}
    ${llvm_libraries}
    #libboost_program_options-vc141-mt-x32-1_70.lib
    #libboost_system-vc141-mt-x32-1_70.lib
    #libboost_filesystem-vc141-mt-x32-1_70.lib
    icudt.lib
    icuin.lib
    icuio.lib
    icutu.lib
    icuuc.lib
    )

set(CLIBD
    ${Boost_LIBRARIES}
    ${llvm_libraries}
    #libboost_program_options-vc141-mt-gd-x32-1_70.lib
    #libboost_system-vc141-mt-gd-x32-1_70.lib
    #libboost_filesystem-vc141-mt-gd-x32-1_70.lib
    icudt.lib
    icuind.lib
    icuiod.lib
    icutud.lib
    icuucd.lib
    )

set(CLIB_DIR
    icu/icu4c/lib
    ${Boost_LIBRARY_DIRS}
    ${LLVM_LIBRARY_DIRS}
    )

add_definitions(-DFMT_HEADER_ONLY)
add_definitions(${Boost_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
include_directories(${CINC})
link_directories(${CLIB_DIR})

add_library(colicore STATIC ${CCORE})
target_include_directories(colicore PRIVATE ${CINC})
#target_link_libraries(colicore debug ${CLIBD})
target_link_libraries(colicore ${CLIB})
set_target_properties(colicore PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(coli ${CSRC})
target_include_directories(coli PRIVATE ${CINC})
#target_link_libraries(coli debug ${CLIBD} colicore)
target_link_libraries(coli ${CLIB} colicore)
set_target_properties(coli PROPERTIES VERSION ${PROJECT_VERSION})
