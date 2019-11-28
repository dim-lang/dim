// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/ComparatorToken.h"

namespace fastype {

ComparatorToken::ComparatorToken(const icu::UnicodeString &value)
    : Token(TokenType::TT_COMPARATOR), value_(value) {}

icu::UnicodeString ComparatorToken::literal() const { return value_; }

bool ComparatorToken::equal(const std::shared_ptr<Token> &t) const {
  return t.get() && t->isComparator() && t->literal() == literal();
}

std::string ComparatorToken::toString() const {
  std::string _1;
  return fmt::format("[ @ComparatorToken id_:{}, type_:{}, value_:{} ]", id_,
                     Token::tokenTypeName(type_), value_.toUTF8String(_1));
}

} // namespace fastype
