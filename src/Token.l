%{
#include <string>
#include "Parser.hpp"
#define FT_SAVE_TOKEN yylval.literal = std::string(yytext, yyleng)
#define FT_TOKEN(t) (yylval.token = t)
%}

%%

"True"                            return FT_TRUE;
"False"                           return FT_FALSE;
"let"                             return FT_LET;
"Nil"                             return FT_NIL;
"if"                              return FT_IF;
"elseif"                          return FT_ELSEIF;
"else"                            return FT_ELSE;
"for"                             return FT_FOR;
"while"                           return FT_WHILE;
"break"                           return FT_BREAK;
"continue"                        return FT_CONTINUE;
"func"                            return FT_FUNC;
"class"                           return FT_CLASS;
"type"                            return FT_TYPE;
"isinstance"                      return FT_ISINSTANCE;
"import"                          return FT_IMPORT;
"return"                          return FT_RETURN;
"void"                            return FT_VOID;
"and"                             return FT_LOGICALAND;
"or"                              return FT_LOGICALOR;
"not"                             return FT_LOGICALNOT;
[ \t\v\n\f\r]                     ;
[a-zA-Z_][a-zA-Z0-9_]*            FT_SAVE_TOKEN; return FT_IDENTIFIER;
[0-9]+"."[0-9]+([Ee][+-]?[0-9]+)? FT_SAVE_TOKEN; return FT_REAL;
[0-9]+([Ee][+-]?[0-9]+)?          FT_SAVE_TOKEN; return FT_INTEGER;
\"(\\.|[^\\"])*\"                 FT_SAVE_TOKEN; return FT_STRING;
"+"                               return FT_TOKEN(FT_ADD);
"-"                               return FT_TOKEN(FT_SUB);
"*"                               return FT_TOKEN(FT_MUL);
"/"                               return FT_TOKEN(FT_DIV);
"%"                               return FT_TOKEN(FT_MOD);
"!"                               return FT_TOKEN(FT_BITNOT);
"&"                               return FT_TOKEN(FT_BITAND);
"|"                               return FT_TOKEN(FT_BITOR);
"~"                               return FT_TOKEN(FT_BITCOMPLEMENT);
"^"                               return FT_TOKEN(FT_BITXOR);
"="                               return FT_TOKEN(FT_ASSIGN);
"+="                              return FT_TOKEN(FT_ADDASSIGN);
"-="                              return FT_TOKEN(FT_SUBASSIGN);
"*="                              return FT_TOKEN(FT_MULASSIGN);
"/="                              return FT_TOKEN(FT_DIVASSIGN);
"%="                              return FT_TOKEN(FT_MODASSIGN);
"=="                              return FT_TOKEN(FT_EQ);
"!="                              return FT_TOKEN(FT_NEQ);
"<"                               return FT_TOKEN(FT_LT);
"<="                              return FT_TOKEN(FT_LE);
">"                               return FT_TOKEN(FT_GT);
">="                              return FT_TOKEN(FT_GE);
"("                               return FT_TOKEN(FT_LPAREN);
")"                               return FT_TOKEN(FT_RPAREN);
"["                               return FT_TOKEN(FT_LBRACKET);
"]"                               return FT_TOKEN(FT_RBRACKET);
"{"                               return FT_TOKEN(FT_LBRACE);
"}"                               return FT_TOKEN(FT_RBRACE);
","                               return FT_TOKEN(FT_COMMA);
";"                               return FT_TOKEN(FT_SEMI);
"?"                               return FT_TOKEN(FT_QUESTION);
":"                               return FT_TOKEN(FT_COLON);
"."                               return FT_TOKEN(FT_DOT);
.                                 printf("Unknown token!n"); yyterminate();

%%
