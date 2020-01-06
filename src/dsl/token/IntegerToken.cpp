// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/IntegerToken.h"

namespace fastype {

IntegerToken::IntegerToken(int64_t value)
    : Token(F_TYPE_INTEGER), value_(value) {}

int64_t IntegerToken::integer() const { return value_; }

bool IntegerToken::equal(const std::shared_ptr<Token> &t) const {
  return t && t->isInteger() && integer() == t->integer();
}

std::string IntegerToken::toString() const {
  return fmt::format("[ @IntegerToken type_:{}, value_:{} ]",
                     Type::nameUTF8(type_), value_);
}

} // namespace fastype
