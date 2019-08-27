// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/NumberToken.h"
#include <fmt/format.h>

namespace fastype {

bool NumberToken::isNumber() const { return true; }

int NumberToken::number() const { return value_; }

std::string NumberToken::text() const { return std::to_string(value_); }

std::string NumberToken::toString() const {
  return fmt::format("[ @NumberToken lineNumber_:{} value_:{} ]", lineNumber_,
                     value_);
}

NumberToken::NumberToken(int line, int64_t value)
    : Token(line), value_(value) {}

} // namespace fastype
