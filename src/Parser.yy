%define api.pure full
%locations
%param { yyscan_t yyscanner }

%code top {
#include <cstdarg>
#include <cstdio>
#include "Ast.h"
#include "Buffer.h"
#include "Scanner.h"
#include "Position.h"
#include "Exception.h"
#include "Tokenizer.yy.h"
#define Y_POS(x) Position((x).first_line, (x).first_column, (x).last_line, (x).last_column)
}

%code requires {
class Scanner;
class Ast;
}

%union {
    Ast *ast;
    char *str;
    int tok;
}

%token <tok> T_EOF T_EMPTY

 /* keyword */
%token <tok> T_TRUE T_FALSE T_TRY T_CATCH T_FINALLY T_THROW T_YIELD T_VAR T_VAL T_NIL T_NEW T_DELETE T_DEF T_IF T_THEN T_ELSE T_MATCH T_ENUM T_SWITCH T_CASE T_FOR T_FOREACH T_IN T_WHILE T_DO T_BREAK T_CONTINUE
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
%token <tok> T_UNDERSCORE T_COMMA T_SEMI T_QUESTION T_COLON T_COLON2 T_DOT T_DOT2 T_LARROW T_RARROW T_DOUBLE_RARROW
%token <tok> T_NEWLINE

 /* str */
%token <str> T_INTEGER_LITERAL T_FLOAT_LITERAL T_STRING_LITERAL T_CHARACTER_LITERAL
%token <str> T_VAR_ID

 /* semi */
%type <tok> semi optionalSemi optionalNewline optionalNewlines newlines
 /* operator */
%type <tok> assignOp prefixOp infixOp postfixOp

 /* literal */
%type <ast> literal booleanLiteral
 /* id */
%type <ast> id varId
 /* expr */
%type <ast> expr enumerators assignExpr prefixExpr postfixExpr infixExpr primaryExpr exprs callExpr block blockStat blockStats
%type <ast> optionalElse optionalCatch optionalFinally optionalYield optionalExpr optionalBlockStats optionalForInit
 /* type */
%type <ast> type plainType
 /* def */
%type <ast> def funcDef varDef funcSign paramClause params param
%type <ast> optionalResultType optionalParams
 /* compile unit */
%type <ast> compileUnit topStatSeq topStat topStats
%type <ast> optionalTopStats

 /* operator precedence, low -> high */
 /* comma */
%left T_COMMA /* , */

 /* equal */
%right T_AMPERSAND_EQUAL T_BAR_EQUAL T_CARET_EQUAL /* &= |= ^= */
%right T_LSHIFT_EQUAL T_RSHIFT_EQUAL T_ARSHIFT_EQUAL /* >>= <<= >>>= */
%right T_ASTERISK_EQUAL T_SLASH_EQUAL T_PERCENT_EQUAL /* *= /= %= */
%right T_PLUS_EQUAL T_MINUS_EQUAL /* += -= */
%right T_EQUAL /* = */

 /* conditional */
%left T_QUESTION T_COLON /* ? : */

 /* binary_operator */
%left T_BAR2 /* || */
%left T_OR /* or */
%left T_AMPERSAND2 /* && */
%left T_AND /* and */
%left T_BAR /* | */
%left T_CARET /* ^ */
%left T_AMPERSAND /* & */
%left T_EQ T_NEQ /* == != */
%left T_LT T_LE T_GT T_GE /* < <= > >= */
%left T_LSHIFT T_RSHIFT T_ARSHIFT /* << >> >>> */
%left T_PLUS T_MINUS /* + - */
%left T_ASTERISK T_SLASH T_PERCENT /* * / % */
%left T_PLUS2 T_MINUS2 /* ++ -- */
%left T_ASTERISK2 T_SLASH2 T_PERCENT2 /* ** // %% */
%left T_CARET2 /* ^^ */
%right T_COLON2 /* :: */
 /* other */
%nonassoc T_NOT T_TILDE T_EXCLAM /* not ~ ! */
%left T_DOT T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET T_LBRACE T_RBRACE /* . () [] {} */
 /* fix if-else shift/reduce */
%nonassoc "lower_than_else"
%nonassoc T_ELSE

 /* top */
%start compileUnit

%%

 /* semi and newline { */

semi : T_SEMI { $$ = $1; }
     | T_NEWLINE { $$ = $1; }
     ;

optionalSemi : semi { $$ = $1; }
             | %empty { $$ = T_EMPTY; }
             ;

optionalNewline : T_NEWLINE { $$ = $1; }
                | %empty { $$ = T_EMPTY; }
                ;

optionalNewlines : newlines { $$ = $1; }
                 | %empty { $$ = T_EMPTY; }
                 ;

newlines : T_NEWLINE { $$ = $1; }
         | newlines T_NEWLINE { $$ = T_EMPTY; }
         ;

 /* semi and newline } */

 /* literal { */

