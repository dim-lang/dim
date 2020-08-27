// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "TokenName.h"
#include "Log.h"
#include "parser.tab.hh"
#include <string>

std::string tokenName(int token) {
  LOG_ASSERT(token > yy::parser::symbol_kind::S_YYUNDEF &&
                 token < yy::parser::symbol_kind::YYNTOKENS,
             "S_YYUNDEF {} < token {} < YYNTOKENS {} error!",
             yy::parser::symbol_kind::S_YYUNDEF, token,
             yy::parser::symbol_kind::YYNTOKENS);
  yy::parser::symbol_type st(token, yy::location());
  return st.name();
}

bool tokenExist(int token) {
  return token > yy::parser::symbol_kind::S_YYUNDEF &&
         token < yy::parser::symbol_kind::YYNTOKENS;
}
