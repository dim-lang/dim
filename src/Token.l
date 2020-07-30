%option noyywrap nounput noinput nodefault yylineno 8bit
%option reentrant bison-locations bison-bridge
%option never-interactive
%option extra-type="Scanner *"

%x COMMENT
%x IMPORT

UCN (\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})
INT (0|[1-9][0-9]*)
EXP ([eE][-+]?[0-9]+)
HEX ([0-9a-fA-F])
FS ((f|F)|(l|L))
IS ((u|U)|(u|U)?(l|L)(l|L)?)

%{
#include "Ast.h"
#include "Buffer.h"
#include "Parser.tab.hpp"
#include "Scanner.h"
#include "Strings.h"
#include "Exception.h"
#include "Log.h"

#define Y_LENG          yyget_leng(yyextra->yy_scaninfo())
#define Y_TEXT          yyget_text(yyextra->yy_scaninfo())
#define Y_LVAL          yyget_lval(yyextra->yy_scaninfo())
#define Y_LLOC          yyget_lloc(yyextra->yy_scaninfo())
#define Y_LINENO        yyget_lineno(yyextra->yy_scaninfo())

#define T_LITERAL_TOKEN         (Y_LVAL->literal = Strings::dup(Y_TEXT, Y_LENG))
#define T_LITERAL_TOKEN_X(p, q) (Y_LVAL->literal = Strings::dup(Y_TEXT+(p), Y_LENG-(p)-(q)))
#define T_INTEGER_TOKEN(t)      (Y_LVAL->token = t)

#define YY_USER_ACTION                                                      \
    Y_LLOC->first_line = Y_LLOC->last_line;                                 \
    Y_LLOC->first_column = Y_LLOC->last_column;                             \
    if (Y_LLOC->last_line == Y_LINENO) {                                    \
        Y_LLOC->last_column += Y_LENG;                                      \
    } else {                                                                \
        Y_LLOC->last_line = Y_LINENO;                                       \
        Y_LLOC->last_column = Y_TEXT + Y_LENG - strrchr(Y_TEXT, '\n');      \
    }

%}

%%

 /* block comment */
"/*"                    { BEGIN COMMENT; }
<COMMENT>"*/"           { BEGIN INITIAL; }
<COMMENT>([^*]|\n)+|.   { /* skip everything */ }
<COMMENT><<EOF>>        {
                            EX_ASSERT(false, "{}:{} error! unterminated comment!", yyextra->currentBuffer(), Y_LINENO);
                            return 0;
                        }

 /* line comment */
"//".*\n                { /* skip everything until end of line */ }

 /* keywords */
"true"       { return T_INTEGER_TOKEN(T_TRUE); }
"false"      { return T_INTEGER_TOKEN(T_FALSE); }
"try"        { return T_INTEGER_TOKEN(T_TRY); }
"catch"      { return T_INTEGER_TOKEN(T_CATCH); }
"var"        { return T_INTEGER_TOKEN(T_VAR); }
"val"        { return T_INTEGER_TOKEN(T_VAL); }
"nil"        { return T_INTEGER_TOKEN(T_NIL); }
"new"        { return T_INTEGER_TOKEN(T_NEW); }
"delete"     { return T_INTEGER_TOKEN(T_DELETE); }
"def"        { return T_INTEGER_TOKEN(T_DEF); }
"if"         { return T_INTEGER_TOKEN(T_IF); }
"then"       { return T_INTEGER_TOKEN(T_THEN); }
"else"       { return T_INTEGER_TOKEN(T_ELSE); }
"enum"       { return T_INTEGER_TOKEN(T_ENUM); }
"switch"     { return T_INTEGER_TOKEN(T_SWITCH); }
"case"       { return T_INTEGER_TOKEN(T_CASE); }
"match"      { return T_INTEGER_TOKEN(T_MATCH); }
"for"        { return T_INTEGER_TOKEN(T_FOR); }
"foreach"    { return T_INTEGER_TOKEN(T_FOREACH); }
"in"         { return T_INTEGER_TOKEN(T_IN); }
"while"      { return T_INTEGER_TOKEN(T_WHILE); }
"do"         { return T_INTEGER_TOKEN(T_DO); }
"break"      { return T_INTEGER_TOKEN(T_BREAK); }
"continue"   { return T_INTEGER_TOKEN(T_CONTINUE); }
"class"      { return T_INTEGER_TOKEN(T_CLASS); }
"type"       { return T_INTEGER_TOKEN(T_TYPE); }
"this"       { return T_INTEGER_TOKEN(T_THIS); }
"super"      { return T_INTEGER_TOKEN(T_SUPER); }
"is"         { return T_INTEGER_TOKEN(T_IS); }
"isa"        { return T_INTEGER_TOKEN(T_ISA); }
"isinstanceof"  { return T_INTEGER_TOKEN(T_ISINSTANCEOF); }
"import"     { return T_INTEGER_TOKEN(T_IMPORT); }
"return"     { return T_INTEGER_TOKEN(T_RETURN); }
"void"       { return T_INTEGER_TOKEN(T_VOID); }
"nan"        { return T_INTEGER_TOKEN(T_NAN); }
"inf"        { return T_INTEGER_TOKEN(T_INF); }
"async"      { return T_INTEGER_TOKEN(T_ASYNC); }
"await"      { return T_INTEGER_TOKEN(T_AWAIT); }
"static"     { return T_INTEGER_TOKEN(T_STATIC); }
"public"     { return T_INTEGER_TOKEN(T_PUBLIC); }
"protect"    { return T_INTEGER_TOKEN(T_PROTECT); }
"private"    { return T_INTEGER_TOKEN(T_PRIVATE); }

 /***
   * primitive integer type
   * byte: 8 bit int
   * ubyte: 8 bit unsigned int
   * short: 16 bit int
   * ushort: 16 bit unsigned int
   * int: 32 bit int
   * uint: 32 bit unsigned int
   * long: 64 bit int
   * ulong: 64 bit unsigned int
   * llong: 128 bit int
   * ullong: 128 bit unsigned int
   */
