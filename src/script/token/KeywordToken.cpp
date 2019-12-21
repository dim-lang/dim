// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/KeywordToken.h"

namespace fastype {

KeywordToken::KeywordToken(const icu::UnicodeString &value)
    : Token(F_TYPE_KEYWORD), value_(value) {}

icu::UnicodeString KeywordToken::literal() const { return value_; }

bool KeywordToken::equal(const std::shared_ptr<Token> &t) const {
  return t && t->isKeyword() && literal() == t->literal();
}

std::string KeywordToken::toString() const {
  std::string _1;
  return fmt::format("[ @KeywordToken type_:{}, value_:{} ]",
                     Type::nameUTF8(type_), value_.toUTF8String(_1));
}

} // namespace fastype
