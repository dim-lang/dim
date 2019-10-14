// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/StringToken.h"
#include <fmt/format.h>

namespace fastype {

StringToken::StringToken(int lineNumber, Cowstr literal)
    : Token(lineNumber, TokenType::TT_STRING), literal_(literal) {}

Cowstr StringToken::literal() const { return literal_; }

std::string StringToken::toString() const {
  return fmt::format("[ @StringToken lineNumber_:{}, literal_:{}, type_:{} ]",
                     lineNumber_, literal_.stdstr(), type_.name);
}

} // namespace fastype
