// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/KeywordToken.h"

namespace fastype {

KeywordToken::KeywordToken(const icu::UnicodeString &value)
    : Token(Type::TP_KEYWORD), value_(value) {}

icu::UnicodeString KeywordToken::literal() const { return value_; }

bool KeywordToken::equal(const std::shared_ptr<Token> &t) const {
  return t.get() && t->isKeyword() && literal() == t->literal();
}

std::string KeywordToken::toString() const {
  std::string _1;
  return fmt::format("[ @KeywordToken type_:{}, value_:{} ]", type_.nameUTF8(),
                     value_.toUTF8String(_1));
}

} // namespace fastype
