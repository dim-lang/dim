%define api.pure full
%locations
%param { yyscan_t yyscanner }

%code top {
#include <cstdarg>
#include <cstdio>
#include "Ast.h"
#include "Buffer.h"
#include "Parser.h"
#include "Scanner.h"
#include "Position.h"
#include "Exception.h"
#define Y_EXTRA yyget_extra(yyscanner)
#define Y_POSITION(x) Position((x).first_line, (x).first_column, (x).last_line, (x).last_column)
}

%code requires {
class Scanner;
class A_ExpressionList;
class AstStatementList;
class AstDefinitionList;
class AstExpression;
class AstStatement;
class AstDefinition;
class AstStringLiteral;
using yyscan_t = void *;
using YY_EXTRA_TYPE = Scanner *;
extern YY_EXTRA_TYPE yyget_extra ( yyscan_t yyscanner );
}

 /* different ways to access data */
%union {
    A_ExpressionList *exprList;
    AstStatementList *stmtList;
    AstDefinitionList *defList;
    AstExpression *expr;
    AstStatement *stmt;
    AstDefinition *def;
    AstStringLiteral *str;
    AstId *id;
    char *str;
    int token;
}

%token <token> T_EOF

 /* keyword */
%token <token> T_TRUE T_FALSE T_TRY T_CATCH T_VAR T_VAL T_NIL T_NEW T_DELETE T_DEF T_IF T_THEN T_ELSE T_ENUM T_SWITCH T_CASE T_MATCH T_FOR T_FOREACH T_IN T_WHILE T_DO T_BREAK T_CONTINUE
%token <token> T_FUNC T_CLASS T_TYPE T_THIS T_SUPER T_ISINSTANCEOF T_ISA T_IS T_IMPORT T_RETURN T_VOID T_NAN T_INF T_ASYNC T_AWAIT T_STATIC T_PUBLIC T_PROTECT T_PRIVATE

 /* primitive integer type */
%token <token> T_BYTE T_UBYTE T_SHORT T_USHORT T_INT T_UINT T_LONG T_ULONG T_LLONG T_ULLONG
 /* primitive float type */
%token <token> T_FLOAT T_DOUBLE T_LDOUBLE
 /* primitive boolean type */
%token <token> T_BOOLEAN

 /* binary/unary operator */
%token <token> T_LOGIC_AND T_LOGIC_OR T_LOGIC_NOT T_ADD T_SUB T_MUL T_DIV T_MOD T_ADD2 T_SUB2 T_MUL2 T_DIV2 T_MOD2 T_LOGIC_AND T_LOGIC_OR T_BIT_LSHIFT T_BIT_RSHIFT T_BIT_ARSHIFT
 /* unary operator */
%token <token> T_LOGIC_NOT T_BIT_AND T_BIT_OR T_BIT_NOT T_BIT_XOR
 /* assignment operator */
%token <token> T_ASSIGN T_ADD_ASSIGN T_SUB_ASSIGN T_MUL_ASSIGN T_DIV_ASSIGN T_MOD_ASSIGN T_BIT_AND_ASSIGN T_BIT_OR_ASSIGN T_BIT_XOR_ASSIGN T_BIT_LSHIFT_ASSIGN T_BIT_RSHIFT_ASSIGN T_BIT_ARSHIFT_ASSIGN
 /* compare operator */
%token <token> T_EQ T_NEQ T_LT T_LE T_GT T_GE
 /* parentheses */
%token <token> T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET T_LBRACE T_RBRACE
 /* other punctuation */
%token <token> T_UNDERLINE T_COMMA T_SEMI T_QUESTION T_COLON T_DOT T_SINGLE_RARROW T_DOUBLE_RARROW T_SINGLE_LARROW

 /* id */
%token <id> id

 /* str */
%token <str> T_PLAINID T_INTEGER_LITERAL T_FLOAT_LITERAL T_SINGLE_STRING_LITERAL T_MULTI_STRING_LITERAL T_CHAR_LITERAL

%type <expr> boolean_literal postfix_expression primary_expression unary_expression binary_expression
%type <expr> conditional_expression assignment_expression constant_expression
%type <expr> expression
%type <exprList> argument_expression_list

%type <stmt> compound_statement expression_statement iteration_statement jump_statement return_statement if_statement
%type <stmt> statement
%type <stmtList> statement_list

%type <def> variable_definition function_definition function_signature_definition function_argument_definition
%type <def> definition
%type <defList> compile_unit function_argument_definition_list

%type <str> string_literal single_string_literal multiple_string_literal

 /* operator precedence */
 /* comma */
