// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/IdentifierConstant.h"

namespace fastype {

IdentifierConstant::IdentifierConstant(std::shared_ptr<Token> token)
    : token_(token) {}

icu::UnicodeString IdentifierConstant::value() const {
  return token_->literal();
}

std::string IdentifierConstant::toString() const {
  std::string _1 = token_ ? token_->toString() : "null";
  return fmt::format("[ @IdentifierConstant token_:{} ]", _1);
}

int IdentifierConstant::type() const { return F_TYPE_INTEGER_CONSTANT; }

} // namespace fastype
