grammar fastype;

program :   statement_list
        ;

statement_list  :   statement*
                |   declaration*
                ;

declaration :   variable_declaration
            |   function_declaration
            ;

variable_declaration:   'let' variable '=' expr (',' variable '=' expr)* ';'
                    ;

function_declaration:   'func' IDENTIFIER '(' variable_list ')' compound_statement
                    ;

variable_list   :   variable ( ',' variable )*
                ;

statement   :   compound_statement
            |   assignment_statement
            |   empty_statement
            |   return_statement
            ;

return_statement:   'return' expr ';'
                ;

compound_statement  : '{' statement_list '}'
                    ;

assignment_statement:   variable '=' expr ';'
                    ;


empty_statement :   ';'
                ;

expr:   term
    |   term '+' term
    |   term '-' term
    ;

term:   factor
    |   factor '*' factor
    |   factor '/' factor
    |   factor '%' factor
    ;

factor  :   INTEGER_CONSTANT
        |   FLOATING_CONSTANT
        |   BOOLEAN_CONSTANT
        |   STRING_CONSTANT
        |   variable
        |   '(' expr ')'
        |   '+' factor
        |   '-' factor
        |   '++' factor
        |   '--' factor
        |   factor '++'
        |   factor '--'
        ;

variable:   IDENTIFIER
        ;
