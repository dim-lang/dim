grammar fastype;

program :   statement_list
        ;

statement_list  :   statement*
                ;

statement   :   compound_statement
            |   assignment_statement
            ;

compound_statement  : '{' statement_list '}'
                    ;

assignment_statement:   'let' variable '=' expr ';'
                    ;

variable:   IDENTIFIER
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

factor  :   INTEGER
        |   FLOATING
        |   BOOLEAN
        |   STRING
        |   IDENTIFIER
        |   '(' expr ')'
        |   '+' factor
        |   '-' factor
        |   '++' factor
        |   '--' factor
        |   factor '++'
        |   factor '--'
        ;
