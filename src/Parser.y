%{
#include "config/Platform.h"
#include "Logging.h"
#include "Ast.h"
#include "Scope.h"
#include "Token.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstdint>
#include <cctype>
void yyerror(const char *s) { printf("yyerror: %s\n", s); }
%}

/* Represents the many different ways we can access our data */
%union {
    Ast *ast;
    AstExpressionList *expressions;
    AstStatementList *statements;
    char *literal;
    int token;
}

/* union.token: eof, keyword */
%token <token> FT_EOF
%token <token> FT_TRUE FT_FALSE FT_LET FT_NIL FT_IF FT_ELSEIF FT_ELSE FT_FOR FT_WHILE FT_BREAK FT_CONTINUE FT_SWITCH FT_CASE FT_OTHERWISE
%token <token> FT_FUNC FT_CLASS FT_TYPE FT_ISINSTANCE FT_IMPORT FT_RETURN FT_VOID FT_LOGIC_AND FT_LOGIC_OR FT_LOGIC_NOT
%token <token> FT_INTEGER_KEYWORD FT_UNSIGNED_INTEGER_KEYWORD FT_DOUBLE_KEYWORD

/* union.literal, identifier, integer, double number, string */
%token <literal> FT_IDENTIFIER FT_INTEGER FT_DOUBLE FT_STRING

/* union.token: operator, comparator, punctuation */
%token <token> FT_ADD FT_SUB FT_MUL FT_DIV FT_MOD FT_BIT_NOT FT_BIT_AND FT_BIT_OR FT_BIT_COMPLEMENT FT_BIT_XOR FT_BIT_LSHIFT FT_BIT_RSHIFT FT_BIT_ZERORSHIFT
%token <token> FT_ASSIGN FT_ADD_ASSIGN FT_SUB_ASSIGN FT_MUL_ASSIGN FT_DIV_ASSIGN FT_MOD_ASSIGN
%token <token> FT_BIT_AND_ASSIGN FT_BIT_OR_ASSIGN FT_BIT_XOR_ASSIGN FT_BILSHIFT_ASSIGN FT_BIT_LSHIFT_ASSIGN FT_BIT_RSHIFT_ASSIGN FT_BIT_ZERORSHIFT_ASSIGN
%token <token> FT_EQ FT_NEQ FT_LT FT_LE FT_GT FT_GE
%token <token> FT_LPAREN FT_RPAREN FT_LBRACKET FT_RBRACKET FT_LBRACE FT_RBRACE FT_COMMA FT_SEMI FT_QUESTION FT_COLON FT_DOT

/*
%type <expr> numeric expr
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <block> program stmts block
%type <stmt> stmt var_decl func_decl
%type <token> comparison
*/

%type <ast> primary_expression postfix_expression unary_expression multiplicative_expression additive_expression relational_expression 
%type <ast> equality_expression logical_and_expression logical_or_expression logical_not_expression
%type <ast> argument_expression conditional_expression assignment_expression constant_expression function_argument
%type <ast> variable_declaration function_declaration
%type <ast> expression
%type <ast> compound_statement expression_statement selection_statement iteration_statement jump_statement
%type <ast> statement
%type <ast> unit declaration

%type <expressions> argument_expression_list function_argument_list
%type <statements> statement_list

/* operator/comparator precedence */

%left FT_ASSIGN FT_ADD_ASSIGN FT_SUB_ASSIGN FT_MUL_ASSIGN FT_DIV_ASSIGN FT_MOD_ASSIGN
%left FT_LOGIC_NOT FT_LOGIC_AND FT_LOGIC_OR
%left FT_BIT_NOT FT_BIT_AND FT_BIT_OR FT_BIT_XOR FT_BIT_COMPLEMENT
%left FT_LT FT_LE FT_GT FT_GE FT_EQ FT_NEQ
%left FT_ADD FT_SUB
%left FT_MUL FT_DIV FT_MOD
%left FT_DOT FT_LPAREN FT_RPAREN FT_LBRACKET FT_RBRACKET

%start unit

/*
%start program
*/

%%

primary_expression : FT_IDENTIFIER { $$ = new AstIdentifierConstant($1); std::free($1); }
                   | FT_INTEGER { $$ = new AstIntegerConstant(std::stoll($1)); std::free($1); }
                   | FT_DOUBLE { $$ = new AstDoubleConstant(std::stod($1)); std::free($1); }
                   | FT_STRING { $$ = new AstStringConstant($1); std::free($1); }
                   | '(' expression ')' { $$ = $2; }
                   ;

