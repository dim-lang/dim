// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/ParameterList.h"

namespace fastype {

ParameterList::ParameterList(
    const std::vector<std::shared_ptr<Token>> tokenList)
    : tokenList_(tokenList) {}

int ParameterList::size() const { return tokenList_.size(); }

std::shared_ptr<Ast> ParameterList::get(int index) const {
  return tokenList_[index];
}

std::string ParameterList::toString() const {
  return fmt::format("[ @ParameterList ]");
}

int ParameterList::type() const { return F_TYPE_PARAMETER_LIST; }

} // namespace fastype