"byte"       { return T_INTEGER_TOKEN(T_BYTE); }
"ubyte"      { return T_INTEGER_TOKEN(T_UBYTE); }
"short"      { return T_INTEGER_TOKEN(T_SHORT); }
"ushort"     { return T_INTEGER_TOKEN(T_USHORT); }
"int"        { return T_INTEGER_TOKEN(T_INT); }
"uint"       { return T_INTEGER_TOKEN(T_UINT); }
"long"       { return T_INTEGER_TOKEN(T_LONG); }
"ulong"      { return T_INTEGER_TOKEN(T_ULONG); }
"llong"      { return T_INTEGER_TOKEN(T_LLONG); }
"ullong"     { return T_INTEGER_TOKEN(T_ULLONG); }

 /***
   * primitive integer type
   * float: 32 bit float
   * double: 64 bit float
   * ldouble: 128 bit float
   */
"float"      { return T_INTEGER_TOKEN(T_FLOAT); }
"double"     { return T_INTEGER_TOKEN(T_DOUBLE); }
"ldouble"    { return T_INTEGER_TOKEN(T_LDOUBLE); }

 /***
   * primitive boolean type
   * boolean: 1 bit unsigned int
   */
"boolean"    { return T_INTEGER_TOKEN(T_BOOLEAN); }

 /* binary/unary operator */
"and"  { return T_INTEGER_TOKEN(T_LOGIC_AND); }
"or"   { return T_INTEGER_TOKEN(T_LOGIC_OR); }
"not"  { return T_INTEGER_TOKEN(T_LOGIC_NOT); }
"+"    { return T_INTEGER_TOKEN(T_ADD); }
"-"    { return T_INTEGER_TOKEN(T_SUB); }
"*"    { return T_INTEGER_TOKEN(T_MUL); }
"/"    { return T_INTEGER_TOKEN(T_DIV); }
"%"    { return T_INTEGER_TOKEN(T_MOD); }
"++"   { return T_INTEGER_TOKEN(T_ADD2); }
"--"   { return T_INTEGER_TOKEN(T_SUB2); }
"**"   { return T_INTEGER_TOKEN(T_MUL2); }
"//"   { return T_INTEGER_TOKEN(T_DIV2); }
"%%"   { return T_INTEGER_TOKEN(T_MOD2); }
"&&"   { return T_INTEGER_TOKEN(T_LOGIC_AND); }
"||"   { return T_INTEGER_TOKEN(T_LOGIC_OR); }
"<<"   { return T_INTEGER_TOKEN(T_BIT_LSHIFT); }
">>"   { return T_INTEGER_TOKEN(T_BIT_RSHIFT); }
">>>"  { return T_INTEGER_TOKEN(T_BIT_ARSHIFT); }

 /* unary operator */
"!"    { return T_INTEGER_TOKEN(T_LOGIC_NOT); }
"&"    { return T_INTEGER_TOKEN(T_BIT_AND); }
"|"    { return T_INTEGER_TOKEN(T_BIT_OR); }
"~"    { return T_INTEGER_TOKEN(T_BIT_NOT); }
"^"    { return T_INTEGER_TOKEN(T_BIT_XOR); }

 /* assignment operator */
"="    { return T_INTEGER_TOKEN(T_ASSIGN); }
"+="   { return T_INTEGER_TOKEN(T_ADD_ASSIGN); }
"-="   { return T_INTEGER_TOKEN(T_SUB_ASSIGN); }
"*="   { return T_INTEGER_TOKEN(T_MUL_ASSIGN); }
"/="   { return T_INTEGER_TOKEN(T_DIV_ASSIGN); }
"%="   { return T_INTEGER_TOKEN(T_MOD_ASSIGN); }
"&="   { return T_INTEGER_TOKEN(T_BIT_AND_ASSIGN); }
"|="   { return T_INTEGER_TOKEN(T_BIT_OR_ASSIGN); }
"^="   { return T_INTEGER_TOKEN(T_BIT_XOR_ASSIGN); }
"<<="  { return T_INTEGER_TOKEN(T_BIT_LSHIFT_ASSIGN); }
">>="  { return T_INTEGER_TOKEN(T_BIT_RSHIFT_ASSIGN); }
">>>=" { return T_INTEGER_TOKEN(T_BIT_ARSHIFT_ASSIGN); }

 /* compare operator */
