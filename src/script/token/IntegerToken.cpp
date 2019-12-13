// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/IntegerToken.h"

namespace fastype {

IntegerToken::IntegerToken(int64_t value)
    : Token(TokenType::TT_INTEGER), value_(value) {}

int64_t IntegerToken::integer() const { return value_; }

bool IntegerToken::equal(const std::shared_ptr<Token> &t) const {
  return t.get() && t->isInteger() && integer() == t->integer();
}

std::string IntegerToken::toString() const {
  std::string _1;
  return fmt::format("[ @IntegerToken id_:{}, type_:{}, value_:{} ]", id_,
                     Token::tokenTypeName(type_).toUTF8String(_1), value_);
}

} // namespace fastype
