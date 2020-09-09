// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "parser.tab.hh"
#include <string>

struct Token {
  int value;
  YYSTYPE yylval;
  YYLTYPE yylloc;

  Token(int a_value, YYSTYPE a_yylval, YYLTYPE a_yylloc);
  virtual ~Token() = default;
};

const std::string &tokenName(int value);

int tokenValue(const std::string &name);

bool tokenValid(int token);

bool tokenValid(const std::string &name);

bool tokenIsLiteral(int value);
