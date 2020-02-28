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
using YY_EXTRA_TYPE = Scanner *;

extern int yylex(YYSTYPE *yylval_param, YYLTYPE *yylloc_param,
                 yyscan_t yyscanner);
extern int yylex_init_extra(YY_EXTRA_TYPE yy_user_defined,
                            yyscan_t *ptr_yy_globals);
extern int yylex_destroy(yyscan_t yyscanner);

class Buffer {
public:
  /* methods */
  Buffer(const std::string &fileName, yyscan_t yy_scaninfo);
  virtual ~Buffer();

  /* members */
  YY_BUFFER_STATE yyBufferState;
  std::string fileName;
  int lineNo;
  FILE *fp;

  /* global methods */
  static std::string moduleToFile(const std::string &name);
  static std::string fileToModule(const std::string &name);

private:
  yyscan_t yy_scaninfo_;
  void release();
};

class BufferStack {
public:
  BufferStack(yyscan_t yy_scaninfo);
  virtual ~BufferStack();
  virtual int push(const std::string &fileName);
  virtual int pop();
  virtual Buffer *top() const;
  virtual int size() const;
  virtual bool empty() const;

private:
  yyscan_t yy_scaninfo_;
  std::stack<Buffer *> bufferStack_;
};
