# Copyright 2019- <shepherd-lang>
# Apache License Version 2.0

project(shepherdc VERSION 0.0.2 LANGUAGES CXX)
configure_file(${PROJECT_SOURCE_DIR}/Configure.h.in ${PROJECT_SOURCE_DIR}/Configure.h)

set(SHEPHERD
    Log.cpp
    Option.cpp
    Timer.cpp
    DateTime.cpp
    File.cpp
    Strings.cpp
    Hash.cpp
    Random.cpp
    Approximate.cpp
    NameGenerator.cpp
    TokenName.cpp
    Token.cpp
    Token.yy.cpp
    Parser.tab.cpp
    Scanner.cpp
    Ast.cpp
    Symbol.cpp
    Type.cpp
    SymbolTable.cpp
    Semantic.cpp
    Counter.cpp
    Dump.cpp
    Ir.cpp
)
set(SHEPHERDC
    shepherdc.cpp
)

if(WIN32)
    #set(Boost_DEBUG ON)
    #set(Boost_DETAILED_FAILURE_MSG ON)
    set(Boost_THREADAPI win32)
    set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/boost")
    set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
    set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
    set(Boost_COMPILER "-vc")
    set(LLVM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/llvm-project/llvm/install/lib/cmake/llvm")
else()
    set(SHEPHERD_INC
        Threads::Threads
        /usr/include
        /usr/local/include
        )
    set(SHEPHERD_LIB
        Threads::Threads
        )
    set(SHEPHERD_LIB_DIR
        /usr/lib
        /usr/local/lib
        )
endif(WIN32)

find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(llvm_libraries analysis support core object irreader executionengine scalaropts instcombine orcjit runtimedyld)

set(SHEPHERD_INC
    ${SHEPHERD_INC}
    .
    spdlog/include
    fmt/include
    better-enums
    ${LLVM_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    )
set(SHEPHERD_LIB
    ${SHEPHERD_LIB}
    ${Boost_LIBRARIES}
    ${llvm_libraries}
    )
set(SHEPHERD_LIB_DIR
    ${SHEPHERD_LIB_DIR}
    .
    ${Boost_LIBRARY_DIRS}
    ${LLVM_LIBRARY_DIRS}
    )

message(CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS})
message(Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS})
message(Boost_LIBRARIES: ${Boost_LIBRARIES})
message(Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS})
message(Boost_DEFINITIONS: ${Boost_DEFINITIONS})
message(LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS})
message(llvm_libraries: ${llvm_libraries})
message(LLVM_LIBRARY_DIRS: ${LLVM_LIBRARY_DIRS})
message(LLVM_DEFINITIONS: ${LLVM_DEFINITIONS})
message(SHEPHERD_INC: ${SHEPHERD_INC})
message(SHEPHERD_LIB: ${SHEPHERD_LIB})
message(SHEPHERD_LIB_DIR: ${SHEPHERD_LIB_DIR})

add_definitions(-DFMT_HEADER_ONLY)
add_definitions(-DSPDLOG_FMT_EXTERNAL)
add_definitions(${Boost_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})
include_directories(${SHEPHERD_INC})
link_directories(${SHEPHERD_LIB_DIR})

add_library(shepherd STATIC ${SHEPHERD})
target_include_directories(shepherd PRIVATE ${SHEPHERD_INC})
target_link_libraries(shepherd ${SHEPHERD_LIB})
set_target_properties(shepherd PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(shepherdc ${SHEPHERDC})
target_include_directories(shepherdc PRIVATE ${SHEPHERD_INC})
target_link_libraries(shepherdc ${SHEPHERD_LIB} shepherd)
set_target_properties(shepherdc PROPERTIES VERSION ${PROJECT_VERSION})
