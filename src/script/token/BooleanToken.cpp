// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/BooleanToken.h"

namespace fastype {

BooleanToken::BooleanToken(bool value)
    : Token(Type::T_BOOLEAN), value_(value) {}

bool BooleanToken::boolean() const { return value_; }

bool BooleanToken::equal(const std::shared_ptr<Token> &t) const {
  return t.get() && t->isBoolean() &&
         std::static_pointer_cast<BooleanToken>(t)->boolean() == value_;
}

std::string BooleanToken::toString() const {
  return fmt::format("[ @BooleanToken type_:{}, value_:{} ]", type_.nameUTF8(),
                     value_);
}

} // namespace fastype
