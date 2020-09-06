%define lr.type ielr
%define api.pure full
%define parse.assert
%define parse.error verbose
%define parse.lac full
%locations
%param {yyscan_t yyscanner}

%code top {
#include <memory>
#include <string>
#include "Log.h"
#include "Ast.h"
#include "Scanner.h"
#include "Location.h"
#include "tokenizer.yy.hh"
#define SP(ptr)         (std::shared_ptr<Ast>(ptr))
#define SP_NULL         (std::shared_ptr<Ast>(nullptr))
}

%code requires {
typedef void* yyscan_t;
class Ast;
}

%union {
    Ast *ast;
    char *literal;
    int token;
}

 /* token { */

 /* keyword */
%token<token> T_TRUE "true"
%token<token> T_FALSE "false"
%token<token> T_TRY "try"
%token<token> T_CATCH "catch"
%token<token> T_FINALLY "finally"
%token<token> T_THROW "throw"
%token<token> T_YIELD "yield"
%token<token> T_VAR "var"
%token<token> T_VAL "val"
%token<token> T_NIL "nil"
%token<token> T_NEW "new"
%token<token> T_DELETE "delete"
%token<token> T_DEF "def"
%token<token> T_IF "if"
%token<token> T_THEN "then"
%token<token> T_ELSE "else"
%token<token> T_MATCH "match"
%token<token> T_ENUM "enum"
%token<token> T_SWITCH "switch"
%token<token> T_CASE "case"
%token<token> T_FOR "for"
%token<token> T_FOREACH "foreach"
%token<token> T_IN "in"
%token<token> T_WHILE "while"
%token<token> T_DO "do"
%token<token> T_BREAK "break"
%token<token> T_CONTINUE "continue"
%token<token> T_CLASS "class"
%token<token> T_TRAIT "trait"
%token<token> T_TYPE "type"
%token<token> T_THIS "this"
%token<token> T_SUPER "super"
%token<token> T_ISINSTANCEOF "isinstanceof"
%token<token> T_ISA "isa"
%token<token> T_IS "is"
%token<token> T_IMPORT "import"
%token<token> T_AS "as"
%token<token> T_RETURN "return"
%token<token> T_VOID "void"
%token<token> T_ANY "any"
%token<token> T_NAN "nan"
%token<token> T_INF "inf"
%token<token> T_ASYNC "async"
%token<token> T_AWAIT "await"
%token<token> T_STATIC "static"
%token<token> T_PUBLIC "public"
%token<token> T_PROTECT "protect"
%token<token> T_PRIVATE "private"
%token<token> T_PREFIX "prefix"
%token<token> T_POSTFIX "postfix"
%token<token> T_PACKAGE "package"

 /* primitive type */
%token<token> T_BYTE "byte"
%token<token> T_UBYTE "ubyte"
%token<token> T_SHORT "short"
%token<token> T_USHORT "ushort"
%token<token> T_INT "int"
%token<token> T_UINT "uint"
%token<token> T_LONG "long"
%token<token> T_ULONG "ulong"
%token<token> T_LLONG "llong"
%token<token> T_ULLONG "ullong"
%token<token> T_FLOAT "float"
%token<token> T_DOUBLE "double"
%token<token> T_BOOLEAN "boolean"
%token<token> T_CHAR "char"

 /* operator */
%token<token> T_AND "and"
%token<token> T_OR "or"
%token<token> T_NOT "not"

%token<token> T_PLUS "+"
%token<token> T_PLUS2 "++"
%token<token> T_MINUS "-"
%token<token> T_MINUS2 "--"
%token<token> T_ASTERISK "*"
%token<token> T_ASTERISK2 "**"
%token<token> T_SLASH "/"
%token<token> T_SLASH2 "//"
%token<token> T_PERCENT "%"
%token<token> T_PERCENT2 "%%"

%token<token> T_AMPERSAND "&"
%token<token> T_AMPERSAND2 "&&"
%token<token> T_BAR "|"
%token<token> T_BAR2 "||"
%token<token> T_TILDE "~"
%token<token> T_EXCLAM "!"
%token<token> T_CARET "^"
%token<token> T_CARET2 "^^"

