%{
#include <cstdio>
#include "Ast.h"
extern FILE *yyin;
extern int yylex();
void yyerror(const char *s) { printf("yyerror: %s\n", s); }
%}

/* Represents the many different ways we can access our data */
%union {
    char *literal;
    int token;
}

/* union.token: eof, keyword */
%token <token> FT_EOF
%token <token> FT_TRUE FT_FALSE FT_LET FT_NIL FT_IF FT_ELSEIF FT_ELSE FT_FOR FT_WHILE FT_BREAK FT_CONTINUE FT_SWITCH FT_CASE FT_OTHERWISE
%token <token> FT_FUNC FT_CLASS FT_TYPE FT_ISINSTANCE FT_IMPORT FT_RETURN FT_VOID FT_LOGICALAND FT_LOGICALOR FT_LOGICALNOT
%token <token> FT_INTEGER_KEYWORD FT_UNSIGNED_INTEGER_KEYWORD FT_DOUBLE_KEYWORD

/* union.literal, identifier, integer, double number, string */
%token <literal> FT_IDENTIFIER FT_INTEGER FT_DOUBLE FT_STRING

/* union.token: operator, comparator, punctuation */
%token <token> FT_ADD FT_SUB FT_MUL FT_DIV FT_MOD FT_BITNOT FT_BITAND FT_BITOR FT_BITCOMPLEMENT FT_BITXOR
%token <token> FT_ASSIGN FT_ADDASSIGN FT_SUBASSIGN FT_MULASSIGN FT_DIVASSIGN FT_MODASSIGN FT_EQ FT_NEQ FT_LT FT_LE FT_GT FT_GE
%token <token> FT_LPAREN FT_RPAREN FT_LBRACKET FT_RBRACKET FT_LBRACE FT_RBRACE FT_COMMA FT_SEMI FT_QUESTION FT_COLON FT_DOT

/*
%type <ident> ident
%type <expr> numeric expr
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <block> program stmts block
%type <stmt> stmt var_decl func_decl
%type <token> comparison
*/

/* operator/comparator precedence */
%left FT_DOT FT_LPAREN FT_RPAREN FT_LBRACKET FT_RBRACKET
%left FT_MUL FT_DIV FT_MOD
%left FT_ADD FT_SUB
%left FT_LT FT_LE FT_GT FT_GE FT_EQ FT_NEQ
%left FT_BITNOT FT_BITAND FT_BITOR FT_BITXOR FT_BITCOMPLEMENT
%left FT_LOGICALNOT FT_LOGICALAND FT_LOGICALOR
%left FT_ASSIGN FT_ADDASSIGN FT_SUBASSIGN FT_MULASSIGN FT_DIVASSIGN FT_MODASSIGN

/*
%start program
*/

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
                 | unary_operator postfix_expression
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
                      | relational_expression FT_LE additive_expression
                      | relational_expression FT_GT additive_expression
                      | relational_expression FT_GE additive_expression
                      ;

equality_expression : relational_expression
                    | equality_expression FT_EQ relational_expression
                    | equality_expression FT_NEQ relational_expression
                    ;

/*
bit_and_expression : equality_expression
                  | bitand_expression '&' equality_expression
                  ;

bit_xor_expression : bit_and_expression
                   | bit_xor_expression '^' bit_and_expression
                   ;

bit_or_expression : bit_xor_expression
                  | bit_or_expression '|' bit_xor_expression
                  ;
*/

logical_not_expression : equality_expression
                       | logical_not_expression FT_LOGICALNOT equality_expression
                       ;

logical_and_expression : logical_not_expression
                       | logical_and_expression FT_LOGICALAND logical_not_expression
                       ;

logical_or_expression : logical_and_expression
                      | logical_or_expression FT_LOGICALOR logical_and_expression
                      ;

assignment_expression : logical_or_expression
                      | unary_expression assignment_operator assignment_expression
                      ;

assignment_operator : FT_ASSIGN
                    | FT_MULASSIGN
                    | FT_DIVASSIGN
                    | FT_MODASSIGN
                    | FT_ADDASSIGN
                    | FT_SUBASSIGN
                    ;

constant_expression : logical_or_expression
                    ;

expression : assignment_expression
           ;

unit : external_declaration
     | unit external_declaration
     ;

external_declaration : function_declaration
                     | declaration
                     ;

declaration : FT_LET declaration_init_list FT_SEMI
            ;

declaration_init_list : declaration_init
                      | declaration_init_list declaration_init
                      ;

declaration_init : FT_IDENTIFIER FT_ASSIGN constant_expression
                 ;

function_declaration : FT_FUNC FT_IDENTIFIER FT_LPAREN function_arg_list FT_RPAREN compound_statement
                     | FT_FUNC FT_IDENTIFIER FT_LPAREN FT_RPAREN compound_statement
                     ;

function_arg_list : function_arg
                  | function_arg_list FT_COMMA function_arg
                  ;

function_arg : FT_IDENTIFIER
             ;

compound_statement : FT_LBRACE FT_RBRACE
                   | FT_LBRACE statement_list FT_RBRACE
                   | FT_LBRACE declaration_list FT_RBRACE
                   ;

statement_list : statement
               | statement_list statement
               ;

declaration_list : declaration
                 | declaration_list declaration
                 ;

statement : compound_statement
          | expression_statement
          | selection_statement
          | iteration_statement
          | jump_statement
          ;

expression_statement : FT_SEMI
                     | expression FT_SEMI
                     ;

selection_statement : FT_IF FT_LPAREN expression FT_RPAREN statement
                    | FT_IF FT_LPAREN expression FT_RPAREN statement FT_ELSE statement
                    | FT_SWITCH FT_LPAREN expression FT_RPAREN statement
                    ;

iteration_statement : FT_WHILE FT_LPAREN expression FT_RPAREN statement
                    | FT_FOR FT_LPAREN expression_statement expression_statement FT_RPAREN statement
                    | FT_FOR FT_LPAREN expression_statement expression_statement expression FT_RPAREN statement
                    ;

jump_statement : FT_CONTINUE FT_SEMI
               | FT_BREAK FT_SEMI
               | FT_RETURN FT_SEMI
               | FT_RETURN expression FT_SEMI
               ;

%%
