%option noyywrap
%option nounput
%option noinput
%option nodefault
%option nounistd
%option reentrant
%option yylineno
%option 8bit

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
#include "Scanner.h"
#include "Buffer.h"
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
#define MK_LITERAL(t)       {yy::parser::token::token_kind_type tk = yy::parser::token:: ## t; return yy::parser::symbol_type(tk, yytext, T_LOCATION);}
#define MK_INTEGER(t)       {yy::parser::token::token_kind_type tk = yy::parser::token:: ## t; return yy::parser::symbol_type(tk, tk, T_LOCATION);}

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
true        { return yy::parser::make_T_TRUE(yy::parser::token::T_TRUE, T_LOCATION); }
false       { return yy::parser::make_T_FALSE(yy::parser::token::T_FALSE, T_LOCATION); }
try         { return yy::parser::make_T_TRY(yy::parser::token::T_TRY, T_LOCATION); }
catch       { return yy::parser::make_T_CATCH(yy::parser::token::T_CATCH, T_LOCATION); }
finally     { return yy::parser::make_T_FINALLY(yy::parser::token::T_FINALLY, T_LOCATION); }
throw       { return yy::parser::make_T_THROW(yy::parser::token::T_THROW, T_LOCATION); }
yield       { return yy::parser::make_T_YIELD(yy::parser::token::T_YIELD, T_LOCATION); }
var         { return yy::parser::make_T_VAR(yy::parser::token::T_VAR, T_LOCATION); }
val         { return yy::parser::make_T_VAL(yy::parser::token::T_VAL, T_LOCATION); }
nil         { return yy::parser::make_T_NIL(yy::parser::token::T_NIL, T_LOCATION); }
new         { return yy::parser::make_T_NEW(yy::parser::token::T_NEW, T_LOCATION); }
delete      { return yy::parser::make_T_DELETE(yy::parser::token::T_DELETE, T_LOCATION); }
def         { return yy::parser::make_T_DEF(yy::parser::token::T_DEF, T_LOCATION); }
if          { return yy::parser::make_T_IF(yy::parser::token::T_IF, T_LOCATION); }
then        { return yy::parser::make_T_THEN(yy::parser::token::T_THEN, T_LOCATION); }
else        { return yy::parser::make_T_ELSE(yy::parser::token::T_ELSE, T_LOCATION); }
match       { return yy::parser::make_T_MATCH(yy::parser::token::T_MATCH, T_LOCATION); }
enum        { return yy::parser::make_T_ENUM(yy::parser::token::T_ENUM, T_LOCATION); }
switch      { return yy::parser::make_T_SWITCH(yy::parser::token::T_SWITCH, T_LOCATION); }
case        { return yy::parser::make_T_CASE(yy::parser::token::T_CASE, T_LOCATION); }
for         { return yy::parser::make_T_FOR(yy::parser::token::T_FOR, T_LOCATION); }
foreach     { return yy::parser::make_T_FOREACH(yy::parser::token::T_FOREACH, T_LOCATION); }
in          { return yy::parser::make_T_IN(yy::parser::token::T_IN, T_LOCATION); }
while       { return yy::parser::make_T_WHILE(yy::parser::token::T_WHILE, T_LOCATION); }
do          { return yy::parser::make_T_DO(yy::parser::token::T_DO, T_LOCATION); }
break       { return yy::parser::make_T_BREAK(yy::parser::token::T_BREAK, T_LOCATION); }
continue    { return yy::parser::make_T_CONTINUE(yy::parser::token::T_CONTINUE, T_LOCATION); }
class       { return yy::parser::make_T_CLASS(yy::parser::token::T_CLASS, T_LOCATION); }
trait       { return yy::parser::make_T_TRAIT(yy::parser::token::T_TRAIT, T_LOCATION); }
type        { return yy::parser::make_T_TYPE(yy::parser::token::T_TYPE, T_LOCATION); }
this        { return yy::parser::make_T_THIS(yy::parser::token::T_THIS, T_LOCATION); }
super       { return yy::parser::make_T_SUPER(yy::parser::token::T_SUPER, T_LOCATION); }
is          { return yy::parser::make_T_IS(yy::parser::token::T_IS, T_LOCATION); }
isa         { return yy::parser::make_T_ISA(yy::parser::token::T_ISA, T_LOCATION); }
isinstanceof    { return yy::parser::make_T_ISINSTANCEOF(yy::parser::token::T_ISINSTANCEOF, T_LOCATION); }
import      { return yy::parser::make_T_IMPORT(yy::parser::token::T_IMPORT, T_LOCATION); }
as          { return yy::parser::make_T_AS(yy::parser::token::T_AS, T_LOCATION); }
return      { return yy::parser::make_T_RETURN(yy::parser::token::T_RETURN, T_LOCATION); }
void        { return yy::parser::make_T_VOID(yy::parser::token::T_VOID, T_LOCATION); }
any         { return yy::parser::make_T_ANY(yy::parser::token::T_ANY, T_LOCATION); }
nan         { return yy::parser::make_T_NAN(yy::parser::token::T_NAN, T_LOCATION); }
inf         { return yy::parser::make_T_INF(yy::parser::token::T_INF, T_LOCATION); }
async       { return yy::parser::make_T_ASYNC(yy::parser::token::T_ASYNC, T_LOCATION); }
await       { return yy::parser::make_T_AWAIT(yy::parser::token::T_AWAIT, T_LOCATION); }
static      { return yy::parser::make_T_STATIC(yy::parser::token::T_STATIC, T_LOCATION); }
public      { return yy::parser::make_T_PUBLIC(yy::parser::token::T_PUBLIC, T_LOCATION); }
protect     { return yy::parser::make_T_PROTECT(yy::parser::token::T_PROTECT, T_LOCATION); }
private     { return yy::parser::make_T_PRIVATE(yy::parser::token::T_PRIVATE, T_LOCATION); }
prefix      { return yy::parser::make_T_PREFIX(yy::parser::token::T_PREFIX, T_LOCATION); }
postfix     { return yy::parser::make_T_POSTFIX(yy::parser::token::T_POSTFIX, T_LOCATION); }
package     { return yy::parser::make_T_PACKAGE(yy::parser::token::T_PACKAGE, T_LOCATION); }

 /* primitive type */
