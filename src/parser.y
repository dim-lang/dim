%require "3.2"
%language "c++"
%define lr.type ielr
%define api.value.type variant
%define api.token.constructor
/* %define api.pure full */
%define parse.assert
%define parse.trace
%define parse.error verbose
%define parse.lac full
%locations

%code top {
#include <cstdarg>
#include <cstdio>
#include <memory>
#include "Ast.h"
#include "Buffer.h"
#include "Position.h"
#include "tokenizer.yy.hh"
void yyerror(YYLTYPE *yyllocp, yyscan_t yyscanner, const char *fmt, ...);
#define Y_POS(x) Position((x).first_line, (x).first_column, (x).last_line, (x).last_column)
}

%code requires {
#include "tokenizer.yy.hh"
class Ast;
}

%param { yyscan_t yyscanner }

 /* %union { */
 /*     std::shared_ptr<Ast> ast; */
 /*     std::string str; */
 /*     int tok; */
 /* } */

 /* token { */

 /* keyword */
%token <int> TRUEE "true"
%token <int> FALSEE "false"
%token <int> TRY "try"
%token <int> CATCH "catch"
%token <int> FINALLY "finally"
%token <int> THROW "throw"
%token <int> YIELD "yield"
%token <int> VAR "var"
%token <int> VAL "val"
%token <int> NIL "nil"
%token <int> NEW "new"
%token <int> DELETE "delete"
%token <int> DEF "def"
%token <int> IF "if"
%token <int> THEN "then"
%token <int> ELSE "else"
%token <int> SEMI_ELSE "semi_else"
%token <int> MATCH "match"
%token <int> ENUM "enum"
%token <int> SWITCH "switch"
%token <int> CASE "case"
%token <int> FOR "for"
%token <int> FOREACH "foreach"
%token <int> IN "in"
%token <int> WHILE "while"
%token <int> SEMI_WHILE "semi_while"
%token <int> DO "do"
%token <int> BREAK "break"
%token <int> CONTINUE "continue"
%token <int> CLASS "class"
%token <int> TRAIT "trait"
%token <int> TYPE "type"
%token <int> THIS "this"
%token <int> SUPER "super"
%token <int> ISINSTANCEOF "isinstanceof"
%token <int> ISA "isa"
%token <int> IS "is"
%token <int> IMPORT "import"
%token <int> AS "as"
%token <int> RETURN "return"
%token <int> VOID "void"
%token <int> ANY "any"
%token <int> NAN "nan"
%token <int> INF "inf"
%token <int> ASYNC "async"
%token <int> AWAIT "await"
%token <int> STATIC "static"
%token <int> PUBLIC "public"
%token <int> PROTECT "protect"
%token <int> PRIVATE "private"
%token <int> PREFIX "prefix"
%token <int> POSTFIX "postfix"
%token <int> PACKAGE "package"

 /* primitive type */
%token <int> BYTE "byte"
%token <int> UBYTE "ubyte"
%token <int> SHORT "short"
%token <int> USHORT "ushort"
%token <int> INT "int"
%token <int> UINT "uint"
%token <int> LONG "long"
%token <int> ULONG "ulong"
%token <int> LLONG "llong"
%token <int> ULLONG "ullong"
%token <int> FLOAT "float"
%token <int> DOUBLE "double"
%token <int> BOOLEAN "boolean"
%token <int> CHAR "char"

 /* operator */
%token <int> AND "and"
%token <int> OR "or"
%token <int> NOT "not"

%token <int> PLUS "+"
%token <int> PLUS2 "++"
%token <int> MINUS "-"
%token <int> MINUS2 "--"
%token <int> ASTERISK "*"
%token <int> ASTERISK2 "**"
%token <int> SLASH "/"
%token <int> SLASH2 "//"
%token <int> PERCENT "%"
%token <int> PERCENT2 "%%"

%token <int> AMPERSAND "&"
%token <int> AMPERSAND2 "&&"
%token <int> BAR "|"
%token <int> BAR2 "||"
%token <int> TILDE "~"
%token <int> EXCLAM "!"
%token <int> CARET "^"
%token <int> CARET2 "^^"

%token <int> LSHIFT "<<"
%token <int> RSHIFT ">>"
%token <int> ARSHIFT ">>>"

