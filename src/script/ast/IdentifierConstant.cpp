// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/IdentifierConstant.h"
#include <fmt/format.h>

namespace fastype {

IdentifierConstant::IdentifierConstant(std::shared_ptr<Token> token)
    : token_(token) {}

icu::UnicodeString IdentifierConstant::value() const {
  return token_->literal();
}

std::string IdentifierConstant::toString() const {
  return fmt::format("[ @IdentifierConstant token_:{} ]", token_->toString());
}

Ast::AstType IdentifierConstant::type() const {
  return Ast::AstType::INTEGER_CONSTANT;
}

} // namespace fastype
