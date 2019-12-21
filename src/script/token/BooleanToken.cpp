// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/BooleanToken.h"

namespace fastype {

BooleanToken::BooleanToken(bool value) : Token(F_TYPE_BOOLEAN), value_(value) {}

bool BooleanToken::boolean() const { return value_; }

bool BooleanToken::equal(const std::shared_ptr<Token> &t) const {
  return t && t->isBoolean() &&
         std::static_pointer_cast<BooleanToken>(t)->boolean() == value_;
}

std::string BooleanToken::toString() const {
  return fmt::format("[ @BooleanToken type_:{}, value_:{} ]",
                     Type::nameUTF8(type_), value_);
}

} // namespace fastype
