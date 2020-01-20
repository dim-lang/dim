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
%token <token> T_EOF
%token <token> T_TRUE T_FALSE T_LET T_NIL T_IF T_ELSEIF T_ELSE T_FOR T_WHILE T_BREAK T_CONTINUE T_SWITCH T_CASE T_OTHERWISE
%token <token> T_FUNC T_CLASS T_TYPE T_ISINSTANCE T_IMPORT T_RETURN T_VOID T_LOGIC_AND T_LOGIC_OR T_LOGIC_NOT
%token <token> T_INTEGER_KEYWORD T_UNSIGNED_INTEGER_KEYWORD T_DOUBLE_KEYWORD

/* union.literal, identifier, integer, double number, string */
%token <literal> T_IDENTIFIER T_INTEGER T_DOUBLE T_STRING

/* union.token: operator, comparator, punctuation */
%token <token> T_ADD T_SUB T_MUL T_DIV T_MOD T_BIT_NOT T_BIT_AND T_BIT_OR T_BIT_COMPLEMENT T_BIT_XOR T_BIT_LSHIFT T_BIT_RSHIFT T_BIT_ZERORSHIFT
%token <token> T_ASSIGN T_ADD_ASSIGN T_SUB_ASSIGN T_MUL_ASSIGN T_DIV_ASSIGN T_MOD_ASSIGN
%token <token> T_BIT_AND_ASSIGN T_BIT_OR_ASSIGN T_BIT_XOR_ASSIGN T_BIT_LSHIT_ASSIGN T_BIT_RSHIT_ASSIGN T_BIT_ZERORSHIT_ASSIGN
%token <token> T_EQ T_NEQ T_LT T_LE T_GT T_GE
%token <token> T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET T_LBRACE T_RBRACE T_COMMA T_SEMI T_QUESTION T_COLON T_DOT

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
%left T_COMMA
%right T_BIT_AND_ASSIGN T_BIT_OR_ASSIGN T_BIT_XOR_ASSIGN
%right T_BIT_LSHIT_ASSIGN T_BIT_RSHIT_ASSIGN T_BIT_ZERORSHIT_ASSIGN
%right T_MUL_ASSIGN T_DIV_ASSIGN T_MOD_ASSIGN
%right T_ADD_ASSIGN T_SUB_ASSIGN
%right T_ASSIGN
/* conditional */
%right T_QUESTION T_COLON
/* comparison */
%left T_LOGIC_OR
%left T_LOGIC_AND
%left T_BIT_OR
%left T_BIT_XOR
%left T_BIT_AND
%left T_EQ T_NEQ
%left T_LT T_LE T_GT T_GE
/* calculator */
%left T_BIT_LSHIFT T_BIT_RSHIFT T_BIT_ZERORSHIFT
%left T_ADD T_SUB
%left T_MUL T_DIV T_MOD
/* other */
%left T_LOGIC_NOT T_BIT_NOT T_BIT_COMPLEMENT
%left T_DOT
%left T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET

%start translation_unit

%%

/* part-1 expression */

primary_expression : T_IDENTIFIER { $$ = new AstIdentifierConstant($1); std::free($1); FINFO("primary_expression: {}", $$->toString()); }
                   | T_INTEGER { $$ = new AstIntegerConstant(std::stoll($1)); std::free($1); FINFO("primary_expression: {}", $$->toString()); }
                   | T_DOUBLE { $$ = new AstDoubleConstant(std::stod($1)); std::free($1); FINFO("primary_expression: {}", $$->toString()); }
                   | T_STRING { $$ = new AstStringConstant($1); std::free($1); FINFO("primary_expression: {}", $$->toString()); }
                   | T_LPAREN expression T_RPAREN { $$ = $2; FINFO("primary_expression: {}", $$->toString()); }
                   ;

postfix_expression : primary_expression { $$ = $1; FINFO("postfix_expression: {}", $$->toString()); }
                   /*| postfix_expression '[' expression ']'*/
                   | postfix_expression T_LPAREN T_RPAREN { $$ = new AstFunctionCallExpression($1, nullptr); std::free($1); FINFO("postfix_expression: {}", $$->toString()); }
                   | postfix_expression T_LPAREN argument_expression_list T_RPAREN { $$ = new AstFunctionCallExpression($1, $3); std::free($1); FINFO("postfix_expression: {}", $$->toString()); }
                   /*| postfix_expression '.' T_IDENTIFIER */
                   ;

