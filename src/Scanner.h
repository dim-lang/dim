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
  Scanner(const std::string &fileName);
  virtual ~Scanner();

  // attributes
  virtual const std::string &fileName() const;
  virtual const Ast *compileUnit() const;
  virtual Ast *&compileUnit();

  // wrapper for flex/bison
  virtual Token tokenize();
  virtual int parse();

  virtual int topParentheses() const;
  virtual int eatParentheses(int tok);
  virtual int newlineEnabled() const;
  virtual bool parenthesesEmpty() const;
  virtual int parenthesesSize() const;

private:
  std::string fileName_;
  YY_BUFFER_STATE yyBufferState_;
  FILE *fp_;
  yyscan_t yyscanner_;
  Ast *compileUnit_;
  // tokenizer parentheses stack
  std::stack<int> parenthesesStack_;
};
