// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/TokenType.h"

namespace fastype {

static TokenType TokenType::TT_EOF(1, UNICODE_STRING_SIMPLE("EOF"));
static TokenType TokenType::TT_OPERATOR(2, UNICODE_STRING_SIMPLE("OPERATOR"));
static TokenType TokenType::TT_BOOLEAN(3, UNICODE_STRING_SIMPLE("BOOLEAN"));
static TokenType TokenType::TT_INTEGER(4, UNICODE_STRING_SIMPLE("INTEGER"));
static TokenType TokenType::TT_COMPARATOR(5,
                                          UNICODE_STRING_SIMPLE("COMPARATOR"));
static TokenType TokenType::TT_ASSIGNMENT(6,
                                          UNICODE_STRING_SIMPLE("ASSIGNMENT"));

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

const icu::UnicodeString &TokenType::name() const { return name_; }

int TokenType::value() const { return value_; }

std::string TokenType::toString() const {
  std::string utf8;
  return fmt::format("[ @TokenType value_:{}, name_:{} ]", value_,
                     name_.toUTF8String(utf8));
}

TokenType TokenType::fromName(const icu::UnicodeString &name) {
  for (const TokenType &tt : types()) {
    if (name == tt.name()) {
      return tt;
    }
  }
  std::string utf8;
  F_THROW(NotFoundException, "TokenType not found! name:{}",
          name.toUTF8String(utf8));
}

TokenType TokenType::fromValue(int value) {
  for (const TokenType &tt : types()) {
    if (tt.value() == value) {
      return tt;
    }
  }
  F_THROW(NotFoundException, "TokenType not found! value:{}", value);
}

static const std::vector<TokenType> &TokenType::types() {
  const static std::vector<TokenType> types = {
      TokenType::TT_EOF,     TokenType::TT_OPERATOR,   TokenType::TT_BOOLEAN,
      TokenType::TT_INTEGER, TokenType::TT_COMPARATOR, TokenType::TT_ASSIGNMENT,
  };
  return types;
}

TokenType::TokenType(int value, const icu::UnicodeString &name)
    : value_(value), name_(name) {}

} // namespace fastype
