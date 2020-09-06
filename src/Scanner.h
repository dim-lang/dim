// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "parser.tab.hh"
#include "tokenizer.yy.hh"
#include <cstdio>
#include <memory>
#include <stack>
#include <string>
#include <utility>

struct Token {
  int token;
  YYSTYPE yylval;
  YYLTYPE yylloc;
};

class Scanner {
public:
  Scanner(const std::string &a_fileName);
  virtual ~Scanner();

  // attributes
  std::string fileName;
  YY_BUFFER_STATE yyBufferState;
  Location location;
  FILE *fp;
  yyscan_t yyscanner;
  std::shared_ptr<Ast> compileUnit;

  // wrapper for flex/bison
  virtual Token tokenize();
  virtual int parse();

  virtual int topParentheses() const;
  virtual int eatParentheses(int tok);
  virtual int newlineEnabled() const;
  virtual bool parenthesesEmpty() const;
  virtual int parenthesesSize() const;

private:
  // tokenizer parentheses stack
  std::stack<int> parenthesesStack_;
};
