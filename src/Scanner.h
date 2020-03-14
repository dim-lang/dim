// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Parser.tab.hpp"
#include "Symbol.h"
#include "SymbolTable.h"
#include "Token.h"
#include "Type.h"
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
  virtual const AstTranslateUnit *translateUnit() const;
  virtual AstTranslateUnit *&translateUnit();
  virtual const SymbolTable *symtable() const;
  virtual SymbolTable *&symtable();
  virtual const yyscan_t yy_scaninfo() const;
  virtual yyscan_t &yy_scaninfo();
  virtual const std::string &fileName() const;

  // wrapper for flex/bison
  virtual std::tuple<int, YYSTYPE, YYLTYPE> tokenize();
  virtual int parse();

private:
  std::string fileName_;

  AstTranslateUnit *translateUnit_;
  yyscan_t yy_scaninfo_;
  BufferStack *bufferStack_;
  SymbolTable *symtable_;
};