%left T_COMMA
 /* assignment */
%right T_BIT_AND_ASSIGN T_BIT_OR_ASSIGN T_BIT_XOR_ASSIGN
%right T_BIT_LSHIFT_ASSIGN T_BIT_RSHIFT_ASSIGN T_BIT_ARSHIFT_ASSIGN
%right T_MUL_ASSIGN T_DIV_ASSIGN T_MOD_ASSIGN
%right T_ADD_ASSIGN T_SUB_ASSIGN
%right T_ASSIGN
 /* conditional */
%left T_QUESTION T_COLON
 /* binary operator */
%left T_LOGIC_OR
%left T_LOGIC_AND
%left T_BIT_OR
%left T_BIT_XOR
%left T_BIT_AND
%left T_EQ T_NEQ
%left T_LT T_LE T_GT T_GE
%left T_BIT_LSHIFT T_BIT_RSHIFT T_BIT_ARSHIFT
%left T_ADD T_SUB
%left T_MUL T_DIV T_MOD
 /* other */
%nonassoc T_LOGIC_NOT T_BIT_NOT
%left T_DOT
 /* fix if statement shift/reduce */
%nonassoc "lower_than_else"
%nonassoc T_ELSE

%start compile_unit

%%

 /* id { */

id : T_PLAINID { $$ = new A_PlainId($1, Y_POSITION(@1)); std::free($1); }
   ;

 /* id } */

 /* literal { */

boolean_literal : T_TRUE { $$ = new A_BooleanLiteral($1, Y_POSITION(@1)); std::free($1); }
                | T_FALSE { $$ = new A_BooleanLiteral($1, Y_POSITION(@1)); std::free($1); }
                ;

single_string_literal : T_SINGLE_STRING_LITERAL { $$ = new A_SingleStringLiteral(); $$->join($1, Y_POSITION(@1)); std::free($1); }
                      | T_SINGLE_STRING_LITERAL single_string_literal { $3->join($1, Y_POSITION(@1)); $$ = $3; std::free($1); }
                      ;

string_literal : single_string_literal { $$ = $1; }
               | T_MULTI_STRING_LITERAL { $$ = new A_MultiStringLiteral($1, Y_POSITION(@1)); std::free($1); }
               ;

 /* literal } */

 /* expression { */

primary_expression : id { $$ = $1; }
                   | boolean_literal { $$ = $1; }
                   | string_literal { $$ = $1; }
                   | T_INTEGER_LITERAL { $$ = new A_IntegerLiteral($1, Y_POSITION(@1)); std::free($1); }
                   | T_FLOAT_LITERAL { $$ = new A_FloatLiteral($1, Y_POSITION(@1)); std::free($1); }
                   | T_LPAREN expression T_RPAREN { $$ = $2; }
                   ;

postfix_expression : primary_expression { $$ = $1; }
                   /*| postfix_expression '[' expression ']'*/
                   | id T_LPAREN T_RPAREN { $$ = new A_CallExpression($1, new A_ExpressionList(), Y_POSITION(@1)); std::free($1); }
                   | id T_LPAREN argument_expression_list T_RPAREN { $$ = new A_CallExpression($1, $3, Y_POSITION(@1)); std::free($1); }
                   /*| postfix_expression '.' id */
                   ;

argument_expression_list : assignment_expression { $$ = new A_ExpressionList(); $$->add($1); }
                         | assignment_expression T_COMMA argument_expression_list { $3->add($1); $$ = $3; }
                         ;

unary_expression : postfix_expression { $$ = $1; }
                 | T_SUB unary_expression { $$ = new A_UnaryExpression($1, $2, Y_POSITION(@1)); }
                 | T_BIT_NOT unary_expression { $$ = new A_UnaryExpression($1, $2, Y_POSITION(@1)); }
                 | T_LOGIC_NOT unary_expression { $$ = new A_UnaryExpression($1, $2, Y_POSITION(@1)); }
                 ;

 /*
cast_expression : unary_expression
                | '(' type_name ')' cast_expression
                ;
 */

binary_expression : unary_expression { $$ = $1; }
                  | binary_expression T_MUL unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_DIV unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_MOD unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_ADD unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_SUB unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_BIT_LSHIFT unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_BIT_RSHIFT unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_BIT_ARSHIFT unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_LT unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_LE unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_GT unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_GE unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_EQ unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_NEQ unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_BIT_AND unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_BIT_XOR unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_BIT_OR unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_LOGIC_AND unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_LOGIC_OR unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  ;

