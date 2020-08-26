%require "3.2"
%language "c++"
%define lr.type ielr
/* %glr-parser */
%expect 1
%define api.value.type variant
%define api.token.constructor
%define parse.assert
%define parse.error verbose
%define parse.lac full
%locations

%code top {
#include <memory>
#include "Log.h"
#include "Ast.h"
#include "Buffer.h"
#include "tokenizer.yy.hh"
#define SP_NULL             (std::shared_ptr<Ast>(nullptr))
#define SP_NEW(x, ...)      (std::shared_ptr<Ast>(new x(__VA_ARGS__)))
}

%code requires {
#include "tokenizer.yy.hh"
#include "Ast.h"
}

%param { yyscan_t yyscanner }

 /* %union { */
 /*     std::shared_ptr<Ast> ast; */
 /*     std::string str; */
 /*     int tok; */
 /* } */

 /* token<int> { */

%token<int> T_EMPTY "empty"

 /* keyword */
%token<std::string> T_TRUE "true"
%token<std::string> T_FALSE "false"
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
%token<int> T_SEMI_ELSE "semi_else"
%token<int> T_MATCH "match"
%token<int> T_ENUM "enum"
%token<int> T_SWITCH "switch"
%token<int> T_CASE "case"
%token<int> T_FOR "for"
%token<int> T_FOREACH "foreach"
%token<int> T_IN "in"
%token<int> T_WHILE "while"
%token<int> T_SEMI_WHILE "semi_while"
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

 /* semi */
%token<int> T_NEWLINE "\n"

 /* separator and operator */
%type<int> semi optionalNewline optionalNewlines newlines
%type<int> assignOp prefixOp infixOp postfixOp

 /* str */
%token<std::string> T_INTEGER_LITERAL T_FLOAT_LITERAL T_STRING_LITERAL T_CHARACTER_LITERAL
%token<std::string> T_VAR_ID

 /* literal */
%type<std::shared_ptr<Ast>> literal booleanLiteral
 /* id */
%type<std::shared_ptr<Ast>> id varId
 /* expr */
%type<std::shared_ptr<Ast>> expr enumerators assignExpr prefixExpr postfixExpr infixExpr primaryExpr exprs callExpr block blockStat blockStats
%type<std::shared_ptr<Ast>> optionalYield optionalExpr optionalBlockStats optionalForInit optionalExprs
 /* type */
%type<std::shared_ptr<Ast>> type plainType
 /* def */
%type<std::shared_ptr<Ast>> def funcDef varDef funcSign params param resultType
%type<std::shared_ptr<Ast>> /* optionalResultType */ optionalParams
 /* compile unit */
%type<std::shared_ptr<Ast>> compileUnit topStat topStats
%type<std::shared_ptr<Ast>> optionalTopStats

 /* token<int> } */

 /* low -> high precedence { */

 /* try-catch-finally */
%nonassoc "try_catch"
%nonassoc "try_catch_finally"
 /* do-while */
%nonassoc "while"
%nonassoc "semi_while"
 /* if-else */
%nonassoc "then"
%nonassoc "else"
%nonassoc "semi_else"
 /* return */
%nonassoc "return"
%nonassoc "return_expr"

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

 /* semi and newline { */

semi : ";" { $$ = $1; }
     | "\n" { $$ = $1; }
     ;

optionalNewline : "\n" { $$ = $1; }
                | %empty { $$ = yy::parser::token::T_EMPTY; }
                ;

optionalNewlines : newlines { $$ = $1; }
                 | %empty { $$ = yy::parser::token::T_EMPTY; }
                 ;

newlines : "\n" { $$ = $1; }
         | newlines "\n" { $$ = $1; }
         ;

 /* semi and newline } */

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

