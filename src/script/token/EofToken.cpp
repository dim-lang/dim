// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/EofToken.h"
#include <fmt/format.h>

namespace fastype {

std::string EofToken::toString() const {
  return fmt::format("[ @EofToken lineNumber_:{} ]", lineNumber_);
}

EofToken::EofToken(int line) : Token(line) {}

} // namespace fastype
