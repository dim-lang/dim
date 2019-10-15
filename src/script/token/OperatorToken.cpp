// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/OperatorToken.h"
#include "exception/NotFoundException.h"
#include <unordered_set>

namespace fastype {

OperatorToken::OperatorToken(const icu::UnicodeString &value)
    : Token(TokenType::TT_OPERATOR), value_(value) {}

icu::UnicodeString OperatorToken::literal() const { return value_; }

std::string OperatorToken::toString() const {
  return fmt::format("[ @OperatorToken type_:{}, value_:{} ]", type_.name(),
                     value_.stdstr());
}

} // namespace fastype