%token <int> EQUAL "="
%token <int> PLUS_EQUAL "+="
%token <int> MINUS_EQUAL "-="
%token <int> ASTERISK_EQUAL "*="
%token <int> SLASH_EQUAL "/="
%token <int> PERCENEQUAL "%="
%token <int> AMPERSAND_EQUAL "&="
%token <int> BAR_EQUAL "|="
%token <int> CAREEQUAL "^="
%token <int> LSHIFEQUAL "<<="
%token <int> RSHIFEQUAL ">>="
%token <int> ARSHIFEQUAL ">>>="

%token <int> EQ "=="
%token <int> NEQ "!="
%token <int> LT "<"
%token <int> LE "<="
%token <int> GT ">"
%token <int> GE ">="

%token <int> LPAREN "("
%token <int> RPAREN ")"
%token <int> LBRACKET "["
%token <int> RBRACKET "]"
%token <int> LBRACE "{"
%token <int> RBRACE "}"

%token <int> UNDERSCORE "_"
%token <int> COMMA ","
%token <int> SEMICOLON ";"
%token <int> QUESTION "?"
%token <int> COLON ":"
%token <int> COLON2 "::"
%token <int> DOT "."
%token <int> DOT2 ".."
%token <int> LARROW "<-"
%token <int> RARROW "->"
%token <int> DOUBLE_RARROW "=>"
%token <int> COLON_LARROW ":>"
%token <int> COLON_RARROW "<:"

 /* semi */
%token <int> NEWLINE "\n"

 /* str */
%token <std::string> INTEGER_LITERAL FLOALITERAL STRING_LITERAL CHARACTER_LITERAL
%token <std::string> VAR_ID

 /* separator and operator */
%type <std::shared_ptr<Ast>> semi optionalNewline optionalNewlines newlines
%type <std::shared_ptr<Ast>> assignOp prefixOp infixOp postfixOp
 /* literal */
%type <std::shared_ptr<Ast>> literal booleanLiteral
 /* id */
%type <std::shared_ptr<Ast>> id varId
 /* expr */
%type <std::shared_ptr<Ast>> expr enumerators assignExpr prefixExpr postfixExpr infixExpr primaryExpr exprs callExpr block blockStat blockStats
%type <std::shared_ptr<Ast>> optionalYield optionalExpr optionalBlockStats optionalForInit optionalExprs
 /* type */
%type <std::shared_ptr<Ast>> type plainType
 /* def */
%type <std::shared_ptr<Ast>> def funcDef varDef funcSign paramClause params param
%type <std::shared_ptr<Ast>> optionalResultType optionalParams
 /* compile unit */
%type <std::shared_ptr<Ast>> compileUnit topStatSeq topStat topStats
%type <std::shared_ptr<Ast>> optionalTopStats

 /* token } */

 /* low -> high precedence { */

 /* if-else */
%nonassoc "then"
%nonassoc "else"
%nonassoc "semi_else"
 /* try-catch-finally */
%nonassoc "try"
%nonassoc "catch"
%nonassoc "try_catch"
%nonassoc "try_finally"
%nonassoc "try_catch_finally"
 /* return */
%nonassoc "return"
%nonassoc "return_expr"
 /* do-while */
%nonassoc "while"
%nonassoc "semi_while"

 /* comma */
%left COMMA

 /* equal */
%right AMPERSAND_EQUAL BAR_EQUAL CAREEQUAL
%right LSHIFEQUAL RSHIFEQUAL ARSHIFEQUAL
%right ASTERISK_EQUAL SLASH_EQUAL PERCENEQUAL
%right PLUS_EQUAL MINUS_EQUAL
%right EQUAL

 /* conditional */
%left QUESTION COLON

 /* binary op*/
%left BAR2 OR
%left AMPERSAND2 AND
%left BAR
%left CARET
%left AMPERSAND
%left EQ NEQ
%left LT LE GT GE
%left LSHIFT RSHIFT ARSHIFT
%left PLUS MINUS
%left ASTERISK SLASH PERCENT
%left PLUS2 MINUS2
%left ASTERISK2 SLASH2 PERCENT2
%left CARET2 DOT2
%right COLON2

 /* unary op */
%nonassoc NOT TILDE EXCLAM

 /* parantheses */
%left DOT LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE

 /* low -> high precedence } */

%start compileUnit

%%

 /* semi and newline { */

semi : ";" { $$ = nullptr; }
     | "\n" { $$ = nullptr; }
     ;

optionalNewline : "\n" { $$ = nullptr; }
                | %empty { $$ = nullptr; }
                ;

optionalNewlines : newlines { $$ = nullptr; }
                 | %empty { $$ = nullptr; }
                 ;

newlines : "\n" { $$ = nullptr; }
         | newlines "\n" { $$ = nullptr; }
         ;

 /* semi and newline } */

 /* literal { */

