// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/StatementList.h"
#include "script/Util.h"

namespace fastype {

StatementList::StatementList(const std::vector<std::shared_ptr<Ast>> &children)
    : children_() {
  for (int i = 0; i < children.size(); i++) {
    children_.push_back(children[i]);
  }
}

StatementList::~StatementList() {
  for (int i = 0; i < children_.size(); i++) {
    children_[i].reset();
  }
}

int StatementList::size() const { return children_.size(); }

std::shared_ptr<Ast> StatementList::get(int i) const { return children_[i]; }

std::string StatementList::toString() const {
  return detail::astVectorToString(children_, "StatementList");
}

Ast::AstType StatementList::type() const {
  return Ast::AstType::STATEMENT_LIST;
}

} // namespace fastype
