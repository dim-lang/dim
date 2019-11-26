// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/VariableDeclaration.h"
#include <fmt/format.h>

namespace fastype {

VariableDeclaration::VariableDeclaration(Sptr<Token> token) : token_(token) {}

icu::UnicodeString VariableDeclaration::value() const {
  return token_->literal();
}

std::string VariableDeclaration::toString() const {
  return fmt::format("[ @VariableDeclaration token_:{} ]", token_->toString());
}

Ast::AstType VariableDeclaration::type() const {
  return Ast::AstType::VARIABLE;
}

} // namespace fastype
