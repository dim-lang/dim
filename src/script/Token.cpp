// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Token.h"
#include "Logging.h"
#include "script/token/EofToken.h"
#include <string>

namespace fastype {

const Token Token::T_EOF(-1, "", Token::TokenType::TT_EOF);

Token::Token(int lineNumber, Cowstr literal, TokenType type)
    : lineNumber_(lineNumber), literal_(literal), type_(type) {
  F_INFO("Constructor:{}", toString());
}

bool Token::operator==(const Token &t) const {
  return literal_ == t.literal_ && type_ == t.type_;
}

bool Token::operator!=(const Token &t) const {
  return literal_ != t.literal_ || type_ != t.type_;
}

const int &Token::lineNumber() const { return lineNumber_; }

const TokenType &Token::type() const { return type_; }

const Cowstr &TokenType::literal() const { return literal_; }

bool Token::isEof() const { return type_ == Token::TokenType::TT_EOF; }

bool Token::isEol() const { return type_ == Token::TokenType::TT_EOL; }

bool Token::isKeyword() const { return type_ == Token::TokenType::TT_KEYWORD; }

bool Token::isOperator() const {
  return type_ == Token::TokenType::TT_OPERATOR;
}

bool Token::isString() const { return type_ == Token::TokenType::TT_STRING; }

bool Token::isBoolean() const { return type_ == Token::TokenType::TT_BOOLEAN; }

bool Token::isInteger() const { return type_ == Token::TokenType::TT_INTEGER; }

bool Token::isFloating() const {
  return type_ == Token::TokenType::TT_FLOATING;
}

bool Token::isPunctuation() const {
  return type_ == Token::TokenType::TT_PUNCTUATION;
}

bool Token::isComment() const { return type_ == TT_COMMENT; }

bool Token::isIdentifier() const { return type_ == TT_IDENTIFIER; }

const Cowstr &Token::text() const { return literal_; }

int64_t Token::integer() const { return; }

float Token::floating() const { return; }

bool Token::boolean() const { return literal_ == "True"; }

} // namespace fastype
