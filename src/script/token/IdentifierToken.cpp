// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/IdentifierToken.h"

namespace fastype {

IdentifierToken::IdentifierToken(const icu::UnicodeString &value)
    : Token(Type::T_IDENTIFIER), value_(value) {}

icu::UnicodeString IdentifierToken::literal() const { return value_; }

bool IdentifierToken::equal(const std::shared_ptr<Token> &t) const {
  return t.get() && t->isIdentifier() && literal() == t->literal();
}

std::string IdentifierToken::toString() const {
  std::string _1;
  return fmt::format("[ @IdentifierToken type_:{}, value_:{} ]",
                     type_.nameUTF8(), value_.toUTF8String(_1));
}

} // namespace fastype
