// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/TokenType.h"

namespace fastype {

static TokenType TokenType::TT_EOF(100, "EOF");
static TokenType TokenType::TT_EOL(200, "EOL");
static TokenType TokenType::TT_KEYWORD(300, "KEYWORD");
static TokenType TokenType::TT_OPERATOR(400, "OPERATOR");
static TokenType TokenType::TT_STRING(500, "STRING");
static TokenType TokenType::TT_BOOLEAN(600, "BOOLEAN");
static TokenType TokenType::TT_INTEGER(700, "INTEGER");
static TokenType TokenType::TT_FLOATING(800, "FLOATING");
static TokenType TokenType::TT_PUNCTUATION(900, "PUNCTUATION");
static TokenType TokenType::TT_COMMENT(1000, "COMMENT");
static TokenType TokenType::TT_IDENTIFIER(1100, "IDENTIFIER");

bool TokenType::operator==(const TokenType &t) const {
  return value_ == t.value_;
}

bool TokenType::operator!=(const TokenType &t) const {
  return value_ != t.value_;
}

bool TokenType::operator<(const TokenType &t) const {
  return value_ < t.value_;
}

bool TokenType::operator<=(const TokenType &t) const {
  return value_ <= t.value_;
}

bool TokenType::operator>(const TokenType &t) const {
  return value_ > t.value_;
}

bool TokenType::operator>=(const TokenType &t) const {
  return value_ >= t.value_;
}

std::string TokenType::name() const { return name_; }

int TokenType::value() const { return value_; }

TokenType TokenType::fromName(const std::string &name) {
  for (auto tt : all()) {
    if (name == tt.name()) {
      return tt;
    }
  }
  F_THROW(NotFoundException, "TokenType not found! name:{}", name);
}

TokenType TokenType::fromName(Cowstr name) {
  for (auto tt : all()) {
    if (name == tt.name()) {
      return tt;
    }
  }
  F_THROW(NotFoundException, "TokenType not found! Cowstr name:{}",
          name.toString());
}

TokenType TokenType::fromValue(int value) {
  for (auto tt : all()) {
    if (tt.value() == value) {
      return tt;
    }
  }
  F_THROW(NotFoundException, "TokenType not found! value:{}", value);
}

static const std::unordered_set<TokenType> &TokenType::all() {
  const static std::unordered_set<TokenType> types = {
      TokenType::TT_EOF,         TokenType::TT_EOL,
      TokenType::TT_KEYWORD,     TokenType::TT_OPERATOR,
      TokenType::TT_STRING,      TokenType::TT_BOOLEAN,
      TokenType::TT_INTEGER,     TokenType::TT_FLOATING,
      TokenType::TT_PUNCTUATION, TokenType::TT_COMMENT,
      TokenType::TT_IDENTIFIER};
  return types;
}

TokenType::TokenType(int value, const std::string &name)
    : value_(value), name_(name) {}

} // namespace fastype
