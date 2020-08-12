%option noyywrap nounput noinput nodefault yylineno 8bit
%option reentrant bison-locations bison-bridge
%option never-interactive
%option extra-type="Scanner *"

%x COMMENT
%x IMPORT

UCN (\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})
DEC (0|[1-9][0-9]*)
HEX ([0-9a-fA-F])
OCT ([0-7])
BIN ([01])
EXP ([eE][-+]?[0-9]+)
FS  ([fF])
IS  ([uU]|([uU]?[lL]))

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
 // #define T_LITERAL_TOKEN_X(p, q) (Y_LVAL->literal = Strings::dup(Y_TEXT+(p), Y_LENG-(p)-(q)))
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
"finally"    { return T_INTEGER_TOKEN(T_FINALLY); }
"throw"      { return T_INTEGER_TOKEN(T_THROW); }
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
"trait"      { return T_INTEGER_TOKEN(T_TRAIT); }
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
"prefix"     { return T_INTEGER_TOKEN(T_PREFIX); }
"postfix"    { return T_INTEGER_TOKEN(T_POSTFIX); }
"package"    { return T_INTEGER_TOKEN(T_PACKAGE); }

 /***
   * primitive integer type
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
   */
"float"      { return T_INTEGER_TOKEN(T_FLOAT); }
"double"     { return T_INTEGER_TOKEN(T_DOUBLE); }

 /***
   * primitive boolean type
   */
"boolean"    { return T_INTEGER_TOKEN(T_BOOLEAN); }

 /* and or not operator */
"and"   { return T_INTEGER_TOKEN(T_AND); }
"or"    { return T_INTEGER_TOKEN(T_OR); }
"not"   { return T_INTEGER_TOKEN(T_NOT); }

 /* operator */
"+"     { return T_INTEGER_TOKEN(T_PLUS); }
"++"    { return T_INTEGER_TOKEN(T_PLUS2); }
"-"     { return T_INTEGER_TOKEN(T_MINUS); }
"--"    { return T_INTEGER_TOKEN(T_MINUS2); }
"*"     { return T_INTEGER_TOKEN(T_STAR); }
"**"    { return T_INTEGER_TOKEN(T_STAR2); }
"/"     { return T_INTEGER_TOKEN(T_SLASH); }
"//"    { return T_INTEGER_TOKEN(T_SLASH2); }
"%"     { return T_INTEGER_TOKEN(T_PERCENT); }
"%%"    { return T_INTEGER_TOKEN(T_PERCENT2); }

"&"     { return T_INTEGER_TOKEN(T_AMPERSAND); }
"&&"    { return T_INTEGER_TOKEN(T_AMPERSAND2); }
"|"     { return T_INTEGER_TOKEN(T_BAR); }
"||"    { return T_INTEGER_TOKEN(T_BAR2); }
"~"     { return T_INTEGER_TOKEN(T_TILDE); }
"!"     { return T_INTEGER_TOKEN(T_EXCLAM); }
"^"     { return T_INTEGER_TOKEN(T_CARET); }

"<<"    { return T_INTEGER_TOKEN(T_LSHIFT); }
">>"    { return T_INTEGER_TOKEN(T_RSHIFT); }
">>>"   { return T_INTEGER_TOKEN(T_ARSHIFT); }

 /* equal operator */
"="     { return T_INTEGER_TOKEN(T_EQUAL); }
"+="    { return T_INTEGER_TOKEN(T_PLUS_EQUAL); }
"-="    { return T_INTEGER_TOKEN(T_MINUS_EQUAL); }
"*="    { return T_INTEGER_TOKEN(T_STAR_EQUAL); }
"/="    { return T_INTEGER_TOKEN(T_SLASH_EQUAL); }
"%="    { return T_INTEGER_TOKEN(T_PERCENT_EQUAL); }
"&="    { return T_INTEGER_TOKEN(T_AMPERSAND_EQUAL); }
"|="    { return T_INTEGER_TOKEN(T_BAR_EQUAL); }
"^="    { return T_INTEGER_TOKEN(T_CARET_EQUAL); }
"<<="   { return T_INTEGER_TOKEN(T_LSHIFT_EQUAL); }
">>="   { return T_INTEGER_TOKEN(T_RSHIFT_EQUAL); }
">>>="  { return T_INTEGER_TOKEN(T_ARSHIFT_EQUAL); }

 /* compare operator */
"=="    { return T_INTEGER_TOKEN(T_EQ); }
"!="    { return T_INTEGER_TOKEN(T_NEQ); }
"<"     { return T_INTEGER_TOKEN(T_LT); }
"<="    { return T_INTEGER_TOKEN(T_LE); }
">"     { return T_INTEGER_TOKEN(T_GT); }
">="    { return T_INTEGER_TOKEN(T_GE); }

 /* parentheses */
