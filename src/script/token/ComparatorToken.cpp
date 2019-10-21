// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/ComparatorToken.h"
#include <unordered_set>

namespace fastype {

ComparatorToken::ComparatorToken(const icu::UnicodeString &value)
    : Token(TokenType::TT_COMPARATOR), value_(value) {}

icu::UnicodeString ComparatorToken::literal() const { return value_; }

std::string ComparatorToken::toString() const {
  std::string _1;
  std::string _2;
  return fmt::format("[ @ComparatorToken type_:{}, value_:{} ]",
                     type_.name().toUTF8String(_1), value_.toUTF8String(_2));
}

} // namespace fastype
