# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype VERSION 0.1.0 LANGUAGES CXX)
configure_file(Configure.h.in Configure.h)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ".")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

message(CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR})
message(CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR})
message(CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH})
message(CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE})
message(CMAKE_VERBOSE_MAKEFILE: ${CMAKE_VERBOSE_MAKEFILE})
message(F_OS: ${F_OS})

set(F_INCLUDE_DIR
    ${CURSES_INCLUDE_DIR}
    )

set(F_LIBRARIES
    ${CURSES_LIBRARIES}
    Threads::Threads
    )

set(F_LIBRARY_DIR
    ${UNIX_LIBRARY_DIR}
    )

message(F_INC: ${F_INC})
message(F_LIB_DIR: ${F_LIB_DIR})
message(F_LIB: ${F_LIB})

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
    CursesTerm.cpp

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
    event/poll/WinSelect.cpp

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

include_directories(${F_INC})
link_directories(${F_LIB_DIR})

add_library(fastypecore SHARED ${F_CORE})
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
