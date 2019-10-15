// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/EofToken.h"
#include <fmt/format.h>

namespace fastype {

EofToken::EofToken() : Token(TokenType::TT_EOF) {}

std::string EofToken::toString() const {
  return fmt::format("[ @EofToken type_:{} ]", type_.name());
}

} // namespace fastype
