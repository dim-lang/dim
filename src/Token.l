%option noyywrap nodefault yylineno

%x COMMENT
%x IMPORT

EXP ([Ee][-+]?[0-9]+)

%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <memory>
#include <string>
#include <stack>
#include <algorithm>
#include "config/Platform.h"
#include "Log.h"
#include "Ast.h"
#include "Scope.h"
#include "Parser.tab.hpp"
#define FT_SAVE_TOKEN yylval.literal = strndup(yytext, yyleng)
#define FT_TOKEN(t) (yylval.token = t)
#define FT_FILE_EXT ".ft"

std::string TokenCurrentFile;

class TokenBuffer {
public:
 /* methods */
    TokenBuffer(const char *module);
    virtual ~TokenBuffer();
    void release();

 /* members */
    YY_BUFFER_STATE yyBufferState;
    std::string fileName;
    int lineNo;
    FILE *fp;
};

 /* global variable */
std::stack<TokenBuffer*> TokenBufferStack;
int tokenImportFile(const char *module);
int tokenImportFile(const std::string &module);
int tokenPopFile();
static std::string tokenModuleToFile(const std::string &name);
static std::string tokenFileToModule(const std::string &name);

%}

%%

 /* block comment */
"/*"          { BEGIN COMMENT; }
<COMMENT>"*/" { BEGIN INITIAL; }
<COMMENT>([^*]|\en)+|.
 /* line comment */
"//".*\n

 /* keywords */
"True"                   return FT_TRUE;
"False"                  return FT_FALSE;
"let"                    return FT_LET;
"Nil"                    return FT_NIL;
"if"                     return FT_IF;
"elseif"                 return FT_ELSEIF;
"else"                   return FT_ELSE;
"switch"                 return FT_SWITCH;
"case"                   return FT_CASE;
"otherwise"              return FT_OTHERWISE;
"for"                    return FT_FOR;
"while"                  return FT_WHILE;
"break"                  return FT_BREAK;
"continue"               return FT_CONTINUE;
"func"                   return FT_FUNC;
"class"                  return FT_CLASS;
"type"                   return FT_TYPE;
"isinstance"             return FT_ISINSTANCE;
 /* "import"                 return FT_IMPORT; */
"return"                 return FT_RETURN;
"void"                   return FT_VOID;
"and"                    return FT_LOGIC_AND;
"or"                     return FT_LOGIC_OR;
"not"                    return FT_LOGIC_NOT;
"int"                    return FT_INTEGER_KEYWORD;
"uint"                   return FT_UNSIGNED_INTEGER_KEYWORD;
"double"                 return FT_DOUBLE_KEYWORD;

 /* punctuators */
"+"                      return FT_TOKEN(FT_ADD);
"-"                      return FT_TOKEN(FT_SUB);
"*"                      return FT_TOKEN(FT_MUL);
"/"                      return FT_TOKEN(FT_DIV);
"%"                      return FT_TOKEN(FT_MOD);
"!"                      return FT_TOKEN(FT_BIT_NOT);
"&"                      return FT_TOKEN(FT_BIT_AND);
"|"                      return FT_TOKEN(FT_BIT_OR);
"~"                      return FT_TOKEN(FT_BIT_COMPLEMENT);
"^"                      return FT_TOKEN(FT_BIT_XOR);
"<<"                     return FT_TOKEN(FT_BIT_LSHIFT);
">>"                     return FT_TOKEN(FT_BIT_RSHIFT);
">>>"                    return FT_TOKEN(FT_BIT_ZERORSHIFT);
"="                      return FT_TOKEN(FT_ASSIGN);
"+="                     return FT_TOKEN(FT_ADD_ASSIGN);
"-="                     return FT_TOKEN(FT_SUB_ASSIGN);
"*="                     return FT_TOKEN(FT_MUL_ASSIGN);
"/="                     return FT_TOKEN(FT_DIV_ASSIGN);
"%="                     return FT_TOKEN(FT_MOD_ASSIGN);
"&="                     return FT_TOKEN(FT_BIT_AND_ASSIGN);
"|="                     return FT_TOKEN(FT_BIT_OR_ASSIGN);
"^="                     return FT_TOKEN(FT_BIT_XOR_ASSIGN);
"<<="                    return FT_TOKEN(FT_BIT_LSHIFT_ASSIGN);
">>="                    return FT_TOKEN(FT_BIT_RSHIFT_ASSIGN);
">>>="                   return FT_TOKEN(FT_BIT_ZERORSHIFT_ASSIGN);
"=="                     return FT_TOKEN(FT_EQ);
"!="                     return FT_TOKEN(FT_NEQ);
"<"                      return FT_TOKEN(FT_LT);
"<="                     return FT_TOKEN(FT_LE);
">"                      return FT_TOKEN(FT_GT);
">="                     return FT_TOKEN(FT_GE);
"("                      return FT_TOKEN(FT_LPAREN);
")"                      return FT_TOKEN(FT_RPAREN);
"["                      return FT_TOKEN(FT_LBRACKET);
"]"                      return FT_TOKEN(FT_RBRACKET);
"{"                      return FT_TOKEN(FT_LBRACE);
"}"                      return FT_TOKEN(FT_RBRACE);
","                      return FT_TOKEN(FT_COMMA);
";"                      return FT_TOKEN(FT_SEMI);
"?"                      return FT_TOKEN(FT_QUESTION);
":"                      return FT_TOKEN(FT_COLON);
"."                      return FT_TOKEN(FT_DOT);

 /* constants */
