// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/TokenType.h"
#include "exception/NotFoundException.h"
#include <fmt/format.h>

namespace fastype {

const TokenType TokenType::TT_EOF(1, UNICODE_STRING_SIMPLE("EOF"));
const TokenType TokenType::TT_OPERATOR(2, UNICODE_STRING_SIMPLE("OPERATOR"));
const TokenType TokenType::TT_BOOLEAN(3, UNICODE_STRING_SIMPLE("BOOLEAN"));
const TokenType TokenType::TT_INTEGER(4, UNICODE_STRING_SIMPLE("INTEGER"));
const TokenType TokenType::TT_COMPARATOR(5,
                                         UNICODE_STRING_SIMPLE("COMPARATOR"));
const TokenType TokenType::TT_ASSIGNMENT(6,
                                         UNICODE_STRING_SIMPLE("ASSIGNMENT"));

bool TokenType::operator==(const TokenType &t) const { return compare(t) == 0; }

bool TokenType::operator!=(const TokenType &t) const { return compare(t) != 0; }

bool TokenType::operator<(const TokenType &t) const { return compare(t) < 0; }

bool TokenType::operator<=(const TokenType &t) const { return compare(t) <= 0; }

bool TokenType::operator>(const TokenType &t) const { return compare(t) > 0; }

bool TokenType::operator>=(const TokenType &t) const { return compare(t) >= 0; }

int TokenType::compare(const TokenType &t) const { return value_ - t.value_; }

const icu::UnicodeString &TokenType::name() const { return name_; }

int TokenType::value() const { return value_; }

TokenType TokenType::fromName(const icu::UnicodeString &name) {
  for (TokenType tt : types()) {
    if (name == tt.name()) {
      return tt;
    }
  }
  std::string utf8;
  F_THROW(NotFoundException, "TokenType not found! name:{}",
          name.toUTF8String(utf8));
}

TokenType TokenType::fromValue(int value) {
  for (TokenType tt : types()) {
    if (tt.value() == value) {
      return tt;
    }
  }
  F_THROW(NotFoundException, "TokenType not found! value:{}", value);
}

const std::vector<TokenType> &TokenType::types() {
  const static std::vector<TokenType> types = {
      TokenType::TT_EOF,     TokenType::TT_OPERATOR,   TokenType::TT_BOOLEAN,
      TokenType::TT_INTEGER, TokenType::TT_COMPARATOR, TokenType::TT_ASSIGNMENT,
  };
  return types;
}

TokenType::TokenType(int value, const icu::UnicodeString &name)
    : value_(value), name_(name) {}

} // namespace fastype
