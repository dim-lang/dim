grammar fastype;

program :   statement_list
        ;

statement_list  :   declaration*
                |   statement*
                ;

declaration :   variable_declaration
            |   function_declaration
            ;

variable_declaration:   'let' variable (':' IDENTIFIER)? '=' expression ';'
                    ;

function_declaration:   'func' IDENTIFIER '(' variable (':' IDENTIFIER)? (',' variable (':' IDENTIFIER)?)* ')' compound_statement
                    ;

statement   :   compound_statement
            |   assignment_statement
            |   empty_statement
            |   return_statement
            ;

compound_statement  : '{' statement_list '}'
                    ;

assignment_statement:   variable '=' expression ';'
                    ;

empty_statement :   ';'
                ;

return_statement:   'return' expression ';'
                ;

expression  :   term ('+' | '-' term)*
            ;

term:   factor ('*' | '/' | '%' factor)*
    ;

factor  :   INTEGER_CONSTANT
        |   FLOATING_CONSTANT
        |   BOOLEAN_CONSTANT
        |   STRING_CONSTANT
        |   variable
        |   '(' expression ')'
        |   '+' factor
        |   '-' factor
        |   '++' factor
        |   '--' factor
        |   factor '++'
        |   factor '--'
        ;

variable:   IDENTIFIER
        ;