"=="   { return T_INTEGER_TOKEN(T_EQ); }
"!="   { return T_INTEGER_TOKEN(T_NEQ); }
"<"    { return T_INTEGER_TOKEN(T_LT); }
"<="   { return T_INTEGER_TOKEN(T_LE); }
">"    { return T_INTEGER_TOKEN(T_GT); }
">="   { return T_INTEGER_TOKEN(T_GE); }

 /* parentheses */
"("    { return T_INTEGER_TOKEN(T_LPAREN); }
")"    { return T_INTEGER_TOKEN(T_RPAREN); }
"["    { return T_INTEGER_TOKEN(T_LBRACKET); }
"]"    { return T_INTEGER_TOKEN(T_RBRACKET); }
"{"    { return T_INTEGER_TOKEN(T_LBRACE); }
"}"    { return T_INTEGER_TOKEN(T_RBRACE); }

 /* other punctuation */
"_"    { return T_INTEGER_TOKEN(T_UNDERLINE); }
","    { return T_INTEGER_TOKEN(T_COMMA); }
";"    { return T_INTEGER_TOKEN(T_SEMI); }
"?"    { return T_INTEGER_TOKEN(T_QUESTION); }
":"    { return T_INTEGER_TOKEN(T_COLON); }
"."    { return T_INTEGER_TOKEN(T_DOT); }
"->"   { return T_INTEGER_TOKEN(T_SINGLE_RARROW); }
"=>"   { return T_INTEGER_TOKEN(T_DOUBLE_RARROW); }
"<-"   { return T_INTEGER_TOKEN(T_SINGLE_LARROW); }

 /* decimal integer literal */
{INT}{IS}?                              { T_LITERAL_TOKEN; return T_INTEGER_LITERAL; }
 /* heximal integer literal */
0[xX]{HEX}+{IS}?                        { T_LITERAL_TOKEN; return T_INTEGER_LITERAL; }
 /* octal integer literal */
0[oO][0-7]+{IS}?                        { T_LITERAL_TOKEN; return T_INTEGER_LITERAL; }
 /* binary integer literal */
0[bB][01]+{IS}?                         { T_LITERAL_TOKEN; return T_INTEGER_LITERAL; }

 /* float literal : 92.312(f|F), 92.312e-10(f|F), 10.2e+91(f|F), 1.0e+7(l|L) */
{INT}"."[0-9]+{EXP}?{FS}?               { T_LITERAL_TOKEN; return T_FLOAT_LITERAL; }
{INT}{EXP}{FS}?                         { T_LITERAL_TOKEN; return T_FLOAT_LITERAL; }

 /* multiple line string literal */
["]{3}(["]{0,2}([^\\"]|\\(.|\n)))*["]{3}    { T_LITERAL_TOKEN; return T_MULTI_STRING_LITERAL; }
 /* empty string */
\"\"                                    { T_LITERAL_TOKEN; return T_SINGLE_STRING_LITERAL; }
 /* one line string literal */
\"([^\"\\]|\\['"?\\abfnrtv]|{UCN})+\"   { T_LITERAL_TOKEN; return T_SINGLE_STRING_LITERAL; }

 /* char literal */
\'([^'\\]|\\['"?\\abfnrtv]|{UCN})\'     { T_LITERAL_TOKEN; return T_CHAR_LITERAL; }

 /* identifier */
[a-zA-Z_][a-zA-Z0-9_]*                  { T_LITERAL_TOKEN; return T_PLAINID; }

 /* whitespace */
[ \t\n\v\f\r]                           { /* skip whitespace */ }

 /* import module */
^[ \t\n\v\f\r]*import[ \t\n\v\f\r]*     { BEGIN IMPORT; }
<IMPORT>[^ \t\n\v\f\r\;]+               {
                                            yyextra->pushBuffer(Y_TEXT);
                                            BEGIN INITIAL;
                                        }
<IMPORT>.|\n                            {
                                            EX_ASSERT(false, "{}:{} error! bad import:{}, {}!", yyextra->currentBuffer(), Y_LINENO, Y_TEXT, Y_LENG);
                                            BEGIN INITIAL;
                                        }
<<EOF>>                                 {
                                            if (!yyextra->popBuffer())
                                                yyterminate();
                                        }

 /* invalid token */
.                                       {
                                            EX_ASSERT(false, "{}:{} error! invalid token:{}, {}!", yyextra->currentBuffer(), Y_LINENO, Y_TEXT, Y_LENG);
                                        }

%%
