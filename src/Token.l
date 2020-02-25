%option noyywrap nodefault yylineno

%x COMMENT
%x IMPORT

UCN (\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})
INT (0|[1-9][0-9]*)
EXP ([eE][-+]?[0-9]+)
HEX ([0-9a-fA-F])
HEXEXP ([pP][-+]?[0-9]+)

%{
#include "Log.h"
#include "TokenBuffer.h"
#include "Ast.h"
#include "Parser.tab.hpp"

#define T_SAVE_TOKEN            yylval.literal = strndup(yytext, yyleng)
#define T_SAVE_TOKEN_X(p, q)    yylval.literal = strndup(yytext+(p), yyleng-(p)-(q))
#define T_SAVE_NO_TOKEN         yylval.literal = nullptr
#define T_TOKEN(t)              (yylval.token = t)

int yycolumn = 1;

#define YY_USER_ACTION do { \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng; \
} while (0);

%}

%%

 /* block comment */
"/*"                    { BEGIN COMMENT; }
<COMMENT>"*/"           { BEGIN INITIAL; }
<COMMENT>([^*]|\n)+|.   { /* skip everything */ }
<COMMENT><<EOF>>        {
                            CASSERT(false, "{}:{} error! unterminated comment!", TokenBuffer::CurrentBuffer, yylineno);
                            return 0;
                        }

 /* line comment */
"//".*\n                { /* skip everything until end of line */ }

 /* keywords */
"true"       { return T_TOKEN(T_TRUE); }
"false"      { return T_TOKEN(T_FALSE); }
"let"        { return T_TOKEN(T_LET); }
"var"        { return T_TOKEN(T_VAR); }
"val"        { return T_TOKEN(T_VAL); }
"nil"        { return T_TOKEN(T_NIL); }
"if"         { return T_TOKEN(T_IF); }
"else"       { return T_TOKEN(T_ELSE); }
"switch"     { return T_TOKEN(T_SWITCH); }
"case"       { return T_TOKEN(T_CASE); }
"caseof"     { return T_TOKEN(T_CASEOF); }
"default"    { return T_TOKEN(T_DEFAULT); }
"for"        { return T_TOKEN(T_FOR); }
"foreach"    { return T_TOKEN(T_FOREACH); }
"in"         { return T_TOKEN(T_IN); }
"while"      { return T_TOKEN(T_WHILE); }
"break"      { return T_TOKEN(T_BREAK); }
"continue"   { return T_TOKEN(T_CONTINUE); }
"func"       { return T_TOKEN(T_FUNC); }
"class"      { return T_TOKEN(T_CLASS); }
"type"       { return T_TOKEN(T_TYPE); }
"is"         { return T_TOKEN(T_IS); }
"isinstance" { return T_TOKEN(T_ISINSTANCE); }
"import"     { return T_TOKEN(T_IMPORT); }
"return"     { return T_TOKEN(T_RETURN); }
"void"       { return T_TOKEN(T_VOID); }
"and"        { return T_TOKEN(T_LOGIC_AND); }
"or"         { return T_TOKEN(T_LOGIC_OR); }
"not"        { return T_TOKEN(T_LOGIC_NOT); }
"i8"         { return T_TOKEN(T_I8); }
"u8"         { return T_TOKEN(T_U8); }
"i16"        { return T_TOKEN(T_I16); }
"u16"        { return T_TOKEN(T_U16); }
"i32"        { return T_TOKEN(T_I32); }
"u32"        { return T_TOKEN(T_U32); }
"i64"        { return T_TOKEN(T_I64); }
"u64"        { return T_TOKEN(T_U64); }
"f32"        { return T_TOKEN(T_F32); }
"f64"        { return T_TOKEN(T_F64); }
"string"     { return T_TOKEN(T_STRING); }
"boolean"    { return T_TOKEN(T_BOOLEAN); }
"async"      { return T_TOKEN(T_ASYNC); }
"await"      { return T_TOKEN(T_AWAIT); }

 /* punctuators */
