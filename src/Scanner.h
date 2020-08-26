// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Buffer.h"
#include "BufferStack.h"
#include "location.hh"
#include "parser.tab.hh"
#include "tokenizer.yy.hh"
#include <string>
#include <tuple>
#include <utility>

class Scanner {
public:
  Scanner();
  virtual ~Scanner();

  // attributes
  A_CompileUnit *compileUnit;
  yyscan_t yyscanner;
  BufferStack bufferStack;

  // wrapper for flex/bison
  virtual int tokenize();
  virtual int parse();
};
