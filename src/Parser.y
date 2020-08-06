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
    A_ExpressionList *expr_list;
    AstStatementList *stmt_list;
    AstDefinitionList *def_list;
    AstExpression *expr;
    AstStatement *stmt;
    AstDefinition *def;
    /* AstToken *token; */
    char *str;
    int tok;
}

%token <tok> T_EOF

 /* keyword */
%token <tok> T_TRUE T_FALSE T_TRY T_CATCH T_FINALLY T_THROW T_VAR T_VAL T_NIL T_NEW T_DELETE T_DEF T_IF T_THEN T_ELSE T_ENUM T_SWITCH T_CASE T_MATCH T_FOR T_FOREACH T_IN T_WHILE T_DO T_BREAK T_CONTINUE
%token <tok> T_FUNC T_CLASS T_TYPE T_THIS T_SUPER T_ISINSTANCEOF T_ISA T_IS T_IMPORT T_RETURN T_VOID T_NAN T_INF T_ASYNC T_AWAIT T_STATIC T_PUBLIC T_PROTECT T_PRIVATE T_PREFIX T_POSTFIX T_PACKAGE

 /* primitive integer type */
%token <tok> T_BYTE T_UBYTE T_SHORT T_USHORT T_INT T_UINT T_LONG T_ULONG T_LLONG T_ULLONG
 /* primitive float type */
%token <tok> T_FLOAT T_DOUBLE
 /* primitive boolean type */
%token <tok> T_BOOLEAN

 /* and or not operator */
 /* %token <tok> T_AND T_OR T_NOT */

 /* operator */
%token <tok> T_PLUS T_MINUS T_ASTERISK T_SLASH T_PERCENT T_PLUS2 T_MINUS2 T_ASTERISK2 T_SLASH2 T_PERCENT2
 /* operator */
%token <tok> T_AMPERSAND T_AMPERSAND2 T_BAR T_BAR2 T_TILDE T_EXCLAM T_CARET
 /* operator */
%token <tok> T_LSHIFT T_RSHIFT T_ARSHIFT

 /* equal operator */
%token <tok> T_EQUAL T_PLUS_EQUAL T_MINUS_EQUAL T_ASTERISK_EQUAL T_SLASH_EQUAL T_PERCENT_EQUAL T_AMPERSAND_EQUAL T_BAR_EQUAL T_CARET_EQUAL T_LSHIFT_EQUAL T_RSHIFT_EQUAL T_ARSHIFT_EQUAL
 /* compare operator */
%token <tok> T_EQ T_NEQ T_LT T_LE T_GT T_GE
 /* parentheses */
%token <tok> T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET T_LBRACE T_RBRACE
 /* other punctuation */
%token <tok> T_UNDERSCORE T_COMMA T_SEMI T_NEWLINE T_QUESTION T_COLON T_COLON2 T_DOT T_DOT2 T_THIN_LARROW T_THIN_RARROW T_FAT_RARROW

 /* str */
%token <str> T_INTEGER_LITERAL T_FLOATING_POINT_LITERAL T_STRING_LITERAL T_CHARACTER_LITERAL
%token <str> T_VAR_ID

%type <expr> literal boolean_literal
%type <expr> id var_id op_id prefix_id postfix_id
%type <expr> optional_semi semi optional_newline repetible_newline repetible_newline_impl
%type <expr> prefix_operator equal_operator
%type <expr> expression prefix_expression postfix_expression infix_expression simple_expression block_expression nonblock_expression
%type <expr> postfix_operator
%type <expr_list> argument_expression_list

%type <stmt> compound_statement expression_statement iteration_statement jump_statement return_statement if_statement
%type <stmt> statement
%type <stmt_list> statement_list

%type <def> variable_definition function_definition function_signature_definition function_argument_definition
%type <def> definition
%type <def_list> compile_unit function_argument_definition_list

 /* operator precedence, low -> high */

 /* comma */
%left T_COMMA
 /* equal */
%right T_AMPERSAND_EQUAL T_BAR_EQUAL T_CARET_EQUAL
%right T_LSHIFT_EQUAL T_RSHIFT_EQUAL T_ARSHIFT_EQUAL
%right T_ASTERISK_EQUAL T_SLASH_EQUAL T_PERCENT_EQUAL
%right T_PLUS_EQUAL T_MINUS_EQUAL
%right T_EQUAL
 /* conditional */
%left T_QUESTION T_COLON
 /* binary_operator */
%left /* T_OR */ T_BAR2
%left /* T_AND */ T_AMPERSAND2
%left T_BAR
%left T_CARET
%left T_AMPERSAND
%left T_EQ T_NEQ
%left T_LT T_LE T_GT T_GE
%left T_LSHIFT T_RSHIFT T_ARSHIFT
%left T_PLUS T_MINUS
%left T_ASTERISK T_SLASH T_PERCENT
%left T_PLUS2 T_MINUS2
%left T_ASTERISK2 T_SLASH2 T_PERCENT2
%left T_CARET2
%right T_COLON2
 /* other */
