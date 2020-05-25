%option noyywrap nounput noinput nodefault yylineno 8bit
%option reentrant bison-locations bison-bridge
%option extra-type="Scanner *"

%x COMMENT
%x IMPORT

UCN (\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})
INT (0|[1-9][0-9]*)
EXP ([eE][-+]?[0-9]+)
HEX ([0-9a-fA-F])
HEXEXP ([pP][-+]?[0-9]+)
FS (d|D)
IS ((u|U)|(u|U)?(l|L))

%{
#include "Log.h"
#include "Token.h"
#include "Ast.h"
#include "Parser.tab.hpp"
#include "Scanner.h"
#include "Strings.h"

#define Y_LENG          yyget_leng(yyextra->yy_scaninfo())
#define Y_TEXT          yyget_text(yyextra->yy_scaninfo())
#define Y_LVAL          yyget_lval(yyextra->yy_scaninfo())
#define Y_LLOC          yyget_lloc(yyextra->yy_scaninfo())
#define Y_LINENO        yyget_lineno(yyextra->yy_scaninfo())

#define T_LITERAL_TOKEN         (Y_LVAL->literal = Strings::copy(Y_TEXT, Y_LENG))
#define T_LITERAL_TOKEN_X(p, q) (Y_LVAL->literal = Strings::copy(Y_TEXT+(p), Y_LENG-(p)-(q)))

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
                            LOG_ASSERT(false, "{}:{} error! unterminated comment!", yyextra->currentBuffer(), Y_LINENO);
                            return 0;
                        }

 /* line comment */
"//".*\n                { /* skip everything until end of line */ }

 /* keywords */
"true"       { T_LITERAL_TOKEN; return T_TRUE; }
"false"      { T_LITERAL_TOKEN; return T_FALSE; }
"let"        { T_LITERAL_TOKEN; return T_LET; }
"try"        { T_LITERAL_TOKEN; return T_TRY; }
"catch"      { T_LITERAL_TOKEN; return T_CATCH; }
"var"        { T_LITERAL_TOKEN; return T_VAR; }
"val"        { T_LITERAL_TOKEN; return T_VAL; }
"nil"        { T_LITERAL_TOKEN; return T_NIL; }
"if"         { T_LITERAL_TOKEN; return T_IF; }
"else"       { T_LITERAL_TOKEN; return T_ELSE; }
"switch"     { T_LITERAL_TOKEN; return T_SWITCH; }
"case"       { T_LITERAL_TOKEN; return T_CASE; }
"match"      { T_LITERAL_TOKEN; return T_MATCH; }
"default"    { T_LITERAL_TOKEN; return T_DEFAULT; }
"for"        { T_LITERAL_TOKEN; return T_FOR; }
"foreach"    { T_LITERAL_TOKEN; return T_FOREACH; }
"in"         { T_LITERAL_TOKEN; return T_IN; }
"while"      { T_LITERAL_TOKEN; return T_WHILE; }
"break"      { T_LITERAL_TOKEN; return T_BREAK; }
"continue"   { T_LITERAL_TOKEN; return T_CONTINUE; }
"func"       { T_LITERAL_TOKEN; return T_FUNC; }
"class"      { T_LITERAL_TOKEN; return T_CLASS; }
"type"       { T_LITERAL_TOKEN; return T_TYPE; }
"is"         { T_LITERAL_TOKEN; return T_IS; }
"isinstance" { T_LITERAL_TOKEN; return T_ISINSTANCE; }
"import"     { T_LITERAL_TOKEN; return T_IMPORT; }
"return"     { T_LITERAL_TOKEN; return T_RETURN; }
"void"       { T_LITERAL_TOKEN; return T_VOID; }
"and"        { T_LITERAL_TOKEN; return T_LOGIC_AND; }
"or"         { T_LITERAL_TOKEN; return T_LOGIC_OR; }
"not"        { T_LITERAL_TOKEN; return T_LOGIC_NOT; }
"int8"       { T_LITERAL_TOKEN; return T_INT8; }
"uint8"      { T_LITERAL_TOKEN; return T_UINT8; }
"int16"      { T_LITERAL_TOKEN; return T_INT16; }
"uint16"     { T_LITERAL_TOKEN; return T_UINT16; }
"int32"      { T_LITERAL_TOKEN; return T_INT32; }
"uint32"     { T_LITERAL_TOKEN; return T_UINT32; }
"int64"      { T_LITERAL_TOKEN; return T_INT64; }
"uint64"     { T_LITERAL_TOKEN; return T_UINT64; }
"int128"     { T_LITERAL_TOKEN; return T_INT128; }
"uint128"    { T_LITERAL_TOKEN; return T_UINT128; }
"float32"    { T_LITERAL_TOKEN; return T_FLOAT32; }
"float64"    { T_LITERAL_TOKEN; return T_FLOAT64; }
"float128"   { T_LITERAL_TOKEN; return T_FLOAT128; }
"string"     { T_LITERAL_TOKEN; return T_STRING; }
"boolean"    { T_LITERAL_TOKEN; return T_BOOLEAN; }
"async"      { T_LITERAL_TOKEN; return T_ASYNC; }
"await"      { T_LITERAL_TOKEN; return T_AWAIT; }
"public"     { T_LITERAL_TOKEN; return T_PUBLIC; }
"protect"    { T_LITERAL_TOKEN; return T_PROTECT; }
"private"    { T_LITERAL_TOKEN; return T_PRIVATE; }
"nan"        { T_LITERAL_TOKEN; return T_NAN; }
"inf"        { T_LITERAL_TOKEN; return T_INF; }

 /* punctuators */
