# Copyright 2019- <shepherd-lang>
# Apache License Version 2.0

project(shepherdc VERSION 0.0.2 LANGUAGES CXX)
configure_file(${PROJECT_SOURCE_DIR}/Configure.h.in ${PROJECT_SOURCE_DIR}/Configure.h)
set(CMAKE_CXX_STANDARD 14)

if(WIN32)
    set(CMAKE_C_COMPILER cl)
    set(CMAKE_CXX_COMPILER cl)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14 /W4")
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
    #set(Boost_DEBUG ON)
    #set(Boost_DETAILED_FAILURE_MSG ON)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_THREADAPI win32)
    set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/boost")
    set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
    set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
    set(LLVM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/llvm-project/llvm/build_release/lib/cmake/llvm")
else()
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    find_package(Threads REQUIRED)
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
llvm_map_components_to_libnames(llvm_libraries
    analysis support core object irreader executionengine scalaropts instcombine orcjit runtimedyld codegen linker bitreader bitstreamreader asmparser asmprinter native
    # xcore
    xcorecodegen xcoredesc xcoredisassembler xcoreinfo
    # x86
    x86asmparser x86codegen x86desc x86disassembler x86info x86utils
    # webassembly
    webassemblyasmparser webassemblycodegen webassemblydesc webassemblydisassembler webassemblyinfo
    # systemz
    systemzasmparser systemzcodegen systemzdesc systemzdisassembler systemzinfo
    # sparc
    sparcasmparser sparccodegen sparcdesc sparcdisassembler sparcinfo
    # riscv
    riscvasmparser riscvcodegen riscvdesc riscvdisassembler riscvinfo riscvutils
    # powerpc
    powerpcasmparser powerpccodegen powerpcdesc powerpcdisassembler powerpcinfo
    # nvptx
    nvptxcodegen nvptxdesc nvptxinfo
    # msp430
    msp430asmparser msp430codegen msp430desc msp430disassembler msp430info
    # mips
    mipsasmparser mipscodegen mipsdesc mipsdisassembler mipsinfo
    # lanai
    lanaiasmparser lanaicodegen lanaidesc lanaidisassembler lanaiinfo
    # hexagon
    hexagonasmparser hexagoncodegen hexagondesc hexagondisassembler hexagoninfo
    # bpf
    bpfasmparser bpfcodegen bpfdesc bpfdisassembler bpfinfo
    # arm
    armasmparser armcodegen armdesc armdisassembler arminfo armutils
    # amdgpu
    amdgpuasmparser amdgpucodegen amdgpudesc amdgpudisassembler amdgpuinfo amdgpuutils
    # aarch64
    aarch64asmparser aarch64codegen aarch64desc aarch64disassembler aarch64info aarch64utils
    )
find_package(FLEX)
find_package(BISON)
FLEX_TARGET(ShepherdTokenizer Token.l ${CMAKE_CURRENT_SOURCE_DIR}/Token.yy.cpp)
BISON_TARGET(ShepherdParser Parser.y ${CMAKE_CURRENT_SOURCE_DIR}/Parser.tab.cpp 
    DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Parser.tab.hpp)
ADD_FLEX_BISON_DEPENDENCY(ShepherdTokenizer ShepherdParser)

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
set(SHEPHERD
    Log.cpp
    Exception.cpp
    container/CycleBuffer.cpp
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
    Buffer.cpp
    Scanner.cpp
    Ast.cpp
    Position.cpp
    Symbol.cpp
    SymbolTable.cpp
    Counter.cpp
    Dump.cpp
    Ir.cpp
    IrContext.cpp
    IrFactory.cpp
    IrUtil.cpp
    IrWriter.cpp
    ${BISON_ShepherdParser_OUTPUTS}
    ${FLEX_ShepherdTokenizer_OUTPUTS}
)
set(SHEPHERDC
    shepherdc.cpp
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
set_target_properties(shepherd PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(shepherdc ${SHEPHERDC})
target_include_directories(shepherdc PRIVATE ${SHEPHERD_INC})
target_link_libraries(shepherdc ${SHEPHERD_LIB} shepherd)
set_target_properties(shepherdc PROPERTIES VERSION ${PROJECT_VERSION})