%nonassoc /* T_NOT */ T_TILDE T_EXCLAM
%left T_DOT T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET T_LBRACE T_RBRACE
 /* fix if-else shift/reduce */
%nonassoc "lower_than_else"
%nonassoc T_ELSE

 /* top */
%start compile_unit

%%

 /**
  * Naming Rule:
  *
  * optional: 0 or 1
  * repetible: 0 or more
  */

 /* literal { */

literal : T_INTEGER_LITERAL { $$ = new A_IntegerLiteral($1, Y_POSITION(@1)); std::free($1); }
        | T_FLOATING_POINT_LITERAL { $$ = new A_FloatingPointLiteral($1, Y_POSITION(@1)); std::free($1); }
        | boolean_literal { $$ = $1; }
        | T_CHARACTER_LITERAL { $$ = new A_CharacterLiteral($1, Y_POSITION(@1)); std::free($1); }
        | T_STRING_LITERAL { $$ = new A_StringLiteral($1, Y_POSITION(@1)); std::free($1); }
        | T_NIL { $$ = new A_NilLiteral(Y_POSITION(@1)); std::free($1); }
        | T_VOID { $$ = new A_VoidLiteral(Y_POSITION(@1)); std::free($1); }
        ;

boolean_literal : T_TRUE { $$ = new A_BooleanLiteral($1, Y_POSITION(@1)); std::free($1); }
                | T_FALSE { $$ = new A_BooleanLiteral($1, Y_POSITION(@1)); std::free($1); }
                ;

 /* literal } */

 /* semi and newline { */

optional_semi : semi { $$ = $1; }
              | { $$ = new A_TokenId(A_TokenId::TokenIdCategory::SEMI); }
              ;

semi : T_SEMI { $$ = new A_TokenId(A_TokenId::TokenIdCategory::SEMI, $1, Y_POSITION(@1));  }
     | T_NEWLINE { $$ = new A_TokenId(A_TokenId::TokenIdCategory::SEMI, $1, Y_POSITION(@1)) }
     ;

optional_newline : T_NEWLINE { $$ = new A_TokenId(A_TokenId::TokenIdCategory::SEMI, $1, Y_POSITION(@1)); }
                 | { $$ = new A_TokenId(A_TokenId::TokenIdCategory::SEMI); }
                 ;

repetible_newline : repetible_newline_impl { $$ = $1; }
                  | { $$ = new A_TokenId(A_TokenId::TokenIdCategory::SEMI); }
                  ;

repetible_newline_impl : T_NEWLINE { $$ = new A_TokenId(A_TokenId::TokenIdCategory::SEMI, $1, Y_POSITION(@1)); }
                       | T_NEWLINE repetible_newline_impl { $2->join($1, Y_POSITION(@1)); $$ = $2; }
                       ;

 /* semi and newline } */

 /* id { */

id : var_id { $$ = $1; }
   | op_id { $$ = $1; }
   | prefix_id { $$ = $1; }
   | postfix_id { $$ = $1; }
   ;

var_id : T_VAR_ID { $$ = new A_LiteralId($1, Y_POSITION(@1)); std::free($1); }
       ;

