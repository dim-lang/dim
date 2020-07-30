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
    A_ExpressionList *a_expr_list;
    AstStatementList *a_stmt_list;
    AstDefinitionList *a_def_list;
    AstExpression *a_expr;
    AstStatement *a_stmt;
    AstDefinition *a_def;
    AstStringLiteral *a_string;
    AstId *a_id;
    AstToken *a_token;
    char *literal;
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

 /* and or not operator */
%token <token> T_LOGIC_AND T_LOGIC_OR T_LOGIC_NOT 

 /* operator */
%token <token> T_PLUS T_MINUS T_ASTERISK T_SLASH T_PERCENT T_PLUS2 T_MINUS2 T_ASTERISK2 T_SLASH2 T_PERCENT2
 /* operator */
%token <token> T_AMPERSAND T_BAR T_TILDE T_CARET
 /* operator */
%token <token> T_LSHIFT T_RSHIFT T_ARSHIFT

 /* equal operator */
%token <token> T_EQUAL T_PLUS_EQUAL T_MINUS_EQUAL T_ASTERISK_EQUAL T_SLASH_EQUAL T_PERCENT_EQUAL T_AMPERSAND_EQUAL T_BAR_EQUAL T_CARET_EQUAL T_LSHIFT_EQUAL T_RSHIFT_EQUAL T_ARSHIFT_EQUAL
 /* compare operator */
%token <token> T_EQ T_NEQ T_LT T_LE T_GT T_GE
 /* parentheses */
%token <token> T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET T_LBRACE T_RBRACE
 /* other punctuation */
%token <token> T_UNDERSCORE T_COMMA T_SEMI T_QUESTION T_COLON T_DOT T_THIN_LARROW T_THIN_RARROW T_FAT_RARROW 

 /* literal */
%token <literal> T_INTEGER_LITERAL T_FLOAT_LITERAL T_SINGLE_STRING_LITERAL T_MULTI_STRING_LITERAL T_CHAR_LITERAL
%token <literal> T_PLAINID

 /* id */
%token <a_id> id

 /* token */
%token <a_token> tok

%type <a_expr> boolean_literal postfix_expression primary_expression unary_expression binary_expression
%type <a_expr> conditional_expression assignment_expression constant_expression
%type <a_expr> expression
%type <a_expr_list> argument_expression_list

%type <a_stmt> compound_statement expression_statement iteration_statement jump_statement return_statement if_statement
%type <a_stmt> statement
%type <a_stmt_list> statement_list

%type <a_def> variable_definition function_definition function_signature_definition function_argument_definition
%type <a_def> definition
%type <a_def_list> compile_unit function_argument_definition_list

%type <a_string> string_literal single_string_literal multiple_string_literal

 /* operator precedence */
 /* comma */
%left T_COMMA
 /* assignment */
%right T_AMPERSAND_EQUAL T_BAR_EQUAL T_CARET_EQUAL
%right T_LSHIFT_EQUAL T_RSHIFT_EQUAL T_ARSHIFT_EQUAL
%right T_ASTERISK_EQUAL T_SLASH_EQUAL T_PERCENT_EQUAL
%right T_PLUS_EQUAL T_MINUS_EQUAL
%right T_EQUAL
 /* conditional */
%left T_QUESTION T_COLON
 /* binary operator */
%left T_LOGIC_OR
%left T_LOGIC_AND
%left T_BIT_OR
%left T_BIT_XOR
%left T_AMPERSAND
%left T_EQ T_NEQ
%left T_LT T_LE T_GT T_GE
%left T_LSHIFT T_RSHIFT T_ARSHIFT
%left T_PLUS T_MINUS
%left T_ASTERISK T_SLASH T_PERCENT
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
                 | T_MINUS unary_expression { $$ = new A_UnaryExpression($1, $2, Y_POSITION(@1)); }
                 | T_BIT_NOT unary_expression { $$ = new A_UnaryExpression($1, $2, Y_POSITION(@1)); }
                 | T_LOGIC_NOT unary_expression { $$ = new A_UnaryExpression($1, $2, Y_POSITION(@1)); }
                 ;

 /*
cast_expression : unary_expression
                | '(' type_name ')' cast_expression
                ;
 */

binary_expression : unary_expression { $$ = $1; }
                  | binary_expression T_ASTERISK unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_SLASH unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_PERCENT unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_PLUS unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_MINUS unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_LSHIFT unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_RSHIFT unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_ARSHIFT unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_LT unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_LE unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_GT unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_GE unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_EQ unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_NEQ unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_AMPERSAND unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_BIT_XOR unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_BIT_OR unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_LOGIC_AND unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  | binary_expression T_LOGIC_OR unary_expression { $$ = new A_BinaryExpression($1, $2, $3, Y_POSITION(@2)); }
                  ;

binary_operator : T_PLUS
                | T_MUNIS
                | T_ASTERISK
                | T_SLASH
                ;

conditional_expression : binary_expression { $$ = $1; }
                       /* | T_IF T_LPAREN binary_expression T_RPAREN expression T_ELSE conditional_expression { $$ = new A_ConditionalExpression($2, $4, $6); } */
                       | binary_expression T_QUESTION expression T_COLON conditional_expression { $$ = new A_ConditionalExpression($1, $3, $5); }
                       ;

assignment_expression : conditional_expression { $$ = $1; }
                      | unary_expression T_EQUAL assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_ASTERISK_EQUAL assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_SLASH_EQUAL assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_PERCENT_EQUAL assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_PLUS_EQUAL assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_MINUS_EQUAL assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_AMPERSAND_EQUAL assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_BAR_EQUAL assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_CARET_EQUAL assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_LSHIFT_EQUAL assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_RSHIFT_EQUAL assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
                      | unary_expression T_ARSHIFT_EQUAL assignment_expression { $$ = new A_AssignmentExpression($1, $2, $3, Y_POSITION(@2)); }
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
                    | T_VAR T_IDENTIFIER T_EQUAL constant_expression T_SEMI { $$ = new AstVariableDefinition($2, ); std::free($2); }
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
                      | T_BIT_OR T_UNDERSCORE T_COLON statement_list {}
                      ;

match_statement : T_MATCH T_LPAREN argument_expression_list T_RPAREN statement { $$ = new AstMatchStatement($3, $5); }
                ;

match_body_statement : T_BIT_OR assignment_expression T_COLON statement { $$ = new AstMatchBodyStatement($1, $3); }
                     | T_BIT_OR T_UNDERSCORE T_COLON statement {}
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
