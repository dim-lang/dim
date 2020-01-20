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

 /**
  * union
  */

%token <literal> T_IDENTIFIER T_INTEGER T_DOUBLE T_STRING

%token <token> T_EOF
%token <token> T_TRUE T_FALSE T_LET T_NIL T_IF T_ELSEIF T_ELSE T_FOR T_WHILE T_BREAK T_CONTINUE T_SWITCH T_CASE T_OTHERWISE
%token <token> T_FUNC T_CLASS T_TYPE T_ISINSTANCE T_IMPORT T_RETURN T_VOID T_LOGIC_AND T_LOGIC_OR T_LOGIC_NOT
%token <token> T_INTEGER_KEYWORD T_UNSIGNED_INTEGER_KEYWORD T_DOUBLE_KEYWORD
%token <token> T_ADD T_SUB T_MUL T_DIV T_MOD T_BIT_NOT T_BIT_AND T_BIT_OR T_BIT_XOR T_BIT_LSHIFT T_BIT_RSHIFT T_BIT_ZERORSHIFT
%token <token> T_ASSIGN T_ADD_ASSIGN T_SUB_ASSIGN T_MUL_ASSIGN T_DIV_ASSIGN T_MOD_ASSIGN
%token <token> T_BIT_AND_ASSIGN T_BIT_OR_ASSIGN T_BIT_XOR_ASSIGN T_BIT_LSHIT_ASSIGN T_BIT_RSHIT_ASSIGN T_BIT_ZERORSHIT_ASSIGN
%token <token> T_EQ T_NEQ T_LT T_LE T_GT T_GE
%token <token> T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET T_LBRACE T_RBRACE T_COMMA T_SEMI T_QUESTION T_COLON T_DOT

%type <ast> translation_unit

%type <expression> postfix_expression primary_expression unary_expression logical_or_expression logical_and_expression
%type <expression> conditional_expression assignment_expression constant_expression bit_and_expression bit_or_expression bit_xor_expression
%type <expression> equality_expression relational_expression shift_expression additive_expression multiplicative_expression
%type <expression> expression

%type <statement> compound_statement expression_statement selection_statement iteration_statement jump_statement
%type <statement> statement statement_or_declaration 

%type <declaration> declaration variable_declaration function_declaration

%type <expressionList> argument_expression_list conditional_expression_list unary_expression_list
%type <expressionList> function_argument_list variable_declaration_name_list variable_declaration_expression_list

%type <statementList> statement_or_declaration_list

 /**
  * operator precedence
  */

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
%left T_LOGIC_NOT T_BIT_NOT
%left T_DOT
%left T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET

%start translation_unit

%%

 /* part-1 expression */

primary_expression : T_IDENTIFIER { $$ = new AstIdentifierConstant($1); std::free($1); FINFO("primary_expression: {}", $$->toString()); }
                   | T_INTEGER { $$ = new AstIntegerConstant(std::stoll($1)); std::free($1); FINFO("primary_expression: {}", $$->toString()); }
                   | T_DOUBLE { $$ = new AstDoubleConstant(std::stod($1)); std::free($1); FINFO("primary_expression: {}", $$->toString()); }
                   | T_STRING { $$ = new AstStringConstant($1); std::free($1); FINFO("primary_expression: {}", $$->toString()); }
                   | T_LPAREN expression T_RPAREN { $$ = $2; FINFO("primary_expression: {}", $$ ? $$->toString() : "null"); }
                   ;

postfix_expression : primary_expression { $$ = $1; FINFO("postfix_expression: {}", $$ ? $$->toString() : "null"); }
                   /*| postfix_expression '[' expression ']'*/
                   | T_IDENTIFIER T_LPAREN T_RPAREN { $$ = new AstFunctionCallExpression($1, nullptr); std::free($1); FINFO("postfix_expression: {}", $$->toString()); }
                   | T_IDENTIFIER T_LPAREN argument_expression_list T_RPAREN { $$ = new AstFunctionCallExpression($1, $3); std::free($1); FINFO("postfix_expression: {}", $$->toString()); }
                   /*| postfix_expression '.' T_IDENTIFIER */
                   ;