"("     { return T_INTEGER_TOKEN(T_LPAREN); }
")"     { return T_INTEGER_TOKEN(T_RPAREN); }
"["     { return T_INTEGER_TOKEN(T_LBRACKET); }
"]"     { return T_INTEGER_TOKEN(T_RBRACKET); }
"{"     { return T_INTEGER_TOKEN(T_LBRACE); }
"}"     { return T_INTEGER_TOKEN(T_RBRACE); }

 /* other punctuation */
"_"     { return T_INTEGER_TOKEN(T_UNDERSCORE); }
","     { return T_INTEGER_TOKEN(T_COMMA); }
";"     { return T_INTEGER_TOKEN(T_SEMI); }
 /* [\n]+   { return T_INTEGER_TOKEN(T_NEWLINE); } */
"?"     { return T_INTEGER_TOKEN(T_QUESTION); }
":"     { return T_INTEGER_TOKEN(T_COLON); }
"::"    { return T_INTEGER_TOKEN(T_COLON2); }
"."     { return T_INTEGER_TOKEN(T_DOT); }
".."    { return T_INTEGER_TOKEN(T_DOT2); }
"<-"    { return T_INTEGER_TOKEN(T_THIN_LARROW); }
"->"    { return T_INTEGER_TOKEN(T_THIN_RARROW); }
"=>"    { return T_INTEGER_TOKEN(T_FAT_RARROW); }
":>"    { return T_INTEGER_TOKEN(T_COLON_LARROW);  }
"<:"    { return T_INTEGER_TOKEN(T_COLON_RARROW); }

 /* decimal integer literal */
{DEC}{IS}?                                  { T_LITERAL_TOKEN; return T_INTEGER_LITERAL; }
 /* heximal integer literal */
0[xX]{HEX}+{IS}?                            { T_LITERAL_TOKEN; return T_INTEGER_LITERAL; }
 /* octal integer literal */
0[oO]{OCT}+{IS}?                            { T_LITERAL_TOKEN; return T_INTEGER_LITERAL; }
 /* binary integer literal */
0[bB]{BIN}+{IS}?                            { T_LITERAL_TOKEN; return T_INTEGER_LITERAL; }

 /* float literal : 92.312(f|F), 92.312e-10(f|F), 10.2e+91(f|F), 1.0e+7(f|F) */
{DEC}"."[0-9]+{EXP}?{FS}?                   { T_LITERAL_TOKEN; return T_FLOATING_POINT_LITERAL; }
{DEC}{EXP}{FS}?                             { T_LITERAL_TOKEN; return T_FLOATING_POINT_LITERAL; }

 /* multiple line string literal */
["]{3}(["]{0,2}([^\\"]|\\(.|\n)))*["]{3}    { T_LITERAL_TOKEN; return T_STRING_LITERAL; }

 /* empty string */
\"\"                                        { T_LITERAL_TOKEN; return T_STRING_LITERAL; }
 /* one line string literal */
\"([^\"\\]|\\['"?\\abfnrtv]|{UCN})+\"       { T_LITERAL_TOKEN; return T_STRING_LITERAL; }

 /* char literal */
\'([^'\\]|\\['"?\\abfnrtv]|{UCN})\'         { T_LITERAL_TOKEN; return T_CHARACTER_LITERAL; }

 /* var id */
[a-zA-Z_][a-zA-Z0-9_]*                      { T_LITERAL_TOKEN; return T_VAR_ID; }

 /* whitespace except newline */
[ \n\t\v\f\r]                                 { /* skip whitespace except newline */ }

 /* import module */
^[ \n\t\v\f\r]*import[ \n\t\v\f\r]*             { BEGIN IMPORT; }
<IMPORT>[^ \t\v\f\r\;]+                     {
                                                yyextra->pushBuffer(Y_TEXT);
                                                BEGIN INITIAL;
                                            }
<IMPORT>.|\n                                {
                                                EX_ASSERT(false, "{}:{} error! bad import:{}, {}!", yyextra->currentBuffer(), Y_LINENO, Y_TEXT, Y_LENG);
                                                BEGIN INITIAL;
                                            }
<<EOF>>                                     {
                                                if (!yyextra->popBuffer())
                                                    yyterminate();
                                            }

 /* invalid token */
.                                           {
                                                EX_ASSERT(false, "{}:{} error! invalid token:{}, {}!", yyextra->currentBuffer(), Y_LINENO, Y_TEXT, Y_LENG);
                                            }

%%
