// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/FloatingToken.h"
#include <fmt/format.h>

namespace fastype {

FloatingToken::FloatingToken(double value, int size)
    : Token(TokenType::TT_FLOATING), value_(value), size_(size) {}

int FloatingToken::size() const { return size_; }

double FloatingToken::floating() const { return value_; }

std::string FloatingToken::toString() const {
  return fmt::format("[ @FloatingToken id_:{}, type_:{}, value_:{}, size_:{} ]",
                     id_, Token::tokenTypeName(type_), value_, size_);
}

} // namespace fastype