argument_expression_list : assignment_expression { $$ = new AstExpressionList(); $$->push_back($1); FINFO("argument_expression_list: {}", Ast::dump($$)); }
                         | argument_expression_list T_COMMA assignment_expression { $$->push_back($3); FINFO("argument_expression_list: {}", Ast::dump($$)); }
                         ;

unary_expression : postfix_expression { $$ = $1; FINFO("unary_expression: {}", $$ ? $$->toString() : "null"); }
                 | T_ADD unary_expression { $$ = new AstUnaryExpression($1, $2); FINFO("unary_expression: {}", $$->toString()); }
                 | T_SUB unary_expression { $$ = new AstUnaryExpression($1, $2); FINFO("unary_expression: {}", $$->toString()); }
                 | T_LOGIC_NOT unary_expression { $$ = new AstUnaryExpression($1, $2); FINFO("unary_expression: {}", $$->toString()); }
                 | T_BIT_NOT unary_expression { $$ = new AstUnaryExpression($1, $2); FINFO("unary_expression: {}", $$->toString()); }
                 ;

 /*
cast_expression : unary_expression
                | '(' type_name ')' cast_expression
                ;
 */

multiplicative_expression : unary_expression { $$ = $1; FINFO("multiplicative_expression : {}", $$ ? $$->toString() : "null"); }
                          | multiplicative_expression T_MUL unary_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("multiplicative_expression : {}", $$->toString()); }
                          | multiplicative_expression T_DIV unary_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("multiplicative_expression : {}", $$->toString()); }
                          | multiplicative_expression T_MOD unary_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("multiplicative_expression : {}", $$->toString()); }
                          ;

additive_expression : multiplicative_expression { $$ = $1; FINFO("additive_expression: {}", $$ ? $$->toString() : "null"); }
                    | additive_expression T_ADD multiplicative_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("additive_expression: {}", $$->toString()); }
                    | additive_expression T_SUB multiplicative_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("additive_expression: {}", $$->toString()); }
                    ;

shift_expression : additive_expression { $$ = $1; FINFO("shift_expression: {}", $$ ? $$->toString() : "null"); }
                 | shift_expression T_BIT_LSHIFT additive_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("shift_expression: {}", $$->toString()); }
                 | shift_expression T_BIT_RSHIFT additive_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("shift_expression: {}", $$->toString()); }
                 | shift_expression T_BIT_ZERORSHIFT additive_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("shift_expression: {}", $$->toString()); }
                 ;

relational_expression : shift_expression { $$ = $1; FINFO("relational_expression: {}", $$ ? $$->toString() : "null"); }
                      | relational_expression T_LT shift_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("relational_expression: {}", $$->toString()); }
                      | relational_expression T_LE shift_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("relational_expression: {}", $$->toString()); }
                      | relational_expression T_GT shift_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("relational_expression: {}", $$->toString()); }
                      | relational_expression T_GE shift_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("relational_expression: {}", $$->toString()); }
                      ;

equality_expression : relational_expression { $$ = $1; FINFO("equality_expression: {}", $$ ? $$->toString() : "null"); }
                    | equality_expression T_EQ relational_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("equality_expression: {}", $$->toString()); }
                    | equality_expression T_NEQ relational_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("equality_expression: {}", $$->toString()); }
                    ;

bit_and_expression : equality_expression { $$ = $1; FINFO("bit_and_expression: {}", $$ ? $$->toString() : "null"); }
                   | bit_and_expression T_BIT_AND equality_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("bit_and_expression: {}", $$->toString()); }
                   ;

