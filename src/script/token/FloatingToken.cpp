// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/FloatingToken.h"

namespace fastype {

FloatingToken::FloatingToken(double value)
    : Token(F_TYPE_FLOATING), value_(value) {}

double FloatingToken::floating() const { return value_; }

bool FloatingToken::equal(const std::shared_ptr<Token> &t) const {
  return t && t->isFloating() && floating() == t->floating();
}

std::string FloatingToken::toString() const {
  return fmt::format("[ @FloatingToken type_:{}, value_:{} ]",
                     Type::nameUTF8(type_), value_);
}

} // namespace fastype
