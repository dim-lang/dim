// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/EofToken.h"

namespace fastype {

EofToken::EofToken() : Token(TokenType::TT_EOF) {}

bool EofToken::equal(const std::shared_ptr<Token> &t) const {
  return t.get() && t->isEof();
}

std::string EofToken::toString() const {
  std::string _1;
  return fmt::format("[ @EofToken id_:{}, type_:{} ]", id_,
                     Token::tokenTypeName(type_).toUTF8String(_1));
}

} // namespace fastype
