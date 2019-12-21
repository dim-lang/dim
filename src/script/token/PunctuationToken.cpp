// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/PunctuationToken.h"

namespace fastype {

PunctuationToken::PunctuationToken(const icu::UnicodeString &value)
    : Token(F_TYPE_PUNCTUATION), value_(value) {}

icu::UnicodeString PunctuationToken::literal() const { return value_; }

bool PunctuationToken::equal(const std::shared_ptr<Token> &t) const {
  return t && t->isPunctuation() && literal() == t->literal();
}

std::string PunctuationToken::toString() const {
  std::string _1;
  return fmt::format("[ @PunctuationToken type_:{}, value_:{} ]",
                     Type::nameUTF8(type_), value_.toUTF8String(_1));
}

} // namespace fastype
