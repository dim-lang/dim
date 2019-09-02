// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/BooleanToken.h"
#include <fmt/format.h>

namespace fastype {

bool BooleanToken::isBoolean() const { return true; }

bool BooleanToken::boolean() const { return value_; }

std::string BooleanToken::toString() const {
  return fmt::format("[ @BooleanToken lineNumber_:{} value_:{} ]", lineNumber_,
                     value_);
}

BooleanToken::BooleanToken(int line, bool value) : Token(line), value_(value) {}

} // namespace fastype
