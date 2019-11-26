// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/Program.h"
#include <fmt/format.h>

namespace fastype {

Program::Program(const std::vector<Ast *> &children)
    : children_(children.size()) {
  for (int i = 0; i < children.size(); i++) {
    children_.push_back(children[i]);
  }
}

int Program::size() const { return children_.size(); }

Ast *Program::get(int i) const { return children_[i]; }

std::string Program::toString() const {
  return fmt::format("[ @Program children_#size: {} ]", children_.size());
}

Ast::AstType Program::type() const { return Ast::AstType::STATEMENT_LIST; }

} // namespace fastype
