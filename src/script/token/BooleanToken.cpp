// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/BooleanToken.h"

namespace fastype {

BooleanToken::BooleanToken(bool value)
    : Token(TokenType::TT_BOOLEAN), value_(value) {}

bool BooleanToken::boolean() const { return value_; }

bool BooleanToken::equal(const std::shared_ptr<Token> &t) const {
  return t.get() && t->isBoolean() &&
         std::static_pointer_cast<BooleanToken>(t)->boolean() == value_;
}

std::string BooleanToken::toString() const {
  std::string _1;
  return fmt::format("[ @BooleanToken id_:{}, type_:{}, value_:{} ]", id_,
                     Token::tokenTypeName(type_).toUTF8String(_1), value_);
}

} // namespace fastype
