# Copyright 2019- <nerd-lang>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(nerd VERSION 0.0.2 LANGUAGES CXX)
configure_file(${PROJECT_SOURCE_DIR}/src/Configure.h.in ${PROJECT_SOURCE_DIR}/src/Configure.h)
set(CMAKE_CXX_STANDARD 14)

if(DEFINED NDEBUG)
    set(TokenizerFlags "")
    set(ParserFlags "")
else()
    set(TokenizerFlags "--debug --perf-report --verbose --warn")
    set(ParserFlags "-Wall -Dparse.trace --verbose")
    add_definitions(-DYYDEBUG=1)
endif(DEFINED NDEBUG)

if(WIN32)
    set(CMAKE_C_COMPILER cl)
    set(CMAKE_CXX_COMPILER cl)
    add_compile_options("/std:c++14")
    add_compile_options("/W4")
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
    #set(Boost_DEBUG ON)
    #set(Boost_DETAILED_FAILURE_MSG ON)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_THREADAPI win32)
    set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src/boost")
    set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
    set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
    set(LLVM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/llvm-project/llvm/build_release/lib/cmake/llvm")
    set(TokenizerFlags "${TokenizerFlags} --wincompat")
else()
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
    add_compile_options("-std=c++14")
    add_compile_options("-Wall")
    find_package(Threads REQUIRED)
    set(NERD_INC
        Threads::Threads
        /usr/include
        /usr/local/include
        )
    set(NERD_LIB
        Threads::Threads
        )
    set(NERD_LIB_DIR
        /usr/lib
        /usr/lib64
        /usr/local/lib
        /usr/local/lib64
        )
endif(WIN32)

find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
find_package(LLVM REQUIRED CONFIG)
execute_process(COMMAND llvm-config --libs all OUTPUT_VARIABLE llvm_libs)
execute_process(COMMAND llvm-config --system-libs all OUTPUT_VARIABLE llvm_system_libs)
string(REGEX REPLACE "\n$" "" llvm_libs "${llvm_libs}")
string(REGEX REPLACE "\n$" "" llvm_system_libs "${llvm_system_libs}")
if(WIN32)
    string(REGEX REPLACE ".lib " ".lib;" llvm_libs "${llvm_libs}")
    string(REGEX REPLACE ".lib " ".lib;" llvm_system_libs "${llvm_system_libs}")
endif(WIN32)

find_package(FLEX)
find_package(BISON)
FLEX_TARGET(Tokenizer ${CMAKE_CURRENT_SOURCE_DIR}/src/tokenizer.l ${CMAKE_CURRENT_SOURCE_DIR}/src/tokenizer.yy.cc
    COMPILE_FLAGS ${TokenizerFlags}
    DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/tokenizer.yy.hh)
BISON_TARGET(Parser ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.y ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.tab.cc
    COMPILE_FLAGS ${ParserFlags}
    DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.tab.hh)
ADD_FLEX_BISON_DEPENDENCY(Tokenizer Parser)

set(NERD_INC
    ${NERD_INC}
    src
    src/spdlog/include
    src/fmt/include
    src/better-enums
    ${LLVM_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    )
set(NERD_LIB
    ${NERD_LIB}
    ${Boost_LIBRARIES}
    ${llvm_libs}
    ${llvm_system_libs}
    cgraph
    )
set(NERD_LIB_DIR
    ${NERD_LIB_DIR}
    .
    ${Boost_LIBRARY_DIRS}
    ${LLVM_LIBRARY_DIRS}
    )
set(NERD_CORE
    # src/Log.cpp
    # src/Exception.cpp
    src/container/CycleBuffer.cpp
    src/Option.cpp
    # src/Timer.cpp
    # src/DateTime.cpp
    src/Files.cpp
    src/Strings.cpp
    # src/Hash.cpp
    # src/Random.cpp
    # src/Approximate.cpp
    # src/NameGenerator.cpp
    src/Name.cpp
    src/Location.cpp
    src/Token.cpp
    # src/Buffer.cpp
    # src/BufferStack.cpp
    src/Scanner.cpp
    src/Ast.cpp
    # src/Position.cpp
    # src/Symbol.cpp
    # src/SymbolTable.cpp
    src/Counter.cpp
    # src/Dump.cpp
    src/Graph.cpp
    # src/Ir.cpp
    # src/IrContext.cpp
    # src/IrFactory.cpp
    # src/IrUtil.cpp
    # src/IrWriter.cpp
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Tokenizer_OUTPUTS}
)
set(NERDC
    src/nerdc.cpp
)

if(NOT DEFINED NDEBUG)
    message(CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS})
    message(Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS})
    message(Boost_LIBRARIES: ${Boost_LIBRARIES})
    message(Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS})
    message(Boost_DEFINITIONS: ${Boost_DEFINITIONS})
    message(LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS})
    message(LLVM_LIBRARY_DIRS: ${LLVM_LIBRARY_DIRS})
    message(LLVM_DEFINITIONS: ${LLVM_DEFINITIONS})
    message(llvm_libs: ${llvm_libs})
    message(llvm_system_libs: ${llvm_system_libs})
    message(NERD_INC: ${NERD_INC})
    message(NERD_LIB: ${NERD_LIB})
    message(NERD_LIB_DIR: ${NERD_LIB_DIR})
