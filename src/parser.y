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

 /* token { */

 /* keyword */
%token <std::string> T_TRUE "true"
%token <std::string> T_FALSE "false"
%token T_TRY "try"
%token T_CATCH "catch"
%token T_FINALLY "finally"
%token T_THROW "throw"
%token T_YIELD "yield"
%token T_VAR "var"
%token T_VAL "val"
%token T_NIL "nil"
%token T_NEW "new"
%token T_DELETE "delete"
%token T_DEF "def"
%token T_IF "if"
%token T_THEN "then"
%token T_ELSE "else"
%token T_SEMI_ELSE "semi_else"
%token T_MATCH "match"
%token T_ENUM "enum"
%token T_SWITCH "switch"
%token T_CASE "case"
%token T_FOR "for"
%token T_FOREACH "foreach"
%token T_IN "in"
%token T_WHILE "while"
%token T_SEMI_WHILE "semi_while"
%token T_DO "do"
%token T_BREAK "break"
%token T_CONTINUE "continue"
%token T_CLASS "class"
%token T_TRAIT "trait"
%token T_TYPE "type"
%token T_THIS "this"
%token T_SUPER "super"
%token T_ISINSTANCEOF "isinstanceof"
%token T_ISA "isa"
%token T_IS "is"
%token T_IMPORT "import"
%token T_AS "as"
%token T_RETURN "return"
%token T_VOID "void"
%token T_ANY "any"
%token T_NAN "nan"
%token T_INF "inf"
%token T_ASYNC "async"
%token T_AWAIT "await"
%token T_STATIC "static"
%token T_PUBLIC "public"
%token T_PROTECT "protect"
%token T_PRIVATE "private"
%token T_PREFIX "prefix"
%token T_POSTFIX "postfix"
%token T_PACKAGE "package"

 /* primitive type */
%token T_BYTE "byte"
%token T_UBYTE "ubyte"
%token T_SHORT "short"
%token T_USHORT "ushort"
%token T_INT "int"
%token T_UINT "uint"
%token T_LONG "long"
%token T_ULONG "ulong"
%token T_LLONG "llong"
%token T_ULLONG "ullong"
%token T_FLOAT "float"
%token T_DOUBLE "double"
%token T_BOOLEAN "boolean"
%token T_CHAR "char"

 /* operator */
%token T_AND "and"
%token T_OR "or"
%token T_NOT "not"

%token T_PLUS "+"
%token T_PLUS2 "++"
%token T_MINUS "-"
%token T_MINUS2 "--"
%token T_ASTERISK "*"
%token T_ASTERISK2 "**"
%token T_SLASH "/"
%token T_SLASH2 "//"
%token T_PERCENT "%"
%token T_PERCENT2 "%%"

%token T_AMPERSAND "&"
%token T_AMPERSAND2 "&&"
%token T_BAR "|"
%token T_BAR2 "||"
%token T_TILDE "~"
%token T_EXCLAM "!"
%token T_CARET "^"
%token T_CARET2 "^^"

%token T_LSHIFT "<<"
%token T_RSHIFT ">>"
%token T_ARSHIFT ">>>"

%token T_EQUAL "="
%token T_PLUS_EQUAL "+="
%token T_MINUS_EQUAL "-="
%token T_ASTERISK_EQUAL "*="
%token T_SLASH_EQUAL "/="
%token T_PERCENT_EQUAL "%="
%token T_AMPERSAND_EQUAL "&="
%token T_BAR_EQUAL "|="
%token T_CARET_EQUAL "^="
%token T_LSHIFT_EQUAL "<<="
%token T_RSHIFT_EQUAL ">>="
%token T_ARSHIFT_EQUAL ">>>="

%token T_EQ "=="
%token T_NEQ "!="
%token T_LT "<"
%token T_LE "<="
%token T_GT ">"
%token T_GE ">="

%token T_LPAREN "("
%token T_RPAREN ")"
%token T_LBRACKET "["
%token T_RBRACKET "]"
%token T_LBRACE "{"
%token T_RBRACE "}"

%token T_UNDERSCORE "_"
%token T_COMMA ","
%token T_SEMI ";"
%token T_QUESTION "?"
%token T_COLON ":"
%token T_COLON2 "::"
%token T_DOT "."
%token T_DOT2 ".."
%token T_LARROW "<-"
%token T_RARROW "->"
%token T_DOUBLE_RARROW "=>"
%token T_COLON_LARROW ":>"
%token T_COLON_RARROW "<:"

 /* semi */
%token T_NEWLINE "\n"

 /* str */
%token <std::string> T_INTEGER_LITERAL T_FLOAT_LITERAL T_STRING_LITERAL T_CHARACTER_LITERAL
%token <std::string> T_VAR_ID

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

literal : T_INTEGER_LITERAL { $$ = new A_Integer($1, Y_POS(@1)); std::free($1); }
        | T_FLOAT_LITERAL { $$ = new A_Float($1, Y_POS(@1)); std::free($1); }
        | booleanLiteral { $$ = $1; }
        | T_CHARACTER_LITERAL { $$ = new A_Character($1, Y_POS(@1)); std::free($1); }
        | T_STRING_LITERAL { $$ = new A_String($1, Y_POS(@1)); std::free($1); }
        | "nil" { $$ = new A_Nil(Y_POS(@1)); }
        | "void" { $$ = new A_Void(Y_POS(@1)); }
        ;

booleanLiteral : "true" { $$ = new A_Boolean(true, Y_POS(@1)); }
               | "false" { $$ = new A_Boolean(false, Y_POS(@1)); }
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