literal : INTEGER_LITERAL { $$ = new A_Integer($1, Y_POS(@1)); std::free($1); }
        | FLOALITERAL { $$ = new A_Float($1, Y_POS(@1)); std::free($1); }
        | booleanLiteral { $$ = $1; }
        | CHARACTER_LITERAL { $$ = new A_Character($1, Y_POS(@1)); std::free($1); }
        | STRING_LITERAL { $$ = new A_String($1, Y_POS(@1)); std::free($1); }
        | "nil" { $$ = new A_Nil(Y_POS(@1)); std::free($1); }
        | "void" { $$ = new A_Void(Y_POS(@1)); std::free($1); }
        ;

booleanLiteral : "true" { $$ = new A_Boolean($1, Y_POS(@1)); std::free($1); }
               | "false" { $$ = new A_Boolean($1, Y_POS(@1)); std::free($1); }
               ;

 /* literal } */

 /* id { */

id : varId { $$ = $1; }
   /* | Opid */
   ;

varId : VAR_ID { $$ = new A_varId($1, Y_POS(@1)); std::free($1); }
      ;

/* Opid : assignOp */
/*      | infixOp */
/*      | PREFIX prefixOp */
/*      | POSTFIX postfixOp */
/*      ; */

 /* id } */

 /* expression { */

expr : "if" "(" expr ")" optionalNewlines expr %prec "then" { $$ = nullptr; } /* %prec fix optional if-else shift/reduce */
     | "if" "(" expr ")" optionalNewlines expr "else" expr %prec "else" { $$ = nullptr; }
     | "if" "(" expr ")" optionalNewlines expr "semi_else" expr %prec "semi_else" { $$ = nullptr; }
     | "while" "(" expr ")" optionalNewlines expr { $$ = nullptr; }
     | "do" expr "while" "(" expr ")" %prec "while" { $$ = nullptr; } /* %prec fix optional do-while shift/reduce */
     | "do" expr "semi_while" "(" expr ")" %prec "semi_while" { $$ = nullptr; }
     | "try" block %prec "try" { $$ = nullptr; } /* %prec fix optional try-catch-finally shift/reduce */
     | "try" block "finally" block %prec "try_finally" { $$ = nullptr; }
     | "try" block "catch" block %prec "try_catch" { $$ = nullptr; }
     | "try" block "catch" block "finally" block %prec "try_catch_finally" { $$ = nullptr; }
     | "for" "(" enumerators ")" optionalNewlines optionalYield expr { $$ = nullptr; }
     | "throw" expr { $$ = nullptr; }
     | "return" %prec "return" { $$ = nullptr; } /* %prec fix optional return shift/reduce */
     | "return" expr %prec "return_expr" { $$ = nullptr; }
     | assignExpr { $$ = nullptr; }
     | postfixExpr { $$ = nullptr; }
     ;

enumerators : optionalForInit semi optionalExpr semi optionalExpr { $$ = nullptr; }
            | id "<-" expr { $$ = nullptr; }
            ;

optionalForInit : varDef { $$ = nullptr; }
                | %empty { $$ = nullptr; }
                ;

optionalExpr : expr { $$ = nullptr; }
             | %empty { $$ = nullptr; }
             ;

optionalYield : "yield" { $$ = nullptr; }
              | %empty { $$ = nullptr; }
              ;

assignExpr : id assignOp expr { $$ = nullptr; }
           ;

assignOp : "=" { $$ = $1; }
         | "+=" { $$ = $1; }
         | "-=" { $$ = $1; }
         | "*=" { $$ = $1; }
         | "/=" { $$ = $1; }
         | "%=" { $$ = $1; }
         | "&=" { $$ = $1; }
         | "|=" { $$ = $1; }
         | "^=" { $$ = $1; }
         | "<<=" { $$ = $1; }
         | ">>=" { $$ = $1; }
         | ">>>=" { $$ = $1; }
         ;

postfixExpr : infixExpr { $$ = nullptr; }
            | infixExpr postfixOp { $$ = nullptr; }
            ;

postfixOp : "++" { $$ = $1; }
          | "--" { $$ = $1; }
          ;

infixExpr : prefixExpr { $$ = nullptr; }
          | infixExpr infixOp optionalNewline prefixExpr { $$ = nullptr; }
          ;