"+"    { T_LITERAL_TOKEN; return T_ADD; }
"-"    { T_LITERAL_TOKEN; return T_SUB; }
"*"    { T_LITERAL_TOKEN; return T_MUL; }
"/"    { T_LITERAL_TOKEN; return T_DIV; }
"%"    { T_LITERAL_TOKEN; return T_MOD; }
"_"    { T_LITERAL_TOKEN; return T_UNDERLINE; }
"++"   { T_LITERAL_TOKEN; return T_ADD2; }
"--"   { T_LITERAL_TOKEN; return T_SUB2; }
"**"   { T_LITERAL_TOKEN; return T_MUL2; }
"//"   { T_LITERAL_TOKEN; return T_DIV2; }
"%%"   { T_LITERAL_TOKEN; return T_MOD2; }
"&&"   { T_LITERAL_TOKEN; return T_LOGIC_AND; }
"||"   { T_LITERAL_TOKEN; return T_LOGIC_OR; }
"!"    { T_LITERAL_TOKEN; return T_LOGIC_NOT; }
"&"    { T_LITERAL_TOKEN; return T_BIT_AND; }
"|"    { T_LITERAL_TOKEN; return T_BIT_OR; }
"~"    { T_LITERAL_TOKEN; return T_BIT_NOT; }
"^"    { T_LITERAL_TOKEN; return T_BIT_XOR; }
"<<"   { T_LITERAL_TOKEN; return T_BIT_LSHIFT; }
">>"   { T_LITERAL_TOKEN; return T_BIT_RSHIFT; }
">>>"  { T_LITERAL_TOKEN; return T_BIT_ARSHIFT; }
"="    { T_LITERAL_TOKEN; return T_ASSIGN; }
"+="   { T_LITERAL_TOKEN; return T_ADD_ASSIGN; }
"-="   { T_LITERAL_TOKEN; return T_SUB_ASSIGN; }
"*="   { T_LITERAL_TOKEN; return T_MUL_ASSIGN; }
"/="   { T_LITERAL_TOKEN; return T_DIV_ASSIGN; }
"%="   { T_LITERAL_TOKEN; return T_MOD_ASSIGN; }
"&="   { T_LITERAL_TOKEN; return T_BIT_AND_ASSIGN; }
"|="   { T_LITERAL_TOKEN; return T_BIT_OR_ASSIGN; }
"^="   { T_LITERAL_TOKEN; return T_BIT_XOR_ASSIGN; }
"<<="  { T_LITERAL_TOKEN; return T_BIT_LSHIFT_ASSIGN; }
">>="  { T_LITERAL_TOKEN; return T_BIT_RSHIFT_ASSIGN; }
">>>=" { T_LITERAL_TOKEN; return T_BIT_ARSHIFT_ASSIGN; }
"=="   { T_LITERAL_TOKEN; return T_EQ; }
"!="   { T_LITERAL_TOKEN; return T_NEQ; }
"<"    { T_LITERAL_TOKEN; return T_LT; }
"<="   { T_LITERAL_TOKEN; return T_LE; }
">"    { T_LITERAL_TOKEN; return T_GT; }
">="   { T_LITERAL_TOKEN; return T_GE; }
"("    { T_LITERAL_TOKEN; return T_LPAREN; }
")"    { T_LITERAL_TOKEN; return T_RPAREN; }
"["    { T_LITERAL_TOKEN; return T_LBRACKET; }
"]"    { T_LITERAL_TOKEN; return T_RBRACKET; }
"{"    { T_LITERAL_TOKEN; return T_LBRACE; }
"}"    { T_LITERAL_TOKEN; return T_RBRACE; }
","    { T_LITERAL_TOKEN; return T_COMMA; }
";"    { T_LITERAL_TOKEN; return T_SEMI; }
"?"    { T_LITERAL_TOKEN; return T_QUESTION; }
":"    { T_LITERAL_TOKEN; return T_COLON; }
"."    { T_LITERAL_TOKEN; return T_DOT; }
"=>"   { T_LITERAL_TOKEN; return T_BIG_ARROW; }

 /* integer constants */
 /* decimal integer: 102813(u|U|l|L|ul|UL) */
{INT}{IS}?                              { T_LITERAL_TOKEN; return T_INTEGER_CONSTANT; }
 /* heximal integer: 0x03ff(u|U|l|L|ul|UL) */
