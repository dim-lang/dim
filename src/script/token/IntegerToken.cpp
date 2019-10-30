// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/IntegerToken.h"
#include <fmt/format.h>

namespace fastype {

IntegerToken::IntegerToken(long long value, int size)
    : Token(TokenType::TT_INTEGER), value_(value), size_(size) {}

int IntegerToken::size() const { return size_; }

long long IntegerToken::integer() const { return value_; }

std::string IntegerToken::toString() const {
  return fmt::format("[ @IntegerToken id_:{}, type_:{}, value_:{}, size_:{} ]",
                     id_, Token::tokenTypeName(type_), value_, size_);
}

} // namespace fastype