conditional_expression : binary_expression { $$ = $1; }
                       /* | T_IF T_LPAREN binary_expression T_RPAREN expression T_ELSE conditional_expression { $$ = new A_ConditionalExpression($2, $4, $6); } */
                       | binary_expression T_QUESTION expression T_COLON conditional_expression { $$ = new A_ConditionalExpression($1, $3, $5); }
                       ;

assignment_expression : conditional_expression { $$ = $1; }
                      | unary_expression T_ASSIGN assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_MUL_ASSIGN assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_DIV_ASSIGN assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_MOD_ASSIGN assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_ADD_ASSIGN assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_SUB_ASSIGN assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_BIT_AND_ASSIGN assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_BIT_OR_ASSIGN assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_BIT_XOR_ASSIGN assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_BIT_LSHIFT_ASSIGN assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_BIT_RSHIFT_ASSIGN assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_BIT_ARSHIFT_ASSIGN assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      ;

expression : assignment_expression { $$ = $1; }
           | expression1
           ;

expression1 : if_expression
            | loop_expression
            | compound_expression
            | return_expression
            | jump_expression
            ;

if_expression : T_IF T_LPAREN expression T_RPAREN expression                    %prec "lower_than_else" { $$ = new A_IfStatement($3, $5, new AstEmptyStatement(), Y_POSITION(@1)); }
              | T_IF T_LPAREN expression T_RPAREN expression T_ELSE expression  { $$ = new A_IfStatement($3, $5, $7, Y_POSITION(@1)); }
              ;

loop_expression : T_WHILE T_LPAREN expression T_RPAREN expression
                | T_FOR T_LPAREN expression T_IN expression T_RPAREN expression
                ;

return_expression : T_RETURN expression
                  | T_RETURN
                  ;

compound_expression : T_LBRACE T_RBRACE { $$ = new AstCompoundStatement(new AstStatementList(), Y_POSITION(@1), Y_POSITION(@2)); }
                    | T_LBRACE expression_list T_RBRACE { $$ = new AstCompoundStatement($2, Y_POSITION(@1), Y_POSITION(@3)); }
                    ;

expression_list :
                ;

jump_expression : T_CONTINUE T_SEMI { $$ = new AstContinueStatement(Y_POSITION(@1), Y_POSITION(@2)); }
                | T_BREAK T_SEMI { $$ = new AstBreakStatement(Y_POSITION(@1), Y_POSITION(@2)); }
                ;

constant_expression : conditional_expression { $$ = $1; }
                    ;

 /* expression } */

 /* statement { */

statement : expression T_SEMI
          | definition
          /* import */
          ;

 /* statement } */

 /* part-2 definition */

definition : function_definition { $$ = $1; }
            | variable_definition { $$ = $1; }
            /*| import_module { $$ = $1; }*/
            ;

 /*
import_module : T_IMPORT import_module_list T_SEMI
              ;

import_module_list : T_IDENTIFIER
                   | T_IDENTIFIER T_DOT import_module_list 
                   ;
 */


 /**
  * var x = 1;
  * var x:i64 = 100;
  * var x:i64 = 100, y = 1, z:string = "hello";
  */
variable_definition : T_VAR T_IDENTIFIER T_SEMI { $$ = new AstVariableDefinition($2, Y_POSITION(@1), Y_POSITION(@3)); std::free($2); }
                    | T_VAR T_IDENTIFIER T_ASSIGN constant_expression T_SEMI { $$ = new AstVariableDefinition($2, ); std::free($2); }
                    ;

 /**
  * func hello() { print("world"); }
  * func hello2() => { print("world"); }
  * func hello3() => print("world");
  * func min(a: i64, b: i64): i64 { return a < b ? a : b; }
  * func min2(a: i64, b: i64): i64 => { return a < b ? a : b; }
  * func max(a: i64, b: i64): i64 => a > b ? a : b;
  * func abs(x: i64): i64 => if (x > 0) return x; else return -x;
  */
function_definition : function_signature_definition compound_statement { $$ = new AstFunctionDefinition(dynamic_cast<AstFunctionSignatureDefinition*>($1), $2); }
                    | function_signature_definition T_RIGHT_ARROW expression_statement { $$ = new AstFunctionDefinition(dynamic_cast<AstFunctionSignatureDefinition*>($1), $3); }
                    ;

