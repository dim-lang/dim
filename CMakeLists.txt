# Copyright 2019- <dim-lang>
# Apache License Version 2.0

# common {

cmake_minimum_required(VERSION 3.8)
project(dim VERSION 0.0.3 LANGUAGES CXX)
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
    # set(Boost_DEBUG ON)
    # set(Boost_DETAILED_FAILURE_MSG ON)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_THREADAPI win32)
    set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src/boost")
    if(${DIM_BUILD_DEBUG} EQUAL 1)
        set(BOOST_LIBRARYDIR "${BOOST_ROOT}/Debug/lib")
        set(LLVM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/llvm-project/llvm/Debug/lib/cmake/llvm")
    else(${DIM_BUILD_DEBUG} EQUAL 1)
        set(BOOST_LIBRARYDIR "${BOOST_ROOT}/Release/lib")
        set(LLVM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/llvm-project/llvm/Release/lib/cmake/llvm")
    endif(${DIM_BUILD_DEBUG} EQUAL 1)
    set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
    set(TokenizerFlags "${TokenizerFlags} --wincompat")
else()
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
    add_compile_options("-std=c++14")
    add_compile_options("-Wall")
    find_package(Threads REQUIRED)
    set(DIM_CORE_INC
        Threads::Threads
        )
    if(IS_DIRECTORY /usr/include)
        set(DIM_CORE_INC
            ${DIM_CORE_INC}
            /usr/include
            )
    endif(IS_DIRECTORY /usr/include)
    if(IS_DIRECTORY /usr/local/include)
        set(DIM_CORE_INC
            ${DIM_CORE_INC}
            /usr/local/include
            )
    endif(IS_DIRECTORY /usr/local/include)
    set(DIM_CORE_LIB
        Threads::Threads
        )
    set(DIM_CORE_LIB_DIR
        /usr/lib)
    if(IS_DIRECTORY /usr/lib64)
        set(DIM_CORE_LIB_DIR
            ${DIM_CORE_LIB_DIR}
            /usr/lib64)
    endif(IS_DIRECTORY /usr/lib64)
    if(IS_DIRECTORY /usr/local/lib)
        set(DIM_CORE_LIB_DIR
            ${DIM_CORE_LIB_DIR}
            /usr/local/lib)
    endif(IS_DIRECTORY /usr/local/lib)
    if(IS_DIRECTORY /usr/local/lib64)
        set(DIM_CORE_LIB_DIR
            ${DIM_CORE_LIB_DIR}
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

set(DIM_CORE_INC
    ${DIM_CORE_INC}
    src
    src/spdlog/include
    src/fmt/include
    src/better-enums
    ${LLVM_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    )
set(DIM_CORE_LIB
    ${DIM_CORE_LIB}
    ${Boost_LIBRARIES}
    ${llvm_libs}
    # ${llvm_system_libs}
    )
set(DIM_CORE_LIB_DIR
    ${DIM_CORE_LIB_DIR}
    .
    ${Boost_LIBRARY_DIRS}
    ${LLVM_LIBRARY_DIRS}
    )

add_definitions(-DFMT_HEADER_ONLY)
add_definitions(-DSPDLOG_FMT_EXTERNAL)
add_definitions(${Boost_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})
include_directories(${DIM_CORE_INC})
link_directories(${DIM_CORE_LIB_DIR})

message(DIM_BUILD_DEBUG: ${DIM_BUILD_DEBUG})
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
message(DIM_CORE_INC: ${DIM_CORE_INC})
message(DIM_CORE_LIB: ${DIM_CORE_LIB})
message(DIM_CORE_LIB_DIR: ${DIM_CORE_LIB_DIR})

# common }

# dimcore {

set(DIM_CORE_SRC
    src/iface/Identifiable.cpp
    # src/iface/LLVMModular.cpp
    # src/iface/LLVMTypable.cpp
    # src/iface/LLVMValuable.cpp
    src/iface/Locationable.cpp
    src/iface/Nameable.cpp
    src/iface/Phase.cpp
    src/iface/Scoped.cpp
    src/iface/Symbolizable.cpp
    src/iface/TypeSymbolizable.cpp
    src/iface/Visitor.cpp

    src/infra/Counter.cpp
    src/infra/Cowstr.cpp
    src/infra/CycleBuffer.cpp
    src/infra/Files.cpp
    src/infra/Log.cpp
    src/infra/Strings.cpp

    src/Ast.cpp
    src/Compiler.cpp
    src/Drawer.cpp
    src/Dumper.cpp
    src/IrBuilder.cpp
    # src/Label.cpp
    src/Location.cpp
    src/NameGenerator.cpp
    src/Option.cpp
    src/Scanner.cpp
    src/Symbol.cpp
    src/SymbolBuilder.cpp
    src/SymbolResolver.cpp
    src/Token.cpp

    ${BISON_Parser_OUTPUTS}
    ${FLEX_Tokenizer_OUTPUTS}
)

add_library(dimcore STATIC ${DIM_CORE_SRC})
target_include_directories(dimcore PRIVATE ${DIM_CORE_INC})
target_link_libraries(dimcore ${DIM_CORE_LIB})
set_target_properties(dimcore PROPERTIES VERSION ${PROJECT_VERSION})

# dimcore }

# dimc {

set(DIMC_SRC
    src/dimc.cpp
)

add_executable(dimc ${DIMC_SRC})
target_include_directories(dimc PRIVATE ${DIM_CORE_INC})
target_link_libraries(dimc ${DIM_CORE_LIB} dimcore)
set_target_properties(dimc PROPERTIES VERSION ${PROJECT_VERSION})

# dimc }

# dim-test {

set(DIM_TEST_SRC
    test/iface/IdentifiableTest.cpp
    # test/iface/LLVMModularTest.cpp
    # test/iface/LLVMTypableTest.cpp
    # test/iface/LLVMValuableTest.cpp
    test/iface/NameableTest.cpp

    test/infra/CounterTest.cpp
    test/infra/CowstrTest.cpp
    test/infra/CycleBufferTest.cpp
    test/infra/FilesTest.cpp
    test/infra/LinkedHashMapTest.cpp
    test/infra/LogTest.cpp

    test/ConfigureTest.cpp
    test/DrawerTest.cpp
    test/DumperTest.cpp
    test/IrBuilderTest.cpp
    test/LocationTest.cpp
    test/OptionTest.cpp
    test/ParserTest.cpp
    test/SymbolBuilderTest.cpp
    test/SymbolResolverTest.cpp
    test/TokenizerTest.cpp
    test/UnitTest.cpp
)
set(DIM_TEST_INC
    ${DIM_CORE_INC}
    test/Catch2/single_include
)

add_executable(dim-test ${DIM_TEST_SRC})
target_include_directories(dim-test PRIVATE ${DIM_TEST_INC})
target_link_libraries(dim-test ${DIM_CORE_LIB} dimcore)
set_target_properties(dim-test PROPERTIES VERSION ${PROJECT_VERSION})

# dim-test }

# dim-example {

set(DIM_EXAMPLE_llvms_BinaryOperation
    example/llvms/BinaryOperation.cpp
    example/llvms/Util.cpp
    )
set(DIM_EXAMPLE_INC
    ${DIM_CORE_INC}
    example
    )

add_executable(dim-example-llvms-BinaryOperation ${DIM_EXAMPLE_llvms_BinaryOperation})
target_include_directories(dim-example-llvms-BinaryOperation PRIVATE ${DIM_EXAMPLE_INC})
target_link_libraries(dim-example-llvms-BinaryOperation ${DIM_CORE_LIB})
set_target_properties(dim-example-llvms-BinaryOperation PROPERTIES VERSION ${PROJECT_VERSION})

# dim-example }
