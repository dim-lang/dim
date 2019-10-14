// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/KeywordToken.h"

namespace fastype {

KeywordToken::KeywordToken(int lineNumber, Cowstr literal)
    : Token(lineNumber, TokenType::TT_KEYWORD), literal_(literal) {}

Cowstr KeywordToken::literal() const { return literal_; }

std::string KeywordToken::toString() const {
  return fmt::format("[ @KeywordToken lineNumber_:{}, literal_:{}, type_:{} ]",
                     lineNumber_, literal_.stdstr(), type_.name());
}

} // namespace fastype
