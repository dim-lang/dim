// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/EolToken.h"
#include <fmt/format.h>

namespace fastype {

EolToken::EolToken(int lineNumber, Cowstr literal)
    : Token(lineNumber, TokenType::TT_EOL), literal_(literal) {}

Cowstr EolToken::literal() const { return literal_; }

std::string EolToken::toString() const {
  return fmt::format("[ @EolToken lineNumber_:{}, literal_:{}, type:_{} ]",
                     lineNumber_, literal_.stdstr(), type_.name());
}

} // namespace fastype