postfix_expression : primary_expression { $$ = $1; }
                   /*| postfix_expression '[' expression ']'*/
                   | FT_IDENTIFIER '(' ')' { $$ = new AstFunctionCallExpression($1, nullptr); std::free($1); }
                   | FT_IDENTIFIER '(' argument_expression_list ')' { $$ = new AstFunctionCallExpression($1, $3); std::free($1); }
                   /*| postfix_expression '.' IDENTIFIER*/
                   ;

argument_expression_list : argument_expression { $$ = new AstExpressionList(); $$->push_back($1); }
                         | argument_expression_list ',' argument_expression { $$->push_back($3); }
                         ;

argument_expression : conditional_expression { $$ = $1; }
                    ;

unary_expression : postfix_expression { $$ = $1; }
                 | FT_BIT_AND postfix_expression { $$ = new AstUnaryExpression(FT_BIT_AND, $2); }
                 | FT_BIT_OR postfix_expression { $$ = new AstUnaryExpression(FT_BIT_OR, $2); }
                 | FT_BIT_NOT postfix_expression { $$ = new AstUnaryExpression(FT_BIT_NOT, $2); }
                 | FT_BIT_COMPLEMENT postfix_expression { $$ = new AstUnaryExpression(FT_BIT_COMPLEMENT, $2); }
                 | FT_BIT_XOR postfix_expression { $$ = new AstUnaryExpression(FT_BIT_XOR, $2); }
                 | FT_ADD postfix_expression { $$ = new AstUnaryExpression(FT_ADD, $2); }
                 | FT_SUB postfix_expression { $$ = new AstUnaryExpression(FT_SUB, $2); }
                 ;

/*
cast_expression : unary_expression
                | '(' type_name ')' cast_expression
                ;
*/

multiplicative_expression : unary_expression { $$ = $1; }
                          | multiplicative_expression FT_MUL unary_expression { $$ = new AstBinaryExpression($1, FT_MUL, $3); }
                          | multiplicative_expression FT_DIV unary_expression { $$ = new AstBinaryExpression($1, FT_DIV, $3); }
                          | multiplicative_expression FT_MOD unary_expression { $$ = new AstBinaryExpression($1, FT_MOD, $3); }
                          ;

additive_expression : multiplicative_expression { $$ = $1; }
                    | additive_expression FT_ADD multiplicative_expression { $$ = new AstBinaryExpression($1, FT_ADD, $3); }
                    | additive_expression FT_SUB multiplicative_expression { $$ = new AstBinaryExpression($1, FT_SUB, $3); }
                    ;

/*
shift_expression : additive_expression
                 | shift_expression '<<' additive_expression
                 | shift_expression '>>' additive_expression
                 ;
*/

relational_expression : additive_expression { $$ = $1; }
                      | relational_expression FT_LT additive_expression { $$ = new AstBinaryExpression($1, FT_LT, $3); }
                      | relational_expression FT_LE additive_expression { $$ = new AstBinaryExpression($1, FT_LE, $3); }
                      | relational_expression FT_GT additive_expression { $$ = new AstBinaryExpression($1, FT_GT, $3); }
                      | relational_expression FT_GE additive_expression { $$ = new AstBinaryExpression($1, FT_GE, $3); }
                      ;

equality_expression : relational_expression { $$ = $1; }
                    | equality_expression FT_EQ relational_expression { $$ = new AstBinaryExpression($1, FT_EQ, $3); }
                    | equality_expression FT_NEQ relational_expression { $$ = new AstBinaryExpression($1, FT_NEQ, $3); }
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

logical_not_expression : equality_expression { $$ = $1; }
                       | FT_LOGIC_NOT equality_expression { $$ = new AstUnaryExpression(FT_LOGIC_NOT, $2); }
                       ;

logical_and_expression : logical_not_expression { $$ = $1; }
                       | logical_and_expression FT_LOGIC_AND logical_not_expression { $$ = new AstBinaryExpression($1, FT_LOGIC_AND, $3); }
                       ;

logical_or_expression : logical_and_expression { $$ = $1; }
                      | logical_or_expression FT_LOGIC_OR logical_and_expression { $$ = new AstBinaryExpression($1, FT_LOGIC_OR, $3); }
                      ;

/* conditional_expression is the expression entry */
conditional_expression : logical_or_expression { $$ = $1; }
                       | logical_or_expression '?' expression ':' conditional_expression { $$ = new AstConditionalExpression($1, $3, $5); }
                       ;

