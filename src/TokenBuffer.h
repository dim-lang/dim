// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Token.h"
#include <cstdio>
#include <stack>
#include <string>

class TokenBuffer {
public:
  /* methods */
  TokenBuffer(const std::string &module);
  virtual ~TokenBuffer();

  /* members */
  void *yyBufferState;
  std::string fileName;
  int lineNo;
  FILE *fp;

  /* global members */
  static std::stack<TokenBuffer *> BufferStack;
  static std::string CurrentBuffer;
  static int pushImport(const std::string &module);
  static int popImport();
  static std::string moduleToFile(const std::string &name);
  static std::string fileToModule(const std::string &name);

private:
  void release();
};