%token<token> T_LSHIFT "<<"
%token<token> T_RSHIFT ">>"
%token<token> T_ARSHIFT ">>>"

%token<token> T_EQUAL "="
%token<token> T_PLUS_EQUAL "+="
%token<token> T_MINUS_EQUAL "-="
%token<token> T_ASTERISK_EQUAL "*="
%token<token> T_SLASH_EQUAL "/="
%token<token> T_PERCENT_EQUAL "%="
%token<token> T_AMPERSAND_EQUAL "&="
%token<token> T_BAR_EQUAL "|="
%token<token> T_CARET_EQUAL "^="
%token<token> T_LSHIFT_EQUAL "<<="
%token<token> T_RSHIFT_EQUAL ">>="
%token<token> T_ARSHIFT_EQUAL ">>>="

%token<token> T_EQ "=="
%token<token> T_NEQ "!="
%token<token> T_LT "<"
%token<token> T_LE "<="
%token<token> T_GT ">"
%token<token> T_GE ">="

%token<token> T_LPAREN "("
%token<token> T_RPAREN ")"
%token<token> T_LBRACKET "["
%token<token> T_RBRACKET "]"
%token<token> T_LBRACE "{"
%token<token> T_RBRACE "}"

%token<token> T_UNDERSCORE "_"
%token<token> T_COMMA ","
%token<token> T_SEMI ";"
%token<token> T_QUESTION "?"
%token<token> T_COLON ":"
%token<token> T_COLON2 "::"
%token<token> T_DOT "."
%token<token> T_DOT2 ".."
%token<token> T_LARROW "<-"
%token<token> T_RARROW "->"
%token<token> T_DOUBLE_RARROW "=>"
%token<token> T_COLON_LARROW ":>"
%token<token> T_COLON_RARROW "<:"

%token<token> T_NEWLINE "\n"

 /* separator and operator */
%type<token> optionalNewline optionalNewlines newlines optionalYield
%type<token> assignOp prefixOp infixOp postfixOp

 /* str */
%token<literal> T_INTEGER_LITERAL T_FLOAT_LITERAL T_STRING_LITERAL T_CHARACTER_LITERAL
%token<literal> T_VAR_ID

 /* literal */
%type<ast> literal booleanLiteral
 /* id */
%type<ast> id varId
 /* expr */
%type<ast> expr exprs enumerators assignExpr prefixExpr postfixExpr infixExpr primaryExpr callExpr block blockStat blockStats exprSeq
%type<ast> optionalExprs optionalBlockStats
 /* type */
%type<ast> type plainType
 /* def */
%type<ast> def funcDef varDef funcSign resultType param params
%type<ast> /* optionalResultType */ optionalParams
 /* compile unit */
%type<ast> compileUnit topStat topStats optionalTopStats

 /* token } */

 /* low -> high precedence { */

 /* try-catch-finally */
%right "catch" "finally"
%precedence "try_catch"
%precedence "try_catch_finally"
 /* do-while */
%precedence "while"
%precedence "do_while"
 /* if-else */
%precedence "then"
%precedence "else"
 /* return */
%precedence "return"
%precedence "return_expr"

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

 /* binary op*/
%left T_BAR2 T_OR
%left T_AMPERSAND2 T_AND
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
%left T_CARET2 T_DOT2
%right T_COLON2

 /* unary op */
%nonassoc T_NOT T_TILDE T_EXCLAM

 /* parantheses */
%left T_DOT T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET T_LBRACE T_RBRACE

 /* low -> high precedence } */

%start compileUnit

%%

 /* newline { */

optionalNewline : "\n" { $$ = $1; }
                | %empty { $$ = yytokentype::YYUNDEF; }
                ;

optionalNewlines : newlines { $$ = $1; }
                 | %empty { $$ = yytokentype::YYUNDEF; }
                 ;

newlines : "\n" { $$ = $1; }
         | newlines "\n" { $$ = $2; }
         ;

 /* newline } */

 /* literal { */

