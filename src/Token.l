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

#define T_TOKEN         (Y_LVAL->literal = Strings::copy(Y_TEXT, Y_LENG))
#define T_TOKEN_X(p, q) (Y_LVAL->literal = Strings::copy(Y_TEXT+(p), Y_LENG-(p)-(q)))

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
"true"       { T_TOKEN; return T_TRUE; }
"false"      { T_TOKEN; return T_FALSE; }
"let"        { T_TOKEN; return T_LET; }
"try"        { T_TOKEN; return T_TRY; }
"catch"      { T_TOKEN; return T_CATCH; }
"var"        { T_TOKEN; return T_VAR; }
"val"        { T_TOKEN; return T_VAL; }
"nil"        { T_TOKEN; return T_NIL; }
"if"         { T_TOKEN; return T_IF; }
"else"       { T_TOKEN; return T_ELSE; }
"switch"     { T_TOKEN; return T_SWITCH; }
"case"       { T_TOKEN; return T_CASE; }
"match"      { T_TOKEN; return T_MATCH; }
"default"    { T_TOKEN; return T_DEFAULT; }
"for"        { T_TOKEN; return T_FOR; }
"foreach"    { T_TOKEN; return T_FOREACH; }
"in"         { T_TOKEN; return T_IN; }
"while"      { T_TOKEN; return T_WHILE; }
"break"      { T_TOKEN; return T_BREAK; }
"continue"   { T_TOKEN; return T_CONTINUE; }
"func"       { T_TOKEN; return T_FUNC; }
"class"      { T_TOKEN; return T_CLASS; }
"type"       { T_TOKEN; return T_TYPE; }
"is"         { T_TOKEN; return T_IS; }
"isinstance" { T_TOKEN; return T_ISINSTANCE; }
"import"     { T_TOKEN; return T_IMPORT; }
"return"     { T_TOKEN; return T_RETURN; }
"void"       { T_TOKEN; return T_VOID; }
"and"        { T_TOKEN; return T_LOGIC_AND; }
"or"         { T_TOKEN; return T_LOGIC_OR; }
"not"        { T_TOKEN; return T_LOGIC_NOT; }
"int8"       { T_TOKEN; return T_INT8; }
"uint8"      { T_TOKEN; return T_UINT8; }
"int16"      { T_TOKEN; return T_INT16; }
"uint16"     { T_TOKEN; return T_UINT16; }
"int32"      { T_TOKEN; return T_INT32; }
"uint32"     { T_TOKEN; return T_UINT32; }
"int64"      { T_TOKEN; return T_INT64; }
"uint64"     { T_TOKEN; return T_UINT64; }
"int128"     { T_TOKEN; return T_INT128; }
"uint128"    { T_TOKEN; return T_UINT128; }
"float32"    { T_TOKEN; return T_FLOAT32; }
"float64"    { T_TOKEN; return T_FLOAT64; }
"float128"   { T_TOKEN; return T_FLOAT128; }
"string"     { T_TOKEN; return T_STRING; }
"boolean"    { T_TOKEN; return T_BOOLEAN; }
"async"      { T_TOKEN; return T_ASYNC; }
"await"      { T_TOKEN; return T_AWAIT; }
"public"     { T_TOKEN; return T_PUBLIC; }
"protect"    { T_TOKEN; return T_PROTECT; }
"private"    { T_TOKEN; return T_PRIVATE; }
"nan"        { T_TOKEN; return T_NAN; }
"inf"        { T_TOKEN; return T_INF; }

 /* punctuators */
