// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/IntegerConstant.h"
#include <fmt/format.h>

namespace fastype {

IntegerConstant::IntegerConstant(Sptr<AstTree> op, Sptr<AstTree> expr)
    : op_(op), expr_(expr) {}

Sptr<AstTree> IntegerConstant::op() const { return op_; }

Sptr<AstTree> IntegerConstant::expr() const { return expr_; }

std::string IntegerConstant::toString() const {
  return fmt::format("[ @IntegerConstant op_:{}, expr_:{} ]", op_->toString(),
                     expr_->toString());
}

std::string IntegerConstant::name() const { return "IntegerConstant"; }

} // namespace fastype
