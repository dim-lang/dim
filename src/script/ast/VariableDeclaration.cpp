// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/VariableDeclaration.h"
#include <fmt/format.h>

namespace fastype {

VariableDeclaration::VariableDeclaration(const std::vector<Sptr<Ast>> &children)
    : children_(children) {}

int VariableDeclaration::size() { return children_.size(); }

Sptr<Ast> VariableDeclaration::get(int i) { return children_[i]; }

std::string VariableDeclaration::toString() const {
  return ast::AstVectortoString(children_, "VariableDeclaration");
}

Ast::AstType VariableDeclaration::type() const {
  return Ast::AstType::VARIABLE_DECLARATION;
}

} // namespace fastype
