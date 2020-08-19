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
    AstDef *def;
    AstExpr *expr;
    char *str;
    int tok;
}

%token <tok> T_EOF

 /* keyword */
%token <tok> T_TRUE T_FALSE T_TRY T_CATCH T_FINALLY T_THROW T_YIELD T_VAR T_VAL T_NIL T_NEW T_DELETE T_DEF T_IF T_THEN T_ELSE T_MATCH T_ENUM T_SWITCH T_CASE T_MATCH T_FOR T_FOREACH T_IN T_WHILE T_DO T_BREAK T_CONTINUE
%token <tok> T_CLASS T_TRAIT T_TYPE T_THIS T_SUPER T_ISINSTANCEOF T_ISA T_IS T_IMPORT T_AS T_RETURN T_VOID T_NAN T_INF T_ASYNC T_AWAIT T_STATIC T_PUBLIC T_PROTECT T_PRIVATE T_PREFIX T_POSTFIX T_PACKAGE

 /* primitive integer type */
%token <tok> T_BYTE T_UBYTE T_SHORT T_USHORT T_INT T_UINT T_LONG T_ULONG T_LLONG T_ULLONG
 /* primitive float type */
%token <tok> T_FLOAT T_DOUBLE
 /* primitive boolean type */
%token <tok> T_BOOLEAN
 /* primitive character type */
%token <tok> T_CHAR

 /* and or not operator */
%token <tok> T_AND T_OR T_NOT

 /* operator */
%token <tok> T_PLUS T_MINUS T_STAR T_SLASH T_PERCENT T_PLUS2 T_MINUS2 T_STAR2 T_SLASH2 T_PERCENT2
 /* operator */
%token <tok> T_AMPERSAND T_AMPERSAND2 T_BAR T_BAR2 T_TILDE T_EXCLAM T_CARET
 /* operator */
%token <tok> T_LSHIFT T_RSHIFT T_ARSHIFT

 /* equal operator */
%token <tok> T_EQUAL T_PLUS_EQUAL T_MINUS_EQUAL T_STAR_EQUAL T_SLASH_EQUAL T_PERCENT_EQUAL T_AMPERSAND_EQUAL T_BAR_EQUAL T_CARET_EQUAL T_LSHIFT_EQUAL T_RSHIFT_EQUAL T_ARSHIFT_EQUAL
 /* compare operator */
%token <tok> T_EQ T_NEQ T_LT T_LE T_GT T_GE
 /* parentheses */
%token <tok> T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET T_LBRACE T_RBRACE
 /* other punctuation */
%token <tok> T_UNDERSCORE T_COMMA T_SEMI T_QUESTION T_COLON T_COLON2 T_DOT T_DOT2 T_LARROW T_RARROW T_DOUBLE_RARROW
%token <tok> T_NEWLINE

 /* semi */
%token <tok> Semi OptionalSemi OptionalNewline OptionalNewlines Newlines
 /* Operator */
%token <tok> AssignOp PrefixOp InfixOp PostfixOp

 /* str */
%token <str> T_INTEGER_LITERAL T_FLOAT_LITERAL T_STRING_LITERAL T_CHARACTER_LITERAL
%token <str> T_VAR_ID

%type <expr> Literal BooleanLiteral
%type <expr> Id VarId
%type <expr> PrefixOp EqualOp
%type <expr> Expr PrefixExpr PostfixExpr InfixExpr SimpleExpr NonBlockExpr
%type <expr> PostfixOp
%type <expr> ArgExprs

%type <expr> compound_statement Expr_statement iteration_statement jump_statement return_statement if_statement
%type <expr> statement
%type <expr> statement_list

%type <def> VarDef FuncDef FuncSignDef FuncArg
%type <def> Def
%type <def> CompileUnit FuncArgs

 /* operator precedence, low -> high */

 /* comma */
%left T_COMMA /* , */

 /* equal */
%right T_AMPERSAND_EQUAL T_BAR_EQUAL T_CARET_EQUAL /* &= |= ^= */
%right T_LSHIFT_EQUAL T_RSHIFT_EQUAL T_ARSHIFT_EQUAL /* >>= <<= >>>= */
%right T_STAR_EQUAL T_SLASH_EQUAL T_PERCENT_EQUAL /* *= /= %= */
%right T_PLUS_EQUAL T_MINUS_EQUAL /* += -= */
%right T_EQUAL /* = */

 /* conditional */
%left T_QUESTION T_COLON /* ? : */

 /* binary_operator */
