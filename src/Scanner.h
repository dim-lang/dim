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

class Scanner {
public:
  Scanner(const std::string &fileName);
  virtual ~Scanner();

  // buffer stack
  virtual int push(const std::string &fileName);
  virtual int pop();
  virtual const std::string &currentBuffer() const;
  virtual Buffer *top() const;
  virtual int size() const;
  virtual bool empty() const;

  // attribute access
  virtual const AstProgram *program() const;
  virtual AstProgram *&program();
  virtual const Symtab *gss() const;
  virtual Symtab *&gss();
  virtual const Symtab *css() const;
  virtual Symtab *&css();
  virtual const Tytab *gts() const;
  virtual Tytab *&gts();
  virtual const Tytab *cts() const;
  virtual Tytab *&cts();
  virtual const yyscan_t yy_scaninfo() const;
  virtual yyscan_t &yy_scaninfo();
  virtual const std::string &fileName() const;

  // wrapper for flex/bison
  virtual std::tuple<int, YYSTYPE, YYLTYPE> tokenize();
  virtual int parse();

private:
  std::string fileName_;

  AstProgram *program_;
  Symtab *gss_; // global symbol scope
  Symtab *css_; // current symbol scope
  Tytab *gts_;  // global type scope
  Tytab *cts_;  // current type scope
  yyscan_t yy_scaninfo_;
  BufferStack *bufferStack_;
};
