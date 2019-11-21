// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/KeywordToken.h"
#include "exception/NotFoundException.h"
#include <unordered_set>

namespace fastype {

KeywordToken::KeywordToken(const icu::UnicodeString &value)
    : Token(TokenType::TT_KEYWORD), value_(value) {}

icu::UnicodeString KeywordToken::literal() const { return value_; }

bool KeywordToken::equal(const Sptr<Token> t) const {
  return t.get() && t->isKeyword() && literal() == t->literal();
}

std::string KeywordToken::toString() const {
  std::string _1;
  return fmt::format("[ @KeywordToken id_:{}, type_:{}, value_:{} ]", id_,
                     Token::tokenTypeName(type_), value_.toUTF8String(_1));
}

} // namespace fastype
