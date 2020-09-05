%require "3.2"
%language "c++"
%define lr.type ielr
/* %expect 2 */
%define api.value.type variant
%define api.token.constructor
%define parse.assert
%define parse.error verbose
%define parse.lac full
%locations
%param {yyscan_t yyscanner}

%printer { LOG_INFO("integer: {}", $$); } <int>;
%printer { LOG_INFO("string: {}", $$); } <std::string>;
%printer { LOG_INFO("ast: {}", $$ ? $$->category()._to_string() : "unknown"); } <std::shared_ptr<Ast>>;

%code top {
#include <memory>
#include <string>
#include <sstream>
#include "Log.h"
#include "Ast.h"
#include "Scanner.h"
extern YY_DECL;
#define SP_NULL             (std::shared_ptr<Ast>(nullptr))
#define SP_NEW(x, ...)      (std::shared_ptr<Ast>(new x(__VA_ARGS__)))
#define SP_SPC(x, y)        (std::static_pointer_cast<x>(y))
}

%code requires {
typedef void* yyscan_t;
class Ast;
}

 /* %union { */
 /*     std::shared_ptr<Ast> ast; */
 /*     std::string str; */
 /*     int tok; */
 /* } */

 /* token<int> { */

 /* keyword */
%token<int> T_TRUE "true"
%token<int> T_FALSE "false"
%token<int> T_TRY "try"
%token<int> T_CATCH "catch"
%token<int> T_FINALLY "finally"
%token<int> T_THROW "throw"
%token<int> T_YIELD "yield"
%token<int> T_VAR "var"
%token<int> T_VAL "val"
%token<int> T_NIL "nil"
%token<int> T_NEW "new"
%token<int> T_DELETE "delete"
%token<int> T_DEF "def"
%token<int> T_IF "if"
%token<int> T_THEN "then"
%token<int> T_ELSE "else"
%token<int> T_MATCH "match"
%token<int> T_ENUM "enum"
%token<int> T_SWITCH "switch"
%token<int> T_CASE "case"
%token<int> T_FOR "for"
%token<int> T_FOREACH "foreach"
%token<int> T_IN "in"
%token<int> T_WHILE "while"
%token<int> T_DO "do"
%token<int> T_BREAK "break"
%token<int> T_CONTINUE "continue"
%token<int> T_CLASS "class"
%token<int> T_TRAIT "trait"
%token<int> T_TYPE "type"
%token<int> T_THIS "this"
%token<int> T_SUPER "super"
%token<int> T_ISINSTANCEOF "isinstanceof"
%token<int> T_ISA "isa"
%token<int> T_IS "is"
%token<int> T_IMPORT "import"
%token<int> T_AS "as"
%token<int> T_RETURN "return"
%token<int> T_VOID "void"
%token<int> T_ANY "any"
%token<int> T_NAN "nan"
%token<int> T_INF "inf"
%token<int> T_ASYNC "async"
%token<int> T_AWAIT "await"
%token<int> T_STATIC "static"
%token<int> T_PUBLIC "public"
%token<int> T_PROTECT "protect"
%token<int> T_PRIVATE "private"
%token<int> T_PREFIX "prefix"
%token<int> T_POSTFIX "postfix"
%token<int> T_PACKAGE "package"

 /* primitive type */
%token<int> T_BYTE "byte"
%token<int> T_UBYTE "ubyte"
%token<int> T_SHORT "short"
%token<int> T_USHORT "ushort"
%token<int> T_INT "int"
%token<int> T_UINT "uint"
%token<int> T_LONG "long"
%token<int> T_ULONG "ulong"
%token<int> T_LLONG "llong"
%token<int> T_ULLONG "ullong"
%token<int> T_FLOAT "float"
%token<int> T_DOUBLE "double"
%token<int> T_BOOLEAN "boolean"
%token<int> T_CHAR "char"

 /* operator */
%token<int> T_AND "and"
%token<int> T_OR "or"
%token<int> T_NOT "not"