endif(NOT DEFINED NDEBUG)

add_definitions(-DFMT_HEADER_ONLY)
add_definitions(-DSPDLOG_FMT_EXTERNAL)
add_definitions(${Boost_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})
include_directories(${NERD_INC})
link_directories(${NERD_LIB_DIR})

# 1. nerdcore

add_library(nerdcore STATIC ${NERD_CORE})
target_include_directories(nerdcore PRIVATE ${NERD_INC})
target_link_libraries(nerdcore ${NERD_LIB})
set_target_properties(nerdcore PROPERTIES VERSION ${PROJECT_VERSION})

# 2. nerdc

# add_executable(nerdc ${NERDC})
# target_include_directories(nerdc PRIVATE ${NERD_INC})
# target_link_libraries(nerdc ${NERD_LIB} nerdcore)
# set_target_properties(nerdc PROPERTIES VERSION ${PROJECT_VERSION})

# 3. testing
set(NERD_TEST
    test/MainTest.cpp
    test/ConfigureTest.cpp
    test/LogTest.cpp
    test/container/CycleBufferTest.cpp
    test/container/LinkedHashMapTest.cpp
    test/FilesTest.cpp
    # test/ExceptionTest.cpp
    # test/TimerTest.cpp
    # test/RandomTest.cpp
    # test/ApproximateTest.cpp
    # test/HashTest.cpp
    test/TokenizerTest.cpp
    test/ParserTest.cpp
    # test/SemanticTest.cpp
    test/GraphTest.cpp
    # test/NameGeneratorTest.cpp
    # test/IrUtilTest.cpp
    # test/IrWriterTest.cpp
)
set(NERD_TEST_INC
    ${NERD_INC}
    test/Catch2/single_include
)

add_executable(nerd-test ${NERD_TEST})
target_include_directories(nerd-test PRIVATE ${NERD_TEST_INC})
target_link_libraries(nerd-test ${NERD_LIB} nerdcore)
set_target_properties(nerd-test PROPERTIES VERSION ${PROJECT_VERSION})

# 4. examples
add_executable(nerd-llvm_IRBuilder_Add example/llvm_IRBuilder_Add.cpp example/llvm_IRBuilder_Util.cpp)
target_include_directories(nerd-llvm_IRBuilder_Add PRIVATE ${NERD_INC})
target_link_libraries(nerd-llvm_IRBuilder_Add ${NERD_LIB})
set_target_properties(nerd-llvm_IRBuilder_Add PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(nerd-llvm_IRBuilder_Sub example/llvm_IRBuilder_Sub.cpp example/llvm_IRBuilder_Util.cpp)
target_include_directories(nerd-llvm_IRBuilder_Sub PRIVATE ${NERD_INC})
target_link_libraries(nerd-llvm_IRBuilder_Sub ${NERD_LIB})
set_target_properties(nerd-llvm_IRBuilder_Sub PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(nerd-llvm_IRBuilder_Mul example/llvm_IRBuilder_Mul.cpp example/llvm_IRBuilder_Util.cpp)
target_include_directories(nerd-llvm_IRBuilder_Mul PRIVATE ${NERD_INC})
target_link_libraries(nerd-llvm_IRBuilder_Mul ${NERD_LIB})
set_target_properties(nerd-llvm_IRBuilder_Mul PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(nerd-llvm_IRBuilder_Div example/llvm_IRBuilder_Div.cpp example/llvm_IRBuilder_Util.cpp)
target_include_directories(nerd-llvm_IRBuilder_Div PRIVATE ${NERD_INC})
target_link_libraries(nerd-llvm_IRBuilder_Div ${NERD_LIB})
set_target_properties(nerd-llvm_IRBuilder_Div PROPERTIES VERSION ${PROJECT_VERSION})
