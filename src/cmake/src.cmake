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
    Parser.cpp

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

    #script/Token.cpp
    #script/Lexer.cpp
    #script/Parser.cpp
    #script/Interpreter.cpp
    #script/Util.cpp
    #script/Type.cpp
    #script/ast/BinaryOp.cpp
    #script/ast/UnaryOp.cpp
    #script/ast/IntegerConstant.cpp
    #script/ast/FloatingConstant.cpp
    #script/ast/StringConstant.cpp
    #script/ast/BooleanConstant.cpp
    #script/ast/IdentifierConstant.cpp
    #script/ast/AssignmentStatement.cpp
    #script/ast/CompoundStatement.cpp
    #script/ast/EmptyStatement.cpp
    #script/ast/StatementList.cpp
    #script/ast/VariableDeclaration.cpp
    #script/ast/FunctionDeclaration.cpp
    #script/ast/Variable.cpp
    #script/ast/Program.cpp
    #script/token/AssignmentToken.cpp
    #script/token/BooleanToken.cpp
    #script/token/ComparatorToken.cpp
    #script/token/EofToken.cpp
    #script/token/FloatingToken.cpp
    #script/token/IdentifierToken.cpp
    #script/token/IntegerToken.cpp
    #script/token/OperatorToken.cpp
    #script/token/PunctuationToken.cpp
    #script/token/KeywordToken.cpp
    #script/token/StringToken.cpp
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
