# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype VERSION 0.1.0 LANGUAGES CXX)
configure_file(Configure.h.in Configure.h)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ".")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14 /W4")

message(CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR})
message(CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR})
message(CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH})
message(CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE})
message(CMAKE_VERBOSE_MAKEFILE: ${CMAKE_VERBOSE_MAKEFILE})

set(F_INC
    .
    spdlog/include
    fmt/include
    boost
    icu/icu4c/include
    )
set(F_LIB
    libboost_program_options-vc141-mt-x64-1_70.lib
    libboost_system-vc141-mt-x64-1_70.lib
    icudt.lib
    icuin.lib
    icuio.lib
    icutu.lib
    icuuc.lib
    )
set(F_LIB_DIR
    boost/stage/lib
    icu/icu4c/lib64
    )

set(F_CORE
    Logging.cpp
    Term.cpp
    Option.cpp
    Timer.cpp
    DateTime.cpp
    Filer.cpp
    Hasher.cpp
    Line.cpp
    Buffer.cpp
    Cache.cpp
    Random.cpp
    Approximate.cpp
    WinTerm.cpp

    graph/Dot.cpp

    view/Position.cpp
    view/Area.cpp
    #view/Window.cpp
    view/Vec.cpp
    #view/Cursor.cpp

    controller/Key.cpp

    event/Poll.cpp
    event/EventLoopImpl.cpp
    event/poll/Epoll.cpp
    event/poll/Kqueue.cpp
    event/poll/Select.cpp
    event/poll/MswinSelect.cpp

    script/Token.cpp
    script/Lexer.cpp
    script/Parser.cpp
    script/Interpreter.cpp
    script/Util.cpp
    script/ast/BinaryOp.cpp
    script/ast/UnaryOp.cpp
    script/ast/IntegerConstant.cpp
    script/ast/FloatingConstant.cpp
    script/ast/StringConstant.cpp
    script/ast/BooleanConstant.cpp
    script/ast/IdentifierConstant.cpp
    script/ast/AssignmentStatement.cpp
    script/ast/CompoundStatement.cpp
    script/ast/EmptyStatement.cpp
    script/ast/StatementList.cpp
    script/ast/VariableDeclaration.cpp
    script/ast/Variable.cpp
    script/ast/Program.cpp
    script/token/AssignmentToken.cpp
    script/token/BooleanToken.cpp
    script/token/ComparatorToken.cpp
    script/token/EofToken.cpp
    script/token/FloatingToken.cpp
    script/token/IdentifierToken.cpp
    script/token/IntegerToken.cpp
    script/token/OperatorToken.cpp
    script/token/PunctuationToken.cpp
    script/token/KeywordToken.cpp
    script/token/StringToken.cpp
    )
set(F_SRC
    fastype.cpp
    )

set(FD_SRC
    fastyped.cpp
    )

add_definitions(-DFMT_HEADER_ONLY)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
include_directories(${F_INC})
link_directories(${F_LIB_DIR})

add_library(fastypecore STATIC ${F_CORE})
target_include_directories(fastypecore PRIVATE ${F_INC})
target_link_libraries(fastypecore ${F_LIB})
set_target_properties(fastypecore PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype ${F_SRC})
target_include_directories(fastype PRIVATE ${F_INC})
target_link_libraries(fastype ${F_LIB} fastypecore)
set_target_properties(fastype PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastyped ${FD_SRC})
target_include_directories(fastyped PRIVATE ${F_INC})
target_link_libraries(fastyped ${F_LIB} fastypecore)
set_target_properties(fastyped PROPERTIES VERSION ${PROJECT_VERSION})
