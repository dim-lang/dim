%option noyywrap
%option noinput
%option nounput
%option nodefault
%option nounistd
%option reentrant
%option bison-bridge
%option bison-locations
%option yylineno
%option 8bit

 /* INITIAL enable newline as a token */
 /* NL_IGNORE ignore newline as a whitespace */
%s NL_IGNORE

DIG ([0-9])
OCT ([0-7])
HEX ([0-9a-fA-F])
BIN ([01])
DEC (0|[1-9][0-9]*)
EXP ([eE][-+]?(0|[1-9][0-9]*))
FS  ([dD])
IS  ([uU]|([uU]?[lL]))
NES ([^"\\\r\n])
NL  ("\\\n"|"\\\r\n")
SES (\\[\'\"?abfnrtv\\])
OES (\\[0-7]{1,3})
UCN (\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})
WS_NO_NL    ([ \t\v\f\r])
WS_NL       ([ \n\t\v\f\r])
BC  ("/*"((("*"[^/])?)|[^*])*"*/")
LC  ("//".*\n)

%{
#include <string>
#include <cctype>
#include "Scanner.h"
#include "Strings.h"
#include "parser.tab.hh"
#include "Log.h"

#define T_SCANNER               (static_cast<Scanner *>(yyextra))
#define T_EAT_PARENTHESES(t)    do {    \
    T_SCANNER->eatParentheses(yytokentype::t);       \
    BEGIN(T_SCANNER->newlineEnabled() ? INITIAL : NL_IGNORE);     \
} while (0)

#define MK_LITERAL(t)       yylval->literal = Strings::dup(yytext, yyleng); return yytokentype::t
#define MK_INTEGER(t)       return yylval->token = yytokentype::t

#define YY_USER_ACTION                                                      \
    yylloc->first_line = yylloc->last_line;                                 \
    yylloc->first_column = yylloc->last_column;                             \
    if (yylloc->last_line == yylineno) {                                    \
        yylloc->last_column += yyleng;                                      \
    } else {                                                                \
        yylloc->last_line = yylineno;                                       \
        yylloc->last_column = yytext + yyleng - strrchr(yytext, '\n');      \
    }

%}

%%

 /* multiple line comment */
{BC}        { /* eat everything */ }

 /* one line comment */
{LC}        { /* eat everything until newline */ }

 /* keywords */
true        { MK_INTEGER(T_TRUE); }
false       { MK_INTEGER(T_FALSE); }
try({WS_NL}|{BC}|{LC})*                         { MK_INTEGER(T_TRY); }
({WS_NL}|{BC}|{LC})*catch({WS_NL}|{BC}|{LC})*   { MK_INTEGER(T_CATCH); }
({WS_NL}|{BC}|{LC})*finally({WS_NL}|{BC}|{LC})* { MK_INTEGER(T_FINALLY); }
throw       { MK_INTEGER(T_THROW); }
yield       { MK_INTEGER(T_YIELD); }
var         { MK_INTEGER(T_VAR); }
val         { MK_INTEGER(T_VAL); }
nil         { MK_INTEGER(T_NIL); }
new         { MK_INTEGER(T_NEW); }
delete      { MK_INTEGER(T_DELETE); }
def         { MK_INTEGER(T_DEF); }
if          { MK_INTEGER(T_IF); }
then        { MK_INTEGER(T_THEN); }
({WS_NL}|{BC}|{LC})*else({WS_NL}|{BC}|{LC})*    { MK_INTEGER(T_ELSE); }
match       { MK_INTEGER(T_MATCH); }
enum        { MK_INTEGER(T_ENUM); }
switch      { MK_INTEGER(T_SWITCH); }
case        { MK_INTEGER(T_CASE); }
for         { MK_INTEGER(T_FOR); }
foreach     { MK_INTEGER(T_FOREACH); }
in          { MK_INTEGER(T_IN); }
while       { MK_INTEGER(T_WHILE); }
do({WS_NL}|{BC}|{LC})*                          { MK_INTEGER(T_DO); }
break       { MK_INTEGER(T_BREAK); }
continue    { MK_INTEGER(T_CONTINUE); }
class       { MK_INTEGER(T_CLASS); }
trait       { MK_INTEGER(T_TRAIT); }
type        { MK_INTEGER(T_TYPE); }
this        { MK_INTEGER(T_THIS); }
super       { MK_INTEGER(T_SUPER); }
is          { MK_INTEGER(T_IS); }
isa         { MK_INTEGER(T_ISA); }
isinstanceof    { MK_INTEGER(T_ISINSTANCEOF); }
import      { MK_INTEGER(T_IMPORT); }
as          { MK_INTEGER(T_AS); }
return      { MK_INTEGER(T_RETURN); }
void        { MK_INTEGER(T_VOID); }
 /* any         { MK_INTEGER(T_ANY); } */
nan         { MK_INTEGER(T_NAN); }
inf         { MK_INTEGER(T_INF); }
async       { MK_INTEGER(T_ASYNC); }
await       { MK_INTEGER(T_AWAIT); }
static      { MK_INTEGER(T_STATIC); }
public      { MK_INTEGER(T_PUBLIC); }
protect     { MK_INTEGER(T_PROTECT); }
private     { MK_INTEGER(T_PRIVATE); }
prefix      { MK_INTEGER(T_PREFIX); }
postfix     { MK_INTEGER(T_POSTFIX); }
package     { MK_INTEGER(T_PACKAGE); }

 /* primitive type */
byte        { MK_INTEGER(T_BYTE); }
ubyte       { MK_INTEGER(T_UBYTE); }
short       { MK_INTEGER(T_SHORT); }
ushort      { MK_INTEGER(T_USHORT); }
int         { MK_INTEGER(T_INT); }
uint        { MK_INTEGER(T_UINT); }
long        { MK_INTEGER(T_LONG); }
ulong       { MK_INTEGER(T_ULONG); }
llong       { MK_INTEGER(T_LLONG); }
ullong      { MK_INTEGER(T_ULLONG); }
float       { MK_INTEGER(T_FLOAT); }
double      { MK_INTEGER(T_DOUBLE); }
boolean     { MK_INTEGER(T_BOOLEAN); }
char        { MK_INTEGER(T_CHAR); }

 /* operator */
and         { MK_INTEGER(T_AND); }
or          { MK_INTEGER(T_OR); }
not         { MK_INTEGER(T_NOT); }

"+"         { MK_INTEGER(T_PLUS); }
"++"        { MK_INTEGER(T_PLUS2); }
"-"         { MK_INTEGER(T_MINUS); }
"--"        { MK_INTEGER(T_MINUS2); }
"*"         { MK_INTEGER(T_ASTERISK); }
"/"         { MK_INTEGER(T_SLASH); }
"%"         { MK_INTEGER(T_PERCENT); }

"&"         { MK_INTEGER(T_AMPERSAND); }
"&&"        { MK_INTEGER(T_AMPERSAND2); }
"|"         { MK_INTEGER(T_BAR); }
"||"        { MK_INTEGER(T_BAR2); }
"~"         { MK_INTEGER(T_TILDE); }
"!"         { MK_INTEGER(T_EXCLAM); }
"^"         { MK_INTEGER(T_CARET); }

"<<"        { MK_INTEGER(T_LSHIFT); }
">>"        { MK_INTEGER(T_RSHIFT); }
">>>"       { MK_INTEGER(T_ARSHIFT); }

 /* equal operator */
"="         { MK_INTEGER(T_EQUAL); }
"+="        { MK_INTEGER(T_PLUS_EQUAL); }
"-="        { MK_INTEGER(T_MINUS_EQUAL); }
"*="        { MK_INTEGER(T_ASTERISK_EQUAL); }
"/="        { MK_INTEGER(T_SLASH_EQUAL); }
"%="        { MK_INTEGER(T_PERCENT_EQUAL); }
"&="        { MK_INTEGER(T_AMPERSAND_EQUAL); }
"|="        { MK_INTEGER(T_BAR_EQUAL); }
"^="        { MK_INTEGER(T_CARET_EQUAL); }
"<<="       { MK_INTEGER(T_LSHIFT_EQUAL); }
">>="       { MK_INTEGER(T_RSHIFT_EQUAL); }
">>>="      { MK_INTEGER(T_ARSHIFT_EQUAL); }

 /* compare operator */
"=="        { MK_INTEGER(T_EQ); }
"!="        { MK_INTEGER(T_NEQ); }
"<"         { MK_INTEGER(T_LT); }
"<="        { MK_INTEGER(T_LE); }
">"         { MK_INTEGER(T_GT); }
">="        { MK_INTEGER(T_GE); }

 /* parentheses */
"("         { T_EAT_PARENTHESES(T_LPAREN); MK_INTEGER(T_LPAREN); }
")"         { T_EAT_PARENTHESES(T_RPAREN); MK_INTEGER(T_RPAREN); }
"["         { T_EAT_PARENTHESES(T_LBRACKET); MK_INTEGER(T_LBRACKET); }
"]"         { T_EAT_PARENTHESES(T_RBRACKET); MK_INTEGER(T_RBRACKET); }
"{"         { T_EAT_PARENTHESES(T_LBRACE); MK_INTEGER(T_LBRACE); }
"}"         { T_EAT_PARENTHESES(T_RBRACE); MK_INTEGER(T_RBRACE); }

 /* other punctuation */
"_"         { MK_INTEGER(T_UNDERSCORE); }
","         { MK_INTEGER(T_COMMA); }
";"         { MK_INTEGER(T_SEMI); }
"?"         { MK_INTEGER(T_QUESTION); }
":"         { MK_INTEGER(T_COLON); }
"::"        { MK_INTEGER(T_COLON2); }
"."         { MK_INTEGER(T_DOT); }
".."        { MK_INTEGER(T_DOT2); }
"<-"        { MK_INTEGER(T_LARROW); }
"->"        { MK_INTEGER(T_RARROW); }
"=>"        { MK_INTEGER(T_DOUBLE_RARROW); }
":>"        { MK_INTEGER(T_COLON_LARROW);  }
"<:"        { MK_INTEGER(T_COLON_RARROW); }

 /* whitespace and newline */
<INITIAL>{WS_NO_NL}+                        { /* eat whitespace */ }
<INITIAL>[\n]+                              { MK_INTEGER(T_NEWLINE); }
<NL_IGNORE>{WS_NL}+                         { /* eat whitespace */ }

 /* decimal integer literal */
{DEC}{IS}?                                  { MK_LITERAL(T_INTEGER_LITERAL); }
 /* heximal integer literal */
0[xX]{HEX}+{IS}?                            { MK_LITERAL(T_INTEGER_LITERAL); }
 /* octal integer literal */
0[oO]{OCT}+{IS}?                            { MK_LITERAL(T_INTEGER_LITERAL); }
 /* binary integer literal */
0[bB]{BIN}+{IS}?                            { MK_LITERAL(T_INTEGER_LITERAL); }

 /* float literal : 92.312, 92.312e-10, 10.2e+91, 1.0e+7 */
{DEC}"."{DIG}+{EXP}?{FS}?                   { MK_LITERAL(T_FLOAT_LITERAL); }
{DEC}{EXP}{FS}?                             { MK_LITERAL(T_FLOAT_LITERAL); }

 /* multiple line string literal */
["]{3}(["]{0,2}([^\\"]|\\(.|\n)))*["]{3}    { MK_LITERAL(T_STRING_LITERAL); }

 /* one line string */
\"({NES}|{NL}|{SES}|{OES}|{UCN})*\"         { MK_LITERAL(T_STRING_LITERAL); }

 /* char literal */
\'({NES}|{NL}|{SES}|{OES}|{UCN})\'          { MK_LITERAL(T_CHARACTER_LITERAL); }

 /* var id */
([a-zA-Z][a-zA-Z0-9_]*)|("_"[a-zA-Z0-9_]+)  { MK_LITERAL(T_VAR_ID); }

<<EOF>>                                     {
                                                LOG_ASSERT(T_SCANNER->parenthesesEmpty(), "parentheses stack must be empty:{}", T_SCANNER->parenthesesSize());
                                                yyterminate();
                                            }

 /* invalid token */
.                                           {
                                                LOG_ASSERT(false, "{}:{} tokenize error! invalid:{}:{}!", T_SCANNER->fileName(), yylineno, yytext, yyleng);
                                            }

%%
