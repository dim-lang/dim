// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/IntegerToken.h"
#include <fmt/format.h>

namespace fastype {

IntegerToken::IntegerToken(int lineNumber, long long value)
    : Token(lineNumber_, TokenType::TT_INTEGER), value_(value) {}

long long IntegerToken::integer() const { return value_; }

std::string IntegerToken::toString() const {
  return fmt::format("[ @IntegerToken lineNumber_:{}, value_:{}, type_:{} ]",
                     lineNumber_, value_, type_.name());
}

} // namespace fastype
