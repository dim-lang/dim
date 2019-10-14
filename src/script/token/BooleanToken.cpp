// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/BooleanToken.h"
#include <fmt/format.h>

namespace fastype {

BooleanToken::BooleanToken(int lineNumber, bool value)
    : Token(lineNumber, TokenType::TT_BOOLEAN), value_(value) {}

bool BooleanToken::boolean() const { return value_; }

std::string BooleanToken::toString() const {
  return fmt::format("[ @BooleanToken lineNumber_:{}, value_:{}, type_:{} ]",
                     lineNumber_, value_, type_.name());
}

} // namespace fastype
