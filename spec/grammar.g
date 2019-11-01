grammar fastype;


// expression-1: primary expression
primaryExpr
    :   Identifier
    |   Constant
    |   StringLiteral+
    |   '(' expression ')'
    ;

// expression-2: postfix expression
postExpr
    :   primaryExpr
    |   postExpr '[' expression ']'   // array indexing, for example: people[0]
    |   postExpr '(' argumentExprList? ')'   // function call, for example: print("hello world", 10, 20)
    |   postExpr '.' Identifier   // get field, for example: student.score
    |   postExpr '++'
    |   postExpr '--'
    ;

argumentExprList
    :   assignmentExpr
    |   argumentExprList ',' assignmentExpr
    ;

// expression-3: unary expression
unaryExpr
    :   postExpr
    |   '++' unaryExpr
    |   '--' unaryExpr
    |   unaryOperator unaryExpr
    ;

unaryOperator
    :   '&' | '*' | '+' | '-' | '~' | '!'
    ;

// expression-5: multiplicative expression
multipleExpr
    :   unaryExpr
    |   multipleExpr '*' unaryExpr
    |   multipleExpr '/' unaryExpr
    |   multipleExpr '%' unaryExpr
    ;

// expression-6: additive expression
addExpr
    :   multipleExpr
    |   addExpr '+' multipleExpr
    |   addExpr '-' multipleExpr
    ;

// expression-7: relational expression
relationExpr
    :   addExpr
    |   relationExpr '<' addExpr
    |   relationExpr '>' addExpr
    |   relationExpr '<=' addExpr
    |   relationExpr '>=' addExpr
    ;

// expression-8: equality expression
eqExpr
    :   relationExpr
    |   eqExpr '==' relationExpr
    |   eqExpr '!=' relationExpr
    ;

// expression-9: logical and expression
andExpr
    :   addExpr
    |   andExpr '&&' addExpr
    ;

// expression-10: logical or expression
orExpr
    :   andExpr
    |   orExpr '||' andExpr
    ;

// expression-11: assignment expression
assignmentExpr
    :   orExpr
    |   'let' Identifier assignmentOperator assignmentExpr
    ;

assignmentOperator
    : '='
    | '*='
    | '/='
    | '%='
    | '+='
    | '-='
    ;

// expression-12: expression
expression
    :   orExpr
    |   expression ',' orExpr
    ;

// declaration-1: declaration
declaration
    : 'let' initDeclaratorList ';'
    | declarationSpecifiers ';'
    ;

declarationSpecifiers
    :   declarationSpecifier+
    ;

declarationSpecifier
    :   functionSpecifier
    |   alignmentSpecifier
    ;

initDeclaratorList
    :   initDeclarator
    |   initDeclaratorList ',' initDeclarator
    ;

initDeclarator
    :   'let' declarator '=' initializer
    ;

structDeclaratorList
    :   structDeclarator
    |   structDeclaratorList ',' structDeclarator
    ;

declarator
    :   directDeclarator
    ;

directDeclarator
    :   Identifier
    |   '(' declarator ')'
    |   directDeclarator '[' assignmentExpr? ']'
    |   directDeclarator '[' 'static' assignmentExpr ']'
    |   directDeclarator '(' identifierList? ')'   // function call, like: print(x, y)
    |   Identifier ':' DigitSequence  // bit field
    ;

parameterList
    :   Identifier
    |   parameterList ',' Identifier
    ;

identifierList
    :   Identifier
    |   identifierList ',' Identifier
    ;

abstractDeclarator
    :   directAbstractDeclarator
    ;

directAbstractDeclarator
    :   '(' abstractDeclarator ')'
    |   '[' assignmentExpr? ']'
    |   '(' parameterTypeList? ')'
    |   directAbstractDeclarator '[' assignmentExpr? ']'
    |   directAbstractDeclarator '(' parameterTypeList? ')'
    ;

initializer
    :   assignmentExpr
    |   '{' initializerList '}'
    |   '{' initializerList ',' '}'
    ;

initializerList
    :   designation? initializer
    |   initializerList ',' designation? initializer
    ;

designation
    :   designatorList '='
    ;

designatorList
    :   designator
    |   designatorList designator
    ;

designator
    :   '[' orExpr ']'
    |   '.' Identifier
    ;

