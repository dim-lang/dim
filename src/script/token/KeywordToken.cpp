// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/KeywordToken.h"

namespace fastype {

KeywordToken::KeywordToken(const icu::UnicodeString &value)
    : Token(TokenType::TT_KEYWORD), value_(value) {}

icu::UnicodeString KeywordToken::literal() const { return value_; }

bool KeywordToken::equal(const std::shared_ptr<Token> &t) const {
  return t.get() && t->isKeyword() && literal() == t->literal();
}

std::string KeywordToken::toString() const {
  std::string _1, _2;
  return fmt::format("[ @KeywordToken id_:{}, type_:{}, value_:{} ]", id_,
                     Token::tokenTypeName(type_).toUTF8String(_1),
                     value_.toUTF8String(_2));
}

} // namespace fastype
