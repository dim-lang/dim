// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/PunctuationToken.h"
#include "exception/NotFoundException.h"
#include <unordered_set>

namespace fastype {

PunctuationToken::PunctuationToken(const icu::UnicodeString &value)
    : Token(TokenType::TT_PUNCTUATION), value_(value) {}

icu::UnicodeString PunctuationToken::literal() const { return value_; }

bool PunctuationToken::equal(const Sptr<Token> t) const {
  return t.get() && t->isPunctuation() && literal() == t->literal();
}

std::string PunctuationToken::toString() const {
  std::string _1;
  return fmt::format("[ @PunctuationToken id_:{}, type_:{}, value_:{} ]", id_,
                     Token::tokenTypeName(type_), value_.toUTF8String(_1));
}

} // namespace fastype
