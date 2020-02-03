%option noyywrap nodefault yylineno

%x COMMENT
%x IMPORT

EXP ([Ee][-+]?[0-9]+)

%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <cctype>
#include <string>
#include <stack>
#include <algorithm>
#include "config/Platform.h"
#include "Log.h"
#include "Ast.h"
#include "Scope.h"
#include "Parser.tab.hpp"
#define T_SAVE_TOKEN yylval.literal = strndup(yytext, yyleng)
#define T_SAVE_TOKEN_N(c) yylval.literal = strndup(yytext, yyleng-(c))
#define T_TOKEN(t) (yylval.token = t)
#define T_FILE_EXT ".co"

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
std::string tokenModuleToFile(const std::string &name);
std::string tokenFileToModule(const std::string &name);

%}

%%

 /* block comment */
"/*"          { BEGIN COMMENT; }
<COMMENT>"*/" { BEGIN INITIAL; }
<COMMENT>([^*]|\en)+|.
 /* line comment */
"//".*\n

 /* keywords */
"true"       return T_TOKEN(T_TRUE);
"false"      return T_TOKEN(T_FALSE);
"let"        return T_TOKEN(T_LET);
"var"        return T_TOKEN(T_VAR);
"val"        return T_TOKEN(T_VAL);
"Nil"        return T_TOKEN(T_NIL);
"if"         return T_TOKEN(T_IF);
"elseif"     return T_TOKEN(T_ELSEIF);
"else"       return T_TOKEN(T_ELSE);
"switch"     return T_TOKEN(T_SWITCH);
"case"       return T_TOKEN(T_CASE);
"default"    return T_TOKEN(T_DEFAULT);
"for"        return T_TOKEN(T_FOR);
"while"      return T_TOKEN(T_WHILE);
"break"      return T_TOKEN(T_BREAK);
"continue"   return T_TOKEN(T_CONTINUE);
"func"       return T_TOKEN(T_FUNC);
"class"      return T_TOKEN(T_CLASS);
"type"       return T_TOKEN(T_TYPE);
"isinstance" return T_TOKEN(T_ISINSTANCE);
"import"     return T_TOKEN(T_IMPORT);
"return"     return T_TOKEN(T_RETURN);
"void"       return T_TOKEN(T_VOID);
"and"        return T_TOKEN(T_LOGIC_AND);
"or"         return T_TOKEN(T_LOGIC_OR);
"not"        return T_TOKEN(T_LOGIC_NOT);
"bit"        return T_TOKEN(T_BIT);
"i8"         return T_TOKEN(T_I8);
"ui8"        return T_TOKEN(T_UI8);
"i16"        return T_TOKEN(T_I16);
"ui16"       return T_TOKEN(T_UI16);
"i32"        return T_TOKEN(T_I32);
"ui32"       return T_TOKEN(T_UI32);
"i64"        return T_TOKEN(T_I64);
"ui64"       return T_TOKEN(T_UI64);
"f32"        return T_TOKEN(T_F32);
"f64"        return T_TOKEN(T_F64);
"string"     return T_TOKEN(T_STRING);
"boolean"    return T_TOKEN(T_BOOLEAN);

 /* punctuators */