%left T_OR T_BAR2 /* or || */
%left T_AND T_AMPERSAND2 /* and && */
%left T_BAR /* | */
%left T_CARET /* ^ */
%left T_AMPERSAND /* & */
%left T_EQ T_NEQ /* == != */
%left T_LT T_LE T_GT T_GE /* < <= > >= */
%left T_LSHIFT T_RSHIFT T_ARSHIFT /* << >> >>> */
%left T_PLUS T_MINUS /* + - */
%left T_STAR T_SLASH T_PERCENT /* * / % */
%left T_PLUS2 T_MINUS2 /* ++ -- */
%left T_STAR2 T_SLASH2 T_PERCENT2 /* ** // %% */
%left T_CARET2 /* ^^ */
%right T_COLON2 /* :: */
 /* other */
%nonassoc T_NOT T_TILDE T_EXCLAM /* not ~ ! */
%left T_DOT T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET T_LBRACE T_RBRACE /* . () [] {} */
 /* fix if-else shift/reduce */
%nonassoc "lower_than_else"
%nonassoc T_ELSE

 /* top */
%start CompileUnit

%%

 /* semi and newline { */

Semi : T_SEMI
     | T_NEWLINE
     ;

OptionalSemi : Semi
             |
             ;

OptionalNewline : T_NEWLINE
                |
                ;

OptionalNewlines : Newlines
                 |
                 ;

Newlines : T_NEWLINE
         | T_NEWLINE Newlines
         ;

 /* semi and newline } */

 /* literal { */

Literal : T_INTEGER_LITERAL { $$ = new A_Integer($1, Y_POSITION(@1)); std::free($1); }
        | T_FLOAT_LITERAL { $$ = new A_Float($1, Y_POSITION(@1)); std::free($1); }
        | BooleanLiteral { $$ = $1; }
        | T_CHARACTER_LITERAL { $$ = new A_Character($1, Y_POSITION(@1)); std::free($1); }
        | T_STRING_LITERAL { $$ = new A_String($1, Y_POSITION(@1)); std::free($1); }
        | T_NIL { $$ = new A_Nil(Y_POSITION(@1)); std::free($1); }
        | T_VOID { $$ = new A_Void(Y_POSITION(@1)); std::free($1); }
        ;

BooleanLiteral : T_TRUE { $$ = new A_Boolean($1, Y_POSITION(@1)); std::free($1); }
               | T_FALSE { $$ = new A_Boolean($1, Y_POSITION(@1)); std::free($1); }
               ;

 /* literal } */

 /* id { */

Id : VarId { $$ = $1; }
   /* | OpId */
   ;

VarId : T_VAR_ID { $$ = new A_VarId($1, Y_POSITION(@1)); std::free($1); }
      ;

/* OpId : AssignOp */
/*      | InfixOp */
/*      | T_PREFIX PrefixOp */
/*      | T_POSTFIX PostfixOp */
/*      ; */

 /* id } */

 /* expression { */

Expr : T_IF T_LPAREN Expr T_RPAREN OptionalNewlines Expr OptionalElse
     | T_WHILE T_LPAREN Expr T_RPAREN OptionalNewlines Expr
     | T_TRY Expr OptionalCatch OptionalFinally
     | T_DO Expr OptionalSemi T_WHILE T_LPAREN Expr T_RPAREN
     | T_FOR T_LPAREN Enumerators T_RPAREN OptionalNewlines OptionalYield Expr
     | T_THROW Expr
     | T_RETURN OptionalExpr
     | AssignExpr
     | PostfixExpr
     ;

OptionalElse : OptionalSemi T_ELSE Expr
             |
             ;

OptionalCatch : T_CATCH Expr
              |
              ;

OptionalFinally : T_FINALLY Expr
                |
                ;

Enumerators : OptionalForInit Semi OptionalExpr Semi OptionalExpr
            | Id T_LARROW Expr
            ;

OptionalYield : T_YIELD
              |
              ;

OptionalExpr : Expr
             |
             ;

AssignExpr : Id AssignOp Expr
           ;

AssignOp : T_EQUAL { $$ = $1; }
         | T_STAR_EQUAL { $$ = $1; }
         | T_SLASH_EQUAL { $$ = $1; }
         | T_PERCENT_EQUAL { $$ = $1; }
         | T_PLUS_EQUAL { $$ = $1; }
         | T_MINUS_EQUAL { $$ = $1; }
         | T_AMPERSAND_EQUAL { $$ = $1; }
         | T_BAR_EQUAL { $$ = $1; }
         | T_CARET_EQUAL { $$ = $1; }
         | T_LSHIFT_EQUAL { $$ = $1; }
         | T_RSHIFT_EQUAL { $$ = $1; }
         | T_ARSHIFT_EQUAL { $$ = $1; }
         ;

