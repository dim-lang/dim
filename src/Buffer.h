// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "parser.tab.hh"
#include "tokenizer.yy.hh"
#include <cstdio>
#include <stack>
#include <string>

class Scanner;

class Buffer {
public:
  /* methods */
  Buffer(const std::string &a_fileName, Scanner *scanner);
  virtual ~Buffer();

  /* members */
  YY_BUFFER_STATE yyBufferState;
  std::string fileName;
  int lineNo;
  yy::position yyPosition;
  FILE *fp;

  /* global methods */
  static std::string moduleToFile(const std::string &name);
  static std::string fileToModule(const std::string &name);

private:
  Scanner *scanner_;
  void release();
};

class BufferStack {
public:
  BufferStack(Scanner *scanner);
  virtual ~BufferStack();
  virtual int push(const std::string &fileName);
  virtual int pop();
  virtual Buffer *top() const;
  virtual int size() const;
  virtual bool empty() const;

private:
  Scanner *scanner_;
  std::stack<Buffer *> bufferStack_;
};