argument_expression_list : assignment_expression { $$ = new AstExpressionList(); $$->push_back($1); FINFO("argument_expression_list: {}", Ast::dump($$)); }
                         | argument_expression_list T_COMMA assignment_expression { $$->push_back($3); FINFO("argument_expression_list: {}", Ast::dump($$)); }
                         ;

unary_expression : postfix_expression { $$ = $1; FINFO("unary_expression: {}", $$ ? $$->toString() : "null"); }
                 | unary_operator unary_expression { $$ = new AstUnaryExpression(T_ADD, $2); FINFO("unary_expression: {}", $$->toString()); }
                 ;

unary_operator : T_MUL
               | T_DIV
               | T_MOD
               | T_ADD
               | T_SUB
               | T_BIT_NOT
               | T_BIT_COMPLEMENT
               ;

/*
cast_expression : unary_expression
                | '(' type_name ')' cast_expression
                ;
*/

multiplicative_expression : unary_expression
                          | multiplicative_expression T_MUL unary_expression
                          | multiplicative_expression T_DIV unary_expression
                          | multiplicative_expression T_MOD unary_expression
                          ;

additive_expression : multiplicative_expression
                    | additive_expression T_ADD multiplicative_expression
                    | additive_expression T_SUB multiplicative_expression
                    ;

shift_expression : additive_expression
                 | shift_expression T_BIT_LSHIFT additive_expression
                 | shift_expression T_BIT_RSHIFT additive_expression
                 | shift_expression T_BIT_ZERORSHIFT additive_expression
                 ;

relational_expression : shift_expression
                      | relational_expression T_LT shift_expression
                      | relational_expression T_LE shift_expression
                      | relational_expression T_GT shift_expression
                      | relational_expression T_GE shift_expression
                      ;

equality_expression : relational_expression
                    | equality_expression T_EQ relational_expression
                    | equality_expression T_NEQ relational_expression
                    ;

bit_and_expression : equality_expression
                   | bit_and_expression T_BIT_AND equality_expression
                   ;

bit_xor_expression : bit_and_expression
                   | bit_xor_expression T_BIT_XOR bit_and_expression
                   ;

bit_or_expression : bit_xor_expression
                  | bit_or_expression T_BIT_OR bit_xor_expression
                  ;

logical_and_expression : bit_or_expression
                       | logical_and_expression T_LOGIC_AND bit_or_expression
                       ;

logical_or_expression : logical_and_expression
                      | logical_or_expression T_LOGIC_OR logical_and_expression
                      ;

logical_or_expression : logical_and_expression
                      | logical_or_expression T_LOGIC_OR logical_and_expression
                      ;

conditional_expression : logical_or_expression { $$ = $1; }
                       | logical_or_expression T_QUESTION expression T_COLON conditional_expression { $$ = new AstConditionalExpression($1, $3, $5); }
                       ;

assignment_expression : conditional_expression { $$ = $1; }
                      | unary_expression assignment_operator assignment_expression { $$ = new AstAssignmentExpression($1, T_ASSIGN, $3); }
                      ;

assignment_operator : T_ASSIGN
                    | T_MUL_ASSIGN
                    | T_DIV_ASSIGN
                    | T_MOD_ASSIGN
                    | T_ADD_ASSIGN
                    | T_SUB_ASSIGN
                    | T_BIT_AND_ASSIGN
                    | T_BIT_OR_ASSIGN
                    | T_BIT_LSHIT_ASSIGN
                    | T_BIT_RSHIT_ASSIGN
                    | T_BIT_ZERORSHIT_ASSIGN
                    ;

expression : /* nothing */ { $$ = nullptr; }
           | assignment_expression { $$ = $1; }
           | expression T_COMMA assignment_expression
           ;

constant_expression : conditional_expression { $$ = $1; }
                    ;

/* part-2 declaration */

declaration : function_declaration { $$ = $1; }
            | variable_declaration { $$ = $1; }
            ;

 /**
  * let x = 1; 
  * let x, y, z = 1, "hello world", 2.3; 
  */
