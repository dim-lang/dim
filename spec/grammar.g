grammar fastype;


// expressions

primaryExpr
    :   Identifier
    |   IntegerConstant
    |   FloatingConstant
    |   BooleanConstant
    |   StringLiteral+
    |   '(' expression ')'
    ;

postExpr
    :   primaryExpr
    |   postExpr '[' expression ']'   // array indexing, for example: people[0]
    |   postExpr '(' argumentExprList? ')'   // function call, for example: print(x="hello world", 10, 20)
    |   postExpr '.' Identifier   // get field, for example: student.score
    |   postExpr '++'
    |   postExpr '--'
    ;

argumentExprList
    :   assignmentExpr
    |   argumentExprList ',' assignmentExpr
    ;

unaryExpr
    :   postExpr
    |   '++' unaryExpr
    |   '--' unaryExpr
    |   unaryOperator unaryExpr
    ;

unaryOperator
    :   '+' | '-' | '~' | '!'
    ;

multipleExpr
    :   unaryExpr
    |   multipleExpr '*' unaryExpr
    |   multipleExpr '/' unaryExpr
    |   multipleExpr '%' unaryExpr
    ;

addExpr
    :   multipleExpr
    |   addExpr '+' multipleExpr
    |   addExpr '-' multipleExpr
    ;

relationExpr
    :   addExpr
    |   relationExpr '<' addExpr
    |   relationExpr '>' addExpr
    |   relationExpr '<=' addExpr
    |   relationExpr '>=' addExpr
    ;

eqExpr
    :   relationExpr
    |   eqExpr '==' relationExpr
    |   eqExpr '!=' relationExpr
    ;

andExpr
    :   addExpr
    |   andExpr 'and' addExpr
    ;

orExpr
    :   andExpr
    |   orExpr 'or' andExpr
    ;

assignmentExpr
    :   orExpr
    |   Identifier assignmentOperator assignmentExpr
    ;

assignmentOperator
    : '='
    | '*='
    | '/='
    | '%='
    | '+='
    | '-='
    ;

expr
    :   orExpr
    ;

initDeclarator
    :   'let' Identifier '=' initializer

initializer
    :   assignmentExpr   // single value
    |   '[' arrayInitializerList? ']'     // array value, for example: [1, 2, 3]
    |   '[' arrayInitializerList ',' ']'  // array value, for example: [1, 2, 3,]
    |   '{' mapInitializerList '}'        // map value, for example: { "a":"b", "hello":"world" }
    |   '{' mapInitializerList ',' '}'    // map value, for example: { "a":"b", "hello":"world", }
    ;

arrayInitializerList   // 1, 2, 3
    :   initializer
    |   arrayInitializerList ',' initializer
    ;

mapInitializerList   // "a":"b", "hello":"world"
    :   mapInitializer
    |   mapInitializer ',' mapInitializerList
    ;

mapInitializer
    :   initializer ':' initializer
    ;

state
    :   '{' blockItemList? '}'
    |   exprState
    |   selectState
    |   iterateState
    |   jumpState
    |   '(' orExpr ')' ';'
    ;

blockItemList
    :   blockItem
    |   blockItemList blockItem
    ;

blockItem
    :   state
    ;

exprState
    :   expr? ';'
    ;

selectState
    :   'if' '(' expr ')' state ('else' state)?
    ;

iterateState
    :   While '(' expr ')' state
    |   For '(' forCondition ')' state
    ;

forCondition
    :   forDeclaration ';' forExpression? ';' forExpression?
    |   expr? ';' forExpression? ';' forExpression?
    ;

forDeclaration
    :   Identifier '=' expr
    ;

forExpression
    :   assignmentExpr
    ;

jumpState
    :   'continue' ';'
    |   'break' ';'
    |   'return' expr? ';'
    ;

functionDefinition
    :   'func' Identifier '(' identifierList ? ')' '{' blockItemList? '}'
    ;
