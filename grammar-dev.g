grammar fastype;


expr:   term
    |   term '+' term
    |   term '-' term
    ;

term:   factor
    |   factor '*' factor
    |   factor '/' factor
    |   factor '%' factor
    ;

factor: INTEGER
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