variable_declaration : T_LET variable_declaration_name_list T_ASSIGN variable_declaration_expression_list T_SEMI { $$ = new AstVariableDeclaration($2, $4); }
                     ;

 /* variable_declaration left hand */
variable_declaration_name_list : variable_declaration_name { $$ = new AstExpressionList(); $$->push_back($1); }
                               | variable_declaration_name_list T_COMMA variable_declaration_name { $$->push_back($3); }
                               ;

variable_declaration_name : T_IDENTIFIER { $$ = new AstIdentifierConstant($1); std::free($1); }
                          ;

 /* variable_declaration right hand */
variable_declaration_expression_list : variable_declaration_expression { $$ = new AstExpressionList(); $$->push_back($1); }
                                     | variable_declaration_expression_list T_COMMA variable_declaration_expression { $$->push_back($3); }
                                     ;

variable_declaration_expression : constant_expression { $$ = $1; }
                                ;

 /**
  * let min() = return 0;
  * let max() = { return 100 }
  * let abs(x) = return x > 0 ? x : -x; 
  * let abs(x) = { if (x > 0) { return x; } else { return -x; } }
  */
function_declaration : T_LET T_IDENTIFIER T_LPAREN function_argument_list T_RPAREN T_ASSIGN statement {
                            AstStatementList *sl = new AstStatementList();
                            sl->push_back($7);
                            AstCompoundStatement *cs = new AstCompoundStatement(sl);
                            $$ = new AstFunctionDeclaration($2, $4, cs); 
                            std::free($2);
                        }
                     | T_LET T_IDENTIFIER T_LPAREN T_RPAREN T_ASSIGN statement {
                            AstStatementList *sl = new AstStatementList();
                            sl->push_back($6);
                            AstCompoundStatement *cs = new AstCompoundStatement(sl);
                            $$ = new AstFunctionDeclaration($2, nullptr, cs);
                            std::free($2);
                        }
                     | T_LET T_IDENTIFIER T_LPAREN function_argument_list T_RPAREN T_ASSIGN compound_statement { $$ = new AstFunctionDeclaration($2, $4, $7); std::free($2); }
                     | T_LET T_IDENTIFIER T_LPAREN T_RPAREN T_ASSIGN compound_statement { $$ = new AstFunctionDeclaration($2, nullptr, $6); std::free($2); }
                     ;

function_argument_list : function_argument { $$ = new AstExpressionList(); $$->push_back($1); }
                       | function_argument_list T_COMMA function_argument { $$->push_back($3); }
                       ;

function_argument : T_IDENTIFIER { $$ = new AstIdentifierConstant($1); std::free($1); }
                  ;

compound_statement : T_LBRACE T_RBRACE { $$ = new AstCompoundStatement(nullptr); }
                   | T_LBRACE statement_list T_RBRACE { $$ = new AstCompoundStatement($2); }
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

expression_statement : T_SEMI { $$ = nullptr; }
                     | expression T_SEMI { $$ = new AstExpressionStatement($1); }
                     ;

selection_statement : T_IF T_LPAREN expression T_RPAREN statement { $$ = new AstIfStatement($3, $5, nullptr); }
                    | T_IF T_LPAREN expression T_RPAREN statement T_ELSE statement { $$ = new AstIfStatement($3, $5, $7); }
                    /*| T_SWITCH T_LPAREN expression T_RPAREN statement*/
                    ;

iteration_statement : T_WHILE T_LPAREN expression T_RPAREN statement { $$ = new AstWhileStatement($3, $5); }
                    /*| T_FOR T_LPAREN expression_statement expression_statement T_RPAREN statement { $$ = new AstForStatement($3, $4, nullptr, $6); }*/
                    | T_FOR T_LPAREN expression_statement expression_statement expression T_RPAREN statement { $$ = new AstForStatement($3, $4, $5, $7); }
                    ;

jump_statement : T_CONTINUE T_SEMI { $$ = new AstContinueStatement(); }
               | T_BREAK T_SEMI { $$ = new AstBreakStatement(); }
               | T_RETURN T_SEMI { $$ = new AstReturnStatement(nullptr); }
               | T_RETURN expression T_SEMI { $$ = new AstReturnStatement($2); }
               ;

translation_unit : declaration {}
                 | translation_unit declaration {}
                 ;

%%