op_id : T_AMPERSAND2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_BAR2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_BAR { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_CARET { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_AMPERSAND { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_EQ { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_NEQ { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_LT { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_LE { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_GT { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_GE { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_LSHIFT { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_RSHIFT { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_ARSHIFT { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_PLUS { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_MINUS { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_ASTERISK { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_SLASH { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_PERCENT { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_PLUS2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_MINUS2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_ASTERISK2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_SLASH2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_PERCENT2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_CARET2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      | T_COLON2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
      ;

prefix_id : T_PREFIX prefix_operator {
                EX_ASSERT($2->category() == (+AstCategory::TokenId), "$2->category {} == +AstCategory::TokenId", $2->category()._to_string());
                EX_ASSERT(dynamic_cast<A_TokenId*>($2)->count() > 0, "$2->count {} > 0", dynamic_cast<A_TokenId*>($2)->count());
                EX_ASSERT(dynamic_cast<A_TokenId*>($2)->token() != TOKEN_INVALID, "$2->token {} != TOKEN_INVALID", dynamic_cast<A_TokenId*>($2)->token());
                $$ = new A_TokenId(A_TokenId::TokenIdCategory::PREFIX, dynamic_cast<A_TokenId*>($2)->token(), $2->position());
                delete $2;
            }
          ;

postfix_id : T_POSTFIX T_PLUS2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::POSTFIX, $2, Y_POSITION(@1), Y_POSITION(@2)); }
           | T_POSTFIX T_MINUS2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::POSTFIX, $2, Y_POSITION(@1), Y_POSITION(@2)); }
           ;

 /* id } */

 /* expression { */

expression : T_IF T_LPAREN expression T_RPAREN repetible_newline expression         %prec "lower_than_else" { $$ = new A_IfThenExpression($3, $6); delete $5; }
           | T_IF T_LPAREN expression T_RPAREN repetible_newline expression optional_semi T_ELSE expression { $$ = new A_IfElseExpression($3, $6, $9); delete $5; delete $7; }
           | T_WHILE T_LPAREN expression T_RPAREN repetible_newline expression
           /* | T_TRY expression optional_catch_expression optinal_finally_expression */
           /* | T_DO expression optional_semi T_WHILE T_LPAREN expression T_RPAREN */
           /* | T_FOR T_LPAREN enumerators T_RPAREN repetible_newline expression */
           | T_THROW expression { $$ = new A_ThrowExpression($2, Y_POSITION(@1)); }
           | T_RETURN expression { $$ = new A_ReturnExpression($2, Y_POSITION(@1)); }
           | id equal_operator expression { $$ = new A_AssignExpression($1, $2, $3); }
           | postfix_expression { $$ = $1; }
           ;

equal_operator : T_EQUAL { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
               | T_ASTERISK_EQUAL { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
               | T_SLASH_EQUAL { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
               | T_PERCENT_EQUAL { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
               | T_PLUS_EQUAL { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
               | T_MINUS_EQUAL { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
               | T_AMPERSAND_EQUAL { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
               | T_BAR_EQUAL { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
               | T_CARET_EQUAL { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
               | T_LSHIFT_EQUAL { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
               | T_RSHIFT_EQUAL { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
               | T_ARSHIFT_EQUAL { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
               ;

postfix_expression : infix_expression { $$ = $1; }
                   | infix_expression postfix_operator { $$ = new A_PostfixExpression($1, $2); }
                   ;

postfix_operator : id optional_newline { $$ = $1; if ($2) { delete $2; } }
                 ;

infix_expression : prefix_expression { $$ = $1; }
                 | infix_expression infix_operator infix_expression { $$ = new A_InfixExpression($1, $2, $3); }
                 ;

infix_operator : id optional_newline { $$ = $1; if ($2) { delete $2; } }
               ;

prefix_expression : simple_expression { $$ = $1; }
                  | prefix_operator simple_expression { $$ = new A_PrefixExpression($1, $2); }
                  ;

prefix_operator : T_MINUS { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
                | T_PLUS { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
                | T_TILDE { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
                | T_EXCLAM { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
                | T_PLUS2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
                | T_MINUS2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
                ;

simple_expression : block_expression { $$ = $1; }
                  | nonblock_expression { $$ = $1; }
                  ;

nonblock_expression : literal { $$ = $1; }
                    | T_LPAREN T_RPAREN { $$ = nullptr; }
                    | T_LPAREN expression_list T_RPAREN { $$ = nullptr; }
                    /* | simple_expression T_DOT id */
                    | id { $$ = $1; }
                    /* | simple_expression type_arg_list */
                    /* | nonblock_expression argument_expression_list */
                    ;

expression_list : expression
                | expression T_COMMA expression_list
                ;

/* argument_expression_list : T_LPAREN T_RPAREN */
/*                          | T_LPAREN expression_list T_RPAREN */
/*                          | optional_newline block_expression */
/*                          ; */

block_expression : T_LBRACE block T_RBRACE
                 ;

block : block_statement repetible_block_statement optional_result_expression
      ;

repetible_block_statement : repetible_block_statement_impl
                          | { $$ = nullptr; }
                          ;

repetible_block_statement_impl : semi block_statement
                               | semi block_statement repetible_block_statement_impl
                               ;

optional_result_expression : result_expression { $$ = $1; }
                           | { $$ = nullptr; }
                           ;

result_expression : expression { $$ = $1; }
                  ;

 /* exprssion } */

 /* statement { */

block_statement : import
                | expression
                | definition
                | template_definition
                |
                ;

 /* statement } */

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

type : function_argument_type_list T_THIN_RARROW type
     | infix_type
     ;

function_argument_type_list : infix_type
                            | T_LPAREN repetible_parameter_type T_RPAREN
                            ;

repetible_parameter_type : repetible_parameter_type_impl
                         |
                         ;

repetible_parameter_type_impl : parameter_type
                              | parameter_type T_COMMA repetible_parameter_type_impl
                              ;

definition : T_DEF function_definition
           | template_definition
           | variable_definition
           ;

declaration : T_VAR variable_declaration
            | T_DEF function_declaration
            ;

compile_unit : top_statement_sequence
             ;

top_statement_sequence : top_statement
                       | top_statement semi top_statement_sequence
                       ;

top_statement : template_definition
              | import
              | package
              |
              ;

package : T_PACKAGE id optional_newline T_LBRACE top_statement_sequence T_RBRACE
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
