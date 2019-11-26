grammar fastype;

program :   statement_list
        ;

declaration :   variable_declaration
            |   function_declaration
            |   class_declaration
            ;

variable_declaration:   'let' variable '=' expr ';'
                    |   'let' variable '=' expr (',' variable '=' expr )+ ';'
                    ;

statement_list  :   statement*
                |   declaration*
                ;

statement   :   compound_statement
            |   assignment_statement
            |   empty_statement
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
