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
class AstExpressionList;
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
    AstExpressionList *exprList;
    AstStatementList *stmtList;
    AstDefinitionList *defList;
    AstExpression *expr;
    AstStatement *stmt;
    AstDefinition *def;
    AstStringLiteral *str;
    AstId *id;
    char *literal;
    int token;
}

 /* union */
%token <token> T_EOF
%token <token> T_TRUE T_FALSE T_LET T_VAR T_VAL T_NIL T_NULL T_NEW T_DELETE T_TRY T_CATCH T_DEF T_ENUM
%token <token> T_IF T_THEN T_ELSE T_FOR T_FOREACH T_IN T_WHILE T_DO T_BREAK T_CONTINUE T_SWITCH T_CASE T_MATCH T_DEFAULT
%token <token> T_FUNC T_CLASS T_TYPE T_THIS T_SUPER T_IS T_ISA T_ISINSTANCEOF T_IMPORT T_RETURN T_VOID T_LOGIC_AND T_LOGIC_OR T_LOGIC_NOT
%token <token> T_INT8 T_UINT8 T_INT16 T_UINT16 T_INT32 T_UINT32 T_INT64 T_UINT64 T_INT128 T_UINT128 T_FLOAT32 T_FLOAT64 T_FLOAT128 T_CHAR
%token <token> T_STRING T_BOOLEAN T_ASYNC T_AWAIT T_STATIC T_PUBLIC T_PROTECT T_PRIVATE T_NAN T_INF
%token <token> T_ADD T_SUB T_MUL T_DIV T_MOD T_ADD2 T_SUB2 T_MUL2 T_DIV2 T_MOD2 T_UNDERLINE 
%token <token> T_BIT_NOT T_BIT_AND T_BIT_OR T_BIT_XOR T_BIT_LSHIFT T_BIT_RSHIFT T_BIT_ARSHIFT
%token <token> T_ASSIGN T_ADD_ASSIGN T_SUB_ASSIGN T_MUL_ASSIGN T_DIV_ASSIGN T_MOD_ASSIGN
%token <token> T_BIT_AND_ASSIGN T_BIT_OR_ASSIGN T_BIT_XOR_ASSIGN T_BIT_LSHIFT_ASSIGN T_BIT_RSHIFT_ASSIGN T_BIT_ARSHIFT_ASSIGN
%token <token> T_EQ T_NEQ T_LT T_LE T_GT T_GE
%token <token> T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET T_LBRACE T_RBRACE T_COMMA T_SEMI T_QUESTION T_COLON T_DOT T_RIGHT_ARROW

%token <id> id

%token <literal> T_PLAINID T_INTEGER_LITERAL T_FLOAT_LITERAL T_SINGLE_STRING_LITERAL T_MULTI_STRING_LITERAL T_CHAR_LITERAL

%type <expr> boolean_literal postfix_expr primary_expr unary_expr binary_expr
%type <expr> conditional_expr assignment_expr constant_expr  sequel_expr
%type <expr> expr
%type <exprList> argument_expr_list sequel_expr_list

%type <stmt> compound_statement expr_statement iteration_statement jump_statement return_statement if_statement
%type <stmt> statement
%type <stmtList> statement_list

%type <def> variable_definition function_definition function_signature_definition function_argument_definition
%type <def> definition
%type <defList> translation_unit function_argument_definition_list

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

%start translation_unit

%%

 /* id { */

id : T_PLAINID { $$ = new A_PlainId($1, Y_POSITION(@1)); std::free($1); }
   ;

 /* id } */

 /* literal { */

boolean_literal : T_TRUE { $$ = new A_BooleanLiteral($1, Y_POSITION(@1)); std::free($1); }
                | T_FALSE { $$ = new A_BooleanLiteral($1, Y_POSITION(@1)); std::free($1); }
                ;

