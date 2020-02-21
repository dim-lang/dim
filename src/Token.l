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
#include "Parser.tab.hpp"
#define T_SAVE_TOKEN yylval.literal = strndup(yytext, yyleng)
#define T_SAVE_TOKEN_X(start, length) yylval.literal = strndup(yytext+(start), yyleng-(length))
#define T_TOKEN(t) (yylval.token = t)
#define T_FILE_EXT ".co"

class TokenBuffer {
public:
  /* methods */
  TokenBuffer(const std::string &module);
  virtual ~TokenBuffer();

  /* members */
  YY_BUFFER_STATE yyBufferState;
  std::string fileName;
  int lineNo;
  FILE *fp;

private:
    void release();
};

 /* global variable */
static std::stack<TokenBuffer*> BufferStack;
static std::string CurrentBuffer;

int tokenPushImport(const std::string &module);
int tokenPopImport();

std::string tokenModuleToFile(const std::string &name);
std::string tokenFileToModule(const std::string &name);

%}

%%

 /* block comment */
"/*"                    { BEGIN COMMENT; }
<COMMENT>"*/"           { BEGIN INITIAL; }
<COMMENT>([^*]|\en)+|.
 /* line comment */
"//".*\n

 /* keywords */
"true"       return T_TOKEN(T_TRUE);
"false"      return T_TOKEN(T_FALSE);
"let"        return T_TOKEN(T_LET);
"var"        return T_TOKEN(T_VAR);
"val"        return T_TOKEN(T_VAL);
"nil"        return T_TOKEN(T_NIL);
"if"         return T_TOKEN(T_IF);
"else"       return T_TOKEN(T_ELSE);
"switch"     return T_TOKEN(T_SWITCH);
"case"       return T_TOKEN(T_CASE);
"default"    return T_TOKEN(T_DEFAULT);
"for"        return T_TOKEN(T_FOR);
"foreach"    return T_TOKEN(T_FOREACH);
"in"         return T_TOKEN(T_IN);
"while"      return T_TOKEN(T_WHILE);
"break"      return T_TOKEN(T_BREAK);
"continue"   return T_TOKEN(T_CONTINUE);
"func"       return T_TOKEN(T_FUNC);
"class"      return T_TOKEN(T_CLASS);
"type"       return T_TOKEN(T_TYPE);
"is"         return T_TOKEN(T_IS);
"isinstance" return T_TOKEN(T_ISINSTANCE);
"import"     return T_TOKEN(T_IMPORT);
"return"     return T_TOKEN(T_RETURN);
"void"       return T_TOKEN(T_VOID);
"and"        return T_TOKEN(T_LOGIC_AND);
"or"         return T_TOKEN(T_LOGIC_OR);
"not"        return T_TOKEN(T_LOGIC_NOT);
"i8"         return T_TOKEN(T_I8);
"u8"         return T_TOKEN(T_U8);
"i16"        return T_TOKEN(T_I16);
"u16"        return T_TOKEN(T_U16);
"i32"        return T_TOKEN(T_I32);
"u32"        return T_TOKEN(T_U32);
"i64"        return T_TOKEN(T_I64);
"u64"        return T_TOKEN(T_U64);
"f32"        return T_TOKEN(T_F32);
"f64"        return T_TOKEN(T_F64);
"string"     return T_TOKEN(T_STRING);
"boolean"    return T_TOKEN(T_BOOLEAN);
"async"      return T_TOKEN(T_ASYNC);
"await"      return T_TOKEN(T_AWAIT);

 /* punctuators */
