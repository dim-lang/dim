// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/FloatingToken.h"

namespace fastype {

FloatingToken::FloatingToken(double value)
    : Token(TokenType::TT_FLOATING), value_(value) {}

double FloatingToken::floating() const { return value_; }

bool FloatingToken::equal(const std::shared_ptr<Token> &t) const {
  return t.get() && t->isFloating() && floating() == t->floating();
}

std::string FloatingToken::toString() const {
  std::string _1;
  return fmt::format("[ @FloatingToken id_:{}, type_:{}, value_:{} ]", id_,
                     Token::tokenTypeName(type_).toUTF8String(_1), value_);
}

} // namespace fastype