bit_xor_expression : bit_and_expression { $$ = $1; FINFO("bit_xor_expression: {}", $$ ? $$->toString() : "null"); }
                   | bit_xor_expression T_BIT_XOR bit_and_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("bit_xor_expression: {}", $$->toString()); }
                   ;

bit_or_expression : bit_xor_expression { $$ = $1; FINFO("bit_or_expression: {}", $$ ? $$->toString() : "null"); }
                  | bit_or_expression T_BIT_OR bit_xor_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("bit_or_expression: {}", $$->toString()); }
                  ;

logical_and_expression : bit_or_expression { $$ = $1; FINFO("logical_and_expression: {}", $$ ? $$->toString() : "null"); }
                       | logical_and_expression T_LOGIC_AND bit_or_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("logical_and_expression: {}", $$->toString()); }
                       ;

logical_or_expression : logical_and_expression { $$ = $1; FINFO("logical_or_expression: {}", $$ ? $$->toString() : "null"); }
                      | logical_or_expression T_LOGIC_OR logical_and_expression { $$ = new AstBinaryExpression($1, $2, $3); FINFO("logical_or_expression: {}", $$->toString()); }
                      ;

conditional_expression : logical_or_expression { $$ = $1; FINFO("conditional_expression: {}", $$ ? $$->toString() : "null"); }
                       | logical_or_expression T_QUESTION expression T_COLON conditional_expression {
                                $$ = new AstConditionalExpression($1, $3, $5); FINFO("conditional_expression: {}", $$->toString());
                            }
                       ;

 /**
  * 1. allow multiple assignment in an expression
  * x, y, z = 1, "hello", 2.3
  * 2. allow multiple assignment as a chain
  * x,y = u,v = 1,"hello"
  */
assignment_expression : conditional_expression_list { $$ = $1; FINFO("assignment_expression: {}", $$ ? $$->toString() : "null"); }
                      | unary_expression_list T_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); FINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_MUL_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); FINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_DIV_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); FINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_MOD_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); FINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_ADD_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); FINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_SUB_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); FINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_BIT_AND_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); FINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_BIT_OR_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); FINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_BIT_XOR_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); FINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_BIT_LSHIT_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); FINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_BIT_RSHIT_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); FINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_BIT_ZERORSHIT_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); FINFO("assignment_expression: {}", $$->toString()); }
                      ;

unary_expression_list : unary_expression { $$ = new AstExpressionList(); $$->push_back($1); FINFO("unary_expression_list: {}", Ast::dump($$)); }
                      | unary_expression_list T_COMMA unary_expression { $$->push_back($3); FINFO("unary_expression_list: {}", Ast::dump($$)); }
                      ;

conditional_expression_list : conditional_expression { $$ = new AstExpressionList(); $$->push_back($1); FINFO("conditional_expression_list: {}", Ast::dump($$)); }
                            | conditional_expression_list T_COMMA conditional_expression { $$->push_back($3); FINFO("conditional_expression_list: {}", Ast::dump($$)); }
                            ;

expression : /* nothing */ { $$ = nullptr; FINFO("expression: null"); }
           | assignment_expression { $$ = $1; FINFO("expression: {}", $$ ? $$->toString() : "null"); }
           ;

constant_expression : conditional_expression { $$ = $1; FINFO("constant_expression: {}", $$ ? $$->toString() : "null"); }
                    ;

 /* part-2 declaration */

declaration : function_declaration { $$ = $1; FINFO("declaration: {}", $$ ? $$->toString() : "null"); }
            | variable_declaration { $$ = $1; FINFO("declaration: {}", $$ ? $$->toString() : "null"); }
            ;

 /**
  * let x = 1; 
  * let x, y, z = 1, "hello world", 2.3; 
  */
variable_declaration : T_LET variable_declaration_name_list T_ASSIGN variable_declaration_expression_list T_SEMI { $$ = new AstVariableDeclaration($2, $4); FINFO("variable_declaration: {}", $$->toString()); }
                     ;

 /* variable_declaration left hand */
