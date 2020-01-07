# Copyright 2019- <fastype.org>
# Apache License Version 2.0

set(FCORE
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
    Token.cpp

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

    #dsl/Type.cpp
    #dsl/Token.cpp
    #dsl/Lexer.cpp
    #dsl/Parser.cpp
    #dsl/Interpreter.cpp
    #dsl/Util.cpp
    #dsl/ast/BinaryOp.cpp
    #dsl/ast/UnaryOp.cpp
    #dsl/ast/IntegerConstant.cpp
    #dsl/ast/FloatingConstant.cpp
    #dsl/ast/StringConstant.cpp
    #dsl/ast/BooleanConstant.cpp
    #dsl/ast/IdentifierConstant.cpp
    #dsl/ast/AssignmentStatement.cpp
    #dsl/ast/CompoundStatement.cpp
    #dsl/ast/EmptyStatement.cpp
    #dsl/ast/StatementList.cpp
    #dsl/ast/VariableDeclaration.cpp
    #dsl/ast/FunctionDeclaration.cpp
    #dsl/ast/Variable.cpp
    #dsl/ast/Program.cpp
    )

if (WIN32) 
    set(FCORE ${FCORE} WinTerm.cpp)
else ()
    set(FCORE ${FCORE} CursesTerm.cpp)
endif (WIN32)

set(FCLI
    fastype.cpp
    )

set(FSRV
    fastyped.cpp
    )
