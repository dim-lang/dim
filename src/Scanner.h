// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Location.h"
#include "Token.h"
#include <cstdio>
#include <memory>
#include <stack>
#include <string>

struct yy_buffer_state;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
typedef void *yyscan_t;
class Ast;

class Scanner {
public:
  Scanner(const std::string &a_fileName);
  virtual ~Scanner();

  // attributes
  std::string fileName;
  YY_BUFFER_STATE yyBufferState;
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
