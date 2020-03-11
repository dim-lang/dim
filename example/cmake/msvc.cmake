# Copyright 2019- <coli-lang>
# Apache License Version 2.0

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14 /W4")

set(CINC
    .
    ../src
    ../src/spdlog/include
    ../src/fmt/include
    ../src/boost
    ../src/icu/icu4c/include
    )

set(CLIB
    libboost_program_options-vc141-mt-x32-1_70.lib
    libboost_system-vc141-mt-x32-1_70.lib
    icudt.lib
    icuin.lib
    icuio.lib
    icutu.lib
    icuuc.lib
    )

set(CLIBD
    libboost_program_options-vc141-mt-gd-x32-1_70.lib
    libboost_system-vc141-mt-gd-x32-1_70.lib
    icudt.lib
    icuind.lib
    icuiod.lib
    icutud.lib
    icuucd.lib
    )

set(LIB_DIR
    ../src/boost/stage/lib
    ../src/icu/icu4c/lib
    )

add_definitions(-DFMT_HEADER_ONLY)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
include_directories(${CINC})
link_directories(${LIB_DIR})

add_executable(coli-icu_ustdio icu_ustdio.cpp)
target_include_directories(coli-icu_ustdio PRIVATE ${CINC})
#target_link_libraries(coli-icu_ustdio debug ${CLIBD})
target_link_libraries(coli-icu_ustdio ${CLIB})
set_target_properties(coli-icu_ustdio PROPERTIES VERSION ${PROJECT_VERSION})
