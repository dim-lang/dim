// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/EofToken.h"
#include <fmt/format.h>

namespace fastype {

EofToken::EofToken() : Token(TokenType::TT_EOF) {}

std::string EofToken::toString() const {
  std::string _1;
  return fmt::format("[ @EofToken type_:{} ]", type_.name().toUTF8String(_1));
}

} // namespace fastype