"+"    return T_TOKEN(T_ADD);
"-"    return T_TOKEN(T_SUB);
"*"    return T_TOKEN(T_MUL);
"/"    return T_TOKEN(T_DIV);
"%"    return T_TOKEN(T_MOD);
"&&"   return T_TOKEN(T_LOGIC_AND);
"||"   return T_TOKEN(T_LOGIC_OR);
"!"    return T_TOKEN(T_LOGIC_NOT);
"&"    return T_TOKEN(T_BIT_AND);
"|"    return T_TOKEN(T_BIT_OR);
"~"    return T_TOKEN(T_BIT_NOT);
"^"    return T_TOKEN(T_BIT_XOR);
"<<"   return T_TOKEN(T_BIT_LSHIFT);
">>"   return T_TOKEN(T_BIT_RSHIFT);
">>>"  return T_TOKEN(T_BIT_ARSHIFT);
"="    return T_TOKEN(T_ASSIGN);
"+="   return T_TOKEN(T_ADD_ASSIGN);
"-="   return T_TOKEN(T_SUB_ASSIGN);
"*="   return T_TOKEN(T_MUL_ASSIGN);
"/="   return T_TOKEN(T_DIV_ASSIGN);
"%="   return T_TOKEN(T_MOD_ASSIGN);
"&="   return T_TOKEN(T_BIT_AND_ASSIGN);
"|="   return T_TOKEN(T_BIT_OR_ASSIGN);
"^="   return T_TOKEN(T_BIT_XOR_ASSIGN);
"<<="  return T_TOKEN(T_BIT_LSHIFT_ASSIGN);
">>="  return T_TOKEN(T_BIT_RSHIFT_ASSIGN);
">>>=" return T_TOKEN(T_BIT_ARSHIFT_ASSIGN);
"=="   return T_TOKEN(T_EQ);
"!="   return T_TOKEN(T_NEQ);
"<"    return T_TOKEN(T_LT);
"<="   return T_TOKEN(T_LE);
">"    return T_TOKEN(T_GT);
">="   return T_TOKEN(T_GE);
"("    return T_TOKEN(T_LPAREN);
")"    return T_TOKEN(T_RPAREN);
"["    return T_TOKEN(T_LBRACKET);
"]"    return T_TOKEN(T_RBRACKET);
"{"    return T_TOKEN(T_LBRACE);
"}"    return T_TOKEN(T_RBRACE);
","    return T_TOKEN(T_COMMA);
";"    return T_TOKEN(T_SEMI);
"?"    return T_TOKEN(T_QUESTION);
":"    return T_TOKEN(T_COLON);
"."    return T_TOKEN(T_DOT);
"=>"   return T_TOKEN(T_BIG_ARROW);

 /* constants */
[0-9]+"."[0-9]+{EXP}?   T_SAVE_TOKEN; return T_F64_CONSTANT;
[0-9]+{EXP}?            T_SAVE_TOKEN; return T_I64_CONSTANT;
 /* [0-9]+{EXP}?[Uu]    T_SAVE_TOKEN_X(0, 1); return T_U64_CONSTANT; */
\"(\\.|[^\\"])*\"       T_SAVE_TOKEN_X(1, 2); return T_STRING_CONSTANT;

 /* identifier */
[a-zA-Z_][a-zA-Z0-9_]*  T_SAVE_TOKEN; return T_IDENTIFIER;

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
                            tokenPushImport(yytext);
                            BEGIN INITIAL;
                        }
<IMPORT>.|\n            {
                            fprintf(stderr, "%s:%d error! bad import:%s, %d!\n", CurrentBuffer.c_str(), yylineno, yytext, yyleng);
                            CERROR("{}:{} error! bad import:{}, {}!", CurrentBuffer, yylineno, yytext, yyleng);
                            BEGIN INITIAL;
                        }
<<EOF>>                 {
                            if (!tokenPopImport())
                                yyterminate();
                        }

 /* invalid token */
.                       { 
                            fprintf(stderr, "%s:%d error! invalid token:%s, %d!\n", CurrentBuffer.c_str(), yylineno, yytext, yyleng); 
                            CERROR("{}:{} error! invalid token:{}, {}!", CurrentBuffer, yylineno, yytext, yyleng);
                        }


%%

int tokenPushImport(const std::string &module) {
    TokenBuffer *fb = new TokenBuffer(module);

    /* remember state */
    if (!BufferStack.empty()) {
        BufferStack.top()->lineNo = yylineno;
    }

    /* switch to new file */
    BufferStack.push(fb);
    yy_switch_to_buffer(fb->yyBufferState);
    yylineno = 1;
    CurrentBuffer = fb->fileName;
    return 1;
}

int tokenPopImport() {
    CASSERT(!BufferStack.empty(), "BufferStack must not empty! CurrentBuffer:{} yylineno:{}", CurrentBuffer, yylineno);
    TokenBuffer *fb;

    fb = BufferStack.top();
    BufferStack.pop();
    delete fb;
    fb = nullptr;
    if (BufferStack.empty()) {
        CurrentBuffer = "";
        return 0;
    }

    fb = BufferStack.top();
    yy_switch_to_buffer(fb->yyBufferState);
    yylineno = fb->lineNo;
    CurrentBuffer = fb->fileName;
    return 1;
}

std::string tokenModuleToFile(const std::string &name) {
    std::string r(name);
    char replace;
#if PLATFORM_WINDOWS
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

TokenBuffer::TokenBuffer(const std::string &module) : yyBufferState(nullptr), fileName(tokenModuleToFile(module)), lineNo(1), fp(nullptr) {
    CASSERT(fileName.length() > 5, "{}:{} error! fileName {} invalid!", CurrentBuffer, yylineno, fileName);
    fp = std::fopen(fileName.c_str(), "r");
    if (!fp) {
        release();
        CASSERT(fp, "{}:{} error! file {} cannot open!", CurrentBuffer, yylineno, fileName);
    }
    yyBufferState = yy_create_buffer(fp, YY_BUF_SIZE);
    if (!yyBufferState) {
        release();
        CASSERT(yyBufferState, "{}:{} error! yy_create_buffer {} failed!", CurrentBuffer, yylineno, fileName);
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
