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

%type <expr> Literal BooleanLiteral
%type <expr> Id VarId OpId PrefixId PostfixId
%type <expr> OptionalSemi Semi OptionalNl NlSeq Nl
%type <expr> PrefixOp EqualOp
%type <expr> Expr PrefixExpr PostfixExpr InfixExpr SimpleExpr BlockExpr NonBlockExpr
%type <expr> PostfixOp
%type <expr_list> ArgExprs

%type <stmt> compound_statement Expr_statement iteration_statement jump_statement return_statement if_statement
%type <stmt> statement
%type <stmt_list> statement_list

%type <def> VarDef FuncDef FuncSignDef FuncArg
%type <def> Def
%type <def_list> CompileUnit FuncArgs

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
%start CompileUnit

%%

 /**
  * Optional means 0 or 1
  */

 /* literal { */

Literal : T_INTEGER_LITERAL { $$ = new A_IntegerLiteral($1, Y_POSITION(@1)); std::free($1); }
        | T_FLOATING_POINT_LITERAL { $$ = new A_FloatingPointLiteral($1, Y_POSITION(@1)); std::free($1); }
        | BooleanLiteral { $$ = $1; }
        | T_CHARACTER_LITERAL { $$ = new A_CharacterLiteral($1, Y_POSITION(@1)); std::free($1); }
        | T_STRING_LITERAL { $$ = new A_StringLiteral($1, Y_POSITION(@1)); std::free($1); }
        | T_NIL { $$ = new A_NilLiteral(Y_POSITION(@1)); std::free($1); }
        | T_VOID { $$ = new A_VoidLiteral(Y_POSITION(@1)); std::free($1); }
        ;

BooleanLiteral : T_TRUE { $$ = new A_BooleanLiteral($1, Y_POSITION(@1)); std::free($1); }
               | T_FALSE { $$ = new A_BooleanLiteral($1, Y_POSITION(@1)); std::free($1); }
               ;

 /* literal } */

 /* semi and newline { */

OptionalSemi : Semi { $$ = $1; }
             | { $$ = new A_TokenId(A_TokenId::TokenIdCategory::SEMI); }
             ;

Semi : T_SEMI { $$ = new A_TokenId(A_TokenId::TokenIdCategory::SEMI, $1, Y_POSITION(@1));  }
     | T_NEWLINE { $$ = new A_TokenId(A_TokenId::TokenIdCategory::SEMI, $1, Y_POSITION(@1)) }
     ;

OptionalNlSeq : NlSeq
              |
              ;

NlSeq : T_NEWLINE
      | T_NEWLINE NlSeq { $$ = new A_TokenId(A_TokenId::TokenIdCategory::SEMI); }
      ;

OptionalNl : T_NEWLINE { $$ = new A_TokenId(A_TokenId::TokenIdCategory::SEMI, $1, Y_POSITION(@1)); }
           | { $$ = new A_TokenId(A_TokenId::TokenIdCategory::SEMI); }
           ;

 /* semi and newline } */

 /* id { */

Id : VarId { $$ = $1; }
   | OpId { $$ = $1; }
   | PrefixId { $$ = $1; }
   | PostfixId { $$ = $1; }
   ;

VarId : T_VAR_ID { $$ = new A_LiteralId($1, Y_POSITION(@1)); std::free($1); }
       ;

OpId : T_AMPERSAND2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
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

PrefixId : T_PREFIX PrefixOp {
                EX_ASSERT($2->category() == (+AstCategory::TokenId), "$2->category {} == +AstCategory::TokenId", $2->category()._to_string());
                EX_ASSERT(dynamic_cast<A_TokenId*>($2)->count() > 0, "$2->count {} > 0", dynamic_cast<A_TokenId*>($2)->count());
                EX_ASSERT(dynamic_cast<A_TokenId*>($2)->token() != TOKEN_INVALID, "$2->token {} != TOKEN_INVALID", dynamic_cast<A_TokenId*>($2)->token());
                $$ = new A_TokenId(A_TokenId::TokenIdCategory::PREFIX, dynamic_cast<A_TokenId*>($2)->token(), $2->position());
                delete $2;
            }
         ;

PostfixId : T_POSTFIX T_PLUS2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::POSTFIX, $2, Y_POSITION(@1), Y_POSITION(@2)); }
          | T_POSTFIX T_MINUS2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::POSTFIX, $2, Y_POSITION(@1), Y_POSITION(@2)); }
          ;

 /* id } */

 /* Expr { */

Expr : T_IF T_LPAREN Expr T_RPAREN OptionalNlSeq Expr           %prec "lower_than_else" { $$ = new A_IfThenExpression($3, $6); delete $5; }
     | T_IF T_LPAREN Expr T_RPAREN OptionalNlSeq Expr OptionalSemi T_ELSE Expr  { $$ = new A_IfElseExpression($3, $6, $9); delete $5; delete $7; }
     | T_WHILE T_LPAREN Expr T_RPAREN OptionalNlSeq Expr
     /* | T_TRY Expr optional_catch_Expr optinal_finally_Expr */
     /* | T_DO Expr OptionalSemi T_WHILE T_LPAREN Expr T_RPAREN */
     /* | T_FOR T_LPAREN enumerators T_RPAREN OptionalNlSeq Expr */
     | T_THROW Expr { $$ = new A_ThrowExpression($2, Y_POSITION(@1)); }
     | T_RETURN Expr { $$ = new A_ReturnExpression($2, Y_POSITION(@1)); }
     | Id EqualOp Expr { $$ = new A_AssignExpression($1, $2, $3); }
     | PostfixExpr { $$ = $1; }
     ;

