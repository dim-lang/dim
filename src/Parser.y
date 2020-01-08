%{
extern int yylex();
void yyerror(const char *s) { printf("yyerror: %s\n", s); }
%}

/* Represents the many different ways we can access our data */
%union {
    Node *node;
    NBlock *block;
    NExpression *expr;
    NStatement *stmt;
    NIdentifier *ident;
    NVariableDeclaration *var_decl;
    std::vector<NVariableDeclaration*> *varvec;
    std::vector<NExpression*> *exprvec;

    std::string literal;
    int token;
}

/* union.token: eof, keyword */
%token <token> FT_EOF
%token <token> FT_TRUE FT_FALSE FT_LET FT_NIL FT_IF FT_ELSEIF FT_ELSE FT_FOR FT_WHILE FT_BREAK FT_CONTINUE FT_FUNC FT_CLASS
%token <token> FT_TYPE FT_ISINSTANCE FT_IMPORT FT_RETURN FT_VOID FT_LOGICALAND FT_LOGICALOR FT_LOGICALNOT 
%token <token> FT_INTEGER_KEYWORD FT_UNSIGNED_INTEGER_KEYWORD FT_DOUBLE_KEYWORD

/* union.literal, identifier, integer, double number, string */
%token <literal> FT_IDENTIFIER FT_INTEGER FT_DOUBLE FT_STRING

/* union.token: operator, comparator, punctuation */
%token <token> FT_ADD FT_SUB FT_MUL FT_DIV FT_MOD FT_BITNOT FT_BITAND FT_BITOR FT_BITCOMPLEMENT FT_BITXOR
%token <token> FT_ASSIGN FT_ADDASSIGN FT_SUBASSIGN FT_MULASSIGN FT_DIVASSIGN FT_MODASSIGN FT_EQ FT_NEQ FT_LT FT_LE FT_GT FT_GE
%token <token> FT_LPAREN FT_RPAREN FT_LBRACKET FT_RBRACKET FT_LBRACE FT_RBRACE FT_COMMA FT_SEMI FT_QUESTION FT_COLON FT_DOT


%type <ident> ident
%type <expr> numeric expr
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <block> program stmts block
%type <stmt> stmt var_decl func_decl
%type <token> comparison

/* operator/comparator precedence */
%left FT_DOT FT_LPAREN FT_RPAREN FT_LBRACKET FT_RBRACKET
%left FT_MUL FT_DIV FT_MOD
%left FT_ADD FT_SUB
%left FT_LT FT_LE FT_GT FT_GE FT_EQ FT_NEQ
%left FT_BITNOT FT_BITAND FT_BITOR FT_BITXOR FT_BITCOMPLEMENT
%left FT_LOGICALNOT FT_LOGICALAND FT_LOGICALOR
%left FT_ASSIGN FT_ADDASSIGN FT_SUBASSIGN FT_MULASSIGN FT_DIVASSIGN FT_MODASSIGN

%start program

%%

primary_expression : FT_IDENTIFIER
                   | FT_INTEGER
                   | FT_DOUBLE
                   | FT_STRING
                   | '(' expression ')'
                   ;

postfix_expression : primary_expression
                   /*| postfix_expression '[' expression ']'*/
                   | postfix_expression '(' ')'
                   | postfix_expression '(' argument_expression_list ')'
                   /*| postfix_expression '.' IDENTIFIER*/
                   ;

argument_expression_list : assignment_expression
                         | argument_expression_list ',' assignment_expression
                         ;

unary_expression : postfix_expression
                 | unary_operator cast_expression
                 ;

unary_operator : FT_BITAND
               | FT_BITOR
               | FT_BITNOT
               | FT_BITCOMPLEMENT
               | FT_BITXOR
               | FT_ADD
               | FT_SUB
               ;

/*
cast_expression : unary_expression
                | '(' type_name ')' cast_expression
                ;
*/

multiplicative_expression : unary_expression
                          | multiplicative_expression FT_MUL unary_expression
                          | multiplicative_expression FT_DIV unary_expression
                          | multiplicative_expression FT_MOD unary_expression
                          ;
additive_expression : multiplicative_expression
                    | additive_expression FT_ADD multiplicative_expression
                    | additive_expression FT_SUB multiplicative_expression
                    ;

/*
shift_expression : additive_expression
                 | shift_expression '<<' additive_expression
                 | shift_expression '>>' additive_expression
                 ;
*/

relational_expression : additive_expression
                      | relational_expression FT_LT additive_expression
                      | relational_expression FT_GT additive_expression
                      | relational_expression FT_LE additive_expression
                      | relational_expression FT_GE additive_expression
                      ;

type_name : FT_INTEGER_KEYWORD
          | FT_DOUBLE_KEYWORD
          | FT_UNSIGNED_INTEGER_KEYWORD
          ;

program : stmts { programBlock = $1; }
        ;

stmts : stmt { $$ = new NBlock(); $$->statements.push_back($<stmt>1); }
      | stmts stmt { $1->statements.push_back($<stmt>2); }
      ;

stmt : var_decl | func_decl
     | expr { $$ = new NExpressionStatement(*$1); }
     ;

block : FT_LBRACE stmts FT_RBRACE { $$ = $2; }
      | FT_LBRACE FT_RBRACE { $$ = new NBlock(); }
      ;

var_decl : ident ident { $$ = new NVariableDeclaration(*$1, *$2); }
         | ident ident FT_ASSIGN expr { $$ = new NVariableDeclaration(*$1, *$2, $4); }
         ;

func_decl : ident ident FT_LPAREN func_decl_args FT_RPAREN block 
            { $$ = new NFunctionDeclaration(*$1, *$2, *$4, *$6); delete $4; }
          ;

func_decl_args : /*blank*/  { $$ = new VariableList(); }
          | var_decl { $$ = new VariableList(); $$->push_back($<var_decl>1); }
          | func_decl_args FT_COMMA var_decl { $1->push_back($<var_decl>3); }
          ;

ident : FT_IDENTIFIER { $$ = new NIdentifier(*$1); delete $1; }
      ;

numeric : FT_INTEGER { $$ = new NInteger(atol($1->c_str())); delete $1; }
        | FT_DOUBLE { $$ = new NDouble(atof($1->c_str())); delete $1; }
        ;

expr : ident FT_ASSIGN expr { $$ = new NAssignment(*$<ident>1, *$3); }
     | ident FT_LPAREN call_args FT_RPAREN { $$ = new NMethodCall(*$1, *$3); delete $3; }
     | ident { $<ident>$ = $1; }
     | numeric
     | expr comparison expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
     | FT_LPAREN expr FT_RPAREN { $$ = $2; }
     ;

call_args : /*blank*/  { $$ = new ExpressionList(); }
          | expr { $$ = new ExpressionList(); $$->push_back($1); }
          | call_args FT_COMMA expr  { $1->push_back($3); }
          ;

comparison : FT_EQ | FT_NEQ | FT_LT | FT_LE | FT_GT | FT_GE
           | FT_ADD | FT_SUB | FT_MUL | FT_DIV
           ;


%%