single_string_literal : T_SINGLE_STRING_LITERAL { $$ = new A_SingleStringLiteral(); $$->join(new A_SingleStringSingleLiteral($1, Y_POSITION(@1))); std::free($1); }
                      | T_SINGLE_STRING_LITERAL single_string_literal { $$->join(new A_SingleStringSingleLiteral($1, Y_POSITION(@1))); std::free($1); }
                      ;

string_literal : single_string_literal { $$ = $1; }
               | T_MULTI_STRING_LITERAL { $$ = new A_MultiStringLiteral($1, Y_POSITION(@1)); std::free($1); }
               ;

 /* literal } */

 /* expression { */

primary_expr : id { $$ = $1; }
             | boolean_literal { $$ = $1; }
             | string_literal { $$ = $1; }
             | T_INTEGER_LITERAL { $$ = new A_IntegerLiteral($1, Y_POSITION(@1)); std::free($1); }
             | T_FLOAT_LITERAL { $$ = new A_FloatLiteral($1, Y_POSITION(@1)); std::free($1); }
             | T_LPAREN expr T_RPAREN { $$ = $2; }
             ;

postfix_expr : primary_expr { $$ = $1; }
             /*| postfix_expr '[' expr ']'*/
             | id T_LPAREN T_RPAREN { $$ = new A_CallExpression($1, new AstExpressionList(), Y_POSITION(@1)); std::free($1); }
             | id T_LPAREN argument_expr_list T_RPAREN { $$ = new A_CallExpression($1, $3, Y_POSITION(@1)); std::free($1); }
             /*| postfix_expr '.' id */
             ;

argument_expr_list : assignment_expr { $$ = new AstExpressionList(); $$->add($1); }
                   | assignment_expr T_COMMA argument_expr_list { $3->add($1); $$ = $3; }
                   ;

unary_expr : postfix_expr { $$ = $1; }
           | T_SUB unary_expr { $$ = new AstUnaryExpression($1, $2, Y_POSITION(@1)); }
           | T_BIT_NOT unary_expr { $$ = new AstUnaryExpression($1, $2, Y_POSITION(@1)); }
           | T_LOGIC_NOT unary_expr { $$ = new AstUnaryExpression($1, $2, Y_POSITION(@1)); }
           ;

 /*
cast_expr : unary_expr
                | '(' type_name ')' cast_expr
                ;
 */