assignment_expression : conditional_expression { $$ = $1; }
                      | unary_expression FT_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, FT_ASSIGN, $3); }
                      | unary_expression FT_MUL_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, FT_MUL_ASSIGN, $3); }
                      | unary_expression FT_DIV_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, FT_DIV_ASSIGN, $3); }
                      | unary_expression FT_MOD_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, FT_MOD_ASSIGN, $3); }
                      | unary_expression FT_ADD_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, FT_ADD_ASSIGN, $3); }
                      | unary_expression FT_SUB_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, FT_SUB_ASSIGN, $3); }
                      | unary_expression FT_BIT_AND_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, FT_BIT_AND_ASSIGN, $3); }
                      | unary_expression FT_BIT_OR_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, FT_BIT_OR_ASSIGN, $3); }
                      | unary_expression FT_BIT_LSHIFT_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, FT_BIT_LSHIFT_ASSIGN, $3); }
                      | unary_expression FT_BIT_RSHIFT_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, FT_BIT_RSHIFT__ASSIGN, $3); }
                      | unary_expression FT_BIT_ZERORSHIFT_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, FT_BIT_ZERORSHIFT_ASSIGN, $3); }
                      ;

constant_expression : conditional_expression { $$ = $1; }
                    ;

expression : assignment_expression { $$ = $1; }
           ;

unit : declaration { Scope::define(std::shared_ptr<Ast>($1)); }
     | unit declaration { Scope::define(std::shared_ptr<Ast>($2)); }
     ;

declaration : function_declaration { $$ = $1; }
            | variable_declaration { $$ = $1; }
            ;

variable_declaration : FT_LET FT_IDENTIFIER FT_ASSIGN constant_expression FT_SEMI { $$ = new AstVariableDeclarations($2, $4); std::free($2); }
                     ;

function_declaration : FT_FUNC FT_IDENTIFIER FT_LPAREN function_argument_list FT_RPAREN compound_statement { $$ = new AstFunctionDeclaration($2, $4, $6); std::free($2); }
                     | FT_FUNC FT_IDENTIFIER FT_LPAREN FT_RPAREN compound_statement { $$ = new AstFunctionDeclaration($2, nullptr, $5); std::free($2); }
                     ;

function_argument_list : function_argument { $$ = new AstExpressionList(); $$->push_back($1); }
                       | function_argument_list FT_COMMA function_argument { $$->push_back($3); }
                       ;

function_argument : FT_IDENTIFIER { $$ = new AstIdentifierConstant($1); std::free($1); }
                  ;

compound_statement : FT_LBRACE FT_RBRACE { $$ = new AstCompoundStatement(nullptr); }
                   | FT_LBRACE statement_list FT_RBRACE { $$ = new AstCompoundStatement($2); }
                   ;

statement_list : statement { $$ = new AstStatementList(); $$->push_back($1); }
               | statement_list statement { $$->push_back($2); }
               ;

statement : compound_statement { $$ = $1; }
          | expression_statement { $$ = $1; }
          | selection_statement { $$ = $1; }
          | iteration_statement { $$ = $1; }
          | jump_statement { $$ = $1; }
          | variable_declaration { $$ = $1; }
          ;

expression_statement : FT_SEMI { $$ = nullptr; }
                     | expression FT_SEMI { $$ = new AstExpressionStatement($2); }
                     ;

selection_statement : FT_IF FT_LPAREN expression FT_RPAREN statement { $$ = new AstIfStatement($3, $5, nullptr); }
                    | FT_IF FT_LPAREN expression FT_RPAREN statement FT_ELSE statement { $$ = new AstIfStatement($3, $5, $7); }
                    /*| FT_SWITCH FT_LPAREN expression FT_RPAREN statement*/
                    ;

iteration_statement : FT_WHILE FT_LPAREN expression FT_RPAREN statement { $$ = new AstWhileStatement($3, $5); }
                    | FT_FOR FT_LPAREN expression_statement expression_statement FT_RPAREN statement { $$ = new AstForStatement($3, $4, nullptr, $6); }
                    | FT_FOR FT_LPAREN expression_statement expression_statement expression FT_RPAREN statement { $$ = new AstForStatement($3, $4, $5, $7); }
                    ;

jump_statement : FT_CONTINUE FT_SEMI { $$ = new AstContinueStatement(); }
               | FT_BREAK FT_SEMI { $$ = new AstBreakStatement(); }
               | FT_RETURN FT_SEMI { $$ = new AstReturnStatement(nullptr); }
               | FT_RETURN expression FT_SEMI { $$ = new AstReturnStatement($2); }
               ;

%%
