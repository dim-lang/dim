# Copyright 2019- <nerd-lang>
# Apache License Version 2.0

project(nerdc VERSION 0.0.2 LANGUAGES CXX)
configure_file(${PROJECT_SOURCE_DIR}/Configure.h.in ${PROJECT_SOURCE_DIR}/Configure.h)
set(CMAKE_CXX_STANDARD 14)

if(DEFINED NDEBUG)
    set(TokenizerFlags "")
    set(ParserFlags "")
else()
    set(TokenizerFlags "--debug --perf-report --verbose --warn")
    set(ParserFlags "-Wcounterexamples -Wdeprecated -Dparse.trace --verbose --graph=parser.tab.dot")
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
    set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/boost")
    set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
    set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
    set(LLVM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/llvm-project/llvm/build_release/lib/cmake/llvm")
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
        /usr/local/lib
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
FLEX_TARGET(Tokenizer tokenizer.l ${CMAKE_CURRENT_SOURCE_DIR}/tokenizer.yy.cc
    COMPILE_FLAGS ${TokenizerFlags}
    DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/tokenizer.yy.hh)
BISON_TARGET(Parser parser.y ${CMAKE_CURRENT_SOURCE_DIR}/parser.tab.cc
    DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/parser.tab.hh)
ADD_FLEX_BISON_DEPENDENCY(Tokenizer Parser)

set(NERD_INC
    ${NERD_INC}
    .
    spdlog/include
    fmt/include
    better-enums
    ${LLVM_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    )
set(NERD_LIB
    ${NERD_LIB}
    ${Boost_LIBRARIES}
    ${llvm_libs}
    ${llvm_system_libs}
    )
set(NERD_LIB_DIR
    ${NERD_LIB_DIR}
    .
    ${Boost_LIBRARY_DIRS}
    ${LLVM_LIBRARY_DIRS}
    )
set(NERD
    # Log.cpp
    # Exception.cpp
    container/CycleBuffer.cpp
    Option.cpp
    # Timer.cpp
    # DateTime.cpp
    File.cpp
    Strings.cpp
    # Hash.cpp
    # Random.cpp
    # Approximate.cpp
    # NameGenerator.cpp
    Name.cpp
    Namely.cpp
    Locationly.cpp
    TokenName.cpp
    # Buffer.cpp
    # BufferStack.cpp
    Scanner.cpp
    Ast.cpp
    # Position.cpp
    # Symbol.cpp
    # SymbolTable.cpp
    Counter.cpp
    # Dump.cpp
    # Ir.cpp
    # IrContext.cpp
    # IrFactory.cpp
    # IrUtil.cpp
    # IrWriter.cpp
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Tokenizer_OUTPUTS}
)
set(NERDC
    nerdc.cpp
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

add_library(nerdcore STATIC ${NERD})
target_include_directories(nerdcore PRIVATE ${NERD_INC})
target_link_libraries(nerdcore ${NERD_LIB})
set_target_properties(nerdcore PROPERTIES VERSION ${PROJECT_VERSION})

# add_executable(nerdc ${NERDC})
# target_include_directories(nerdc PRIVATE ${NERD_INC})
# target_link_libraries(nerdc ${NERD_LIB} nerdcore)
# set_target_properties(nerdc PROPERTIES VERSION ${PROJECT_VERSION})
