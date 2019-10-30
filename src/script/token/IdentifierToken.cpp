// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/IdentifierToken.h"
#include <fmt/format.h>

namespace fastype {

IdentifierToken::IdentifierToken(const icu::UnicodeString &value)
    : Token(TokenType::TT_IDENTIFIER), value_(value) {}

int IdentifierToken::size() const { return value_.length(); }

icu::UnicodeString IdentifierToken::literal() const { return value_; }

std::string IdentifierToken::toString() const {
  std::string _1;
  return fmt::format("[ @IdentifierToken id_:{}, type_:{}, value_:{} ]", id_,
                     Token::tokenTypeName(type_), value_.toUTF8String(_1));
}

} // namespace fastype
