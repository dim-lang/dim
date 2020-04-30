# Copyright 2019- <shepherd-lang>
# Apache License Version 2.0

project(shepherd-example VERSION 0.0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if(WIN32)
    set(CMAKE_C_COMPILER cl)
    set(CMAKE_CXX_COMPILER cl)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14 /W4")
    set(Boost_DEBUG ON)
    set(Boost_DETAILED_FAILURE_MSG ON)
    set(Boost_THREADAPI win32)
    set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../src/boost")
    set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
    set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
    set(Boost_COMPILER "-vc")
    set(LLVM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../src/llvm-project/llvm/install/lib/cmake/llvm")
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
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

#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#set(Boost_USE_DEBUG_LIBS OFF)
#set(Boost_USE_RELEASE_LIBS ON)
find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(llvm_libraries analysis support core object irreader executionengine scalaropts instcombine orcjit runtimedyld)

set(SHEPHERD_INC
    ${SHEPHERD_INC}
    .
    ../src/spdlog/include
    ../src/fmt/include
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

#add_definitions(-DBOOST_ALL_NO_LIB)
#add_definitions(-DBOOST_FILESYSTEM_DYN_LINK)
#add_definitions(-DBOOST_PROGRAM_OPTIONS_DYN_LINK)
#add_definitions(-DBOOST_SYSTEM_DYN_LINK)
add_definitions(-DFMT_HEADER_ONLY)
add_definitions(-DSPDLOG_FMT_EXTERNAL)
add_definitions(${Boost_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})
include_directories(${SHEPHERD_INC})
link_directories(${SHEPHERD_LIB_DIR})

add_executable(shepherd-llvm_IRBuilder_Add llvm_IRBuilder_Add.cpp llvm_IRBuilder_Util.cpp)
target_include_directories(shepherd-llvm_IRBuilder_Add PRIVATE ${SHEPHERD_INC})
target_link_libraries(shepherd-llvm_IRBuilder_Add ${SHEPHERD_LIB})
set_target_properties(shepherd-llvm_IRBuilder_Add PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(shepherd-llvm_IRBuilder_Sub llvm_IRBuilder_Sub.cpp llvm_IRBuilder_Util.cpp)
target_include_directories(shepherd-llvm_IRBuilder_Sub PRIVATE ${SHEPHERD_INC})
target_link_libraries(shepherd-llvm_IRBuilder_Sub ${SHEPHERD_LIB})
set_target_properties(shepherd-llvm_IRBuilder_Sub PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(shepherd-llvm_IRBuilder_Mul llvm_IRBuilder_Mul.cpp llvm_IRBuilder_Util.cpp)
target_include_directories(shepherd-llvm_IRBuilder_Mul PRIVATE ${SHEPHERD_INC})
target_link_libraries(shepherd-llvm_IRBuilder_Mul ${SHEPHERD_LIB})
set_target_properties(shepherd-llvm_IRBuilder_Mul PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(shepherd-llvm_IRBuilder_Div llvm_IRBuilder_Div.cpp llvm_IRBuilder_Util.cpp)
target_include_directories(shepherd-llvm_IRBuilder_Div PRIVATE ${SHEPHERD_INC})
target_link_libraries(shepherd-llvm_IRBuilder_Div ${SHEPHERD_LIB})
set_target_properties(shepherd-llvm_IRBuilder_Div PROPERTIES VERSION ${PROJECT_VERSION})
