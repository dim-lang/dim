# Copyright 2019- <nerd-lang>
# Apache License Version 2.0

project(nerd-test VERSION 0.0.2 LANGUAGES CXX)
configure_file(${PROJECT_SOURCE_DIR}/../src/Configure.h.in ${PROJECT_SOURCE_DIR}/../src/Configure.h)
set(CMAKE_CXX_STANDARD 14)

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
    set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../src/boost")
    set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
    set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
    set(LLVM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../src/llvm-project/llvm/build_release/lib/cmake/llvm")
else()
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
    add_compile_options("-std=c++14")
    add_compile_options("-Wall")
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
execute_process(COMMAND llvm-config --libs all OUTPUT_VARIABLE llvm_libs)
execute_process(COMMAND llvm-config --system-libs all OUTPUT_VARIABLE llvm_system_libs)
string(REGEX REPLACE "\n$" "" llvm_libs "${llvm_libs}")
string(REGEX REPLACE "\n$" "" llvm_system_libs "${llvm_system_libs}")
if(WIN32)
    string(REGEX REPLACE ".lib " ".lib;" llvm_libs "${llvm_libs}")
    string(REGEX REPLACE ".lib " ".lib;" llvm_system_libs "${llvm_system_libs}")
endif(WIN32)

set(SHEPHERD_INC
    ${SHEPHERD_INC}
    .
    ../src
    ../src/spdlog/include
    ../src/fmt/include
    ../src/better-enums
    Catch2/single_include
    ${LLVM_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    )
set(SHEPHERD_LIB
    ${SHEPHERD_LIB}
    ${Boost_LIBRARIES}
    ${llvm_libs}
    ${llvm_system_libs}
    )
set(SHEPHERD_LIB_DIR
    ${SHEPHERD_LIB_DIR}
    .
    ../src
    ${Boost_LIBRARY_DIRS}
    ${LLVM_LIBRARY_DIRS}
    )
set(SHEPHERD_TEST
    MainTest.cpp
    LogTest.cpp
    ExceptionTest.cpp
    container/CycleBufferTest.cpp
    ConfigureTest.cpp
    # TimerTest.cpp
    # RandomTest.cpp
    FileTest.cpp
    # ApproximateTest.cpp
    # HashTest.cpp
    TokenTest.cpp
    ParserTest.cpp
    # SemanticTest.cpp
    # DumpTest.cpp
    # NameGeneratorTest.cpp
    # IrUtilTest.cpp
    # IrWriterTest.cpp
    container/LinkedHashMapTest.cpp
)


message(CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS})
message(Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS})
message(Boost_LIBRARIES: ${Boost_LIBRARIES})
message(Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS})
message(Boost_DEFINITIONS: ${Boost_DEFINITIONS})
message(LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS})
message(llvm_libs: ${llvm_libs})
message(llvm_system_libs: ${llvm_system_libs})
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

add_executable(nerd-test ${SHEPHERD_TEST})
target_include_directories(nerd-test PRIVATE ${SHEPHERD_INC})
target_link_libraries(nerd-test ${SHEPHERD_LIB} nerd)
set_target_properties(nerd-test PROPERTIES VERSION ${PROJECT_VERSION})
