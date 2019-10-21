// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/StringToken.h"
#include "exception/NotFoundException.h"
#include <unordered_set>

namespace fastype {

StringToken::StringToken(const icu::UnicodeString &value)
    : Token(TokenType::TT_OPERATOR), value_(value) {}

icu::UnicodeString StringToken::literal() const { return value_; }

std::string StringToken::toString() const {
  std::string _1, _2;
  return fmt::format("[ @StringToken type_:{}, value_:{} ]",
                     type_.name().toUTF8String(_1), value_.toUTF8String(_2));
}

} // namespace fastype
