// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/IdToken.h"
#include <fmt/format.h>

namespace fastype {

bool IdToken::isIdentifier() const { return true; }

std::string IdToken::text() const { return value_; }

std::string IdToken::toString() const {
  return fmt::format("[ @IdToken lineNumber_:{} value_:{} ]", lineNumber_,
                     value_);
}

IdToken::IdToken(int line, const std::string &value)
    : Token(line), value_(value) {}

} // namespace fastype
