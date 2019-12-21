// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/OperatorToken.h"

namespace fastype {

OperatorToken::OperatorToken(const icu::UnicodeString &value)
    : Token(F_TYPE_OPERATOR), value_(value) {}

icu::UnicodeString OperatorToken::literal() const { return value_; }

bool OperatorToken::equal(const std::shared_ptr<Token> &t) const {
  return t && t->isOperator() && literal() == t->literal();
}

std::string OperatorToken::toString() const {
  std::string _1;
  return fmt::format("[ @OperatorToken type_:{}, value_:{} ]",
                     Type::nameUTF8(type_), value_.toUTF8String(_1));
}

} // namespace fastype
