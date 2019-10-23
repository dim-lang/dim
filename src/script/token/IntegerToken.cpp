// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/IntegerToken.h"
#include <fmt/format.h>

namespace fastype {

IntegerToken::IntegerToken(long long value)
    : Token(TokenType::TT_INTEGER), value_(value) {}

long long IntegerToken::integer() const { return value_; }

std::string IntegerToken::toString() const {
  return fmt::format("[ @IntegerToken type_:{}, value_:{} ]",
                     Token::tokenTypeName(type_), value_);
}

} // namespace fastype