literal : T_INTEGER_LITERAL { $$ = new A_Integer($1, Y_POS(@1)); std::free($1); }
        | T_FLOAT_LITERAL { $$ = new A_Float($1, Y_POS(@1)); std::free($1); }
        | booleanLiteral { $$ = $1; }
        | T_CHARACTER_LITERAL { $$ = new A_Character($1, Y_POS(@1)); std::free($1); }
        | T_STRING_LITERAL { $$ = new A_String($1, Y_POS(@1)); std::free($1); }
        | T_NIL { $$ = new A_Nil(Y_POS(@1)); std::free($1); }
        | T_VOID { $$ = new A_Void(Y_POS(@1)); std::free($1); }
        ;

booleanLiteral : T_TRUE { $$ = new A_Boolean($1, Y_POS(@1)); std::free($1); }
               | T_FALSE { $$ = new A_Boolean($1, Y_POS(@1)); std::free($1); }
               ;

 /* literal } */

 /* id { */

id : varId { $$ = $1; }
   /* | Opid */
   ;

varId : T_VAR_ID { $$ = new A_varId($1, Y_POS(@1)); std::free($1); }
      ;

/* Opid : assignOp */
/*      | infixOp */
/*      | T_PREFIX prefixOp */
/*      | T_POSTFIX postfixOp */
/*      ; */

 /* id } */

 /* expression { */

expr : T_IF T_LPAREN expr T_RPAREN optionalNewlines expr optionalElse { $$ = nullptr; }
     | T_WHILE T_LPAREN expr T_RPAREN optionalNewlines expr { $$ = nullptr; }
     | T_TRY expr optionalCatch optionalFinally { $$ = nullptr; }
     | T_DO expr optionalSemi T_WHILE T_LPAREN expr T_RPAREN { $$ = nullptr; }
     | T_FOR T_LPAREN enumerators T_RPAREN optionalNewlines optionalYield expr { $$ = nullptr; }
     | T_THROW expr { $$ = nullptr; }
     | T_RETURN optionalExpr { $$ = nullptr; }
     | assignExpr { $$ = nullptr; }
     | postfixExpr { $$ = nullptr; }
     ;

optionalElse : optionalSemi T_ELSE expr { $$ = nullptr; }
             | %empty { $$ = nullptr; }
             ;

optionalCatch : T_CATCH expr { $$ = nullptr; }
              | %empty { $$ = nullptr; }
              ;

optionalFinally : T_FINALLY expr { $$ = nullptr; }
                | %empty { $$ = nullptr; }
                ;

enumerators : optionalForInit semi optionalExpr semi optionalExpr { $$ = nullptr; }
            | id T_LARROW expr { $$ = nullptr; }
            ;

optionalForInit : varDef { $$ = nullptr; }
                | %empty { $$ = nullptr; }
                ;

optionalExpr : expr { $$ = nullptr; }
             | %empty { $$ = nullptr; }
             ;

optionalYield : T_YIELD { $$ = nullptr; }
              | %empty { $$ = nullptr; }
              ;

assignExpr : id assignOp expr { $$ = nullptr; }
           ;

assignOp : T_EQUAL { $$ = $1; }
         | T_ASTERISK_EQUAL { $$ = $1; }
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

postfixExpr : infixExpr { $$ = nullptr; }
            | infixExpr postfixOp { $$ = nullptr; }
            ; 

postfixOp : T_PLUS2 { $$ = $1; }
          | T_MINUS2 { $$ = $1; }
          ;

infixExpr : prefixExpr { $$ = nullptr; }
          | infixExpr infixOp optionalNewline prefixExpr { $$ = nullptr; }
          ;

infixOp : T_BAR2 { $$ = $1; }
        | T_AMPERSAND2 { $$ = $1; }
        | T_BAR { $$ = $1; }
        | T_CARET { $$ = $1; }
        | T_AMPERSAND{ $$ = $1; }
        | T_EQ{ $$ = $1; }
        | T_NEQ{ $$ = $1; }
        | T_LT{ $$ = $1; }
        | T_LE{ $$ = $1; }
        | T_GT{ $$ = $1; }
        | T_GE{ $$ = $1; }
        | T_LSHIFT{ $$ = $1; }
        | T_RSHIFT{ $$ = $1; }
        | T_ARSHIFT{ $$ = $1; }
        | T_PLUS{ $$ = $1; }
        | T_MINUS{ $$ = $1; }
        | T_ASTERISK{ $$ = $1; }
        | T_SLASH{ $$ = $1; }
        | T_PERCENT{ $$ = $1; }
        | T_ASTERISK2{ $$ = $1; }
        | T_SLASH2{ $$ = $1; }
        | T_PERCENT2{ $$ = $1; }
        | T_CARET2{ $$ = $1; }
        | T_COLON2{ $$ = $1; }
        ;

prefixExpr : primaryExpr { $$ = $1; }
           | prefixOp primaryExpr { $$ = new A_prefixExpression($1, $2); }
           ;

