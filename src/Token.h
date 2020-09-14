// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "container/Cowstr.h"
#include "parser.tab.hh"

struct Token {
  int value;
  YYSTYPE yylval;
  YYLTYPE yylloc;

  Token(int a_value, YYSTYPE a_yylval, YYLTYPE a_yylloc);
  virtual ~Token() = default;
};

const Cowstr &tokenName(int value);

int tokenValue(const Cowstr &name);

bool tokenValid(int token);

bool tokenValid(const Cowstr &name);

bool tokenIsLiteral(int value);