PostfixExpr : InfixExpr
            | InfixExpr PostfixOp OptionalNewline
            ;

PostfixOp : T_PLUS2
          | T_MINUS2
          ;

InfixExpr : PrefixExpr
          | InfixExpr InfixOp OptionalNewline InfixExpr
          ;

InfixOp : T_BAR2
        | T_AMPERSAND2
        | T_BAR
        | T_CARET
        | T_AMPERSAND
        | T_EQ
        | T_NEQ
        | T_LT
        | T_LE
        | T_GT
        | T_GE
        | T_LSHIFT
        | T_RSHIFT
        | T_ARSHIFT
        | T_PLUS
        | T_MINUS
        | T_STAR
        | T_SLASH
        | T_PERCENT
        | T_PLUS2
        | T_MINUS2
        | T_STAR2
        | T_SLASH2
        | T_PERCENT2
        | T_CARET2
        | T_COLON2
        ;

PrefixExpr : PrimaryExpr { $$ = $1; }
           | PrefixOp PrimaryExpr { $$ = new A_PrefixExpression($1, $2); }
           ;

PrefixOp : T_MINUS { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
         | T_PLUS { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
         | T_TILDE { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
         | T_EXCLAM { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
         | T_PLUS2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
         | T_MINUS2 { $$ = new A_TokenId(A_TokenId::TokenIdCategory::OP, $1, Y_POSITION(@1)); }
         ;

PrimaryExpr : Literal { $$ = $1; }
            | Id { $$ = $1; }
            | T_LPAREN OptionalExprs T_RPAREN { $$ = $1; }
            | CallExpr { $$ = $1; }
            | Block
            ;

OptionalExprs : Exprs
              |
              ;

Exprs : Expr
      | Expr T_COMMA Exprs
      ;

CallExpr : Id T_LPAREN OptionalExprs T_RPAREN { $$ = $1; }
         ;

Block : T_LBRACE BlockStat OptionalBlockStats T_RBRACE
      ;

BlockStat : Expr
          | Def
          /* | Import */
          |
          ;

OptionalBlockStats : BlockStats
                   |
                   ;

BlockStats : Semi BlockStat
           | Semi BlockStat BlockStats
           ;

 /* expression } */

 /* type { */

Type : PlainType
     /* | FuncArgTypes T_RARROW Type */
     /* | IdType */
     ;

/* FuncArgTypes : T_LPAREN T_RPAREN */
/*              | T_LPAREN ParamTypes T_RPAREN */
/*              ; */

/* ParamTypes : ParamType */
/*            | ParamType T_COMMA ParamTypes */
/*            ; */

/* ParamType : Type */
/*           ; */

PlainType : T_BYTE
          | T_UBYTE
          | T_SHORT
          | T_USHORT
          | T_INT
          | T_UINT
          | T_LONG
          | T_ULONG
          | T_LLONG
          | T_ULLONG
          | T_FLOAT
          | T_DOUBLE
          | T_BOOLEAN
          | T_VOID
          ;

/* IdType : Id */
/*        ; */

 /* type } */

 /* definition declaration { */

Def : FuncDef
    /* | ClassDef */
    /* | TraitDef */
    | VarDef
    ;

FuncDef : T_DEF FuncSign OptionalResultType T_DOUBLE_RARROW Expr
        | T_DEF FuncSign OptionalNewlines Block
        ;

OptionalResultType : T_COLON Type
                   |
                   ;

FuncSign : Id ParamClause
         ;

ParamClause : T_LPAREN Params T_RPAREN
            | T_LPAREN T_RPAREN
            ;

Params : Param
       | Param T_COMMA Params
       ;

Param : Id T_COLON Type
      /* | Id T_COLON Type T_EQUAL Expr */
      ;

VarDef : T_VAR Id T_COLON Type T_EQUAL Expr
       ;

/* Decl : T_VAR VarDecl */
/*      | T_DEF FuncDecl */
/*      ; */

/* FuncDecl : FuncSign T_COLON Type */
/*          ; */

/* VarDecl : Id T_COLON Type */
/*         ; */

 /* definition declaration } */

 /* compile unit { */

CompileUnit : TopStatSeq
            |
            ;

TopStatSeq : TopStat OptionalTopStats
           ;

TopStat : Def
        /* | Import */
        /* | Package */
        |
        ;

OptionalTopStats : TopStats
                 |
                 ;

TopStats : Semi TopStat
         | Semi TopStat TopStats
         ;

/* Package : T_PACKAGE Id T_LBRACE T_RBRACE */
/*         | T_PACKAGE Id T_LBRACE TopStats T_RBRACE */
/*         ; */

/* Import : */
/*        ; */

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
