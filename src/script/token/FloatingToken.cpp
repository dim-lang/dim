// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/FloatingToken.h"
#include <fmt/format.h>

namespace fastype {

FloatingToken::FloatingToken(int lineNumber, float value)
    : lineNumber_(lineNumber, TokenType::TT_FLOATING), value_(value) {}

float FloatingToken::floating() const { return value_; }

std::string FloatingToken::toString() const {
  return fmt::format("[ @FloatingToken lineNumber_:{}, value_:{}, type_:{} ]",
                     lineNumber_, value_;
                     type_.name());
}

} // namespace fastype
