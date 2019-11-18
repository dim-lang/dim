// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/UnaryOp.h"
#include <fmt/format.h>

namespace fastype {

UnaryOp::UnaryOp(Sptr<AstTree> op, Sptr<AstTree> expr) : op_(op), expr_(expr) {}

Sptr<AstTree> UnaryOp::op() const { return op_; }

Sptr<AstTree> UnaryOp::expr() const { return expr_; }

std::string UnaryOp::toString() const {
  return fmt::format("[ @UnaryOp op_:{}, expr_:{} ]", op_->toString(),
                     expr_->toString());
}

std::string UnaryOp::name() const { return "UnaryOp"; }

} // namespace fastype
