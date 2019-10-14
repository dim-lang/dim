// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/EofToken.h"
#include <fmt/format.h>

namespace fastype {

EofToken::EofToken() : Token(-1, TokenType::TT_EOF) {}

std::string EofToken::toString() const {
  return fmt::format("[ @EofToken lineNumber_:{}, type_:{} ]", lineNumber_,
                     type_);
}

} // namespace fastype
