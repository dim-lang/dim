%require "3.2"
%language "c++"
%define lr.type ielr
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
#include "Position.h"
#include "tokenizer.yy.hh"
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
%type<std::shared_ptr<Ast>> def funcDef varDef funcSign paramClause params param
%type<std::shared_ptr<Ast>> optionalResultType optionalParams
 /* compile unit */
%type<std::shared_ptr<Ast>> compileUnit topStatSeq topStat topStats
%type<std::shared_ptr<Ast>> optionalTopStats

 /* token<int> } */

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
%right AMPERSAND_EQUAL BAR_EQUAL CARET_EQUAL
%right LSHIFT_EQUAL RSHIFT_EQUAL ARSHIFT_EQUAL
%right ASTERISK_EQUAL SLASH_EQUAL PERCENT_EQUAL
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

literal : T_INTEGER_LITERAL { $$ = new A_Integer($1, @$); }
        | T_FLOAT_LITERAL { $$ = new A_Float($1, @$); }
        | booleanLiteral { $$ = $1; }
        | T_CHARACTER_LITERAL { $$ = new A_Character($1, @$)); }
        | T_STRING_LITERAL { $$ = new A_String($1, @$); }
        | "nil" { $$ = new A_Nil(@$); }
        | "void" { $$ = new A_Void(@$); }
        ;

booleanLiteral : "true" { $$ = new A_Boolean(true, @$); }
               | "false" { $$ = new A_Boolean(false, @$); }
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

assignOp : "=" { $$ = nullptr; }
         | "+=" { $$ = nullptr; }
         | "-=" { $$ = nullptr; }
         | "*=" { $$ = nullptr; }
         | "/=" { $$ = nullptr; }
         | "%=" { $$ = nullptr; }
         | "&=" { $$ = nullptr; }
         | "|=" { $$ = nullptr; }
         | "^=" { $$ = nullptr; }
         | "<<=" { $$ = nullptr; }
         | ">>=" { $$ = nullptr; }
         | ">>>=" { $$ = nullptr; }
         ;

postfixExpr : infixExpr { $$ = nullptr; }
            | infixExpr postfixOp { $$ = nullptr; }
            ;

postfixOp : "++" { $$ = nullptr; }
          | "--" { $$ = nullptr; }
          ;

infixExpr : prefixExpr { $$ = nullptr; }
          | infixExpr infixOp optionalNewline prefixExpr { $$ = nullptr; }
          ;

infixOp : "||" { $$ = nullptr; }
        | "or" { $$ = nullptr; }
        | "&&" { $$ = nullptr; }
        | "and" { $$ = nullptr; }
        | "|" { $$ = nullptr; }
        | "^" { $$ = nullptr; }
        | "&" { $$ = nullptr; }
        | "==" { $$ = nullptr; }
        | "!=" { $$ = nullptr; }
        | "<" { $$ = nullptr; }
        | "<=" { $$ = nullptr; }
        | ">" { $$ = nullptr; }
        | ">=" { $$ = nullptr; }
        | "<<" { $$ = nullptr; }
        | ">>" { $$ = nullptr; }
        | ">>>" { $$ = nullptr; }
        | "+" { $$ = nullptr; }
        | "-" { $$ = nullptr; }
        | "*" { $$ = nullptr; }
        | "/" { $$ = nullptr; }
        | "%" { $$ = nullptr; }
        | "**" { $$ = nullptr; }
        | "//" { $$ = nullptr; }
        | "%%" { $$ = nullptr; }
        | "^^" { $$ = nullptr; }
        | "::" { $$ = nullptr; }
        ;

prefixExpr : primaryExpr { $$ = $1; }
           | prefixOp primaryExpr { $$ = new A_prefixExpression($1, $2); }
           ;

prefixOp : "-" { $$ = nullptr; }
         | "+" { $$ = nullptr; }
         | "~" { $$ = nullptr; }
         | "!" { $$ = nullptr; }
         | "not" { $$ = nullptr; }
         | "++" { $$ = nullptr; }
         | "--" { $$ = nullptr; }
         ;

primaryExpr : literal { $$ = $1; }
            | id { $$ = $1; }
            | "(" optionalExprs ")" { $$ = $2; }
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
