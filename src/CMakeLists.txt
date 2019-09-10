# Copyright 2019- <fastype.org>
# Apache License Version 2.0

cmake_minimum_required(VERSION 3.8)
project(fastype VERSION 0.1.0 LANGUAGES CXX)
message(input_config: ${CMAKE_CURRENT_SOURCE_DIR})
message(output_config: ${CMAKE_CURRENT_BINARY_DIR})
configure_file(Configure.h.in ../../src/Configure.h)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ".")
message(CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH})
message(CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE})
message(CMAKE_VERBOSE_MAKEFILE: ${CMAKE_VERBOSE_MAKEFILE})
message(F_OS: ${F_OS})

if (APPLE)
    # MacOSX
    include(CMakeMacOSX.cmake)
elseif (UNIX AND NOT APPLE)
    if (${F_OS} STREQUAL "BSD")
        # BSD
        include(CMakeBSD.cmake)
    elseif (${F_OS} STREQUAL "Linux")
        # Linux
        include(CMakeLinux.cmake)
    else()
        message(FATAL_ERROR "${F_OS} Not Support!")
    endif(${F_OS} STREQUAL "BSD")
elseif (MSVC)
    # Windows
    include(CMakeWindows.cmake)
endif(APPLE)

message(F_INC: ${F_INC})
message(F_LIB_DIR: ${F_LIB_DIR})
message(F_LIB: ${F_LIB})


set(F_CORE
    Logging.cpp
    Term.cpp
    Option.cpp
    Timer.cpp
    DateTime.cpp

    graph/Dot.cpp

    view/Position.cpp
    view/Area.cpp
    view/Window.cpp
    view/Vec.cpp
    view/Cursor.cpp

    controller/Key.cpp

    event/Poll.cpp
    event/EventLoopImpl.cpp
    event/poll/Epoll.cpp
    event/poll/Kqueue.cpp
    event/poll/Select.cpp
    event/poll/WinSelect.cpp

    script/Token.cpp
    script/Lexer.cpp
    script/token/EofToken.cpp
    script/token/IdToken.cpp
    script/token/StringToken.cpp
    script/token/NumberToken.cpp
    script/token/BooleanToken.cpp

    Buffer.cpp
    Cowstr.cpp
    Row.cpp
    Buffer.cpp
    Cache.cpp
    ResourceHandler.cpp
    )
set(F_SRC
    fastype.cpp
    )
set(FD_SRC
    fastyped.cpp
    )

if (MSVC)
    set(F_CORE ${F_CORE} WinTerm.cpp)
else()
    set(F_CORE ${F_CORE} NCursesTerm.cpp)
endif(MSVC)

include_directories(${F_INC})
link_directories(${F_LIB_DIR})

add_library(ftcore SHARED ${F_CORE})
target_include_directories(ftcore PRIVATE ${F_INC})
target_link_libraries(ftcore ${F_LIB})
set_target_properties(ftcore PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastype ${F_SRC})
target_include_directories(fastype PRIVATE ${F_INC})
target_link_libraries(fastype ${F_LIB} ftcore)
set_target_properties(fastype PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(fastyped ${FD_SRC})
target_include_directories(fastyped PRIVATE ${F_INC})
target_link_libraries(fastyped ${F_LIB} ftcore)
set_target_properties(fastyped PROPERTIES VERSION ${PROJECT_VERSION})
