// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Token.h"
#include "Logging.h"
#include "exception/NotFoundException.h"
#include "exception/NotImplmentException.h"
#include "script/token/EofToken.h"
#include <fmt/format.h>
#include <string>

namespace fastype {

static long long TokenId = 0LL;

Token::Token(int lineNumber, TokenType type)
    : lineNumber_(lineNumber), type_(type), id_(TokenId++) {
  F_INFO("Constructor:{}", toString());
}

const int &Token::lineNumber() const { return lineNumber_; }

const TokenType &Token::type() const { return type_; }

long long Token::id() const { return id_; }

bool Token::isEof() const { return type_ == TokenType::T_EOF; }

bool Token::isEol() const { return type_ == TokenType::T_EOL; }

bool Token::isKeyword() const { return type_ == TokenType::T_KEYWORD; }

bool Token::isOperator() const { return type_ == TokenType::T_OPERATOR; }

bool Token::isString() const { return type_ == TokenType::T_STRING; }

bool Token::isBoolean() const { return type_ == TokenType::T_BOOLEAN; }

bool Token::isInteger() const { return type_ == TokenType::T_INTEGER; }

bool Token::isFloating() const { return type_ == TokenType::T_FLOATING; }

bool Token::isPunctuation() const { return type_ == TokenType::T_PUNCTUATION; }

bool Token::isComment() const { return type_ == TokenType::T_COMMENT; }

bool Token::isIdentifier() const { return type_ == TokenType::T_IDENTIFIER; }

Cowstr Token::literal() const {
  F_THROW("literal not implement! {}", toString());
}

long long Token::integer() const {
  F_THROW("integer not implement! {}", toString());
}

float Token::floating() const {
  F_THROW("floating not implement! {}", toString());
}

bool Token::boolean() const {
  F_THROW("boolean not implement! {}", toString());
}

} // namespace fastype
