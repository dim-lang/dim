// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "TokenName.h"
#include "Exception.h"
#include "parser.tab.hh"
#include <string>

std::string tokenName(int token) {
  EX_ASSERT(token > yy::parser::symbol_kind::S_YYUNDEF &&
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
