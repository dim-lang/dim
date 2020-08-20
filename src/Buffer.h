// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Parser.tab.h"
#include "Tokenizer.yy.h"
#include <cstdio>
#include <stack>
#include <string>

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
