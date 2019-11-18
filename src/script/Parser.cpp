// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Parser.h"
#include "Logging.h"
#include "exception/ParseException.h"
#include "script/token/BooleanToken.h"
#include "script/token/EofToken.h"
#include "script/token/FloatingToken.h"
#include "script/token/IdentifierToken.h"
#include "script/token/IntegerToken.h"
#include "script/token/OperatorToken.h"
#include "script/token/StringToken.h"
#include <cstdlib>
#include <cstring>
#include <fmt/format.h>
#include <regex>
#include <string>

namespace fastype {

Parser::Parser(Sptr<Lexer> lexer) : token_(null), lexer_(lexer) {
  token_ = lexer_->read();
}

void Parser::eat(Token::TokenType tokenType) {
  F_CHECK(tokenType == token_->type(), "tokenType {} == token_#type {}",
          tokenType, token_->toString());
  token_ = lexer_->read();
}

Sptr<AstTree> Parser::term() {
  Sptr<AstTree> node = factor();
  while (token_ == Token::T_MUL || token_ == Token::T_DIV ||
         token_ == Token::T_MOD) {
    Sptr<Token> t = token_;
    eat(Token::TokenType::TT_OPERATOR);
    node = new BinaryOp(node, t, factor());
  }
  return node;
}

Sptr<AstTree> Parser::factor() {
  Sptr<Token> t = token_;
  if (t.type() == TT_INTEGER) {
    eat(TT_INTEGER);
    return new Num(t);
  } else if (t.type() == TT_LPAREN) {
    eat(TT_LPAREN);
    Ast *node = expr();
    eat(TT_RPAREN);
    return node;
  }
}

std::string Parser::toString() const {
  return fmt::format("[ @Parser token_:{}, lexer_:{} ]", token_->toString(),
                     lexer_->toString());
}

} // namespace fastype
