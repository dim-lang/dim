// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/expr/BinaryExpr.h"

namespace fastype {

BinaryExpr::BinaryExpr(const std::vector<Sptr<AstTree>> &children)
    : AstList(children) {}

icu::UnicodeString BinaryExpr::op() const { return children_[1]->literal(); }

Sptr<AstTree> BinaryExpr::left() const { return children_[0]; }

Sptr<AstTree> BinaryExpr::right() const { return children_[2]; }

} // namespace fastype
