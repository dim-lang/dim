// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/StatementList.h"
#include "script/Util.h"

namespace fastype {

StatementList::StatementList(const std::vector<std::shared_ptr<Ast>> &children)
    : children_(children) {}

StatementList::~StatementList() { children_.clear(); }

int StatementList::size() const { return children_.size(); }

std::shared_ptr<Ast> StatementList::get(int i) const { return children_[i]; }

std::string StatementList::toString() const {
  return detail::astVectorToString(children_, "StatementList");
}

int StatementList::type() const { return F_TYPE_STATEMENT_LIST; }

} // namespace fastype
