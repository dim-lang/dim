# Copyright 2019- <coli-lang>
# Apache License Version 2.0

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
find_package(Threads REQUIRED)

set(CINC
    .
    ../src
    ../src/spdlog/include
    ../src/fmt/include
    Catch2/single_include
    Threads::Threads
    /usr/local/opt/boost/include
    /usr/local/opt/icu4c/include
    /usr/local/opt/llvm/include
    )

set(CLIB
    Threads::Threads
    boost_program_options-mt
    boost_system-mt
    boost_filesystem-mt
    icuuc
    icuio
    icudata
    icui18n
    tcmalloc
    LLVMAnalysis
    LLVMCore
    LLVMExecutionEngine
    LLVMInstCombine
    LLVMObject
    LLVMOrcJIT
    LLVMRuntimeDyld
    LLVMScalarOpts
    LLVMSupport
    )

set(CLIB_DIR
    .
    ../src
    /usr/local/opt/boost/lib
    /usr/local/opt/icu4c/lib
    /usr/local/opt/gperftools/lib
    /usr/local/opt/llvm/lib
    )

add_definitions(-DFMT_HEADER_ONLY)
include_directories(${CINC})
link_directories(${CLIB_DIR})

add_executable(coli-test ${CTEST})
target_include_directories(coli-test PRIVATE ${CINC})
target_link_libraries(coli-test ${CLIB} colicore)
set_target_properties(coli-test PROPERTIES VERSION ${PROJECT_VERSION})