[0-9]+\.[0-9]+{EXP}?     FT_SAVE_TOKEN; return FT_DOUBLE;
[0-9]+{EXP}?             FT_SAVE_TOKEN; return FT_INTEGER;
\"(\\.|[^\\"])*\"        FT_SAVE_TOKEN; return FT_STRING;

 /* identifier */
[a-zA-Z_][a-zA-Z0-9_]*   FT_SAVE_TOKEN; return FT_IDENTIFIER;

 /* whitespace */
[ \t\v\n\f\r]

 /* import module */
^[ \t]*import[ \t]*     { BEGIN IMPORT; }
<IMPORT>[^ \t\n\;]+     {
                            {
                                int c;
                                while ((c = yyinput()) && c != ';')
                                    ;
                            }
                            tokenImportFile(yytext);
                            BEGIN INITIAL;
                        }
<IMPORT>.|\n            {
                            fprintf(stderr, "%s:%d error! bad import!\n", TokenCurrentFile.c_str(), yylineno);
                            BEGIN INITIAL;
                        }
<<EOF>>                 {
                            if (!tokenPopFile())
                                yyterminate();
                        }

 /* invalid token */
.                       { fprintf(stderr, "%s:%d error! invalid token!\n", TokenCurrentFile.c_str(), yylineno); }


%%

int tokenImportFile(const char *module) {
    TokenBuffer *fb = new TokenBuffer(module);

    /* remember state */
    if (!TokenBufferStack.empty()) {
        TokenBufferStack.top()->lineNo = yylineno;
    }

    /* switch to new file */
    TokenBufferStack.push(fb);
    yy_switch_to_buffer(fb->yyBufferState);
    yylineno = 1;
    TokenCurrentFile = fb->fileName;
    return 1;
}

int tokenImportFile(const std::string &module) {
    return tokenImportFile(module.c_str());
}

int tokenPopFile() {
    FCHECK(!TokenBufferStack.empty(), "TokenBufferStack must not empty! TokenCurrentFile:{} yylineno:{}", TokenCurrentFile, yylineno);
    TokenBuffer *fb;

    fb = TokenBufferStack.top();
    TokenBufferStack.pop();
    delete fb;
    fb = nullptr;
    if (TokenBufferStack.empty()) {
        TokenCurrentFile = "";
        return 0;
    }

    fb = TokenBufferStack.top();
    yy_switch_to_buffer(fb->yyBufferState);
    yylineno = fb->lineNo;
    TokenCurrentFile = fb->fileName;
    return 1;
}

static std::string tokenModuleToFile(const std::string &name) {
    std::string r(name);
    char replace;
#if FPLATFORM_WINDOWS
    replace = '\\';
#else
    replace = '/';
#endif
    std::for_each(r.begin(), r.end(), [&replace](char &c) { if (c == '.') {c = replace;} });
    r.append(FT_FILE_EXT);
    return r;
}

static std::string tokenFileToModule(const std::string &name) {
    std::string r(name.substr(0, name.length() - 5));
    std::for_each(r.begin(), r.end(), [](char &c) { if (c == '/' || c == '\\') {c = '.';} });
    return r;
}

TokenBuffer::TokenBuffer(const char *module) : yyBufferState(nullptr), fileName(tokenModuleToFile(module)), lineNo(1), fp(nullptr) {
    FCHECK(fileName.length() > 5, "{}:{} error! fileName {} invalid!", TokenCurrentFile, yylineno, fileName);
    fp = std::fopen(fileName.c_str(), "r");
    if (!fp) {
        release();
        FCHECK(fp, "{}:{} error! file {} cannot open!", TokenCurrentFile, yylineno, fileName);
    }
    yyBufferState = yy_create_buffer(fp, YY_BUF_SIZE);
    if (!yyBufferState) {
        release();
        FCHECK(yyBufferState, "{}:{} error! yy_create_buffer {} failed!", TokenCurrentFile, yylineno, fileName);
    }
}

TokenBuffer::~TokenBuffer() {
    release();
}

void TokenBuffer::release() {
    if (fp) {
        std::fclose(fp);
        fp = nullptr;
    }
    if (yyBufferState) {
        yy_delete_buffer(yyBufferState);
        yyBufferState = nullptr;
    }
}