literal : T_INTEGER_LITERAL { $$ = SP_NEW(A_Integer, $1, @$); }
        | T_FLOAT_LITERAL { $$ = SP_NEW(A_Float, $1, @$); }
        | booleanLiteral { $$ = $1; }
        | T_CHARACTER_LITERAL { $$ = SP_NEW(A_Character, $1, @$); }
        | T_STRING_LITERAL { $$ = SP_NEW(A_String, $1, @$); }
        | "nil" { $$ = SP_NEW(A_Nil, @$); }
        | "void" { $$ = SP_NEW(A_Void, @$); }
        ;

booleanLiteral : "true" { $$ = SP_NEW(A_Boolean, true, @$); }
               | "false" { $$ = SP_NEW(A_Boolean, false, @$); }
               ;

 /* literal } */

 /* id { */

id : varId { $$ = $1; }
   /* | Opid */
   ;

varId : T_VAR_ID { $$ = SP_NEW(A_VarId, $1, @$); }
      ;

/* Opid : assignOp */
/*      | infixOp */
/*      | PREFIX prefixOp */
/*      | POSTFIX postfixOp */
/*      ; */

 /* id } */

 /* expression { */

 /**
  * 1. if-else
  *     use `%prec "then"` and `%prec "else"` to fix dangling else shift/reduce
  *     add `optionalNewlines` after `if (expr)` to enable newlines here
  *     use magic `ws-else-ws`(ws short for eat-all-whitespaces-include-newline) token to eat all whitespaces around real keyword `else` to enable newlines here
  * 2. while and do-while
  *     use `%prec "while"` and `%prec "do_while"` to fix shift/reduce conflict between while and do-while
  *     add `optionalNewlines` after `while (expr)` to enable newlines here
  *     use magic `do-ws` token to eat all whitespaces after real keyword `do` to enable newlines here
  *     add `optionalNewlines` after `do expr` to enable newlines here
  * 3. for
  *     add `optionalNewlines` after `for (enumerators)` to enable newlines here
  * 4. try-catch-finally
  *     use `%prec "try_catch"` and `%prec "try_catch_finally"` and `%right "catch" "finally"` to fix dangling finally shift/reduce
  *     use magic `try-ws` token to eat all whitespaces after real keyword `try`, `ws-catch-ws` `ws-finally-ws` token to eat all whitespaces around real keyword `catch` `finally`
  */

expr : "if" "(" expr ")" optionalNewlines expr %prec "then" { $$ = SP_NEW(A_If, $3, $6, SP_NULL, @$); }
     | "if" "(" expr ")" optionalNewlines expr "else" expr %prec "else" { $$ = SP_NEW(A_If, $3, $6, $8, @$); }
     | "while" "(" expr ")" optionalNewlines expr %prec "while" { std::shared_ptr<Ast> loopCondition(new A_LoopCondition($3, false, @3)); $$ = SP_NEW(A_Loop, loopCondition, $6, @$); }
     | "do" expr optionalNewlines "while" "(" expr ")" %prec "do_while" { std::shared_ptr<Ast> loopCondition(new A_LoopCondition($6, true, @6)); $$ = SP_NEW(A_Loop, loopCondition, $2, @$); }
     | "for" "(" enumerators ")" optionalNewlines optionalYield expr { $$ = SP_NEW(A_Loop, $3, $7, @$); }
     | "try" expr "catch" expr %prec "try_catch" { $$ = SP_NEW(A_Try, $2, $4, SP_NULL, @$); }
     | "try" expr "catch" expr "finally" expr %prec "try_catch_finally" { $$ = SP_NEW(A_Try, $2, $4, $6, @$); }
     | "throw" expr { $$ = SP_NEW(A_Throw, $2, @$); }
     | "return" %prec "return" { $$ = SP_NEW(A_Return, SP_NULL, @$); }
     | "return" expr %prec "return_expr" { $$ = SP_NEW(A_Return, $2, @$); }
     | "continue" { $$ = SP_NEW(A_Continue, @$); }
     | "break" { $$ = SP_NEW(A_Break, @$); }
     | assignExpr { $$ = $1; }
     | postfixExpr { $$ = $1; }
     ;

