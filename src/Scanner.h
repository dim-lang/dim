// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Buffer.h"
#include "parser.tab.h"
#include <string>
#include <tuple>
#include <utility>

class Scanner {
public:
  Scanner();
  virtual ~Scanner();

  // buffer stack
  virtual int pushBuffer(const std::string &fileName);
  virtual int popBuffer();
  virtual const std::string &currentBuffer() const;
  virtual Buffer *top() const;
  virtual int size() const;
  virtual bool empty() const;

  // attribute access
  virtual const std::string &fileName() const;
  virtual const AstTranslateUnit *translateUnit() const;
  virtual AstTranslateUnit *&translateUnit();
  virtual const yyscan_t *yyscanner() const;
  virtual yyscan_t *&yyscanner();

  // wrapper for flex/bison
  virtual std::tuple<int, YYSTYPE, YYLTYPE> tokenize();
  virtual int parse();

private:
  std::string fileName_;
  AstTranslateUnit *translateUnit_;
  yyscan_t yyscanner_;
  BufferStack *bufferStack_;
};
