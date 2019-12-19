// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/StringToken.h"

namespace fastype {

StringToken::StringToken(const icu::UnicodeString &value)
    : Token(Type::T_STRING), value_(value) {}

icu::UnicodeString StringToken::literal() const { return value_; }

bool StringToken::equal(const std::shared_ptr<Token> &t) const {
  return t.get() && t->isString() && literal() == t->literal();
}

std::string StringToken::toString() const {
  std::string _1;
  return fmt::format("[ @StringToken type_:{}, value_:{} ]", type_.nameUTF8(),
                     value_.toUTF8String(_1));
}

} // namespace fastype
