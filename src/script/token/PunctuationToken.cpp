// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/PunctuationToken.h"
#include "exception/NotFoundException.h"
#include <unordered_set>

namespace fastype {

PunctuationToken::PunctuationToken(int lineNumber, Cowstr literal)
    : Token(lineNumber, TokenType::TT_PUNCTUATION), literal_(literal) {}

Cowstr PunctuationToken::literal() const { return literal_; }

std::string PunctuationToken::toString() const {
  return fmt::format(
      "[ @PunctuationToken lineNumber_:{}, literal_:{}, type_:{} ]",
      lineNumber_, literal_.stdstr(), type_.name());
}

} // namespace fastype