byte        { MK_INTEGER(T_BYTE); } */
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

 /* optional newline or semicolon */
[\n]+({BC}|{LC}|[^ \t\v\f\r])*"else"    { MK_INTEGER(T_SEMI_ELSE); }
";"({BC}|{LC}|[^ \t\v\f\r])*"else"      { MK_INTEGER(T_SEMI_ELSE); }
[\n]+({BC}|{LC}|[^ \t\v\f\r])*"while"   { MK_INTEGER(T_SEMI_WHILE); }
";"({BC}|{LC}|[^ \t\v\f\r])*"while"     { MK_INTEGER(T_SEMI_WHILE); }

"+"         { MK_INTEGER(T_PLUS); }
"++"        { MK_INTEGER(T_PLUS2); }
"-"         { MK_INTEGER(T_MINUS); }
"--"        { MK_INTEGER(T_MINUS2); }
"*"         { MK_INTEGER(T_ASTERISK); }
"**"        { MK_INTEGER(T_ASTERISK2); }
"/"         { MK_INTEGER(T_SLASH); }
"//"        { MK_INTEGER(T_SLASH2); }
"%"         { MK_INTEGER(T_PERCENT); }
"%%"        { MK_INTEGER(T_PERCENT2); }

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
"("         { MK_INTEGER(T_LPAREN); }
")"         { MK_INTEGER(T_RPAREN); }
"["         { MK_INTEGER(T_LBRACKET); }
"]"         { MK_INTEGER(T_RBRACKET); }
"{"         { MK_INTEGER(T_LBRACE); }
"}"         { MK_INTEGER(T_RBRACE); }

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
[\n]+       { MK_INTEGER(T_NEWLINE); }

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
[a-zA-Z_][a-zA-Z0-9_]*                      { MK_LITERAL(T_VAR_ID); }

 /* whitespace except newline */
{WS}+                                       { /* skip whitespace except newline */ }

<<EOF>>                                     {
                                                if (!T_SCANNER->bufferStack.pop()) {
                                                    LOG_INFO("{}:{} pop buffer empty at {}:{}!", T_FILE_NAME, yylineno, yytext, yyleng);
                                                    yyterminate();
                                                }
                                                MK_INTEGER(YYEOF);
                                            }

 /* invalid token */
.                                           {
                                                LOG_ASSERT(false, "{}:{} error! invalid token:{}, {}!", T_FILE_NAME, yylineno, yytext, yyleng);
                                            }

%%
