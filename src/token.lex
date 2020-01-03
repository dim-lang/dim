%{
#include <string>
#include "node.h"
#include "parser.hpp"
#define FSAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define FTOKEN(t) (yylval.token = t)
extern "C" int yywarp() {}
%}

%%

"True"                  return FTOKEN(FTRUE);
"False"                 return FTOKEN(FFALSE);
"Nil"                   return FTOKEN(FNIL);
"if"                    return FTOKEN(FIF);
"else"                  return FTOKEN(FELSE);
"for"                   return FTOKEN(FFOR);
"while"                 return FTOKEN(FWHILE);
"break"                 return FTOKEN(FBREAK);
"continue"              return FTOKEN(FCONTINUE);
"void"                  return FTOKEN(FVOID);
"return"                return FTOKEN(FRETURN);
"func"                  return FTOKEN(FFUNC);
"struct"                return FTOKEN(FSTRUCT);
"class"                 return FTOKEN(FCLASS);
"import"                return FTOKEN(FIMPORT);
"enum"                  return FTOKEN(FENUM);
"and"                   return FTOKEN(FLOGICAND);
"or"                    return FTOKEN(FLOGICOR);
[ \t\v\n\f]             ;
[a-zA-Z_][a-zA-Z0-9_]*  FSAVE_TOKEN; return FID;
[0-9]+.[0-9]*           FSAVE_TOKEN; return FDOUBLE;
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
