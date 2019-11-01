// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/AstList.h"
#include <fmt/format.h>
#include <sstream>

namespace fastype {

AstList::AstList(const std::vector<Sptr<AstTree>> &children)
    : children_(children) {}

Sptr<AstTree> AstList::child(int i) const { return children_[i]; }

int AstList::childrenNumber() const { return children_.size(); }

std::string AstList::toString() const {
  std::stringstream ss;
  for (int i = 0; i < children_.size(); i++) {
    ss << " " << children_[i]->toString();
    if (i < children_.size() - 1) {
      ss << ",";
    }
  }
  return fmt::format("[ @AstList children_#size:{}, children_:{} ]",
                     children_.size(), ss.str());
}

} // namespace fastype
