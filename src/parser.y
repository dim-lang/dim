%define lr.type ielr
%define api.pure full
%define parse.assert
%define parse.error verbose
%define parse.lac full
%locations
%param {yyscan_t yyscanner}

%code top {
/* #include <cstdlib> */
#include "Log.h"
#include "Ast.h"
#include "Scanner.h"
#include "Location.h"
#include "Strings.h"
#include "tokenizer.yy.hh"
#define T_EOF           0
#define Y_SCANNER       (static_cast<Scanner *>(yyget_extra(yyscanner)))
void yyerror(YYLTYPE *yyllocp, yyscan_t yyscanner, const char *msg);
template<typename T> T* reverse(T* list) {
  // nil node
  if (!list) {
    return list;
  }
  T* head = nullptr;
  T* tail = nullptr;
  while (list) {
    head = list;
    list = list->next;
    head->next = tail;
    tail = head;
  }
  return head;
}
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
%type<token> seminl optionalNewline optionalNewlines newlines
%type<token> assignOp prefixOp infixOp postfixOp

 /* str */
%token<literal> T_INTEGER_LITERAL T_FLOAT_LITERAL T_STRING_LITERAL T_CHARACTER_LITERAL
%token<literal> T_VAR_ID

 /* literal */
%type<ast> literal booleanLiteral
 /* id */
%type<ast> id varId
 /* expr */
%type<ast> expr exprs enumerators assignExpr prefixExpr postfixExpr infixExpr primaryExpr callExpr block blockStat blockStats
%type<ast> optionalExprs optionalBlockStats optionalVarDef optionalExpr
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

seminl : "\n" { $$ = $1; }
       | ";" { $$ = $1; }
       ;

optionalNewline : "\n" { $$ = $1; }
                | %empty { $$ = T_EOF; }
                ;

optionalNewlines : newlines { $$ = $1; }
                 | %empty { $$ = T_EOF; }
                 ;

newlines : "\n" { $$ = $1; }
         | newlines "\n" { $$ = $2; }
         ;

 /* newline } */

 /* literal { */

literal : T_INTEGER_LITERAL { $$ = new A_Integer($1, @$); /* std::free($1); */ }
        | T_FLOAT_LITERAL { $$ = new A_Float($1, @$); /* std::free($1); */ }
        | booleanLiteral { $$ = $1; }
        | T_CHARACTER_LITERAL { $$ = new A_Character($1, @$); /* std::free($1); */ }
        | T_STRING_LITERAL { $$ = new A_String($1, @$); /* std::free($1); */ }
        | "nil" { $$ = new A_Nil(@$); }
        | "void" { $$ = new A_Void(@$); }
        ;

booleanLiteral : "true" { $$ = new A_Boolean("true", @$); }
               | "false" { $$ = new A_Boolean("false", @$); }
               ;

 /* literal } */

 /* id { */

id : varId { $$ = $1; }
   /* | Opid */
   ;

varId : T_VAR_ID { $$ = new A_VarId($1, @$); /* std::free($1); */ }
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

expr : "if" "(" expr ")" optionalNewlines expr %prec "then" { $$ = new A_If($3, $6, nullptr, @$); }
     | "if" "(" expr ")" optionalNewlines expr "else" expr %prec "else" { $$ = new A_If($3, $6, $8, @$); }
     | "while" "(" expr ")" optionalNewlines expr %prec "while" { Ast* loopCondition = new A_LoopCondition(nullptr, $3, nullptr, @3); $$ = new A_Loop(loopCondition, $6, @$); }
     | "do" expr optionalNewlines "while" "(" expr ")" %prec "do_while" { $$ = new A_DoWhile($2, $6, @$); }
     | "for" "(" enumerators ")" optionalNewlines "yield" expr { Ast* loopYield = new A_Yield($7, @7); $$ = new A_Loop($3, loopYield, @$); }
     | "for" "(" enumerators ")" optionalNewlines expr { $$ = new A_Loop($3, $6, @$); }
     | "try" expr "catch" expr %prec "try_catch" { $$ = new A_Try($2, $4, nullptr, @$); }
     | "try" expr "catch" expr "finally" expr %prec "try_catch_finally" { $$ = new A_Try($2, $4, $6, @$); }
     | "throw" expr { $$ = new A_Throw($2, @$); }
     | "return" %prec "return" { $$ = new A_Return(nullptr, @$); }
     | "return" expr %prec "return_expr" { $$ = new A_Return($2, @$); }
     | "continue" { $$ = new A_Continue(@$); }
     | "break" { $$ = new A_Break(@$); }
     | assignExpr { $$ = $1; }
     | postfixExpr { $$ = $1; }
     ;

enumerators : id "<-" expr { $$ = new A_LoopEnumerator($1, $3, @$); }
            | optionalVarDef ";" optionalExpr ";" optionalExpr { $$ = new A_LoopCondition($1, $3, $5, @$); }
            ;

optionalVarDef : varDef { $$ = $1; }
               | %empty { $$ = nullptr; }
               ;

optionalExpr : expr { $$ = $1; }
             | %empty { $$ = nullptr; }
             ;

assignExpr : id assignOp expr { $$ = new A_Assign($1, $2, $3, @$); }
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
            | infixExpr postfixOp { $$ = new A_PostfixExpr($1, $2, @$); }
            ;

postfixOp : "++" { $$ = $1; }
          | "--" { $$ = $1; }
          ;

infixExpr : prefixExpr { $$ = $1; }
          | infixExpr infixOp optionalNewline prefixExpr { $$ = new A_InfixExpr($1, $2, $4, @$); }
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
           | prefixOp primaryExpr { $$ = new A_PrefixExpr($1, $2, @$); }
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

optionalExprs : exprs { $$ = reverse(static_cast<A_Exprs *>($1)); }
              | %empty { $$ = nullptr; }
              ;

exprs : expr { $$ = new A_Exprs($1, nullptr, @$); }
      | exprs "," expr { $$ = new A_Exprs($3, static_cast<A_Exprs *>($1), @$); }
      ;

callExpr : id "(" optionalExprs ")" { $$ = new A_Call($1, static_cast<A_Exprs *>($3), @$); }
         ;

block : "{" blockStat optionalBlockStats "}" {
            A_BlockStats* blockStats = reverse(static_cast<A_BlockStats *>($3));
            blockStats = ($2)
                ? (new A_BlockStats($2, blockStats, @$))
                : blockStats;
            $$ = new A_Block(blockStats, @$);
        }
      ;

blockStat : expr { $$ = $1; }
          | def { $$ = $1; }
          /* | import */
          | %empty { $$ = nullptr; }
          ;

optionalBlockStats : blockStats { $$ = $1; }
                   | %empty { $$ = nullptr; }
                   ;

blockStats : seminl blockStat { $$ = ($2) ? (new A_BlockStats($2, nullptr, @$)) : nullptr; }
           | blockStats seminl blockStat { $$ = ($3) ? (new A_BlockStats($3, static_cast<A_BlockStats *>($1), @$)) : ($1); }
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

plainType : "byte" { $$ = new A_PlainType($1, @$); }
          | "ubyte" { $$ = new A_PlainType($1, @$); }
          | "short" { $$ = new A_PlainType($1, @$); }
          | "ushort" { $$ = new A_PlainType($1, @$); }
          | "int" { $$ = new A_PlainType($1, @$); }
          | "uint" { $$ = new A_PlainType($1, @$); }
          | "long" { $$ = new A_PlainType($1, @$); }
          | "ulong" { $$ = new A_PlainType($1, @$); }
          | "llong" { $$ = new A_PlainType($1, @$); }
          | "ullong" { $$ = new A_PlainType($1, @$); }
          | "char" { $$ = new A_PlainType($1, @$); }
          | "float" { $$ = new A_PlainType($1, @$); }
          | "double" { $$ = new A_PlainType($1, @$); }
          | "boolean" { $$ = new A_PlainType($1, @$); }
          | "void" { $$ = new A_PlainType($1, @$); }
          | "any" { $$ = new A_PlainType($1, @$); }
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

funcDef : "def" funcSign resultType "=" expr { $$ = new A_FuncDef($2, $3, $5, @$); }
        | "def" funcSign resultType optionalNewlines block { $$ = new A_FuncDef($2, $3, $5, @$); }
        ;

/* optionalResultType : resultType { $$ = nullptr; } */
/*                    | %empty { $$ = nullptr; } */
/*                    ; */

resultType : ":" type { $$ = $2; }
           ;

funcSign : id "(" optionalParams ")" { $$ = new A_FuncSign($1, static_cast<A_Params *>($3), @$); }
         ;

optionalParams : params { $$ = reverse(static_cast<A_Params *>($1)); }
               | %empty { $$ = nullptr; }
               ;

params : param { $$ = new A_Params(static_cast<A_Param *>($1), nullptr, @$); }
       | params "," param { $$ = new A_Params(static_cast<A_Param *>($3), static_cast<A_Params *>($1), @$); }
       ;

param : id ":" type { $$ = new A_Param($1, $3, @$); }
      /* | id ":" type "=" expr */
      ;

varDef : "var" id ":" type "=" expr { $$ = new A_VarDef($2, $4, $6, @$); }
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
                    A_TopStats* topStats = reverse(static_cast<A_TopStats *>($2));
                    topStats = ($1)
                        ? (new A_TopStats($1, topStats, @$))
                        : topStats;
                    Y_SCANNER->compileUnit() = new A_CompileUnit(topStats, @$);
                }
            ;

optionalTopStats : topStats { $$ = $1; }
                 | %empty { $$ = nullptr; }
                 ;

topStats : seminl topStat { $$ = ($2) ? (new A_TopStats($2, nullptr, @$)) : nullptr; }
         | topStats seminl topStat { $$ = ($3) ? (new A_TopStats($3, static_cast<A_TopStats *>($1), @$)) : ($1); }
         ;

topStat : def { $$ = $1; }
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

void yyerror(YYLTYPE *yyllocp, yyscan_t yyscanner, const char *msg) {
  if (yyllocp && yyllocp->first_line) {
    fprintf(stderr, "%s: %d.%d-%d.%d: error: ", 
            yyget_extra(yyscanner) ? Y_SCANNER->fileName().rawstr() : "unknown",
            yyllocp->first_line,
            yyllocp->first_column, 
            yyllocp->last_line, 
            yyllocp->last_column);
  }
  fprintf(stderr, "%s\n", msg);
}