"+"                      return T_TOKEN(T_ADD);
"-"                      return T_TOKEN(T_SUB);
"*"                      return T_TOKEN(T_MUL);
"/"                      return T_TOKEN(T_DIV);
"%"                      return T_TOKEN(T_MOD);
"!"                      return T_TOKEN(T_LOGIC_NOT);
"&"                      return T_TOKEN(T_BIT_AND);
"|"                      return T_TOKEN(T_BIT_OR);
"~"                      return T_TOKEN(T_BIT_NOT);
"^"                      return T_TOKEN(T_BIT_XOR);
"<<"                     return T_TOKEN(T_BIT_LSHIFT);
">>"                     return T_TOKEN(T_BIT_RSHIFT);
">>>"                    return T_TOKEN(T_BIT_ZERORSHIFT);
"="                      return T_TOKEN(T_ASSIGN);
"+="                     return T_TOKEN(T_ADD_ASSIGN);
"-="                     return T_TOKEN(T_SUB_ASSIGN);
"*="                     return T_TOKEN(T_MUL_ASSIGN);
"/="                     return T_TOKEN(T_DIV_ASSIGN);
"%="                     return T_TOKEN(T_MOD_ASSIGN);
"&="                     return T_TOKEN(T_BIT_AND_ASSIGN);
"|="                     return T_TOKEN(T_BIT_OR_ASSIGN);
"^="                     return T_TOKEN(T_BIT_XOR_ASSIGN);
"<<="                    return T_TOKEN(T_BIT_LSHIT_ASSIGN);
">>="                    return T_TOKEN(T_BIT_RSHIT_ASSIGN);
">>>="                   return T_TOKEN(T_BIT_ZERORSHIT_ASSIGN);
"=="                     return T_TOKEN(T_EQ);
"!="                     return T_TOKEN(T_NEQ);
"<"                      return T_TOKEN(T_LT);
"<="                     return T_TOKEN(T_LE);
">"                      return T_TOKEN(T_GT);
">="                     return T_TOKEN(T_GE);
"("                      return T_TOKEN(T_LPAREN);
")"                      return T_TOKEN(T_RPAREN);
"["                      return T_TOKEN(T_LBRACKET);
"]"                      return T_TOKEN(T_RBRACKET);
"{"                      return T_TOKEN(T_LBRACE);
"}"                      return T_TOKEN(T_RBRACE);
","                      return T_TOKEN(T_COMMA);
";"                      return T_TOKEN(T_SEMI);
"?"                      return T_TOKEN(T_QUESTION);
":"                      return T_TOKEN(T_COLON);
"."                      return T_TOKEN(T_DOT);

 /* constants */
[0-9]+\.[0-9]+{EXP}?        T_SAVE_TOKEN; return T_F64_CONSTANT; 
[0-9]+{EXP}?                T_SAVE_TOKEN; return T_I64_CONSTANT;
 /* [0-9]+{EXP}?[Uu]            T_SAVE_TOKEN_N(1); return T_UI64_CONSTANT; */
\"(\\.|[^\\"])*\"           T_SAVE_TOKEN; return T_STRING_CONSTANT;

 /* identifier */
[a-zA-Z_][a-zA-Z0-9_]*      T_SAVE_TOKEN; return T_IDENTIFIER;

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
    CASSERT(!TokenBufferStack.empty(), "TokenBufferStack must not empty! TokenCurrentFile:{} yylineno:{}", TokenCurrentFile, yylineno);
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

std::string tokenModuleToFile(const std::string &name) {
    std::string r(name);
    char replace;
#if FPLATFORM_WINDOWS
    replace = '\\';
#else
    replace = '/';
#endif
    std::for_each(r.begin(), r.end(), [&replace](char &c) { if (c == '.') {c = replace;} });
    r.append(T_FILE_EXT);
    return r;
}

std::string tokenFileToModule(const std::string &name) {
    std::string r(name.substr(0, name.length() - 5));
    std::for_each(r.begin(), r.end(), [](char &c) { if (c == '/' || c == '\\') {c = '.';} });
    return r;
}

TokenBuffer::TokenBuffer(const char *module) : yyBufferState(nullptr), fileName(tokenModuleToFile(module)), lineNo(1), fp(nullptr) {
    CASSERT(fileName.length() > 5, "{}:{} error! fileName {} invalid!", TokenCurrentFile, yylineno, fileName);
    fp = std::fopen(fileName.c_str(), "r");
    if (!fp) {
        release();
        CASSERT(fp, "{}:{} error! file {} cannot open!", TokenCurrentFile, yylineno, fileName);
    }
    yyBufferState = yy_create_buffer(fp, YY_BUF_SIZE);
    if (!yyBufferState) {
        release();
        CASSERT(yyBufferState, "{}:{} error! yy_create_buffer {} failed!", TokenCurrentFile, yylineno, fileName);
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
