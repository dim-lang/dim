// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/IntegerToken.h"
#include <fmt/format.h>

namespace fastype {

IntegerToken::IntegerToken(long long value)
    : Token(TokenType::TT_INTEGER), value_(value) {}

long long IntegerToken::integer() const { return value_; }

bool IntegerToken::equal(const std::shared_ptr<Token> &t) const {
  return t.get() && t->isInteger() && integer() == t->integer();
}

std::string IntegerToken::toString() const {
  return fmt::format("[ @IntegerToken id_:{}, type_:{}, value_:{} ]", id_,
                     Token::tokenTypeName(type_), value_);
}

} // namespace fastype