infixOp : "||" { $$ = $1; }
        | "or" { $$ = $1; }
        | "&&" { $$ = $1; }
        | "and" { $$ = $1; }
        | "|" { $$ = $1; }
        | "^" { $$ = $1; }
        | "&" { $$ = $1; }
        | "==" { $$ = $1; }
        | "!=" { $$ = $1; }
        | "<" { $$ = $1; }
        | "<=" { $$ = $1; }
        | ">" { $$ = $1; }
        | ">=" { $$ = $1; }
        | "<<" { $$ = $1; }
        | ">>" { $$ = $1; }
        | ">>>" { $$ = $1; }
        | "+" { $$ = $1; }
        | "-" { $$ = $1; }
        | "*" { $$ = $1; }
        | "/" { $$ = $1; }
        | "%" { $$ = $1; }
        | "**" { $$ = $1; }
        | "//" { $$ = $1; }
        | "%%" { $$ = $1; }
        | "^^" { $$ = $1; }
        | "::" { $$ = $1; }
        ;

prefixExpr : primaryExpr { $$ = $1; }
           | prefixOp primaryExpr { $$ = new A_prefixExpression($1, $2); }
           ;

prefixOp : "-" { $$ = $1; }
         | "+" { $$ = $1; }
         | "~" { $$ = $1; }
         | "!" { $$ = $1; }
         | "not" { $$ = $1; }
         | "++" { $$ = $1; }
         | "--" { $$ = $1; }
         ;

primaryExpr : literal { $$ = $1; }
            | id { $$ = $1; }
            | "(" optionalExprs ")" { $$ = $1; }
            | callExpr { $$ = $1; }
            | block { $$ = nullptr; }
            ;

optionalExprs : exprs { $$ = nullptr; }
              | %empty { $$ = nullptr; }
              ;

exprs : expr { $$ = nullptr; }
      | exprs "," expr { $$ = nullptr; }
      ;

callExpr : id "(" optionalExprs ")" { $$ = $1; }
         ;

block : "{" blockStat optionalBlockStats "}" { $$ = nullptr; }
      ;

blockStat : expr { $$ = nullptr; }
          | def { $$ = nullptr; }
          /* | import */
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
     /* | FuncArgtypes RARROW type */
     /* | idType */
     ;

/* FuncArgtypes : "(" ")" */
/*              | "(" paramtypes ")" */
/*              ; */

/* paramtypes : paramtype */
/*            | paramtype COMMA paramtypes */
/*            ; */

/* paramtype : type */
/*           ; */

plainType : "byte" { $$ = nullptr; }
          | "ubyte" { $$ = nullptr; }
          | "short" { $$ = nullptr; }
          | "ushort" { $$ = nullptr; }
          | "int" { $$ = nullptr; }
          | "uint" { $$ = nullptr; }
          | "long" { $$ = nullptr; }
          | "ulong" { $$ = nullptr; }
          | "llong" { $$ = nullptr; }
          | "ullong" { $$ = nullptr; }
          | "float" { $$ = nullptr; }
          | "double" { $$ = nullptr; }
          | "boolean" { $$ = nullptr; }
          | "void" { $$ = nullptr; }
          | "any" { $$ = nullptr; }
          ;

/* idType : id */
/*        ; */

 /* type } */

 /* definition declaration { */

def : funcDef { $$ = nullptr; }
    /* | classDef */
    /* | traitDef */
    | varDef { $$ = nullptr; }
    ;

funcDef : "def" funcSign optionalResultType "=" expr { $$ = nullptr; }
        | "def" funcSign resultType optionalNewlines block { $$ = nullptr; }
        ;

optionalResultType : resultType { $$ = nullptr; }
                   | %empty { $$ = nullptr; }
                   ;

resultType : ":" type
           ;

funcSign : id paramClause { $$ = nullptr; }
         ;

paramClause : "(" optionalParams ")" { $$ = nullptr; }
            ;

optionalParams : params { $$ = nullptr; }
               | %empty { $$ = nullptr; }
               ;

params : param { $$ = nullptr; }
       | params "," param { $$ = nullptr; }
       ;

param : id ":" type { $$ = nullptr; }
      /* | id ":" type "=" expr */
      ;

varDef : "var" id ":" type "=" expr { $$ = nullptr; }
       ;

/* Decl : "var" varDecl */
/*      | "def" funcDecl */
/*      ; */

/* funcDecl : funcSign ":" type */
/*          ; */

/* varDecl : id ":" type */
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
        /* | import */
        /* | package */
        | %empty { $$ = nullptr; }
        ;

/* package : PACKAGE id LBRACE RBRACE */
/*         | PACKAGE id LBRACE topStats RBRACE */
/*         ; */

/* import : */
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