enumerators : id "<-" expr { $$ = SP_NEW(A_LoopEnumerator, $1, $3, @$); }
            ;

optionalYield : "yield" { $$ = $1; }
              | %empty { $$ = yytokentype::YYUNDEF; }
              ;

assignExpr : id assignOp expr { $$ = SP_NEW(A_Assign, $1, $2, $3, @$); }
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

postfixExpr : infixExpr { $$ = $1; }
            | infixExpr postfixOp { $$ = SP_NEW(A_PostfixExpr, $1, $2, @$); }
            ;

postfixOp : "++" { $$ = $1; }
          | "--" { $$ = $1; }
          ;

infixExpr : prefixExpr { $$ = $1; }
          | infixExpr infixOp optionalNewline prefixExpr { $$ = SP_NEW(A_InfixExpr, $1, $2, $4, @$); }
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
           | prefixOp primaryExpr { $$ = SP_NEW(A_PrefixExpr, $1, $2, @$); }
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
            | "(" optionalExprs ")" { $$ = $2; }
            | callExpr { $$ = $1; }
            | block { $$ = $1; }
            ;

optionalExprs : exprs { $$ = $1; }
              | %empty { $$ = SP_SPC(A_Exprs, SP_NULL); }
              ;

exprs : expr { $$ = SP_NEW(A_Exprs, $1, SP_SPC(A_Exprs, SP_NULL), @$); }
      | exprs "," expr { $$ = SP_NEW(A_Exprs, $3, SP_SPC(A_Exprs, $1), @$); }
      ;

callExpr : id "(" optionalExprs ")" { $$ = SP_NEW(A_Call, $1, SP_SPC(A_Exprs, $3), @$); }
         ;

block : "{" blockStat optionalBlockStats "}" {
            if ($2) {
                std::shared_ptr<A_BlockStats> blockStats(new A_BlockStats($2, SP_SPC(A_BlockStats, $3), @2 += @3));
                $$ = SP_NEW(A_Block, blockStats, @$);
            } else {
                $$ = SP_NEW(A_Block, SP_SPC(A_BlockStats, $3), @$);
            }
        }
      ;

blockStat : exprSeq { $$ = $1; }
          | def { $$ = $1; }
          /* | import */
          | %empty { $$ = SP_NULL; }
          ;

exprSeq : expr { $$ = SP_NEW(A_Exprs, $1, SP_SPC(A_Exprs, SP_NULL), @$); }
        | exprSeq ";" expr { $$ = SP_NEW(A_Exprs, $3, SP_SPC(A_Exprs, $1), @$); }
        ;

optionalBlockStats : blockStats { $$ = $1; }
                   | %empty { $$ = SP_SPC(A_BlockStats, SP_NULL); }
                   ;

blockStats : "\n" blockStat { $$ = SP_NEW(A_BlockStats, $2, SP_SPC(A_BlockStats, SP_NULL), @$); }
           | blockStats "\n" blockStat { $$ = SP_NEW(A_BlockStats, $3, SP_SPC(A_BlockStats, $1), @$); }
           ;

 /* expression } */

 /* type { */

type : plainType { $$ = $1; }
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

plainType : "byte" { $$ = SP_NEW(A_PlainType, $1, @$); }
          | "ubyte" { $$ = SP_NEW(A_PlainType, $1, @$); }
          | "short" { $$ = SP_NEW(A_PlainType, $1, @$); }
          | "ushort" { $$ = SP_NEW(A_PlainType, $1, @$); }
          | "int" { $$ = SP_NEW(A_PlainType, $1, @$); }
          | "uint" { $$ = SP_NEW(A_PlainType, $1, @$); }
          | "long" { $$ = SP_NEW(A_PlainType, $1, @$); }
          | "ulong" { $$ = SP_NEW(A_PlainType, $1, @$); }
          | "llong" { $$ = SP_NEW(A_PlainType, $1, @$); }
          | "ullong" { $$ = SP_NEW(A_PlainType, $1, @$); }
          | "char" { $$ = SP_NEW(A_PlainType, $1, @$); }
          | "float" { $$ = SP_NEW(A_PlainType, $1, @$); }
          | "double" { $$ = SP_NEW(A_PlainType, $1, @$); }
          | "boolean" { $$ = SP_NEW(A_PlainType, $1, @$); }
          | "void" { $$ = SP_NEW(A_PlainType, $1, @$); }
          | "any" { $$ = SP_NEW(A_PlainType, $1, @$); }
          ;

