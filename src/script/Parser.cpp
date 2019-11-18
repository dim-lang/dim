// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Parser.h"
#include "Logging.h"
#include "exception/ParseException.h"
#include "script/ast/BinaryOp.h"
#include "script/ast/IntegerConstant.h"
#include "script/ast/UnaryOp.h"
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

Parser::Parser(Sptr<Lexer> lexer) : token_(nullptr), lexer_(lexer) {
  token_ = lexer_->read();
}

void Parser::eat(Token::TokenType tokenType) {
  F_CHECK(tokenType == token_->type(), "tokenType {} == token_#type {}",
          tokenType, token_->toString());
  token_ = lexer_->read();
}

void Parser::eat(Sptr<Token> token) {
  F_CHECK(token.get(), "token {} is null", (void *)token.get());
  token_ = lexer_->read();
}

Ast *Parser::term() {
  Ast *node = factor();
  while (token_ == Token::T_MUL || token_ == Token::T_DIV ||
         token_ == Token::T_MOD) {
    Sptr<Token> t = token_;
    eat(token_);
    node = new BinaryOp(node, t, factor());
  }
  return node;
}

Ast *Parser::factor() {
  Sptr<Token> node = token_;
  if (node->type() == Token::TokenType::TT_INTEGER) {
    eat(Token::TokenType::TT_INTEGER);
    return new IntegerConstant(node);
  } else if (node == Token::T_LP) {
    eat(Token::T_LP);
    Ast *t = expr();
    eat(Token::T_RP);
    return t;
  }
  F_CHECK(false, "Parser::factor must not be here, node: {}", node->toString());
}

Ast *Parser::expr() {
  Ast *node = term();
  while (token_ == Token::T_ADD || token_ == Token::T_SUB) {
    Sptr<Token> t = token_;
    eat(t);
    node = new BinaryOp(node, t, term());
  }
  return node;
}

Ast *Parser::parse() { return expr(); }

std::string Parser::toString() const {
  return fmt::format("[ @Parser token_:{}, lexer_:{} ]", token_->toString(),
                     lexer_->toString());
}

} // namespace fastype
