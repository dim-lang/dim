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
  if (tokenType != token_->type()) {
    F_THROW(ParseException, "invlid token type: {}, token_->type: {}",
            tokenType, token_->type());
  }
  token_ = lexer_->read();
}

void Parser::eat(Sptr<Token> token) {
  F_CHECK(token.get(), "token {} is not null", (void *)token.get());
  if (!token.get()) {
    F_THROW(ParseException, "token pointer must not null: {}",
            (void *)token.get());
  }
  token_ = lexer_->read();
}

Ast *Parser::term() {
  Ast *node = factor();
  while (Token::T_MUL->equal(token_) || Token::T_DIV->equal(token_) ||
         token_ == Token::T_MOD) {
    Sptr<Token> t = token_;
    eat(token_);
    node = new BinaryOp(node, t, factor());
  }
  return node;
}

Ast *Parser::factor() {
  Sptr<Token> token = token_;
  if (token == Token::T_ADD) {
    eat(Token::T_ADD);
    Ast *node = new UnaryOp(token, factor());
    return node;
  } else if (token == Token::T_SUB) {
    eat(Token::T_ADD);
    Ast *node = new UnaryOp(token, factor());
    return node;
  } else if (token->type() == Token::TokenType::TT_INTEGER) {
    eat(Token::TokenType::TT_INTEGER);
    return new IntegerConstant(token);
  } else if (token == Token::T_LP) {
    eat(Token::T_LP);
    Ast *node = expr();
    eat(Token::T_RP);
    return node;
  }
  F_CHECK(false, "Parser::factor must not be here, token: {}",
          token->toString());
  F_THROW(ParseException, "invlid token: {}", token->toString());
  return nullptr;
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
