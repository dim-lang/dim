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
    set(ParserFlags "-Dparse.trace --verbose")
    add_definitions(-DYYDEBUG=1)
endif(DEFINED NDEBUG)
MATH(EXPR STACK_SIZE "128 * 1024 * 1024")
MATH(EXPR STACK_COMMIT_SIZE "1024 * 1024")
MATH(EXPR HEAP_SIZE "1024 * 1024 * 1024")
MATH(EXPR HEAP_COMMIT_SIZE "1024 * 1024")

if(WIN32)
    set(CMAKE_C_COMPILER cl)
    set(CMAKE_CXX_COMPILER cl)
    add_compile_options("/std:c++14")
    add_compile_options("/W4")
    add_compile_options("/MP")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:${STACK_SIZE},${STACK_COMMIT_SIZE}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /HEAP:${HEAP_SIZE},${HEAP_COMMIT_SIZE}")
    #set(Boost_DEBUG ON)
    #set(Boost_DETAILED_FAILURE_MSG ON)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_THREADAPI win32)
    set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src/boost")
    if(${NERD_BUILD_TYPE} STREQUAL "Debug")
        set(BOOST_LIBRARYDIR "${BOOST_ROOT}/Debug/lib")
        set(LLVM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/llvm-project/llvm/Debug/lib/cmake/llvm")
    else(${NERD_BUILD_TYPE} STREQUAL "Debug")
        set(BOOST_LIBRARYDIR "${BOOST_ROOT}/Release/lib")
        set(LLVM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/llvm-project/llvm/Release/lib/cmake/llvm")
    endif(${NERD_BUILD_TYPE} STREQUAL "Debug")
    set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
    set(TokenizerFlags "${TokenizerFlags} --wincompat")
else()
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
    add_compile_options("-std=c++14")
    add_compile_options("-Wall")
    find_package(Threads REQUIRED)
    set(NERD_INC
        Threads::Threads
        )
    if(IS_DIRECTORY /usr/include)
        set(NERD_INC
            ${NERD_INC}
            /usr/include
            )
    endif(IS_DIRECTORY /usr/include)
    if(IS_DIRECTORY /usr/local/include)
        set(NERD_INC
            ${NERD_INC}
            /usr/local/include
            )
    endif(IS_DIRECTORY /usr/local/include)
    set(NERD_LIB
        Threads::Threads
        )
    set(NERD_LIB_DIR
        /usr/lib)
    if(IS_DIRECTORY /usr/lib64)
        set(NERD_LIB_DIR
            ${NERD_LIB_DIR}
            /usr/lib64)
    endif(IS_DIRECTORY /usr/lib64)
    if(IS_DIRECTORY /usr/local/lib)
        set(NERD_LIB_DIR
            ${NERD_LIB_DIR}
            /usr/local/lib)
    endif(IS_DIRECTORY /usr/local/lib)
    if(IS_DIRECTORY /usr/local/lib64)
        set(NERD_LIB_DIR
            ${NERD_LIB_DIR}
            /usr/local/lib64)
    endif(IS_DIRECTORY /usr/local/lib64)
endif(WIN32)

find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
find_package(LLVM REQUIRED CONFIG)
# llvm_map_components_to_libnames(llvm_libs AllTargetsCodeGens AllTargetsAsmPrinters AllTargetsAsmParsers AllTargetsDescs AllTargetsDisassemblers AllTargetsInfos)
llvm_map_components_to_libnames(llvm_libs native)
# execute_process(COMMAND llvm-config --libs all OUTPUT_VARIABLE llvm_libs)
# execute_process(COMMAND llvm-config --system-libs all OUTPUT_VARIABLE llvm_system_libs)
# string(REGEX REPLACE "\n$" "" llvm_libs "${llvm_libs}")
# string(REGEX REPLACE "\n$" "" llvm_system_libs "${llvm_system_libs}")
# if(WIN32)
    # string(REGEX REPLACE ".lib " ".lib;" llvm_libs "${llvm_libs}")
    # string(REGEX REPLACE ".lib " ".lib;" llvm_system_libs "${llvm_system_libs}")
# endif(WIN32)

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
    # ${llvm_system_libs}
    )
set(NERD_LIB_DIR
    ${NERD_LIB_DIR}
    .
    ${Boost_LIBRARY_DIRS}
    ${LLVM_LIBRARY_DIRS}
    )
set(NERD_CORE
    src/iface/Identifiable.cpp
    src/iface/LLVMModular.cpp
    src/iface/LLVMTypable.cpp
    src/iface/LLVMValuable.cpp
    src/iface/Locationable.cpp
    src/iface/Nameable.cpp
    src/iface/Scoped.cpp
    src/iface/Symbolizable.cpp
    src/iface/TypeSymbolizable.cpp

    src/infra/Counter.cpp
    src/infra/Cowstr.cpp
    src/infra/CycleBuffer.cpp
    src/infra/Files.cpp
    src/infra/Strings.cpp

    src/Ast.cpp
    src/Compiler.cpp
    src/Drawer.cpp
    src/IrBuilder.cpp
    src/Label.cpp
    src/Location.cpp
    src/NameGenerator.cpp
    src/Phase.cpp
    src/Scanner.cpp
    src/Symbol.cpp
    src/SymbolBuilder.cpp
    src/SymbolResolver.cpp
    src/Token.cpp
    src/Visitor.cpp

    ${BISON_Parser_OUTPUTS}
    ${FLEX_Tokenizer_OUTPUTS}
)
set(NERDC
    src/nerdc.cpp
)

message(NERD_BUILD_TYPE: ${NERD_BUILD_TYPE})
message(CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS})
message(BOOST_ROOT: ${BOOST_ROOT})
message(BOOST_LIBRARYDIR: ${BOOST_LIBRARYDIR})
message(Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS})
message(Boost_LIBRARIES: ${Boost_LIBRARIES})
message(Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS})
message(Boost_DEFINITIONS: ${Boost_DEFINITIONS})
message(LLVM_DIR: ${LLVM_DIR})
message(LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS})
message(LLVM_LIBRARY_DIRS: ${LLVM_LIBRARY_DIRS})
message(LLVM_DEFINITIONS: ${LLVM_DEFINITIONS})
message(llvm_libs: ${llvm_libs})
# message(llvm_system_libs: ${llvm_system_libs})
message(NERD_INC: ${NERD_INC})
message(NERD_LIB: ${NERD_LIB})
message(NERD_LIB_DIR: ${NERD_LIB_DIR})

