// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/EofToken.h"

namespace fastype {

EofToken::EofToken() : Token(F_TYPE_EOF) {}

bool EofToken::equal(const std::shared_ptr<Token> &t) const {
  return t && t->isEof();
}

std::string EofToken::toString() const {
  return fmt::format("[ @EofToken type_:{} ]", Type::nameUTF8(type_));
}

} // namespace fastype