/* idType : id */
/*        ; */

 /* type } */

 /* definition declaration { */

def : funcDef { $$ = $1; }
    /* | classDef */
    /* | traitDef */
    | varDef { $$ = $1; }
    ;

funcDef : "def" funcSign resultType "=" expr { $$ = SP_NEW(A_FuncDef, $2, $3, $5, @$); }
        | "def" funcSign resultType optionalNewlines block { $$ = SP_NEW(A_FuncDef, $2, $3, $5, @$); }
        ;

/* optionalResultType : resultType { $$ = nullptr; } */
/*                    | %empty { $$ = nullptr; } */
/*                    ; */

resultType : ":" type { $$ = $2; }
           ;

funcSign : id "(" optionalParams ")" { $$ = SP_NEW(A_FuncSign, $1, SP_SPC(A_Params, $3), @$); }
         ;

optionalParams : params { $$ = $1; }
               | %empty { $$ = SP_SPC(A_Params, SP_NULL); }
               ;

params : param { $$ = SP_NEW(A_Params, SP_SPC(A_Param, $1), SP_SPC(A_Params, SP_NULL), @$); }
       | params "," param { $$ = SP_NEW(A_Params, SP_SPC(A_Param, $3), SP_SPC(A_Params, $1), @$); }
       ;

param : id ":" type { $$ = SP_NEW(A_Param, $1, $3, @$); }
      /* | id ":" type "=" expr */
      ;

varDef : "var" id ":" type "=" expr { $$ = SP_NEW(A_VarDef, $2, $4, $6, @$); }
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

compileUnit : topStat optionalTopStats {
                    if ($1) {
                        std::shared_ptr<A_TopStats> topStats(new A_TopStats($1, SP_SPC(A_TopStats, $2), @$));
                        static_cast<Scanner *>(yyget_extra(yyscanner))->compileUnit = SP_NEW(A_CompileUnit, topStats, @$);
                    } else {
                        static_cast<Scanner *>(yyget_extra(yyscanner))->compileUnit = SP_NEW(A_CompileUnit, SP_SPC(A_TopStats, $2), @$);
                    }
                }
            ;

optionalTopStats : topStats { $$ = $1; }
                 | %empty { $$ = SP_SPC(A_TopStats, SP_NULL); }
                 ;

topStats : "\n" topStat { $$ = SP_NEW(A_TopStats, $2, SP_SPC(A_TopStats, SP_NULL), @$); }
         | topStats "\n" topStat { $$ = SP_NEW(A_TopStats, $3, SP_SPC(A_TopStats, $1), @$); }
         ;

topStat : def { $$ = $1; }
        /* | import */
        /* | package */
        | %empty { $$ = SP_NULL; }
        ;

/* package : PACKAGE id LBRACE RBRACE */
/*         | PACKAGE id LBRACE topStats RBRACE */
/*         ; */

/* import : */
/*        ; */

 /* compile unit } */

%%

void yyerror(YYLTYPE *yyllocp, yyscan_t yyscanner, const char *msg) {
  if (yyllocp && yyllocp->first_line) {
    fprintf(stderr, "%s: %d.%d-%d.%d: error: ", 
            yyget_extra(yyscanner) ? yyget_extra(yyscanner)->fileName.c_str() : "unknown",
            yyllocp->first_line,
            yyllocp->first_column, 
            yyllocp->last_line, 
            yyllocp->last_column);
  }
  fprintf(stderr, "%s\n", msg);
}
