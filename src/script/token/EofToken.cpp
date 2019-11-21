// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/EofToken.h"
#include <fmt/format.h>

namespace fastype {

EofToken::EofToken() : Token(TokenType::TT_EOF) {}

bool EofToken::equal(const Sptr<Token> t) const {
  return t.get() && t->isEof();
}

std::string EofToken::toString() const {
  return fmt::format("[ @EofToken id_:{}, type_:{} ]", id_,
                     Token::tokenTypeName(type_));
}

} // namespace fastype