variable_declaration_name_list : T_IDENTIFIER { $$ = new AstExpressionList(); $$->push_back(new AstIdentifierConstant($1)); std::free($1); FINFO("variable_declaration_name_list: {}", Ast::dump($$)); }
                               | variable_declaration_name_list T_COMMA T_IDENTIFIER { $$->push_back(new AstIdentifierConstant($3)); std::free($3); FINFO("variable_declaration_name_list: {}", Ast::dump($$)); }
                               ;

 /* variable_declaration right hand */
variable_declaration_expression_list : constant_expression { $$ = new AstExpressionList(); $$->push_back($1); FINFO("variable_declaration_expression_list: {}", Ast::dump($$)); }
                                     | variable_declaration_expression_list T_COMMA constant_expression { $$->push_back($3); FINFO("variable_declaration_expression_list: {}", Ast::dump($$)); }
                                     ;

 /**
  * let min() = return 0;
  * let max() = { return 100; }
  * let abs(x) = return x > 0 ? x : -x; 
  * let abs(x) = { if (x > 0) { return x; } else { return -x; } }
  */
function_declaration : T_LET T_IDENTIFIER T_LPAREN function_argument_list T_RPAREN T_ASSIGN statement {
                            AstStatementList *statementList = new AstStatementList();
                            statementList->push_back($7);
                            $$ = new AstFunctionDeclaration($2, $4, statementList);
                            std::free($2);
                        }
                     | T_LET T_IDENTIFIER T_LPAREN T_RPAREN T_ASSIGN statement {
                            AstStatementList *statementList = new AstStatementList();
                            statementList->push_back($6);
                            $$ = new AstFunctionDeclaration($2, nullptr, statementList);
                            std::free($2);
                        }
                     | T_LET T_IDENTIFIER T_LPAREN function_argument_list T_RPAREN T_ASSIGN compound_statement { 
                            $$ = new AstFunctionDeclaration($2, $4, $7); std::free($2); 
                        }
                     | T_LET T_IDENTIFIER T_LPAREN T_RPAREN T_ASSIGN compound_statement { 
                            $$ = new AstFunctionDeclaration($2, nullptr, $6); std::free($2); 
                        }
                     ;

function_argument_list : T_IDENTIFIER { $$ = new AstExpressionList(); $$->push_back($1); }
                       | function_argument_list T_COMMA T_IDENTIFIER { $$->push_back($3); }
                       ;

 /* part-3 statement */
compound_statement : T_LBRACE T_RBRACE { $$ = new AstCompoundStatement(nullptr); }
                   | T_LBRACE statement_or_declaration_list T_RBRACE { $$ = new AstCompoundStatement($2); }
                   ;

statement_or_declaration_list : statement_or_declaration
                              | statement_or_declaration_list statement_or_declaration
                              ;

statement_or_declaration : statement
                         | declaration
                         ;

statement : compound_statement { $$ = $1; }
          | expression_statement { $$ = $1; }
          | selection_statement { $$ = $1; }
          | iteration_statement { $$ = $1; }
          | jump_statement { $$ = $1; }
          ;

expression_statement : T_SEMI { $$ = nullptr; }
                     | expression T_SEMI { $$ = new AstExpressionStatement($1); }
                     ;

selection_statement : T_IF T_LPAREN expression T_RPAREN statement { $$ = new AstIfStatement($3, $5, nullptr); }
                    | T_IF T_LPAREN expression T_RPAREN statement T_ELSE statement { $$ = new AstIfStatement($3, $5, $7); }
                    /*| T_SWITCH T_LPAREN expression T_RPAREN statement*/
                    ;

iteration_statement : T_WHILE T_LPAREN expression T_RPAREN statement { $$ = new AstWhileStatement($3, $5); }
                    | T_FOR T_LPAREN expression_statement expression_statement T_RPAREN statement { $$ = new AstForStatement($3, $4, nullptr, $6); }
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
