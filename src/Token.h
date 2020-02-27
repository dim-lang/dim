// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include <cstdio>
#include <stack>
#include <string>

class AstProgram;
class Scanner;
struct yy_buffer_state;
struct YYLTYPE;
union YYSTYPE;
class Symbol;
class Type;
using YY_BUFFER_STATE = yy_buffer_state *;
using yyscan_t = void *;

extern int yylex(YYSTYPE *yylval_param, YYLTYPE *yylloc_param,
                 yyscan_t yyscanner);

/* global methods */
std::string moduleToFile(const std::string &name);

std::string fileToModule(const std::string &name);

class TokenBuffer {
public:
  /* methods */
  TokenBuffer(const std::string &module, yyscan_t scaninfo);
  virtual ~TokenBuffer();

  /* members */
  YY_BUFFER_STATE yyBufferState;
  std::string fileName;
  int lineNo;
  FILE *fp;

private:
  yyscan_t scaninfo_;
  void release();
};

class TokenBufferStack {
public:
  TokenBufferStack(Scanner *scanner);
  virtual ~TokenBufferStack();
  virtual const std::string &currentBuffer() const;
  virtual int push(const std::string &module);
  virtual int pop();
  virtual int size() const;
  virtual bool empty() const;
  virtual Scanner *scanner() const;

private:
  std::string currentBuffer_;
  std::stack<TokenBuffer *> stack_;
  Scanner *scanner_;
};

/* Scanner is yyextra */
class Scanner {
public:
  Scanner();
  virtual ~Scanner();
  /** short path for calling
   * tokenBufferStack->push()
   * tokenBufferStack->pop()
   */
  virtual int push(const std::string &module);
  virtual int pop();

  AstProgram *program;
  Symbol *globalSymbolScope;
  Type *globalTypeScope;
  yyscan_t scaninfo;
  TokenBufferStack *tokenBufferStack;
};