statement
    :   '{' blockItemList? '}'
    |   expressionStatement
    |   selectionStatement
    |   iterationStatement
    |   jumpStatement
    |   ('__asm' | '__asm__') ('volatile' | '__volatile__') '(' (orExpr (',' orExpr)*)? (':' (orExpr (',' orExpr)*)?)* ')' ';'
    ;

blockItemList
    :   blockItem
    |   blockItemList blockItem
    ;

blockItem
    :   statement
    ;

expressionStatement
    :   expression? ';'
    ;

selectionStatement
    :   'if' '(' expression ')' statement ('else' statement)?
    |   'switch' '(' expression ')' statement
    ;

iterationStatement
    :   While '(' expression ')' statement
    |   Do statement While '(' expression ')' ';'
    |   For '(' forCondition ')' statement
    ;

forCondition
	:   forDeclaration ';' forExpression? ';' forExpression?
	|   expression? ';' forExpression? ';' forExpression?
	;

forDeclaration
    :   declarationSpecifiers initDeclaratorList
	| 	declarationSpecifiers
    ;

forExpression
    :   assignmentExpr
    |   forExpression ',' assignmentExpr
    ;

jumpStatement
    :   'goto' Identifier ';'
    |   'continue' ';'
    |   'break' ';'
    |   'return' expression? ';'
    |   'goto' unaryExpr ';' // GCC extension
    ;

compilationUnit
    :   translationUnit? EOF
    ;

translationUnit
    :   externalDeclaration
    |   translationUnit externalDeclaration
    ;

functionDefinition
    :   'func' Identifier '(' identifierList ? ')' '{' blockItemList? '}'
    ;

declarationList
    :   declaration
    |   declarationList declaration
    ;

Auto : 'auto';
Break : 'break';
Case : 'case';
Char : 'char';
Const : 'const';
Continue : 'continue';
Default : 'default';
Do : 'do';
Double : 'double';
Else : 'else';
Enum : 'enum';
Extern : 'extern';
Float : 'float';
For : 'for';
Goto : 'goto';
If : 'if';
Inline : 'inline';
Int : 'int';
Long : 'long';
Register : 'register';
Restrict : 'restrict';
Return : 'return';
Short : 'short';
Signed : 'signed';
Sizeof : 'sizeof';
Static : 'static';
Struct : 'struct';
Switch : 'switch';
Typedef : 'typedef';
Union : 'union';
Unsigned : 'unsigned';
Void : 'void';
Volatile : 'volatile';
While : 'while';

Alignas : '_Alignas';
Alignof : '_Alignof';
Atomic : '_Atomic';
Bool : '_Bool';
Complex : '_Complex';
Generic : '_Generic';
Imaginary : '_Imaginary';
Noreturn : '_Noreturn';
StaticAssert : '_Static_assert';
ThreadLocal : '_Thread_local';

LeftParen : '(';
RightParen : ')';
LeftBracket : '[';
RightBracket : ']';
LeftBrace : '{';
RightBrace : '}';

Less : '<';
LessEqual : '<=';
Greater : '>';
GreaterEqual : '>=';
LeftShift : '<<';
RightShift : '>>';

Plus : '+';
PlusPlus : '++';
Minus : '-';
MinusMinus : '--';
Star : '*';
Div : '/';
Mod : '%';

And : '&';
Or : '|';
AndAnd : '&&';
OrOr : '||';
Caret : '^';
Not : '!';
Tilde : '~';

Question : '?';
Colon : ':';
Semi : ';';
Comma : ',';

Assign : '=';
// '*=' | '/=' | '%=' | '+=' | '-=' | '<<=' | '>>=' | '&=' | '^=' | '|='
StarAssign : '*=';
DivAssign : '/=';
ModAssign : '%=';
PlusAssign : '+=';
MinusAssign : '-=';
LeftShiftAssign : '<<=';
RightShiftAssign : '>>=';
AndAssign : '&=';
XorAssign : '^=';
OrAssign : '|=';

Equal : '==';
NotEqual : '!=';

Arrow : '->';
Dot : '.';
Ellipsis : '...';

Constant
    :   IntegerConstant
    |   FloatingConstant
    |   BooleanConstant
    ;

fragment
Sign
    :   '+' | '-'
    ;
