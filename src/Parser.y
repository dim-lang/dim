%{
#include "config/Header.h"
#include "config/Platform.h"
#include "Log.h"
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
    AstExpression *expression;
    AstStatement *statement;
    AstDeclaration *declaration;
    AstExpressionList *expressionList;
    AstStatementList *statementList;
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
%token <token> FT_BIT_AND_ASSIGN FT_BIT_OR_ASSIGN FT_BIT_XOR_ASSIGN FT_BIT_LSHIFT_ASSIGN FT_BIT_RSHIFT_ASSIGN FT_BIT_ZERORSHIFT_ASSIGN
%token <token> FT_EQ FT_NEQ FT_LT FT_LE FT_GT FT_GE
%token <token> FT_LPAREN FT_RPAREN FT_LBRACKET FT_RBRACKET FT_LBRACE FT_RBRACE FT_COMMA FT_SEMI FT_QUESTION FT_COLON FT_DOT

/* union.ast */
%type <ast> unit

/* union.expression */
%type <expression> argument_expression postfix_expression primary_expression unary_expression binary_expression
%type <expression> conditional_expression assignment_expression
%type <expression> constant_expression function_argument variable_declaration_name variable_declaration_expression
%type <expression> expression

/* union.statement */
%type <statement> compound_statement expression_statement selection_statement iteration_statement jump_statement 
%type <statement> statement

/* union.declaration */
%type <declaration> declaration variable_declaration function_declaration

/* union.expressionList */
%type <expressionList> argument_expression_list function_argument_list variable_declaration_name_list variable_declaration_expression_list

/* union.statementList */
%type <statementList> statement_list

/* operator/comparator precedence */
/* assignment */
%left FT_COMMA
%right FT_BIT_AND_ASSIGN FT_BIT_OR_ASSIGN FT_BIT_XOR_ASSIGN
%right FT_BIT_LSHIFT_ASSIGN FT_BIT_RSHIFT_ASSIGN FT_BIT_ZERORSHIFT_ASSIGN
%right FT_MUL_ASSIGN FT_DIV_ASSIGN FT_MOD_ASSIGN
%right FT_ADD_ASSIGN FT_SUB_ASSIGN
%right FT_ASSIGN
/* conditional */
%right FT_QUESTION FT_COLON
/* comparison */
%left FT_LOGIC_OR
%left FT_LOGIC_AND
%left FT_BIT_OR
%left FT_BIT_XOR
%left FT_BIT_AND
%left FT_EQ FT_NEQ
%left FT_LT FT_LE FT_GT FT_GE
/* calculator */
%left FT_BIT_LSHIFT FT_BIT_RSHIFT FT_BIT_ZERORSHIFT
%left FT_ADD FT_SUB
%left FT_MUL FT_DIV FT_MOD
/* other */
%left FT_LOGIC_NOT FT_BIT_NOT FT_BIT_COMPLEMENT
%left FT_DOT
%left FT_LPAREN FT_RPAREN FT_LBRACKET FT_RBRACKET

%start unit

%%

primary_expression : FT_IDENTIFIER { $$ = new AstIdentifierConstant($1); std::free($1); FINFO("primary_expression: {}", $$->toString()); }
                   | FT_INTEGER { $$ = new AstIntegerConstant(std::stoll($1)); std::free($1); FINFO("primary_expression: {}", $$->toString()); }
                   | FT_DOUBLE { $$ = new AstDoubleConstant(std::stod($1)); std::free($1); FINFO("primary_expression: {}", $$->toString()); }
                   | FT_STRING { $$ = new AstStringConstant($1); std::free($1); FINFO("primary_expression: {}", $$->toString()); }
                   | '(' expression ')' { $$ = $2; FINFO("primary_expression: {}", $$->toString()); }
                   ;

postfix_expression : primary_expression { $$ = $1; FINFO("postfix_expression: {}", $$->toString()); }
                   /*| postfix_expression '[' expression ']'*/
                   | FT_IDENTIFIER '(' ')' { $$ = new AstFunctionCallExpression($1, nullptr); std::free($1); FINFO("postfix_expression: {}", $$->toString()); }
                   | FT_IDENTIFIER '(' argument_expression_list ')' { $$ = new AstFunctionCallExpression($1, $3); std::free($1); FINFO("postfix_expression: {}", $$->toString()); }
                   /*| postfix_expression '.' IDENTIFIER*/
                   ;

argument_expression_list : argument_expression { $$ = new AstExpressionList(); $$->push_back($1); FINFO("argument_expression_list: {}", Ast::dump($$)); }
                         | argument_expression_list ',' argument_expression { $$->push_back($3); FINFO("argument_expression_list: {}", Ast::dump($$)); }
                         ;

argument_expression : constant_expression { $$ = $1; FINFO("argument_expression: {}", $$ ? $$->toString() : "null"); }
                    ;

unary_expression : postfix_expression { $$ = $1; FINFO("unary_expression: {}", $$ ? $$->toString() : "null"); }
                 | FT_BIT_AND postfix_expression { $$ = new AstUnaryExpression(FT_BIT_AND, $2); FINFO("unary_expression: {}", $$->toString()); }
                 | FT_BIT_OR postfix_expression { $$ = new AstUnaryExpression(FT_BIT_OR, $2); FINFO("unary_expression: {}", $$->toString()); }
                 | FT_BIT_NOT postfix_expression { $$ = new AstUnaryExpression(FT_BIT_NOT, $2); FINFO("unary_expression: {}", $$->toString()); }
                 | FT_BIT_COMPLEMENT postfix_expression { $$ = new AstUnaryExpression(FT_BIT_COMPLEMENT, $2); FINFO("unary_expression: {}", $$->toString()); }
                 | FT_BIT_XOR postfix_expression { $$ = new AstUnaryExpression(FT_BIT_XOR, $2); FINFO("unary_expression: {}", $$->toString()); }
                 | FT_ADD postfix_expression { $$ = new AstUnaryExpression(FT_ADD, $2); FINFO("unary_expression: {}", $$->toString()); }
                 | FT_SUB postfix_expression { $$ = new AstUnaryExpression(FT_SUB, $2); FINFO("unary_expression: {}", $$->toString()); }
                 ;

/*
cast_expression : unary_expression
                | '(' type_name ')' cast_expression
                ;
*/

binary_expression : unary_expression
                  | binary_expression FT_ADD unary_expression
                  | binary_expression FT_SUB unary_expression
                  | binary_expression FT_MUL unary_expression
                  | binary_expression FT_DIV unary_expression
                  | binary_expression FT_MOD unary_expression
                  | binary_expression FT_BIT_NOT unary_expression
                  | binary_expression FT_BIT_AND unary_expression
                  | binary_expression FT_BIT_OR unary_expression
                  | binary_expression FT_BIT_XOR unary_expression
                  | binary_expression FT_BIT_COMPLEMENT unary_expression
                  | binary_expression FT_BIT_LSHIFT unary_expression
                  | binary_expression FT_BIT_RSHIFT unary_expression
                  | binary_expression FT_BIT_ZERORSHIFT unary_expression
                  | binary_expression FT_LT unary_expression
                  | binary_expression FT_LE unary_expression
                  | binary_expression FT_GT unary_expression
                  | binary_expression FT_GE unary_expression
                  | binary_expression FT_EQ unary_expression
                  | binary_expression FT_NEQ unary_expression
                  | binary_expression FT_LOGIC_AND unary_expression
                  | binary_expression FT_LOGIC_OR unary_expression
                  | binary_expression FT_LOGIC_NOT unary_expression
                  ;

conditional_expression : binary_expression { $$ = $1; }
                       | binary_expression FT_QUESTION binary_expression FT_COLON binary_expression { $$ = new AstConditionalExpression($1, $3, $5); }
                       ;

 /* constant_expression is the expression entry */
constant_expression : conditional_expression { $$ = $1; }
                    ;


assignment_expression : conditional_expression { $$ = $1; }
                      | unary_expression FT_ASSIGN conditional_expression { $$ = new AstAssignmentExpression($1, FT_ASSIGN, $3); }
                      | unary_expression FT_MUL_ASSIGN conditional_expression { $$ = new AstAssignmentExpression($1, FT_MUL_ASSIGN, $3); }
                      | unary_expression FT_DIV_ASSIGN conditional_expression { $$ = new AstAssignmentExpression($1, FT_DIV_ASSIGN, $3); }
                      | unary_expression FT_MOD_ASSIGN conditional_expression { $$ = new AstAssignmentExpression($1, FT_MOD_ASSIGN, $3); }
                      | unary_expression FT_ADD_ASSIGN conditional_expression { $$ = new AstAssignmentExpression($1, FT_ADD_ASSIGN, $3); }
                      | unary_expression FT_SUB_ASSIGN conditional_expression { $$ = new AstAssignmentExpression($1, FT_SUB_ASSIGN, $3); }
                      | unary_expression FT_BIT_AND_ASSIGN conditional_expression { $$ = new AstAssignmentExpression($1, FT_BIT_AND_ASSIGN, $3); }
                      | unary_expression FT_BIT_OR_ASSIGN conditional_expression { $$ = new AstAssignmentExpression($1, FT_BIT_OR_ASSIGN, $3); }
                      | unary_expression FT_BIT_LSHIFT_ASSIGN conditional_expression { $$ = new AstAssignmentExpression($1, FT_BIT_LSHIFT_ASSIGN, $3); }
                      | unary_expression FT_BIT_RSHIFT_ASSIGN conditional_expression { $$ = new AstAssignmentExpression($1, FT_BIT_RSHIFT_ASSIGN, $3); }
                      | unary_expression FT_BIT_ZERORSHIFT_ASSIGN conditional_expression { $$ = new AstAssignmentExpression($1, FT_BIT_ZERORSHIFT_ASSIGN, $3); }
                      ;

expression : /* nothing */ { $$ = nullptr; }
           | assignment_expression { $$ = $1; }
           ;

unit : declaration {}
     | unit declaration {}
     ;

declaration : function_declaration { $$ = $1; }
            | variable_declaration { $$ = $1; }
            ;

 /**
  * let x = 1; 
  * let x, y, z = 1, "hello world", 2.3; 
  */
variable_declaration : FT_LET variable_declaration_name_list FT_ASSIGN variable_declaration_expression_list FT_SEMI { $$ = new AstVariableDeclaration($2, $4); }
                     ;

 /* variable_declaration left hand */
variable_declaration_name_list : variable_declaration_name { $$ = new AstExpressionList(); $$->push_back($1); }
                               | variable_declaration_name_list FT_COMMA variable_declaration_name { $$->push_back($3); }
                               ;

variable_declaration_name : FT_IDENTIFIER { $$ = new AstIdentifierConstant($1); std::free($1); }
                          ;

 /* variable_declaration right hand */
variable_declaration_expression_list : variable_declaration_expression { $$ = new AstExpressionList(); $$->push_back($1); }
                                     | variable_declaration_expression_list FT_COMMA variable_declaration_expression { $$->push_back($3); }
                                     ;

variable_declaration_expression : constant_expression { $$ = $1; }
                                ;

 /**
  * let min() = return 0;
  * let max() = { return 100 }
  * let abs(x) = return x > 0 ? x : -x; 
  * let abs(x) = { if (x > 0) { return x; } else { return -x; } }
  */
function_declaration : FT_LET FT_IDENTIFIER FT_LPAREN function_argument_list FT_RPAREN FT_ASSIGN statement {
                            AstStatementList *sl = new AstStatementList();
                            sl->push_back($7);
                            AstCompoundStatement *cs = new AstCompoundStatement(sl);
                            $$ = new AstFunctionDeclaration($2, $4, cs); 
                            std::free($2);
                        }
                     | FT_LET FT_IDENTIFIER FT_LPAREN FT_RPAREN FT_ASSIGN statement {
                            AstStatementList *sl = new AstStatementList();
                            sl->push_back($6);
                            AstCompoundStatement *cs = new AstCompoundStatement(sl);
                            $$ = new AstFunctionDeclaration($2, nullptr, cs);
                            std::free($2);
                        }
                     | FT_LET FT_IDENTIFIER FT_LPAREN function_argument_list FT_RPAREN FT_ASSIGN compound_statement { $$ = new AstFunctionDeclaration($2, $4, $7); std::free($2); }
                     | FT_LET FT_IDENTIFIER FT_LPAREN FT_RPAREN FT_ASSIGN compound_statement { $$ = new AstFunctionDeclaration($2, nullptr, $6); std::free($2); }
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
          | function_declaration { $$ = $1; }
          ;

expression_statement : FT_SEMI { $$ = nullptr; }
                     | expression FT_SEMI { $$ = new AstExpressionStatement($1); }
                     ;

selection_statement : FT_IF FT_LPAREN expression FT_RPAREN statement { $$ = new AstIfStatement($3, $5, nullptr); }
                    | FT_IF FT_LPAREN expression FT_RPAREN statement FT_ELSE statement { $$ = new AstIfStatement($3, $5, $7); }
                    /*| FT_SWITCH FT_LPAREN expression FT_RPAREN statement*/
                    ;

iteration_statement : FT_WHILE FT_LPAREN expression FT_RPAREN statement { $$ = new AstWhileStatement($3, $5); }
                    /*| FT_FOR FT_LPAREN expression_statement expression_statement FT_RPAREN statement { $$ = new AstForStatement($3, $4, nullptr, $6); }*/
                    | FT_FOR FT_LPAREN expression_statement expression_statement expression FT_RPAREN statement { $$ = new AstForStatement($3, $4, $5, $7); }
                    ;

jump_statement : FT_CONTINUE FT_SEMI { $$ = new AstContinueStatement(); }
               | FT_BREAK FT_SEMI { $$ = new AstBreakStatement(); }
               | FT_RETURN FT_SEMI { $$ = new AstReturnStatement(nullptr); }
               | FT_RETURN expression FT_SEMI { $$ = new AstReturnStatement($2); }
               ;

%%
