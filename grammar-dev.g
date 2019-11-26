grammar fastype;

program :   statement_list
        ;

declaration :   variable_declaration
            |   function_declaration
            |   class_declaration
            ;

variable_declaration:   'let' ( variable '=' expr )+ ';'
                    ;

statement_list  :   statement*
                ;

statement   :   compound_statement
            |   assignment_statement
            |   empty_statement
            ;

compound_statement  : '{' statement_list '}'
                    ;

assignment_statement:   variable '=' expr ';'
                    ;


empty_statement     :   ';'
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