%token<int> T_PLUS "+"
%token<int> T_PLUS2 "++"
%token<int> T_MINUS "-"
%token<int> T_MINUS2 "--"
%token<int> T_ASTERISK "*"
%token<int> T_ASTERISK2 "**"
%token<int> T_SLASH "/"
%token<int> T_SLASH2 "//"
%token<int> T_PERCENT "%"
%token<int> T_PERCENT2 "%%"

%token<int> T_AMPERSAND "&"
%token<int> T_AMPERSAND2 "&&"
%token<int> T_BAR "|"
%token<int> T_BAR2 "||"
%token<int> T_TILDE "~"
%token<int> T_EXCLAM "!"
%token<int> T_CARET "^"
%token<int> T_CARET2 "^^"

%token<int> T_LSHIFT "<<"
%token<int> T_RSHIFT ">>"
%token<int> T_ARSHIFT ">>>"

%token<int> T_EQUAL "="
%token<int> T_PLUS_EQUAL "+="
%token<int> T_MINUS_EQUAL "-="
%token<int> T_ASTERISK_EQUAL "*="
%token<int> T_SLASH_EQUAL "/="
%token<int> T_PERCENT_EQUAL "%="
%token<int> T_AMPERSAND_EQUAL "&="
%token<int> T_BAR_EQUAL "|="
%token<int> T_CARET_EQUAL "^="
%token<int> T_LSHIFT_EQUAL "<<="
%token<int> T_RSHIFT_EQUAL ">>="
%token<int> T_ARSHIFT_EQUAL ">>>="

%token<int> T_EQ "=="
%token<int> T_NEQ "!="
%token<int> T_LT "<"
%token<int> T_LE "<="
%token<int> T_GT ">"
%token<int> T_GE ">="

%token<int> T_LPAREN "("
%token<int> T_RPAREN ")"
%token<int> T_LBRACKET "["
%token<int> T_RBRACKET "]"
%token<int> T_LBRACE "{"
%token<int> T_RBRACE "}"

%token<int> T_UNDERSCORE "_"
%token<int> T_COMMA ","
%token<int> T_SEMI ";"
%token<int> T_QUESTION "?"
%token<int> T_COLON ":"
%token<int> T_COLON2 "::"
%token<int> T_DOT "."
%token<int> T_DOT2 ".."
%token<int> T_LARROW "<-"
%token<int> T_RARROW "->"
%token<int> T_DOUBLE_RARROW "=>"
%token<int> T_COLON_LARROW ":>"
%token<int> T_COLON_RARROW "<:"

%token<int> T_NEWLINE "\n"

 /* separator and operator */
%type<int> optionalNewline optionalNewlines newlines optionalYield
%type<int> assignOp prefixOp infixOp postfixOp

 /* str */
%token<std::string> T_INTEGER_LITERAL T_FLOAT_LITERAL T_STRING_LITERAL T_CHARACTER_LITERAL
%token<std::string> T_VAR_ID

 /* literal */
%type<std::shared_ptr<Ast>> literal booleanLiteral
 /* id */
%type<std::shared_ptr<Ast>> id varId
 /* expr */
%type<std::shared_ptr<Ast>> expr exprs enumerators assignExpr prefixExpr postfixExpr infixExpr primaryExpr callExpr block blockStat blockStats
%type<std::shared_ptr<Ast>> optionalExprs optionalBlockStats
 /* type */
%type<std::shared_ptr<Ast>> type plainType
 /* def */
%type<std::shared_ptr<Ast>> def funcDef varDef funcSign resultType param params
%type<std::shared_ptr<Ast>> /* optionalResultType */ optionalParams
 /* compile unit */
%type<std::shared_ptr<Ast>> compileUnit topStat topStats optionalTopStats

 /* token<int> } */

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
                | %empty { $$ = yy::parser::token::YYUNDEF; }
                ;

optionalNewlines : newlines { $$ = $1; }
                 | %empty { $$ = yy::parser::token::YYUNDEF; }
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
              | %empty { $$ = yy::parser::token::YYUNDEF; }
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

void yy::parser::error(const yy::parser::location_type& l, const std::string& m) {
    LOG_INFO("{}:{} {}", static_cast<Scanner *>(yyget_extra(yyscanner))->fileName, (std::stringstream() << l).str(), m);
}
