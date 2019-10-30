// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/AssignmentToken.h"
#include <unordered_set>

namespace fastype {

AssignmentToken::AssignmentToken(const icu::UnicodeString &value)
    : Token(TokenType::TT_ASSIGNMENT), value_(value) {}

int AssignmentToken::size() const { return 1; }

icu::UnicodeString AssignmentToken::literal() const { return value_; }

std::string AssignmentToken::toString() const {
  std::string _1;
  return fmt::format("[ @AssignmentToken id_:{}, type_:{}, value_:{} ]", id_,
                     Token::tokenTypeName(type_), value_.toUTF8String(_1));
}

} // namespace fastype
