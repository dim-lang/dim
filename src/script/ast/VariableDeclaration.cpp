// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/VariableDeclaration.h"
#include "script/Util.h"

namespace fastype {

VariableDeclaration::VariableDeclaration(
    const std::vector<std::shared_ptr<Ast>> &children)
    : children_(children) {}

VariableDeclaration::~VariableDeclaration() { children_.clear(); }

int VariableDeclaration::size() const { return children_.size(); }

std::shared_ptr<Ast> VariableDeclaration::get(int i) const {
  return children_[i];
}

std::string VariableDeclaration::toString() const {
  return detail::astVectorToString(children_, "VariableDeclaration");
}

Ast::AstType VariableDeclaration::type() const {
  return Ast::AstType::VARIABLE_DECLARATION;
}

} // namespace fastype
