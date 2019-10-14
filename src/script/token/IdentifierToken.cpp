// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/IdentifierToken.h"
#include <fmt/format.h>

namespace fastype {

IdentifierToken::IdentifierToken(int lineNumber, Cowstr literal)
    : Token(lineNumber, TokenType::TT_IDENTIFIER), literal_(literal) {}

Cowstr IdentifierToken::literal() const { return literal_; }

std::string IdentifierToken::toString() const {
  return fmt::format(
      "[ @IdentifierToken lineNumber_:{}, literal_:{}, type_:{} ]", lineNumber_,
      literal_.stdstr(), type_.name());
}

} // namespace fastype
