%option noyywrap nounput noinput nodefault yylineno 8bit
%option reentrant bison-locations bison-bridge
%option never-interactive
%option extra-type="Scanner *"

%x COMMENT
%x IMPORT
%x MULTISTR

UCN (\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})
INT (0|[1-9][0-9]*)
EXP ([eE][-+]?[0-9]+)
HEX ([0-9a-fA-F])
HEXEXP ([pP][-+]?[0-9]+)
FS (d|D)
IS ((u|U)|(u|U)?(l|L))

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
 /* "let"     { return T_INTEGER_TOKEN(T_LET); } */
"try"        { return T_INTEGER_TOKEN(T_TRY); }
"catch"      { return T_INTEGER_TOKEN(T_CATCH); }
"var"        { return T_INTEGER_TOKEN(T_VAR); }
"val"        { return T_INTEGER_TOKEN(T_VAL); }
 /* "nil"        { return T_INTEGER_TOKEN(T_NIL); } */
"null"       { return T_INTEGER_TOKEN(T_NULL); }
"new"        { return T_INTEGER_TOKEN(T_NEW); }
"delete"     { return T_INTEGER_TOKEN(T_DELETE); }
"def"        { return T_INTEGER_TOKEN(T_DEF); }
"if"         { return T_INTEGER_TOKEN(T_IF); }
"then"       { return T_INTEGER_TOKEN(T_THEN); }
"else"       { return T_INTEGER_TOKEN(T_ELSE); }
"switch"     { return T_INTEGER_TOKEN(T_SWITCH); }
"case"       { return T_INTEGER_TOKEN(T_CASE); }
"match"      { return T_INTEGER_TOKEN(T_MATCH); }
"default"    { return T_INTEGER_TOKEN(T_DEFAULT); }
"for"        { return T_INTEGER_TOKEN(T_FOR); }
 /* "foreach"    { return T_INTEGER_TOKEN(T_FOREACH); } */
"in"         { return T_INTEGER_TOKEN(T_IN); }
"while"      { return T_INTEGER_TOKEN(T_WHILE); }
"break"      { return T_INTEGER_TOKEN(T_BREAK); }
"continue"   { return T_INTEGER_TOKEN(T_CONTINUE); }
 /* "func"       { return T_INTEGER_TOKEN(T_FUNC); } */
"class"      { return T_INTEGER_TOKEN(T_CLASS); }
 /* "type"      { return T_INTEGER_TOKEN(T_TYPE); } */
"is"         { return T_INTEGER_TOKEN(T_IS); }
"isa"        { return T_INTEGER_TOKEN(T_ISA); }
"isinstanceof"  { return T_INTEGER_TOKEN(T_ISINSTANCEOF); }
 /* "isinstance" { return T_INTEGER_TOKEN(T_ISINSTANCE); } */
"import"     { return T_INTEGER_TOKEN(T_IMPORT); }
"return"     { return T_INTEGER_TOKEN(T_RETURN); }
"void"       { return T_INTEGER_TOKEN(T_VOID); }
"and"        { return T_INTEGER_TOKEN(T_LOGIC_AND); }
"or"         { return T_INTEGER_TOKEN(T_LOGIC_OR); }
"not"        { return T_INTEGER_TOKEN(T_LOGIC_NOT); }
"int8"       { return T_INTEGER_TOKEN(T_INT8); }
"uint8"      { return T_INTEGER_TOKEN(T_UINT8); }
"int16"      { return T_INTEGER_TOKEN(T_INT16); }
"uint16"     { return T_INTEGER_TOKEN(T_UINT16); }
"int32"      { return T_INTEGER_TOKEN(T_INT32); }
"uint32"     { return T_INTEGER_TOKEN(T_UINT32); }
"int64"      { return T_INTEGER_TOKEN(T_INT64); }
"uint64"     { return T_INTEGER_TOKEN(T_UINT64); }
"int128"     { return T_INTEGER_TOKEN(T_INT128); }
"uint128"    { return T_INTEGER_TOKEN(T_UINT128); }
"float32"    { return T_INTEGER_TOKEN(T_FLOAT32); }
"float64"    { return T_INTEGER_TOKEN(T_FLOAT64); }
"float128"   { return T_INTEGER_TOKEN(T_FLOAT128); }
"string"     { return T_INTEGER_TOKEN(T_STRING); }
"boolean"    { return T_INTEGER_TOKEN(T_BOOLEAN); }
"async"      { return T_INTEGER_TOKEN(T_ASYNC); }
"await"      { return T_INTEGER_TOKEN(T_AWAIT); }
"static"     { return T_INTEGER_TOKEN(T_STATIC); }
"public"     { return T_INTEGER_TOKEN(T_PUBLIC); }
"protect"    { return T_INTEGER_TOKEN(T_PROTECT); }
"private"    { return T_INTEGER_TOKEN(T_PRIVATE); }
"nan"        { return T_INTEGER_TOKEN(T_NAN); }
"inf"        { return T_INTEGER_TOKEN(T_INF); }

 /* punctuators */
