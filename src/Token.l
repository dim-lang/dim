%{
#include <memory>
#include <string>
#include "Ast.h"
#include "Parser.hpp"
#define FSAVE_TOKEN yylval.string = std::shared_ptr<std::string>(new std::string(yytext, yyleng))
#define FTOKEN(t) (yylval.token = t)
extern "C" int yywarp() {}
%}

%%

"/*"                    { fastypeLexerComment(0); }
"//"                    { fastypeLexerComment(1); }
True                    return FTOKEN(FTRUE);
False                   return FTOKEN(FFALSE);
Nil                     return FTOKEN(FNIL);
let                     return FTOKEN(FLET);
if                      return FTOKEN(FIF);
else                    return FTOKEN(FELSE);
for                     return FTOKEN(FFOR);
while                   return FTOKEN(FWHILE);
break                   return FTOKEN(FBREAK);
continue                return FTOKEN(FCONTINUE);
void                    return FTOKEN(FVOID);
return                  return FTOKEN(FRETURN);
func                    return FTOKEN(FFUNC);
struct                  return FTOKEN(FSTRUCT);
class                   return FTOKEN(FCLASS);
import                  return FTOKEN(FIMPORT);
enum                    return FTOKEN(FENUM);
and                     return FTOKEN(FLOGICAND);
or                      return FTOKEN(FLOGICOR);
[ \r\t\v\n\f]           ;
[a-zA-Z_][a-zA-Z_0-9]   FSAVE_TOKEN; return FIDENTIFIER;
[0-9]+.[0-9]+           FSAVE_TOKEN; return FDOUBLE;
[0-9]+                  FSAVE_TOKEN; return FINTEGER;
\"(\\.|[^\\"])*\"       FSAVE_TOKEN; return FSTRING_LITERAL;
"=="                    return FTOKEN(FEQ);
"!="                    return FTOKEN(FNEQ);
"<"                     return FTOKEN(FLT);
"<="                    return FTOKEN(FLE);
">"                     return FTOKEN(FGT);
">="                    return FTOKEN(FGE);
"("                     return FTOKEN(FLPAREN);
")"                     return FTOKEN(FRPAREN);
"["                     return FTOKEN(FLBRACKET);
"]"                     return FTOKEN(FRBRACKET);
"{"                     return FTOKEN(FLBRACE);
"}"                     return FTOKEN(FRBRACE);
"."                     return FTOKEN(FDOT);
","                     return FTOKEN(FCOMMA);
"?"                     return FTOKEN(FQUESTION);
":"                     return FTOKEN(FCOLON);
";"                     return FTOKEN(FSEMI);
"!"                     return FTOKEN(FNOT);
"~"                     return FTOKEN(FCOMPLEMENT);
"&"                     return FTOKEN(FAND);
"|"                     return FTOKEN(FOR);
"^"                     return FTOKEN(FXOR);
"+"                     return FTOKEN(FADD);
"-"                     return FTOKEN(FSUB);
"*"                     return FTOKEN(FMUL);
"/"                     return FTOKEN(FDIV);
"%"                     return FTOKEN(FMOD);
"="                     return FTOKEN(FASSIGN);
"+="                    return FTOKEN(FADDASSIGN);
"-="                    return FTOKEN(FSUBASSIGN);
"*="                    return FTOKEN(FMULASSIGN);
"/="                    return FTOKEN(FDIVASSIGN);
"%="                    return FTOKEN(FMODASSIGN);
.                       printf("Unknown token!n"); yyterminate();

%%

void fastypeLexerComment(int singleComment) {
    if (singleComment) {
        char c;
        while (true) {
            c = input();
            putchar(c);
            if (c == '\n') {
                break;
            }
        }
    } else {
        char c, c1;
        while (true) {
            c = input();
            while (c != '*' && c != 0)
                putchar(c);
            c1 = input();
            if (c1 != '/' && c != 0) {
                unput(c1);
                continue;
            }
            if (c != 0) {
                putchar(c1);
                break;
            }
        }
    }
}
