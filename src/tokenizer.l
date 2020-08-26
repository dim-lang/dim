%option noyywrap
%option nounput
%option noinput
%option nodefault
%option nounistd
%option reentrant
%option yylineno
%option 8bit
%option warn

DIG ([0-9])
OCT ([0-7])
HEX ([0-9a-fA-F])
DEC (0|[1-9][0-9]*)
BIN ([01])
EXP ([eE][-+]?[0-9]+)
FS  ([dD])
IS  ([uU]|([uU]?[lL]))
NES ([^"\\\r\n])
NL  ("\\\n"|"\\\r\n")
SES (\\[\'\"?abfnrtv\\])
OES (\\[0-7]{1,3})
UCN (\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})
WS  ([ \t\v\f\r])
BC  ("/*"((("*"[^/])?)|[^*])*"*/")
LC  ("//".*\n)

%{
#include <string>
#include "boost/preprocessor/cat.hpp"
#include "parser.tab.hh"
#include "location.hh"
#include "Scanner.h"
#include "Exception.h"
#include "Log.h"

#if 0
#define Y_LENG    yyget_leng(yyextra->yy_scanner)
#define Y_TEXT    yyget_text(yyextra->yy_scanner)
#define Y_LVAL    yyget_lval(yyextra->yy_scanner)
#define Y_LLOC    yyget_lloc(yyextra->yy_scanner)
#define Y_LINENO  yyget_lineno(yyextra->yy_scanner)

#define MK_LITERAL     (yylval->str = Strings::duplicate(yytext, yyleng))
#define MK_INTEGER(t)  (yylval->tok = (t))

#define YY_USER_ACTION                                                      \
    yylloc->first_line = yylloc->last_line;                                 \
    yylloc->first_column = yylloc->last_column;                             \
    if (yylloc->last_line == yylineno) {                                    \
        yylloc->last_column += yyleng;                                      \
    } else {                                                                \
        yylloc->last_line = yylineno;                                       \
        yylloc->last_column = yytext + yyleng - strrchr(yytext, '\n');      \
    }
#endif


#define T_SCANNER           (static_cast<Scanner *>(yyextra))
#define T_LOCATION          (T_SCANNER->bufferStack.top()->location)
#define T_FILE_NAME         (T_SCANNER->bufferStack.top()->fileName)
#define MK_LITERAL(t)       (BOOST_PP_CAT(yy::parser::make_, t)(std::string(yytext), T_LOCATION))
#define MK_INTEGER(t)       (BOOST_PP_CAT(yy::parser::make_, t)(BOOST_PP_CAT(yy::parser::token::, t), T_LOCATION))

#define YY_USER_ACTION                         \
    T_LOCATION.step();                         \
    if (T_LOCATION.end.line == yylineno) {     \
        T_LOCATION.end.columns(yyleng);        \
    } else {                                   \
        T_LOCATION.end.line = yylineno;        \
        T_LOCATION.end.column = yytext + yyleng - strrchr(yytext, '\n');   \
    }

%}

%%

 /* multiple line comment */
{BC}        { /* eat everything */ }

 /* one line comment */
{LC}        { /* eat everything until newline */ }

 /* keywords */
true        { return MK_INTEGER(T_TRUE); }
false       { return MK_INTEGER(T_FALSE); }
try         { return MK_INTEGER(T_TRY); }
catch       { return MK_INTEGER(T_CATCH); }
finally     { return MK_INTEGER(T_FINALLY); }
throw       { return MK_INTEGER(T_THROW); }
yield       { return MK_INTEGER(T_YIELD); }
var         { return MK_INTEGER(T_VAR); }
val         { return MK_INTEGER(T_VAL); }
nil         { return MK_INTEGER(T_NIL); }
new         { return MK_INTEGER(T_NEW); }
delete      { return MK_INTEGER(T_DELETE); }
def         { return MK_INTEGER(T_DEF); }
if          { return MK_INTEGER(T_IF); }
then        { return MK_INTEGER(T_THEN); }
else        { return MK_INTEGER(T_ELSE); }
match       { return MK_INTEGER(T_MATCH); }
enum        { return MK_INTEGER(T_ENUM); }
switch      { return MK_INTEGER(T_SWITCH); }
case        { return MK_INTEGER(T_CASE); }
for         { return MK_INTEGER(T_FOR); }
foreach     { return MK_INTEGER(T_FOREACH); }
in          { return MK_INTEGER(T_IN); }
while       { return MK_INTEGER(T_WHILE); }
do          { return MK_INTEGER(T_DO); }
break       { return MK_INTEGER(T_BREAK); }
continue    { return MK_INTEGER(T_CONTINUE); }
class       { return MK_INTEGER(T_CLASS); }
trait       { return MK_INTEGER(T_TRAIT); }
type        { return MK_INTEGER(T_TYPE); }
this        { return MK_INTEGER(T_THIS); }
super       { return MK_INTEGER(T_SUPER); }
is          { return MK_INTEGER(T_IS); }
isa         { return MK_INTEGER(T_ISA); }
isinstanceof    { return MK_INTEGER(T_ISINSTANCEOF); }
import      { return MK_INTEGER(T_IMPORT); }
as          { return MK_INTEGER(T_AS); }
return      { return MK_INTEGER(T_RETURN); }
void        { return MK_INTEGER(T_VOID); }
any         { return MK_INTEGER(T_ANY); }
nan         { return MK_INTEGER(T_NAN); }
inf         { return MK_INTEGER(T_INF); }
async       { return MK_INTEGER(T_ASYNC); }
await       { return MK_INTEGER(T_AWAIT); }
static      { return MK_INTEGER(T_STATIC); }
public      { return MK_INTEGER(T_PUBLIC); }
protect     { return MK_INTEGER(T_PROTECT); }
private     { return MK_INTEGER(T_PRIVATE); }
prefix      { return MK_INTEGER(T_PREFIX); }
postfix     { return MK_INTEGER(T_POSTFIX); }
package     { return MK_INTEGER(T_PACKAGE); }

 /* primitive type */
byte        { return MK_INTEGER(T_BYTE); } */
ubyte       { return MK_INTEGER(T_UBYTE); }
short       { return MK_INTEGER(T_SHORT); }
ushort      { return MK_INTEGER(T_USHORT); }
int         { return MK_INTEGER(T_INT); }
uint        { return MK_INTEGER(T_UINT); }
long        { return MK_INTEGER(T_LONG); }
ulong       { return MK_INTEGER(T_ULONG); }
llong       { return MK_INTEGER(T_LLONG); }
ullong      { return MK_INTEGER(T_ULLONG); }
float       { return MK_INTEGER(T_FLOAT); }
double      { return MK_INTEGER(T_DOUBLE); }
boolean     { return MK_INTEGER(T_BOOLEAN); }
char        { return MK_INTEGER(T_CHAR); }

 /* operator */
and         { return MK_INTEGER(T_AND); }
or          { return MK_INTEGER(T_OR); }
not         { return MK_INTEGER(T_NOT); }

 /* optional newline or semicolon */
[\n]+({BC}|{LC}|[^ \t\v\f\r])*"else"    { return MK_INTEGER(T_SEMI_ELSE); }
";"({BC}|{LC}|[^ \t\v\f\r])*"else"      { return MK_INTEGER(T_SEMI_ELSE); }
[\n]+({BC}|{LC}|[^ \t\v\f\r])*"while"   { return MK_INTEGER(T_SEMI_WHILE); }
";"({BC}|{LC}|[^ \t\v\f\r])*"while"     { return MK_INTEGER(T_SEMI_WHILE); }

"+"         { return MK_INTEGER(T_PLUS); }
"++"        { return MK_INTEGER(T_PLUS2); }
"-"         { return MK_INTEGER(T_MINUS); }
"--"        { return MK_INTEGER(T_MINUS2); }
"*"         { return MK_INTEGER(T_ASTERISK); }
"**"        { return MK_INTEGER(T_ASTERISK2); }
"/"         { return MK_INTEGER(T_SLASH); }
"//"        { return MK_INTEGER(T_SLASH2); }
"%"         { return MK_INTEGER(T_PERCENT); }
"%%"        { return MK_INTEGER(T_PERCENT2); }

"&"         { return MK_INTEGER(T_AMPERSAND); }
"&&"        { return MK_INTEGER(T_AMPERSAND2); }
"|"         { return MK_INTEGER(T_BAR); }
"||"        { return MK_INTEGER(T_BAR2); }
"~"         { return MK_INTEGER(T_TILDE); }
"!"         { return MK_INTEGER(T_EXCLAM); }
"^"         { return MK_INTEGER(T_CARET); }

"<<"        { return MK_INTEGER(T_LSHIFT); }
">>"        { return MK_INTEGER(T_RSHIFT); }
">>>"       { return MK_INTEGER(T_ARSHIFT); }

 /* equal operator */
"="         { return MK_INTEGER(T_EQUAL); }
"+="        { return MK_INTEGER(T_PLUS_EQUAL); }
"-="        { return MK_INTEGER(T_MINUS_EQUAL); }
"*="        { return MK_INTEGER(T_ASTERISK_EQUAL); }
"/="        { return MK_INTEGER(T_SLASH_EQUAL); }
"%="        { return MK_INTEGER(T_PERCENT_EQUAL); }
"&="        { return MK_INTEGER(T_AMPERSAND_EQUAL); }
"|="        { return MK_INTEGER(T_BAR_EQUAL); }
"^="        { return MK_INTEGER(T_CARET_EQUAL); }
"<<="       { return MK_INTEGER(T_LSHIFT_EQUAL); }
">>="       { return MK_INTEGER(T_RSHIFT_EQUAL); }
">>>="      { return MK_INTEGER(T_ARSHIFT_EQUAL); }

 /* compare operator */
"=="        { return MK_INTEGER(T_EQ); }
"!="        { return MK_INTEGER(T_NEQ); }
"<"         { return MK_INTEGER(T_LT); }
"<="        { return MK_INTEGER(T_LE); }
">"         { return MK_INTEGER(T_GT); }
">="        { return MK_INTEGER(T_GE); }

 /* parentheses */
"("         { return MK_INTEGER(T_LPAREN); }
")"         { return MK_INTEGER(T_RPAREN); }
"["         { return MK_INTEGER(T_LBRACKET); }
"]"         { return MK_INTEGER(T_RBRACKET); }
"{"         { return MK_INTEGER(T_LBRACE); }
"}"         { return MK_INTEGER(T_RBRACE); }

 /* other punctuation */
"_"         { return MK_INTEGER(T_UNDERSCORE); }
","         { return MK_INTEGER(T_COMMA); }
";"         { return MK_INTEGER(T_SEMI); }
"?"         { return MK_INTEGER(T_QUESTION); }
":"         { return MK_INTEGER(T_COLON); }
"::"        { return MK_INTEGER(T_COLON2); }
"."         { return MK_INTEGER(T_DOT); }
".."        { return MK_INTEGER(T_DOT2); }
"<-"        { return MK_INTEGER(T_LARROW); }
"->"        { return MK_INTEGER(T_RARROW); }
"=>"        { return MK_INTEGER(T_DOUBLE_RARROW); }
":>"        { return MK_INTEGER(T_COLON_LARROW);  }
"<:"        { return MK_INTEGER(T_COLON_RARROW); }
[\n]+       { return MK_INTEGER(T_NEWLINE); }

 /* decimal integer literal */
{DEC}{IS}?                                  { return MK_LITERAL(T_INTEGER_LITERAL); }
 /* heximal integer literal */
0[xX]{HEX}+{IS}?                            { return MK_LITERAL(T_INTEGER_LITERAL); }
 /* octal integer literal */
0[oO]{OCT}+{IS}?                            { return MK_LITERAL(T_INTEGER_LITERAL); }
 /* binary integer literal */
0[bB]{BIN}+{IS}?                            { return MK_LITERAL(T_INTEGER_LITERAL); }

 /* float literal : 92.312, 92.312e-10, 10.2e+91, 1.0e+7 */
{DEC}"."{DIG}+{EXP}?{FS}?                   { return MK_LITERAL(T_FLOAT_LITERAL); }
{DEC}{EXP}{FS}?                             { return MK_LITERAL(T_FLOAT_LITERAL); }

 /* multiple line string literal */
["]{3}(["]{0,2}([^\\"]|\\(.|\n)))*["]{3}    { return MK_LITERAL(T_STRING_LITERAL); }

 /* one line string */
\"({NES}|{NL}|{SES}|{OES}|{UCN})*\"         { return MK_LITERAL(T_STRING_LITERAL); }

 /* char literal */
\'({NES}|{NL}|{SES}|{OES}|{UCN})\'          { return MK_LITERAL(T_CHARACTER_LITERAL); }

 /* var id */
[a-zA-Z_][a-zA-Z0-9_]*                      { return MK_LITERAL(T_VAR_ID); }

 /* whitespace except newline */
{WS}+                                       { /* skip whitespace except newline */ }

<<EOF>>                                     {
                                                if (!yyextra->bufferStack.pop()) {
                                                    LOG_INFO("{}:{} pop buffer empty at {}:{}!", T_FILE_NAME, yylineno, yytext, yyleng);
                                                    yyterminate();
                                                }
                                                return yy::parser::make_YYEOF(loc);
                                            }

 /* invalid token */
.                                           {
                                                LOG_ERROR("{}:{} error! invalid token:{}, {}!", T_FILE_NAME, yylineno, yytext, yyleng);
                                                EX_ASSERT(false, "{}:{} error! invalid token:{}, {}!", T_FILE_NAME, yylineno, yytext, yyleng);
                                            }

%%