"+"    { T_TOKEN; return T_ADD; }
"-"    { T_TOKEN; return T_SUB; }
"*"    { T_TOKEN; return T_MUL; }
"/"    { T_TOKEN; return T_DIV; }
"%"    { T_TOKEN; return T_MOD; }
"_"    { T_TOKEN; return T_UNDERLINE; }
"++"   { T_TOKEN; return T_ADD2; }
"--"   { T_TOKEN; return T_SUB2; }
"**"   { T_TOKEN; return T_MUL2; }
"//"   { T_TOKEN; return T_DIV2; }
"%%"   { T_TOKEN; return T_MOD2; }
"&&"   { T_TOKEN; return T_LOGIC_AND; }
"||"   { T_TOKEN; return T_LOGIC_OR; }
"!"    { T_TOKEN; return T_LOGIC_NOT; }
"&"    { T_TOKEN; return T_BIT_AND; }
"|"    { T_TOKEN; return T_BIT_OR; }
"~"    { T_TOKEN; return T_BIT_NOT; }
"^"    { T_TOKEN; return T_BIT_XOR; }
"<<"   { T_TOKEN; return T_BIT_LSHIFT; }
">>"   { T_TOKEN; return T_BIT_RSHIFT; }
">>>"  { T_TOKEN; return T_BIT_ARSHIFT; }
"="    { T_TOKEN; return T_ASSIGN; }
"+="   { T_TOKEN; return T_ADD_ASSIGN; }
"-="   { T_TOKEN; return T_SUB_ASSIGN; }
"*="   { T_TOKEN; return T_MUL_ASSIGN; }
"/="   { T_TOKEN; return T_DIV_ASSIGN; }
"%="   { T_TOKEN; return T_MOD_ASSIGN; }
"&="   { T_TOKEN; return T_BIT_AND_ASSIGN; }
"|="   { T_TOKEN; return T_BIT_OR_ASSIGN; }
"^="   { T_TOKEN; return T_BIT_XOR_ASSIGN; }
"<<="  { T_TOKEN; return T_BIT_LSHIFT_ASSIGN; }
">>="  { T_TOKEN; return T_BIT_RSHIFT_ASSIGN; }
">>>=" { T_TOKEN; return T_BIT_ARSHIFT_ASSIGN; }
"=="   { T_TOKEN; return T_EQ; }
"!="   { T_TOKEN; return T_NEQ; }
"<"    { T_TOKEN; return T_LT; }
"<="   { T_TOKEN; return T_LE; }
">"    { T_TOKEN; return T_GT; }
">="   { T_TOKEN; return T_GE; }
"("    { T_TOKEN; return T_LPAREN; }
")"    { T_TOKEN; return T_RPAREN; }
"["    { T_TOKEN; return T_LBRACKET; }
"]"    { T_TOKEN; return T_RBRACKET; }
"{"    { T_TOKEN; return T_LBRACE; }
"}"    { T_TOKEN; return T_RBRACE; }
","    { T_TOKEN; return T_COMMA; }
";"    { T_TOKEN; return T_SEMI; }
"?"    { T_TOKEN; return T_QUESTION; }
":"    { T_TOKEN; return T_COLON; }
"."    { T_TOKEN; return T_DOT; }
"=>"   { T_TOKEN; return T_BIG_ARROW; }

 /* integer constants */
 /* decimal integer: 102813 */
{INT}                                   { T_TOKEN; return T_INT32_CONSTANT; }
{INT}[Uu]                               { T_TOKEN_X(0, 1); return T_UINT32_CONSTANT; }
{INT}[Ll]                               { T_TOKEN_X(0, 1); return T_INT64_CONSTANT; }
{INT}[UL|ul]                            { T_TOKEN_X(0, 2); return T_UINT64_CONSTANT; }
 /* heximal integer: 0x03ff */
0[xX]{HEX}+                             { T_TOKEN; return T_INT32_CONSTANT; }
0[xX]{HEX}+[Uu]                         { T_TOKEN_X(0, 1); return T_UINT32_CONSTANT; }
0[xX]{HEX}+[Ll]                         { T_TOKEN_X(0, 1); return T_INT64_CONSTANT; }
0[xX]{HEX}+[UL|ul]                      { T_TOKEN_X(0, 2); return T_UINT64_CONSTANT; }
 /* octal integer: 0o27121 */
0[oO][0-7]+                             { T_TOKEN; return T_INT32_CONSTANT; }
0[oO][0-7]+[Uu]                         { T_TOKEN_X(0, 1); return T_UINT32_CONSTANT; }
0[oO][0-7]+[Ll]                         { T_TOKEN_X(0, 1); return T_INT64_CONSTANT; }
0[oO][0-7]+[UL|ul]                      { T_TOKEN_X(0, 2); return T_UINT64_CONSTANT; }
 /* binary integer: 0b10011 */
0[bB][01]+                              { T_TOKEN; return T_INT32_CONSTANT; }
0[bB][01]+[Uu]                          { T_TOKEN_X(0, 1); return T_UINT32_CONSTANT; }
0[bB][01]+[Ll]                          { T_TOKEN_X(0, 1); return T_INT64_CONSTANT; }
0[bB][01]+[UL|ul]                       { T_TOKEN_X(0, 2); return T_UINT64_CONSTANT; }

 /* floating constants */
 /* floating number: 92.312, 92.312e-10, 102e+91 */
{INT}"."[0-9]+{EXP}?                    { T_TOKEN; return T_FLOAT32_CONSTANT; }
{INT}{EXP}                              { T_TOKEN; return T_FLOAT32_CONSTANT; }
{INT}"."[0-9]+{EXP}?[Dd]                { T_TOKEN; return T_FLOAT64_CONSTANT; }
{INT}{EXP}[Dd]                          { T_TOKEN; return T_FLOAT64_CONSTANT; }
 /* floating hexcimal number: 0x1fa9p+7, 0x2f.3abfp-3 */
0[xX]{HEX}+{HEXEXP}                     { T_TOKEN; return T_FLOAT32; }
0[xX]({HEX}+"."{HEX}+){HEXEXP}?         { T_TOKEN; return T_FLOAT32_CONSTANT; }
0[xX]{HEX}+{HEXEXP}[Dd]                 { T_TOKEN; return T_FLOAT64_CONSTANT; }
0[xX]({HEX}+"."{HEX}+){HEXEXP}?[Dd]     { T_TOKEN; return T_FLOAT64_CONSTANT; }

 /* string literal */
\"\"                                    { T_TOKEN; return T_STRING_CONSTANT; }
\"([^\"\\]|\\['"?\\abfnrtv]|{UCN})+\"   { T_TOKEN_X(1, 1); return T_STRING_CONSTANT; }
\'([^'\\]|\\['"?\\abfnrtv]|{UCN})\'     { T_TOKEN_X(1, 1); return T_CHAR_CONSTANT; }

 /* identifier */
[a-zA-Z_][a-zA-Z0-9_]*                  { T_TOKEN; return T_IDENTIFIER; }

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