add_definitions(-DFMT_HEADER_ONLY)
add_definitions(-DSPDLOG_FMT_EXTERNAL)
add_definitions(${Boost_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})
include_directories(${NERD_INC})
link_directories(${NERD_LIB_DIR})

# nerdcore
add_library(nerdcore STATIC ${NERD_CORE})
target_include_directories(nerdcore PRIVATE ${NERD_INC})
target_link_libraries(nerdcore ${NERD_LIB})
set_target_properties(nerdcore PROPERTIES VERSION ${PROJECT_VERSION})

# nerdc
add_executable(nerdc ${NERDC})
target_include_directories(nerdc PRIVATE ${NERD_INC})
target_link_libraries(nerdc ${NERD_LIB} nerdcore)
set_target_properties(nerdc PROPERTIES VERSION ${PROJECT_VERSION})

# nerd-test
set(NERD_TEST
    test/iface/IdentifiableTest.cpp
    test/iface/LLVMModularTest.cpp
    test/iface/LLVMTypableTest.cpp
    test/iface/LLVMValuableTest.cpp
    test/iface/NameableTest.cpp

    test/infra/CowstrTest.cpp
    test/infra/CycleBufferTest.cpp
    test/infra/FilesTest.cpp
    test/infra/LinkedHashMapTest.cpp
    test/infra/LogTest.cpp

    test/MainTest.cpp
    test/ConfigureTest.cpp
    test/LocationTest.cpp
    test/TokenizerTest.cpp
    test/ParserTest.cpp
    test/SymbolBuilderTest.cpp
    test/SymbolResolverTest.cpp
    test/DrawerTest.cpp
    test/IrBuilderTest.cpp
)
set(NERD_TEST_INC
    ${NERD_INC}
    test/Catch2/single_include
)

add_executable(nerd-test ${NERD_TEST})
target_include_directories(nerd-test PRIVATE ${NERD_TEST_INC})
target_link_libraries(nerd-test ${NERD_LIB} nerdcore)
set_target_properties(nerd-test PROPERTIES VERSION ${PROJECT_VERSION})

# examples
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