EqualOp : T_EQUAL { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
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

PostfixExpr : InfixExpr { $$ = $1; }
            | InfixExpr PostfixOp { $$ = new A_PostfixExpression($1, $2); }
            ;

PostfixOp : Id OptionalNl { $$ = $1; if ($2) { delete $2; } }
          ;

InfixExpr : PrefixExpr { $$ = $1; }
          | InfixExpr InfixOp InfixExpr { $$ = new A_InfixExpression($1, $2, $3); }
                 ;

InfixOp : Id OptionalNl { $$ = $1; if ($2) { delete $2; } }
        ;

PrefixExpr : SimpleExpr { $$ = $1; }
           | PrefixOp SimpleExpr { $$ = new A_PrefixExpression($1, $2); }
           ;

PrefixOp : T_MINUS { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
         | T_PLUS { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
         | T_TILDE { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
         | T_EXCLAM { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
         | T_PLUS2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
         | T_MINUS2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
         ;

SimpleExpr : BlockExpr { $$ = $1; }
           | NonBlockExpr { $$ = $1; }
           ;

NonBlockExpr : Literal { $$ = $1; }
             | T_LPAREN T_RPAREN { $$ = nullptr; }
             | T_LPAREN ExprList T_RPAREN { $$ = nullptr; }
             /* | SimpleExpr T_DOT id */
             | Id { $$ = $1; }
             /* | SimpleExpr TypeArgs */
             /* | NonBlockExpr ArgExprs */
             ;

ExprList : Expr
         | Expr T_COMMA ExprList
         ;

/* ArgExprs : T_LPAREN T_RPAREN */
/*                          | T_LPAREN ExprList T_RPAREN */
/*                          | OptionalNl BlockExpr */
/*                          ; */

BlockExpr : T_LBRACE Block T_RBRACE
          ;

Block : BlockStat OptionalBlockStatSeq OptionalResultExpr
      ;

OptionalBlockStatSeq : BlockStatSeq
                     |
                     ;

BlockStatSeq : Semi BlockStat
             | Semi BlockStat BlockStatSeq { $$ = nullptr; }
             ;

OptionalResultExpr : ResultExpr { $$ = $1; }
                   | { $$ = nullptr; }
                   ;

ResultExpr : Expr { $$ = $1; }
           ;

 /* exprssion } */

 /* statement { */

BlockStat : Import
          | Expr
          | Def
          | TempDef
          |
          ;

 /* statement } */

Def : FuncDef { $$ = $1; }
    | VarDef { $$ = $1; }
    /*| Import_module { $$ = $1; }*/
    ;

 /*
Import_module : T_IMPORT Import_module_list T_SEMI
              ;

Import_module_list : T_IDENTIFIER
                   | T_IDENTIFIER T_DOT Import_module_list 
                   ;
 */


 /**
  * var x = 1;
  * var x:i64 = 100;
  * var x:i64 = 100, y = 1, z:string = "hello";
  */
VarDef : T_VAR T_IDENTIFIER T_SEMI { $$ = new AstVariableDefinition($2, Y_POSITION(@1), Y_POSITION(@3)); std::free($2); }
                    | T_VAR T_IDENTIFIER T_EQUAL constant_Expr T_SEMI { $$ = new AstVariableDefinition($2, ); std::free($2); }
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
FuncDef : FuncSignDef compound_statement { $$ = new AstFunctionDefinition(dynamic_cast<AstFunctionSignatureDefinition*>($1), $2); }
                    | FuncSignDef T_RIGHT_ARROW Expr_statement { $$ = new AstFunctionDefinition(dynamic_cast<AstFunctionSignatureDefinition*>($1), $3); }
                    ;

FuncSignDef : T_DEF T_IDENTIFIER T_LPAREN FuncArgs T_RPAREN {
                                    $$ = new AstFunctionSignatureDefinition($2, $4, nullptr, Y_POSITION(@1), Y_POSITION(@2));
                                    std::free($2);
                                }
                              | T_DEF T_IDENTIFIER T_LPAREN T_RPAREN {
                                    $$ = new AstFunctionSignatureDefinition($2, new AstDefinitionList(), nullptr, Y_POSITION(@1), Y_POSITION(@2));
                                    std::free($2);
                                }
                              ;

FuncArgs : FuncArg { $$ = new AstDefinitionList(); $$->add($1); }
                                  | FuncArg T_COMMA FuncArgs { $3->add($1); $$ = $3; }
                                  ;

FuncArg : T_IDENTIFIER { $$ = new AstFunctionArgumentDefinition($1, Y_POSITION(@1)); std::free($1); }
                             ;

 /* type { */

Type : FuncArgTypes T_THIN_RARROW Type
     | InfixType
     ;

FuncArgTypes : InfixType
             | T_LPAREN OptionalParamTypeSeq T_RPAREN
             ;

OptionalParamTypeSeq : ParamTypeSeq
                     |
                     ;

ParamTypeSeq : ParamType
             | ParamType T_COMMA ParamTypeSeq
             ;

 /* type } */

Def : T_DEF FuncDef
    | TempDef
    | VarDef
    ;

Decl : T_VAR VarDecl
     | T_DEF FuncDecl
     ;

 /* compile unit { */

CompileUnit : TopStatSeq
            ;

TopStatSeq : TopStat
           | TopStat Semi TopStatSeq
           ;

TopStat : TempDef
        | Import
        | Package
        |
        ;

Package : T_PACKAGE Id OptionalNl T_LBRACE TopStatSeq T_RBRACE
        ;

 /* compile unit } */

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
