// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "BufferStack.h"
#include "yydecl.h"
#include <string>
#include <tuple>
#include <utility>

class Scanner {
public:
  Scanner();
  virtual ~Scanner();

  // attributes
  std::shared_ptr<Ast> compileUnit;
  yyscan_t yyscanner;
  BufferStack bufferStack;
  std::shared_ptr<yy::parser> parser;

  // wrapper for flex/bison
  virtual yy::parser::symbol_type tokenize();
  virtual int parse();
};
