// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Parser.tab.hpp"
#include "Symbol.h"
#include "Token.h"
#include "Type.h"
#include <string>
#include <tuple>
#include <utility>

class AstProgram;
class Symbol;
class Type;

class Scanner {
public:
  Scanner(const std::string &fileName);
  virtual ~Scanner();

  // buffer stack
  virtual int push(const std::string &module);
  virtual int pop();
  virtual const std::string &currentBuffer() const;
  virtual Buffer *top() const;
  virtual int size() const;
  virtual bool empty() const;

  // attribute access
  virtual const AstProgram *program() const;
  virtual AstProgram *&program();
  virtual const Symbol *gss() const;
  virtual Symbol *&gss();
  virtual const Type *gts() const;
  virtual Type *&gts();
  virtual const yyscan_t yy_scaninfo() const;
  virtual yyscan_t &yy_scaninfo();

  // wrapper for flex/bison
  virtual std::tuple<int, YYSTYPE, YYLTYPE> tokenize();
  virtual int parse();

private:
  std::string fileName_;
  std::string moduleName_;

  AstProgram *program_;
  Symbol *gss_;
  Type *gts_;
  yyscan_t yy_scaninfo_;
  BufferStack *bufferStack_;
};
