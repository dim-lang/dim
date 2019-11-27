// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/StringToken.h"
#include "exception/NotFoundException.h"
#include <unordered_set>

namespace fastype {

StringToken::StringToken(const icu::UnicodeString &value)
    : Token(TokenType::TT_STRING), value_(value) {}

icu::UnicodeString StringToken::literal() const { return value_; }

bool StringToken::equal(const Sptr<Token> &t) const {
  return t.get() && t->isString() && literal() == t->literal();
}

std::string StringToken::toString() const {
  std::string _1;
  return fmt::format("[ @StringToken id_:{}, type_:{}, value_:{} ]", id_,
                     Token::tokenTypeName(type_), value_.toUTF8String(_1));
}

} // namespace fastype