prefixOp : T_MINUS { $$ = $1; }
         | T_PLUS { $$ = $1; }
         | T_TILDE { $$ = $1; }
         | T_EXCLAM { $$ = $1; }
         | T_PLUS2 { $$ = $1; }
         | T_MINUS2 { $$ = $1; }
         ;

primaryExpr : literal { $$ = $1; }
            | id { $$ = $1; }
            | T_LPAREN optionalExprs T_RPAREN { $$ = $1; }
            | callExpr { $$ = $1; }
            | block { $$ = nullptr; }
            ;

optionalExprs : exprs { $$ = nullptr; }
              | %empty { $$ = nullptr; }
              ;

exprs : expr { $$ = nullptr; }
      | exprs T_COMMA expr { $$ = nullptr; }
      ;

callExpr : id T_LPAREN optionalExprs T_RPAREN { $$ = $1; }
         ;

block : T_LBRACE blockStat optionalBlockStats T_RBRACE { $$ = nullptr; }
      ;

blockStat : expr { $$ = nullptr; }
          | def { $$ = nullptr; }
          /* | Import */
          | %empty { $$ = nullptr; }
          ;

optionalBlockStats : blockStats { $$ = nullptr; }
                   | %empty { $$ = nullptr; }
                   ;

blockStats : semi blockStat { $$ = nullptr; }
           | blockStats semi blockStat { $$ = nullptr; }
           ;

 /* expression } */

 /* type { */

type : plainType { $$ = nullptr; }
     /* | FuncArgtypes T_RARROW type */
     /* | idtype */
     ;

/* FuncArgtypes : T_LPAREN T_RPAREN */
/*              | T_LPAREN paramtypes T_RPAREN */
/*              ; */

/* paramtypes : paramtype */
/*            | paramtype T_COMMA paramtypes */
/*            ; */

/* paramtype : type */
/*           ; */

plainType : T_BYTE { $$ = nullptr; }
          | T_UBYTE { $$ = nullptr; }
          | T_SHORT { $$ = nullptr; }
          | T_USHORT { $$ = nullptr; }
          | T_INT { $$ = nullptr; }
          | T_UINT { $$ = nullptr; }
          | T_LONG { $$ = nullptr; }
          | T_ULONG { $$ = nullptr; }
          | T_LLONG { $$ = nullptr; }
          | T_ULLONG { $$ = nullptr; }
          | T_FLOAT { $$ = nullptr; }
          | T_DOUBLE { $$ = nullptr; }
          | T_BOOLEAN { $$ = nullptr; }
          | T_VOID { $$ = nullptr; }
          ;

/* idtype : id */
/*        ; */

 /* type } */

 /* definition declaration { */

def : funcDef { $$ = nullptr; }
    /* | Classdef */
    /* | Traitdef */
    | varDef { $$ = nullptr; }
    ;

funcDef : T_DEF funcSign optionalResultType T_EQUAL expr { $$ = nullptr; }
        | T_DEF funcSign resultType optionalNewlines block { $$ = nullptr; }
        ;

optionalResultType : resultType { $$ = nullptr; }
                   | %empty { $$ = nullptr; }
                   ;

resultType : T_COLON type
           ;

funcSign : id paramClause { $$ = nullptr; }
         ;

paramClause : T_LPAREN optionalParams T_RPAREN { $$ = nullptr; }
            ;

optionalParams : params { $$ = nullptr; }
               | %empty { $$ = nullptr; }
               ;

params : param { $$ = nullptr; }
       | params T_COMMA param { $$ = nullptr; }
       ;

param : id T_COLON type { $$ = nullptr; }
      /* | id T_COLON type T_EQUAL expr */
      ;

varDef : T_VAR id T_COLON type T_EQUAL expr { $$ = nullptr; }
       ;

/* Decl : T_VAR VarDecl */
/*      | T_DEF FuncDecl */
/*      ; */

/* FuncDecl : funcSign T_COLON type */
/*          ; */

/* VarDecl : id T_COLON type */
/*         ; */

 /* definition declaration } */

 /* compile unit { */

compileUnit : topStatSeq { $$ = nullptr; }
            ;

topStatSeq : topStat optionalTopStats { $$ = nullptr; }
           ;

optionalTopStats : topStats { $$ = nullptr; }
                 | %empty { $$ = nullptr; }
                 ;

topStats : semi topStat { $$ = nullptr; }
         | topStats semi topStat { $$ = nullptr; }
         ;

topStat : def { $$ = nullptr; }
        /* | Import */
        /* | Package */
        | %empty { $$ = nullptr; }
        ;

/* Package : T_PACKAGE id T_LBRACE T_RBRACE */
/*         | T_PACKAGE id T_LBRACE topStats T_RBRACE */
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
            yyget_extra(yyscanner)->currentBuffer().c_str(),
            yyllocp->first_line,
            yyllocp->first_column, 
            yyllocp->last_line, 
            yyllocp->last_column);
  }
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}