"+"    { return T_INTEGER_TOKEN(T_ADD); }
"-"    { return T_INTEGER_TOKEN(T_SUB); }
"*"    { return T_INTEGER_TOKEN(T_MUL); }
"/"    { return T_INTEGER_TOKEN(T_DIV); }
"%"    { return T_INTEGER_TOKEN(T_MOD); }
"_"    { return T_INTEGER_TOKEN(T_UNDERLINE); }
"++"   { return T_INTEGER_TOKEN(T_ADD2); }
"--"   { return T_INTEGER_TOKEN(T_SUB2); }
"**"   { return T_INTEGER_TOKEN(T_MUL2); }
"//"   { return T_INTEGER_TOKEN(T_DIV2); }
"%%"   { return T_INTEGER_TOKEN(T_MOD2); }
"&&"   { return T_INTEGER_TOKEN(T_LOGIC_AND); }
"||"   { return T_INTEGER_TOKEN(T_LOGIC_OR); }
"!"    { return T_INTEGER_TOKEN(T_LOGIC_NOT); }
"&"    { return T_INTEGER_TOKEN(T_BIT_AND); }
"|"    { return T_INTEGER_TOKEN(T_BIT_OR); }
"~"    { return T_INTEGER_TOKEN(T_BIT_NOT); }
"^"    { return T_INTEGER_TOKEN(T_BIT_XOR); }
"<<"   { return T_INTEGER_TOKEN(T_BIT_LSHIFT); }
">>"   { return T_INTEGER_TOKEN(T_BIT_RSHIFT); }
">>>"  { return T_INTEGER_TOKEN(T_BIT_ARSHIFT); }
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
"=="   { return T_INTEGER_TOKEN(T_EQ); }
"!="   { return T_INTEGER_TOKEN(T_NEQ); }
"<"    { return T_INTEGER_TOKEN(T_LT); }
"<="   { return T_INTEGER_TOKEN(T_LE); }
">"    { return T_INTEGER_TOKEN(T_GT); }
">="   { return T_INTEGER_TOKEN(T_GE); }
"("    { return T_INTEGER_TOKEN(T_LPAREN); }
")"    { return T_INTEGER_TOKEN(T_RPAREN); }
"["    { return T_INTEGER_TOKEN(T_LBRACKET); }
"]"    { return T_INTEGER_TOKEN(T_RBRACKET); }
"{"    { return T_INTEGER_TOKEN(T_LBRACE); }
"}"    { return T_INTEGER_TOKEN(T_RBRACE); }
","    { return T_INTEGER_TOKEN(T_COMMA); }
";"    { return T_INTEGER_TOKEN(T_SEMI); }
"?"    { return T_INTEGER_TOKEN(T_QUESTION); }
":"    { return T_INTEGER_TOKEN(T_COLON); }
"."    { return T_INTEGER_TOKEN(T_DOT); }
"=>"   { return T_INTEGER_TOKEN(T_RIGHT_ARROW); }

 /* integer constants */
 /* decimal integer: 102813(u|U|l|L|ul|UL) */
{INT}{IS}?                              { T_LITERAL_TOKEN; return T_INTEGER_LITERAL; }
 /* heximal integer: 0x03ff(u|U|l|L|ul|UL) */
0[xX]{HEX}+{IS}?                        { T_LITERAL_TOKEN; return T_INTEGER_LITERAL; }
 /* octal integer: 0o27121(u|U|l|L|ul|UL) */
0[oO][0-7]+{IS}?                        { T_LITERAL_TOKEN; return T_INTEGER_LITERAL; }
 /* binary integer: 0b10011(u|U|l|L|ul|UL) */
0[bB][01]+{IS}?                         { T_LITERAL_TOKEN; return T_INTEGER_LITERAL; }

 /* floating constants */
 /* floating number: 92.312(d|D), 92.312e-10(d|D), 102e+91(d|D) */
{INT}"."[0-9]+{EXP}?{FS}?               { T_LITERAL_TOKEN; return T_FLOAT_LITERAL; }
{INT}{EXP}{FS}?                         { T_LITERAL_TOKEN; return T_FLOAT_LITERAL; }
 /* floating hexcimal number: 0x1fa9p+7D, 0x2f.3abfp-3D */
0[xX]{HEX}+{HEXEXP}{FS}?                { T_LITERAL_TOKEN; return T_FLOAT_LITERAL; }
0[xX]({HEX}+"."{HEX}+){HEXEXP}?{FS}?    { T_LITERAL_TOKEN; return T_FLOAT_LITERAL; }

 /* multiple line string literal */
["]{3}(["]{0,2}([^\\"]|\\(.|\n)|{UCN}))*["]{3}  { T_LITERAL_TOKEN_X(3, 3); return T_STRING_LITERAL; }
 /* empty string */
\"\"                                    { T_LITERAL_TOKEN_X(1, 1); return T_STRING_LITERAL; }
 /* one line string literal */
\"([^\"\\]|\\['"?\\abfnrtv]|{UCN})+\"   { T_LITERAL_TOKEN_X(1, 1); return T_STRING_LITERAL; }
 /* char literal */
\'([^'\\]|\\['"?\\abfnrtv]|{UCN})\'     { T_LITERAL_TOKEN_X(1, 1); return T_CHAR_LITERAL; }

 /* identifier */
[a-zA-Z_][a-zA-Z0-9_]*                  { T_LITERAL_TOKEN; return T_IDENTIFIER; }
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
