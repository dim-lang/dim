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
  std::string _1;
  return fmt::format("[ @OperatorToken id_:{}, type_:{}, value_:{} ]", id_,
                     Token::tokenTypeName(type_), value_.toUTF8String(_1));
}

} // namespace fastype
