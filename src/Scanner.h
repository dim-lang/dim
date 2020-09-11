// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Cowstr.h"
#include "Location.h"
#include "Token.h"
#include <cstdio>
#include <stack>

struct yy_buffer_state;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
typedef void *yyscan_t;
class Ast;

class Scanner {
public:
  Scanner(const Cowstr &fileName);
  virtual ~Scanner();

  // attributes
  const Cowstr &fileName() const;
  const Ast *compileUnit() const;
  Ast *&compileUnit();

  // wrapper for flex/bison
  Token tokenize();
  int parse();

  // tokenizer util
  int topParentheses() const;
  int eatParentheses(int tok);
  int newlineEnabled() const;
  bool parenthesesEmpty() const;
  int parenthesesSize() const;

private:
  Cowstr fileName_;
  YY_BUFFER_STATE yyBufferState_;
  FILE *fp_;
  yyscan_t yyscanner_;
  Ast *compileUnit_;
  // tokenizer util
  std::stack<int> parenthesesStack_;
};
