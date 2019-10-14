// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/OperatorToken.h"
#include "exception/NotFoundException.h"
#include <unordered_set>

namespace fastype {

OperatorToken::OperatorToken(int lineNumber, Cowstr literal,
                             OperatorTokenType ott)
    : Token(lineNumber, TokenType::TT_OPERATOR), literal_(literal), ott_(ott) {}

Cowstr OperatorToken::literal() const { return literal_; }

const OperatorTokenType &OperatorToken::operatorTokenType() const {
  return ott_;
}

std::string OperatorToken::toString() const {
  return fmt::format(
      "[ @OperatorToken lineNumber_:{}, literal_:{}, type_:{}, ott_:{} ]",
      lineNumber_, literal_.stdstr(), type_.name(), ott_.name());
}

} // namespace fastype
