// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/ComparatorToken.h"
#include <unordered_set>

namespace fastype {

ComparatorToken::ComparatorToken(const icu::UnicodeString &value)
    : Token(TokenType::TT_COMPARATOR), value_(value) {}

icu::UnicodeString ComparatorToken::literal() const { return value_; }

std::string ComparatorToken::toString() const {
  std::string utf8;
  return fmt::format("[ @ComparatorToken type_:{}, value_:{} ]", type_.name(),
                     value_.toUTF8String(utf8));
}

} // namespace fastype