binary_expr : unary_expr { $$ = $1; }
                  | binary_expr T_MUL unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_DIV unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_MOD unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_ADD unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_SUB unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_BIT_LSHIFT unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_BIT_RSHIFT unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_BIT_ARSHIFT unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_LT unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_LE unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_GT unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_GE unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_EQ unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_NEQ unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_BIT_AND unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_BIT_XOR unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_BIT_OR unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_LOGIC_AND unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expr T_LOGIC_OR unary_expr { $$ = new AstBinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  ;

conditional_expr : binary_expr { $$ = $1; }
                       /* | T_IF T_LPAREN binary_expr T_RPAREN expr T_ELSE conditional_expr { $$ = new AstConditionalExpression($2, $4, $6); } */
                       | binary_expr T_QUESTION expr T_COLON conditional_expr { $$ = new AstConditionalExpression($1, $3, $5); }
                       ;

assignment_expr : conditional_expr { $$ = $1; }
                      | unary_expr T_ASSIGN assignment_expr { $$ = new AstAssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expr T_MUL_ASSIGN assignment_expr { $$ = new AstAssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expr T_DIV_ASSIGN assignment_expr { $$ = new AstAssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expr T_MOD_ASSIGN assignment_expr { $$ = new AstAssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expr T_ADD_ASSIGN assignment_expr { $$ = new AstAssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expr T_SUB_ASSIGN assignment_expr { $$ = new AstAssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expr T_BIT_AND_ASSIGN assignment_expr { $$ = new AstAssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expr T_BIT_OR_ASSIGN assignment_expr { $$ = new AstAssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expr T_BIT_XOR_ASSIGN assignment_expr { $$ = new AstAssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expr T_BIT_LSHIFT_ASSIGN assignment_expr { $$ = new AstAssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expr T_BIT_RSHIFT_ASSIGN assignment_expr { $$ = new AstAssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expr T_BIT_ARSHIFT_ASSIGN assignment_expr { $$ = new AstAssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      ;

sequel_expr_list : assignment_expr { $$ = new AstExpressionList(); $$->add($1); }
                       | assignment_expr T_COMMA sequel_expr_list { $3->add($1); $$ = $3; }
                       ;

sequel_expr : sequel_expr_list { $$ = new AstSequelExpression($1); }
                  ;

expr : sequel_expr { $$ = $1; }
           | /* empty void expr */ { $$ = new AstVoidExpression(); }
           ;

constant_expr : conditional_expr { $$ = $1; }
                    ;

 /* expression } */

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
                    | T_VAR T_IDENTIFIER T_ASSIGN constant_expr T_SEMI { $$ = new AstVariableDefinition($2, ); std::free($2); }
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
                    | function_signature_definition T_RIGHT_ARROW expr_statement { $$ = new AstFunctionDefinition(dynamic_cast<AstFunctionSignatureDefinition*>($1), $3); }
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
          | expr_statement { $$ = $1; }
          | iteration_statement { $$ = $1; }
          | jump_statement { $$ = $1; }
          | return_statement { $$ = $1; }
          /*| switch_statement { $$ = $1; }*/
          /*| switch_body_statement { $$ = $1; }*/
          /*| match_statement { $$ = $1; }*/
          /*| match_body_statement { $$ = $1; }*/
          | definition { $$ = $1; }
          ;

if_statement : T_IF T_LPAREN expr T_RPAREN statement                          %prec "lower_than_else" { $$ = new AstIfStatement($3, $5, new AstEmptyStatement(), Y_POSITION(@1)); }
             | T_IF T_LPAREN expr T_RPAREN statement T_ELSE statement         { $$ = new AstIfStatement($3, $5, $7, Y_POSITION(@1)); }
             ;

 /*

switch_statement : T_SWITCH T_LPAREN assignment_expr T_RPAREN statement { $$ = new AstSwitchStatement($3, $5); }
                 ;

switch_body_statement : T_BIT_OR constant_expr T_COLON statement_list { $$ = new AstSwitchBodyStatement($2, $4); }
                      | T_BIT_OR T_UNDERLINE T_COLON statement_list {}
                      ;

match_statement : T_MATCH T_LPAREN argument_expr_list T_RPAREN statement { $$ = new AstMatchStatement($3, $5); }
                ;

match_body_statement : T_BIT_OR assignment_expr T_COLON statement { $$ = new AstMatchBodyStatement($1, $3); }
                     | T_BIT_OR T_UNDERLINE T_COLON statement {}
                     ;
 */

expr_statement : expr T_SEMI { $$ = new AstExpressionStatement($1, Y_POSITION(@2)); }
                     ;

iteration_statement : T_WHILE T_LPAREN sequel_expr T_RPAREN statement { $$ = new AstWhileStatement($3, $5, Y_POSITION(@1)); }
                    | T_FOR T_LPAREN expr_statement expr_statement expr T_RPAREN statement { $$ = new AstForStatement($3, $4, $5, $7, Y_POSITION(@1)); }
                    ;

jump_statement : T_CONTINUE T_SEMI { $$ = new AstContinueStatement(Y_POSITION(@1), Y_POSITION(@2)); }
               | T_BREAK T_SEMI { $$ = new AstBreakStatement(Y_POSITION(@1), Y_POSITION(@2)); }
               ;

return_statement : T_RETURN expr T_SEMI { $$ = new AstReturnStatement($2, Y_POSITION(@1), Y_POSITION(@3)); }
                 ;

translation_unit : definition {
                        EX_ASSERT(Y_EXTRA, "Y_EXTRA is null");
                        EX_ASSERT(Y_EXTRA->translateUnit(), "Y_EXTRA.translateUnit is null");
                        Y_EXTRA->translateUnit()->add($1);
                    }
                 | definition translation_unit {
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
