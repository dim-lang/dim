// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/token/AssignmentToken.h"
#include <fmt/format.h>

namespace fastype {

AssignmentToken::AssignmentToken(const icu::UnicodeString &value)
    : Token(TokenType::TT_ASSIGNMENT), value_(value) {}

icu::UnicodeString AssignmentToken::literal() const { return value_; }

bool AssignmentToken::equal(const std::shared_ptr<Token> &t) const {
  return t.get() && t->isAssignment() && t->literal() == literal();
}

std::string AssignmentToken::toString() const {
  std::string _1;
  return fmt::format("[ @AssignmentToken id_:{}, type_:{}, value_:{} ]", id_,
                     Token::tokenTypeName(type_), value_.toUTF8String(_1));
}

} // namespace fastype
