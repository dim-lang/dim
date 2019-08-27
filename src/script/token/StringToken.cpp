// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/StringToken.h"
#include <fmt/format.h>

namespace fastype {

bool StringToken::isString() const { return true; }

std::string StringToken::text() const { return value_; }

std::string StringToken::toString() const {
  return fmt::format("[ @StringToken lineNumber_:{} value_:{} ]", lineNumber_,
                     value_);
}

StringToken::StringToken(int line, const std::string &value)
    : Token(line), value_(value) {}

} // namespace fastype