expr : "if" "(" expr ")" optionalNewlines expr %prec "then" { $$ = SP_NEW(A_If, $3, $6, SP_NULL, @$); } /* %prec fix optional if-else shift/reduce */
     | "if" "(" expr ")" optionalNewlines expr "else" expr %prec "else" { $$ = SP_NEW(A_If, $3, $6, $8, @$); }
     | "if" "(" expr ")" optionalNewlines expr "semi_else" expr %prec "semi_else" { $$ = SP_NEW(A_If, $3, $6, $8, @$); }
     | "while" "(" expr ")" optionalNewlines expr { $$ = SP_NEW(A_Loop, $3, $6, @$); }
     | "do" optionalNewlines expr "while" "(" expr ")" %prec "while" { $$ = SP_NEW(A_Loop, $2, $5, @$); } /* %prec fix optional do-while shift/reduce */
     | "do" optionalNewlines expr "semi_while" "(" expr ")" %prec "semi_while" { $$ = SP_NEW(A_Loop, $2, $5, @$); }
     | "for" "(" enumerators ")" optionalNewlines optionalYield expr { $$ = SP_NEW(A_Loop, $3, $7, @$); }
     | "try" optionalNewlines expr "catch" optionalNewlines expr %prec "try_catch" { $$ = SP_NEW(A_Try, $3, $6, SP_NULL, @$); } /* %prec fix optional try-catch-finally shift/reduce */
     | "try" optionalNewlines expr "catch" optionalNewlines expr "finally" optionalNewlines expr %prec "try_catch_finally" { $$ = SP_NEW(A_Try, $3, $6, $9, @$); }
     | "throw" expr { $$ = SP_NEW(A_Throw, $2, @$); }
     | "return" %prec "return" { $$ = SP_NEW(A_Return, SP_NULL, @$); } /* %prec fix optional return shift/reduce */
     | "return" expr %prec "return_expr" { $$ = SP_NEW(A_Return, $2, @$); }
     | assignExpr { $$ = $1; }
     | postfixExpr { $$ = $1; }
     ;

enumerators : optionalForInit semi optionalExpr semi optionalExpr { $$ = SP_NEW(A_LoopCondition, $1, $3, $5, @$); }
            | id "<-" expr { $$ = SP_NEW(A_LoopEnumerator, $1, $3, @$); }
            ;

optionalForInit : varDef { $$ = $1; }
                | %empty { $$ = SP_NULL; }
                ;

optionalExpr : expr { $$ = $1; }
             | %empty { $$ = SP_NULL; }
             ;

optionalYield : "yield" { $$ = $1; }
              | %empty { $$ = yy::parser::token::T_EMPTY; }
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
              | %empty { $$ = SP_NULL; }
              ;

exprs : expr { $$ = SP_NEW(A_Exprs, $1, SP_NULL, @$); }
      | exprs "," expr { $$ = SP_NULL; }
      ;

callExpr : id "(" optionalExprs ")" { $$ = SP_NEW(A_Call, $1, $3, @$); }
         ;

block : "{" blockStat optionalBlockStats "}" { $$ = SP_NEW(A_Block, $2, $3, @$); }
      ;

blockStat : expr { $$ = $1; }
          | def { $$ = $1; }
          /* | import */
          | %empty { $$ = SP_NULL; }
          ;

optionalBlockStats : blockStats { $$ = $1; }
                   | %empty { $$ = SP_NULL; }
                   ;

blockStats : semi blockStat { $$ = SP_NEW(A_BlockStats, $2, SP_NULL, @$); }
           | blockStats semi blockStat { $$ = SP_NEW(A_BlockStats, $3, $1, @$); }
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

funcSign : id "(" optionalParams ")" { $$ = SP_NEW(A_FuncSign, $1, $3, @$); }
         ;

optionalParams : params { $$ = $1; }
               | %empty { $$ = SP_NULL; }
               ;

params : param { $$ = SP_NEW(A_Params, $1, SP_NULL, @$); }
       | params "," param { $$ = SP_NEW(A_Params, $3, $1, @$); }
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

compileUnit : topStat optionalTopStats { $$ = SP_NEW(A_CompileUnit, $1, $2, @$); }
            ;

optionalTopStats : topStats { $$ = $1; }
                 | %empty { $$ = SP_NULL; }
                 ;

topStats : semi topStat { $$ = SP_NEW(A_TopStats, $2, SP_NULL, @$); }
         | topStats semi topStat { $$ = SP_NEW(A_TopStats, $3, $1, @$); }
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