0[xX]{HEX}+{IS}?                        { T_LITERAL_TOKEN; return T_INTEGER_CONSTANT; }
 /* octal integer: 0o27121(u|U|l|L|ul|UL) */
0[oO][0-7]+{IS}?                        { T_LITERAL_TOKEN; return T_INTEGER_CONSTANT; }
 /* binary integer: 0b10011(u|U|l|L|ul|UL) */
0[bB][01]+{IS}?                         { T_LITERAL_TOKEN; return T_INTEGER_CONSTANT; }

 /* floating constants */
 /* floating number: 92.312(d|D), 92.312e-10(d|D), 102e+91(d|D) */
{INT}"."[0-9]+{EXP}?{FS}?               { T_LITERAL_TOKEN; return T_FLOAT_CONSTANT; }
{INT}{EXP}{FS}?                         { T_LITERAL_TOKEN; return T_FLOAT_CONSTANT; }
 /* floating hexcimal number: 0x1fa9p+7D, 0x2f.3abfp-3D */
0[xX]{HEX}+{HEXEXP}{FS}?                { T_LITERAL_TOKEN; return T_FLOAT_CONSTANT; }
0[xX]({HEX}+"."{HEX}+){HEXEXP}?{FS}?    { T_LITERAL_TOKEN; return T_FLOAT_CONSTANT; }

 /* string literal */
\"\"                                    { T_LITERAL_TOKEN; return T_STRING_CONSTANT; }
\"([^\"\\]|\\['"?\\abfnrtv]|{UCN})+\"   { T_LITERAL_TOKEN_X(1, 1); return T_STRING_CONSTANT; }
\'([^'\\]|\\['"?\\abfnrtv]|{UCN})\'     { T_LITERAL_TOKEN_X(1, 1); return T_CHAR_CONSTANT; }

 /* identifier */
[a-zA-Z_][a-zA-Z0-9_]*                  { T_LITERAL_TOKEN; return T_IDENTIFIER; }

 /* whitespace */
[ \t\n\v\f\r]                           { /* skip whitespace */ }

 /* import module */
^[ \t\n\v\f\r]*import[ \t\n\v\f\r]* { BEGIN IMPORT; }
<IMPORT>[^ \t\n\v\f\r\;]+           {
                                        yyextra->pushBuffer(Y_TEXT);
                                        BEGIN INITIAL;
                                    }
<IMPORT>.|\n                        {
                                        LOG_ASSERT(false, "{}:{} error! bad import:{}, {}!", yyextra->currentBuffer(), Y_LINENO, Y_TEXT, Y_LENG);
                                        BEGIN INITIAL;
                                    }
<<EOF>>                             {
                                        if (!yyextra->popBuffer())
                                            yyterminate();
                                    }

 /* invalid token */
.                           {
                                LOG_ASSERT(false, "{}:{} error! invalid token:{}, {}!", yyextra->currentBuffer(), Y_LINENO, Y_TEXT, Y_LENG); 
                            }

%%
