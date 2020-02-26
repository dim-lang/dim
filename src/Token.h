// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include <cstdio>
#include <stack>
#include <string>

class AstProgram;
struct yy_buffer_state;
struct YYLTYPE;
union YYSTYPE;
using YY_BUFFER_STATE = yy_buffer_state *;

extern int yylex(YYSTYPE *yylval_param, YYLTYPE *yylloc_param);

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

  /* global members */
  static std::string CurrentBuffer;
  static int pushImport(const std::string &module);
  static int popImport();
  static std::string moduleToFile(const std::string &name);
  static std::string fileToModule(const std::string &name);

private:
  void release();
};