function_signature_definition : T_DEF T_IDENTIFIER T_LPAREN function_argument_definition_list T_RPAREN {
                                    $$ = new AstFunctionSignatureDefinition($2, $4, nullptr, Y_POSITION(@1), Y_POSITION(@2));
                                    std::free($2);
                                }
                              | T_DEF T_IDENTIFIER T_LPAREN T_RPAREN {
                                    $$ = new AstFunctionSignatureDefinition($2, new AstDefinitionList(), nullptr, Y_POSITION(@1), Y_POSITION(@2));
                                    std::free($2);
                                }
                              ;

function_argument_definition_list : function_argument_definition { $$ = new AstDefinitionList(); $$->add($1); }
                                  | function_argument_definition T_COMMA function_argument_definition_list { $3->add($1); $$ = $3; }
                                  ;

function_argument_definition : T_IDENTIFIER { $$ = new AstFunctionArgumentDefinition($1, Y_POSITION(@1)); std::free($1); }
                             ;

 /* part-3 statement */
compound_statement : T_LBRACE T_RBRACE { $$ = new AstCompoundStatement(new AstStatementList(), Y_POSITION(@1), Y_POSITION(@2)); }
                   | T_LBRACE statement_list T_RBRACE { $$ = new AstCompoundStatement($2, Y_POSITION(@1), Y_POSITION(@3)); }
                   ;

statement_list : statement { $$ = new AstStatementList(); $$->add($1); }
               | statement statement_list { $2->add($1); $$ = $2; }
               ;

statement : if_statement { $$ = $1; }
          | compound_statement { $$ = $1; }
          | expression_statement { $$ = $1; }
          | iteration_statement { $$ = $1; }
          | jump_statement { $$ = $1; }
          | return_statement { $$ = $1; }
          /*| switch_statement { $$ = $1; }*/
          /*| switch_body_statement { $$ = $1; }*/
          /*| match_statement { $$ = $1; }*/
          /*| match_body_statement { $$ = $1; }*/
          | definition { $$ = $1; }
          ;

 /*

switch_statement : T_SWITCH T_LPAREN assignment_expression T_RPAREN statement { $$ = new AstSwitchStatement($3, $5); }
                 ;

switch_body_statement : T_BIT_OR constant_expression T_COLON statement_list { $$ = new AstSwitchBodyStatement($2, $4); }
                      | T_BIT_OR T_UNDERLINE T_COLON statement_list {}
                      ;

match_statement : T_MATCH T_LPAREN argument_expression_list T_RPAREN statement { $$ = new AstMatchStatement($3, $5); }
                ;

match_body_statement : T_BIT_OR assignment_expression T_COLON statement { $$ = new AstMatchBodyStatement($1, $3); }
                     | T_BIT_OR T_UNDERLINE T_COLON statement {}
                     ;
 */

expression_statement : expression T_SEMI { $$ = new AstExpressionStatement($1, Y_POSITION(@2)); }
                     ;

iteration_statement : T_WHILE T_LPAREN sequel_expression T_RPAREN statement { $$ = new AstWhileStatement($3, $5, Y_POSITION(@1)); }
                    | T_FOR T_LPAREN expression_statement expression_statement expression T_RPAREN statement { $$ = new AstForStatement($3, $4, $5, $7, Y_POSITION(@1)); }
                    ;

jump_statement : T_CONTINUE T_SEMI { $$ = new AstContinueStatement(Y_POSITION(@1), Y_POSITION(@2)); }
               | T_BREAK T_SEMI { $$ = new AstBreakStatement(Y_POSITION(@1), Y_POSITION(@2)); }
               ;

return_statement : T_RETURN expression T_SEMI { $$ = new AstReturnStatement($2, Y_POSITION(@1), Y_POSITION(@3)); }
                 ;

compile_unit : definition {
                    EX_ASSERT(Y_EXTRA, "Y_EXTRA is null");
                    EX_ASSERT(Y_EXTRA->translateUnit(), "Y_EXTRA.translateUnit is null");
                    Y_EXTRA->translateUnit()->add($1);
                }
             | definition compile_unit {
                    EX_ASSERT(Y_EXTRA, "Y_EXTRA is null");
                    EX_ASSERT(Y_EXTRA->translateUnit(), "Y_EXTRA.translateUnit is null");
                    Y_EXTRA->translateUnit()->add($1);
                }
             ;

%%

void yyerror(YYLTYPE *yyllocp, yyscan_t yyscanner, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  if (yyllocp && yyllocp->first_line) {
    fprintf(stderr, "%s: %d.%d-%d.%d: error: ", 
            (Y_EXTRA ? Y_EXTRA->currentBuffer().c_str() : "unknown"),
            yyllocp->first_line,
            yyllocp->first_column, 
            yyllocp->last_line, 
            yyllocp->last_column);
  }
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}