"+"    { return T_TOKEN(T_ADD); }
"-"    { return T_TOKEN(T_SUB); }
"*"    { return T_TOKEN(T_MUL); }
"/"    { return T_TOKEN(T_DIV); }
"%"    { return T_TOKEN(T_MOD); }
"&&"   { return T_TOKEN(T_LOGIC_AND); }
"||"   { return T_TOKEN(T_LOGIC_OR); }
"!"    { return T_TOKEN(T_LOGIC_NOT); }
"&"    { return T_TOKEN(T_BIT_AND); }
"|"    { return T_TOKEN(T_BIT_OR); }
"~"    { return T_TOKEN(T_BIT_NOT); }
"^"    { return T_TOKEN(T_BIT_XOR); }
"<<"   { return T_TOKEN(T_BIT_LSHIFT); }
">>"   { return T_TOKEN(T_BIT_RSHIFT); }
">>>"  { return T_TOKEN(T_BIT_ARSHIFT); }
"="    { return T_TOKEN(T_ASSIGN); }
"+="   { return T_TOKEN(T_ADD_ASSIGN); }
"-="   { return T_TOKEN(T_SUB_ASSIGN); }
"*="   { return T_TOKEN(T_MUL_ASSIGN); }
"/="   { return T_TOKEN(T_DIV_ASSIGN); }
"%="   { return T_TOKEN(T_MOD_ASSIGN); }
"&="   { return T_TOKEN(T_BIT_AND_ASSIGN); }
"|="   { return T_TOKEN(T_BIT_OR_ASSIGN); }
"^="   { return T_TOKEN(T_BIT_XOR_ASSIGN); }
"<<="  { return T_TOKEN(T_BIT_LSHIFT_ASSIGN); }
">>="  { return T_TOKEN(T_BIT_RSHIFT_ASSIGN); }
">>>=" { return T_TOKEN(T_BIT_ARSHIFT_ASSIGN); }
"=="   { return T_TOKEN(T_EQ); }
"!="   { return T_TOKEN(T_NEQ); }
"<"    { return T_TOKEN(T_LT); }
"<="   { return T_TOKEN(T_LE); }
">"    { return T_TOKEN(T_GT); }
">="   { return T_TOKEN(T_GE); }
"("    { return T_TOKEN(T_LPAREN); }
")"    { return T_TOKEN(T_RPAREN); }
"["    { return T_TOKEN(T_LBRACKET); }
"]"    { return T_TOKEN(T_RBRACKET); }
"{"    { return T_TOKEN(T_LBRACE); }
"}"    { return T_TOKEN(T_RBRACE); }
","    { return T_TOKEN(T_COMMA); }
";"    { return T_TOKEN(T_SEMI); }
"?"    { return T_TOKEN(T_QUESTION); }
":"    { return T_TOKEN(T_COLON); }
"."    { return T_TOKEN(T_DOT); }
"=>"   { return T_TOKEN(T_BIG_ARROW); }

 /* integer constants */
 /* decimal integer: 102813 */
{INT}                               { T_SAVE_TOKEN; return T_I64_CONSTANT; }
{INT}[Uu]                           { T_SAVE_TOKEN; return T_U64_CONSTANT; }
 /* heximal integer: 0x03ff */
0[xX]{HEX}+                         { T_SAVE_TOKEN; return T_I64_CONSTANT; }
 /* octal integer: 0o27121 */
0[oO][0-7]+                         { T_SAVE_TOKEN; return T_I64_CONSTANT; }
 /* binary integer: 0b10011 */
0[bB][01]+                          { T_SAVE_TOKEN; return T_I64_CONSTANT; }

 /* floating constants */
 /* floating number: 92.312, 92.312e-10, 102e+91 */
{INT}"."[0-9]+{EXP}?                { T_SAVE_TOKEN; return T_F64_CONSTANT; }
{INT}{EXP}                          { T_SAVE_TOKEN; return T_F64_CONSTANT; }
 /* floating hexcimal number: 0x1fa9p+7, 0x2f.3abfp-3 */
0[xX]{HEX}+{HEXEXP}                 { T_SAVE_TOKEN; return T_F64_CONSTANT; }
0[xX]({HEX}+"."{HEX}+){HEXEXP}?     { T_SAVE_TOKEN; return T_F64_CONSTANT; }

 /* string literal */
\"\"                                    { T_SAVE_NO_TOKEN; return T_STRING_CONSTANT; }
\"([^\"\\]|\\['"?\\abfnrtv]|{UCN})+\"   { T_SAVE_TOKEN_X(1, 1); return T_STRING_CONSTANT; }
\'([^'\\]|\\['"?\\abfnrtv]|{UCN})\'     { T_SAVE_TOKEN_X(1, 1); return T_CHAR_CONSTANT; }

 /* identifier */
[a-zA-Z_][a-zA-Z0-9_]*              { T_SAVE_TOKEN; return T_IDENTIFIER; }

 /* whitespace */
[ \t\n\v\f\r]           { /* skip whitespace */ }

 /* import module */
^[ \t]*import[ \t]*     { BEGIN IMPORT; }
<IMPORT>[^ \t\n\;]+     {
                            {
                                int c;
                                while ((c = yyinput()) && c != ';')
                                    ;
                            }
                            TokenBuffer::pushImport(yytext);
                            BEGIN INITIAL;
                        }
<IMPORT>.|\n            {
                            CASSERT(false, "{}:{} error! bad import:{}, {}!", TokenBuffer::CurrentBuffer, yylineno, yytext, yyleng);
                            BEGIN INITIAL;
                        }
<<EOF>>                 {
                            if (!TokenBuffer::popImport())
                                yyterminate();
                        }

 /* invalid token */
.                       {
                            CASSERT(false, "{}:{} error! invalid token:{}, {}!", TokenBuffer::CurrentBuffer, yylineno, yytext, yyleng);
                        }


%%
