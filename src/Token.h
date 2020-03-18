// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Parser.tab.hpp"
#include "config/Platform.h"
#include <cstdio>
#include <stack>
#include <string>

#ifdef PLATFORM_WINDOWS
#define YY_NO_UNISTD_H
#endif

class Scanner;
struct yy_buffer_state;
using YY_BUFFER_STATE = yy_buffer_state *;
using yyscan_t = void *;
using YY_EXTRA_TYPE = Scanner *;

extern int yylex(YYSTYPE *yylval_param, YYLTYPE *yylloc_param,
                 yyscan_t yyscanner);
extern int yylex_init_extra(YY_EXTRA_TYPE yy_user_defined,
                            yyscan_t *ptr_yy_globals);
extern int yylex_destroy(yyscan_t yyscanner);
extern void yyset_lineno(int _line_number, yyscan_t yyscanner);
extern void yy_switch_to_buffer(YY_BUFFER_STATE new_buffer, yyscan_t yyscanner);
extern YY_BUFFER_STATE yy_create_buffer(FILE *file, int size,
                                        yyscan_t yyscanner);
extern void yy_delete_buffer(YY_BUFFER_STATE b, yyscan_t yyscanner);
extern int yyget_lineno(yyscan_t yyscanner);

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
