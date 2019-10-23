// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/BooleanToken.h"
#include <fmt/format.h>

namespace fastype {

BooleanToken::BooleanToken(bool value)
    : Token(TokenType::TT_BOOLEAN), value_(value) {}

bool BooleanToken::boolean() const { return value_; }

std::string BooleanToken::toString() const {
  return fmt::format("[ @BooleanToken type_:{}, value_:{} ]",
                     Token::tokenTypeName(type_), value_ ? "True" : "False");
}

} // namespace fastype
