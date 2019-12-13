// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/OperatorToken.h"
#include <fmt/format.h>

namespace fastype {

OperatorToken::OperatorToken(const icu::UnicodeString &value)
    : Token(TokenType::TT_OPERATOR), value_(value) {}

icu::UnicodeString OperatorToken::literal() const { return value_; }

bool OperatorToken::equal(const std::shared_ptr<Token> &t) const {
  return t.get() && t->isOperator() && literal() == t->literal();
}

std::string OperatorToken::toString() const {
  std::string _1, _2;
  return fmt::format("[ @OperatorToken id_:{}, type_:{}, value_:{} ]", id_,
                     Token::tokenTypeName(type_).toUTF8String(_1),
                     value_.toUTF8String(_2));
}

} // namespace fastype
