// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "TokenName.h"
#include "Log.h"
#include "parser.tab.hh"
#include <string>

std::string tokenName(int token) {
  LOG_ASSERT(tokenExist(token), "token {} must exist", token);
  return yy::parser::symbol_type(token, yy::location()).name();
}

bool tokenExist(int token) {
  return token > yy::parser::token::YYUNDEF &&
         token < yy::parser::token::YYUNDEF + yy::parser::YYNTOKENS;
}
