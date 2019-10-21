// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/AssignmentToken.h"
#include <unordered_set>

namespace fastype {

AssignmentToken::AssignmentToken(const icu::UnicodeString &value)
    : Token(TokenType::TT_ASSIGNMENT), value_(value) {}

icu::UnicodeString AssignmentToken::literal() const { return value_; }

std::string AssignmentToken::toString() const {
  std::string _1;
  std::string _2;
  return fmt::format("[ @AssignmentToken type_:{}, value_:{} ]",
                     type_.name().toUTF8String(_1), value_.toUTF8String(_2));
}

} // namespace fastype
